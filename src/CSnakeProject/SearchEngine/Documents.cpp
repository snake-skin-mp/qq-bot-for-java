#include "Documents.h"
#include "../include/String_convert.h"
#include <algorithm>
#include <omp.h>
#include <functional>
#include <iostream>
#define DEBUG_PRINT

using namespace std;

 
constexpr const char *DICT_PATH = "../dict/jieba.dict.utf8";
constexpr const char *HMM_PATH = "../dict/hmm_model.utf8";
constexpr const char *USER_DICT_PATH = "../dict/user.dict.utf8";
constexpr const char *IDF_PATH = "../dict/idf.utf8";
constexpr const char *STOP_WORD_PATH = "../dict/stop_words.utf8";
 

std::unordered_set<std::string> Documents::stop_words;
std::map<std::string, std::string> Documents::synonym;

cppjieba::Jieba Documents::jieba(DICT_PATH,
								 HMM_PATH,
								 USER_DICT_PATH,
								 IDF_PATH,
								 STOP_WORD_PATH);

void Documents::set_stop_words(const std::string &stop_words_file) {
	// 不包括空白字符
	ifstream infile(stop_words_file);
	if (infile.fail()) 
		throw runtime_error("cannot open file " + stop_words_file);
	istream_iterator<string> in(infile), eof;
	copy(in, eof, inserter(stop_words, stop_words.begin()));

	// 添加空白
	stop_words.insert({ " ", "\t", "\n", "\r", "" });

#ifdef DEBUG_PRINT
	std::cout << "\ttotal: " << stop_words.size() 
			  << " stop words" << endl;
#endif
}

void Documents::set_synonym(const std::string &synonym_file) {
	ifstream infile(synonym_file);
	if (infile.fail())
		throw runtime_error("cannot open file " + synonym_file);
	string line;
	while (getline(infile, line)) {
		istringstream iss(line);
		string s, w;
		iss >> s;
		while (iss >> w) synonym[std::move(w)] = s;
	}
}

vector<string> Documents::get_segmentation(const string &sentence) {
	// 大小写统一变小写
	string s;
	transform(sentence.begin(), sentence.end(), back_inserter(s), tolower);

	// 提取所有可能的分词
	vector<string> words_all;
	jieba.CutForSearch(s, words_all);

	// 处理近义词
	if(!synonym.empty())
		transform(make_move_iterator(words_all.begin()), 
				  make_move_iterator(words_all.end()), 
				  words_all.begin(), 
				  to_synonym);
	return words_all;
}

void Documents::calculate() {
	int n = doc_contents.size();
	tf.resize(n);
	tf_idf.resize(n);
	nrm2.resize(n);

	// 统计 tf df
#ifdef DEBUG_PRINT
	std::cout << "\treading documents   0%%" << flush;
	int cnt_debug = 0;
	int percentage_debug = 0;
	double begin_time = omp_get_wtime();
#endif
	#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		vector<string> words_content = get_segmentation(doc_contents[i]);
		for (const string &word : words_content) {
			if (is_not_stop_word(word)) {
				if (++tf[i][word] == 1)		// 计算 tf
					#pragma omp critical
					++df[word].second;		// 计算 df
			}
		}	
#ifdef DEBUG_PRINT
		#pragma omp critical
		++cnt_debug;
		
		if (cnt_debug * 100 / n > percentage_debug) 
		#pragma omp critical
		{
			percentage_debug = cnt_debug * 100 / n;
			printf("\b\b\b\b%3d%%", percentage_debug);
			std::cout << flush;
		}
#endif
	}

#ifdef DEBUG_PRINT
	std::cout << "   time: " 
		      << omp_get_wtime() - begin_time << " s\n"
			  << "\tcalculating tfidf   0%%" << flush;
	cnt_debug = 0;
	percentage_debug = 0;
	begin_time = omp_get_wtime();
#endif
	// 计算词语编号
	int word_cnt = 0;
	for (auto &pair : df) pair.second.first = word_cnt++;

	// 计算 tf-idf
	#pragma omp parallel for
	for (int i = 0; i < n; ++i) {
		vector<int> &indx = tf_idf[i].first;
		vector<double> &x = tf_idf[i].second;
		double dsum = 0;
		for (auto &pair : tf[i]) {
			const auto &p = df[pair.first];
			int word_idx = p.first;
			double word_df = p.second;
			double word_tf = pair.second;
			double word_tf_idf = (1 + log(word_tf)) * log(n / word_df);
			indx.push_back(word_idx);
			x.push_back(word_tf_idf);
			dsum += word_tf_idf * word_tf_idf;
		}
		nrm2[i] = sqrt(dsum);
#ifdef DEBUG_PRINT
#pragma omp critical
		++cnt_debug;

		if (cnt_debug * 100 / n > percentage_debug)
#pragma omp critical
		{
			percentage_debug = cnt_debug * 100 / n;
			printf("\b\b\b\b%3d%%", percentage_debug);
			std::cout << flush;
		}
#endif
	}
#ifdef DEBUG_PRINT
	std::cout << "   time: " 
			  << omp_get_wtime() - begin_time << " s\n";
#endif
}

ostream &Documents::print_doc_info(size_t doc_idx, int n /*= 12*/, ostream &os /*= std::cout*/) const {
	if (doc_idx >= doc_contents.size()) {
		os << "doc_idx out of range " << endl;
	} else {
		int idx = doc_idx;
		os  << String_convert::utf8_to_string(doc_contents[idx]) << "\n"
			<< "-----------------------------------------------------------\n"
			<< "tf-idf 信息：\n";
		
		const auto &indx = tf_idf[idx].first;
		const auto &x = tf_idf[idx].second;
		n = min((int)indx.size(), n);
		vector<pair<double, int>> v;
		for (int i = 0; i < x.size(); ++i) {
			v.emplace_back(-x[i], indx[i]);
		}
		partial_sort(v.begin(), v.begin() + n, v.end());

		auto print_word_info = [&](const pair<double, int> &p)
		{
			string word; int dfv = -1;
			for (const auto &dfp : df)
				if (dfp.second.first == p.second)
					{ word = dfp.first; dfv = dfp.second.second; }
			os	<< String_convert::utf8_to_string(word) << "\ttf-idf: " << -p.first 
				<< "\t\ttf: " << tf[idx].find(word)->second << "\t\tdf: " << dfv << "\n";
		};

		for_each(v.begin(), v.begin() + n, print_word_info);
	}
	os << flush;
	return os;
}
