#include "Query.h"
#include <mkl.h>
#include "../include/String_convert.h"
using namespace std;

vector<pair<int, double>> Query::do_query(size_t n) {
	clean_member();
	segment();
	calculate_tfidf_sparse_vec();
	sparse_vec_to_full_storage();
	calculate_query_docs_cos();
	calculate_doc_score();
	get_first_n_docs(n);

	return vector<pair<int, double>>(query_doc_score.begin(), 
									 query_doc_score.begin() + min(n, query_doc_score.size()));
}

void Query::clean_member() {
	next_word_id_not_use = 0;
	query_words.clear();
	query_tfidf_sparse_vec.first.clear();
	query_tfidf_sparse_vec.second.clear();
	query_tfidf_full_vec.reset();
	query_tfidf_full_vec_nrm2 = 0;
	query_doc_cos.clear();
	query_doc_score.clear();
}

void Query::segment() {
	transform(query_str.begin(), query_str.end(), query_str.begin(), tolower);
	vector<string> words_all;
	
	docs.jieba.Cut(query_str, words_all);
	for_each(words_all.begin(), words_all.end(), [this](const string &word)
			 {
				 if (Documents::is_not_stop_word(word)) {
					 if (docs.synonym.find(word) != docs.synonym.end())
						 ++query_words[docs.synonym[word]];
					 else
						 ++query_words[word];
				 }

			 });
}

void Query::calculate_tfidf_sparse_vec() {
	const auto &vocabulary = docs.df;
	next_word_id_not_use = vocabulary.size();
	for (const auto &pair : query_words) {
		const string &word = pair.first;
		double tf = pair.second;

		double tfidf; int indx;
		auto it = vocabulary.find(word);
		if (it == vocabulary.cend()) {
			// 词汇表中没有查询中的词汇
			indx = next_word_id_not_use++;
			tfidf = (1 + log(tf)) * log(docs.get_docs_num());
		} else {
			// 词汇表中有查询中的词汇
			indx = it->second.first;
			tfidf = (1 + log(tf)) * log(docs.get_docs_num() / it->second.second);
		}

		query_tfidf_sparse_vec.first.push_back(indx);
		query_tfidf_sparse_vec.second.push_back(tfidf);
	}
	
}

void Query::sparse_vec_to_full_storage() {
	query_tfidf_full_vec.reset(new double[next_word_id_not_use]);
	memset(query_tfidf_full_vec.get(), 0, next_word_id_not_use * sizeof(double));
	cblas_dsctr(query_tfidf_sparse_vec.first.size(), 
				&query_tfidf_sparse_vec.second[0], 
				&query_tfidf_sparse_vec.first[0], 
				query_tfidf_full_vec.get());
	query_tfidf_full_vec_nrm2 = cblas_dnrm2(query_tfidf_sparse_vec.second.size(), 
											&query_tfidf_sparse_vec.second[0], 
											1);
}

void Query::calculate_query_docs_cos() {
	int n = docs.get_docs_num();
	query_doc_cos.resize(n);
	#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		double cos_value = get_cos(docs.get_doc_sparse_tfidf_vec(i),
								   docs.get_doc_sparse_tfidf_vec_nrm2(i));
		query_doc_cos[i] = cos_value;
	}
}

void Query::calculate_doc_score() {
	int n = docs.get_docs_num();
	query_doc_score.resize(n);

	#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		query_doc_score[i] = { i, query_doc_cos[i] };
	}
}

void Query::get_first_n_docs(int n) {
	using ty = decltype(query_doc_score)::const_reference;
	auto sort_func = [](ty a, ty b) { return a.second > b.second; };
	partial_sort(query_doc_score.begin(), query_doc_score.begin() + n, query_doc_score.end(), sort_func);
}

double Query::get_cos(const Documents::sparse_vector_type &vec1,
					  const Documents::sparse_vector_type &vec2,
					  double *full_storage_zero_vec) {

	cblas_dsctr(vec1.first.size(), &vec1.second[0], &vec1.first[0], full_storage_zero_vec);
	double dot = cblas_ddoti(vec2.first.size(), &vec2.second[0], &vec2.first[0], full_storage_zero_vec);
	double vec1_nrm2 = cblas_dnrm2(vec1.second.size(), &vec1.second[0], 1);
	double vec2_nrm2 = cblas_dnrm2(vec2.second.size(), &vec2.second[0], 1);
	double cos_value = dot / (vec1_nrm2 * vec2_nrm2);
	return cos_value;
}

double Query::get_cos(const Documents::sparse_vector_type &vec_doc, 
					  double vec_doc_nrm2) {
	double dot = cblas_ddoti(vec_doc.first.size(), &vec_doc.second[0], 
							 &vec_doc.first[0], query_tfidf_full_vec.get());
	return dot / (vec_doc_nrm2 * query_tfidf_full_vec_nrm2);
}
