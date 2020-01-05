#pragma once

#include "../cppjieba/Jieba.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <unordered_set>
#include <map>

class Documents {
	friend class Query;
public:
	Documents() = default;
	Documents(std::vector<std::string> doc_vec) { doc_contents = doc_vec; }		// �ϲ���ֵ��ֵ����

	static void set_stop_words(const std::string &stop_words_file);
	static void set_synonym(const std::string &synonym_file);

	void set_doc_vec(std::vector<std::string> doc_vec) { doc_contents = doc_vec; }
	void calculate();		// ���� tf-idf ֵ

	size_t get_docs_num() const { return doc_contents.size(); }
	const std::vector<std::string> &get_doc_contents() const { return doc_contents; }
	std::ostream &print_doc_info(size_t doc_idx, int n = 12, std::ostream &os = std::cout) const;
private:
	using sparse_vector_type = std::pair<std::vector<int>,		// indx[]��index ���飬�� mkl sparse BLAS һ��
										 std::vector<double>>;	// x[]��ֵ���飬�� mkl sparse BLAS һ��

	static std::unordered_set<std::string> stop_words;
	static std::map<std::string, std::string> synonym;
	static cppjieba::Jieba jieba;

	std::vector<std::string> doc_contents;				// �ĵ���� -> �ĵ������� int ������߲��ж�
	std::map<std::string, std::pair<int, size_t>> df;	// {���{�����ţ��ĵ�Ƶ��}}��Ҳ�ṩ�ʻ����
	std::vector<std::map<std::string, double>> tf;		// �ĵ���� ->{����1��tf1}{����2��tf2}...
	std::vector<sparse_vector_type> tf_idf;				// �ĵ���� -> tf-idf ϡ������
	std::vector<double> nrm2;							// �ĵ���� -> ����2����

	const sparse_vector_type &get_doc_sparse_tfidf_vec(int doc_idx) const { return tf_idf[doc_idx]; }

	double get_doc_sparse_tfidf_vec_nrm2(int doc_idx) const { return nrm2[doc_idx]; }

	static bool is_not_stop_word(const std::string &s) 
			{ return stop_words.find(s) == stop_words.cend(); }

	static std::string to_synonym(std::string s) 		// �������ֵ����
			{ return synonym.find(s) != synonym.end() ? synonym[s] : s; }

	static std::vector<std::string> get_segmentation(const std::string &sentence);
};

