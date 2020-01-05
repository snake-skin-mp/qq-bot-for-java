#pragma once

#include "Documents.h"
#include "../cppjieba/Jieba.hpp"
#include <string>
#include <vector>

// ʹ��ʱ�����ͷ� Documents ����
class Query {
public:
	Query(const Documents &documents) : docs(documents) {}

	// str Ϊ�����ַ�����n Ϊ��Ҫ���ٸ�����������ĵ� id
	std::vector<std::pair<int, double>> query(std::string str, size_t n = 20)
					{ query_str = std::move(str); return do_query(n); }

private:
	std::vector<std::pair<int, double>> do_query(size_t n);	// ���в�ѯ��������˳��������º���

	void clean_member();					// �����Ա�������ǰ���й���ѯ
	void segment();							// �з� query_str ���� query_words
	void calculate_tfidf_sparse_vec();		// ���� query_words ���� query_tfidf_sparse_vec ��ѯ����
	void sparse_vec_to_full_storage();		// �� query_tfidf_sparse_vec ת��Ϊ�����洢������㣬������nrm2
	void calculate_query_docs_cos();		// ���� query_tfidf_vec �� docs �������нǶ�
	void calculate_doc_score();				// ���������ĵ����ƶȵ÷�
	void get_first_n_docs(int n);			// ���ǰ n ƪ����ĵ�

	double get_cos(const Documents::sparse_vector_type &vec1, 
				   const Documents::sparse_vector_type &vec2,
				   double *full_storage_zero_vec);	// ����������Ϊ��ʼ��Ϊ 0����ʱ�ռ䣬������������ vec

	double get_cos(const Documents::sparse_vector_type &vec_doc,
				   double vec_doc_nrm2);

	const Documents &docs;
	int next_word_id_not_use = 0;
	std::string query_str;
	std::map<std::string, int> query_words;		// �ʣ�Ƶ�� tf
	Documents::sparse_vector_type query_tfidf_sparse_vec;
	std::unique_ptr<double[]> query_tfidf_full_vec;
	double query_tfidf_full_vec_nrm2 = 0;
	std::vector<double> query_doc_cos;		// doc_id��doc_cos_value �� pair
	std::vector<std::pair<int, double>> query_doc_score;	// doc_id���÷ֵ� pair
};

