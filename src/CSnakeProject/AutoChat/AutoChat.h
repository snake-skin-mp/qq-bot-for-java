#pragma once

#include <vector>
#include <string>
#include "../include/Documents.h"

class AutoChat {
public:
	using PEOPLE = enum { _6g3y, _yxy, _mp, _system, _mc2, _zhuzi, _other };
	static void init();
	static std::string get_reply(std::string str);
private:
	static std::vector<std::string> read_train_set();
	static std::vector<std::string> learn_query;
	static std::vector<std::vector<std::string>> learn_replys;
	static Documents docs;
	static Documents docs_learn;
	//static std::vector<PEOPLE> people;
	//static PEOPLE get_people(char num);
	static std::string replace_people(std::string str);
	static std::string replace_symbol(std::string str);
	static const std::vector<std::string> mp_alias;
	static const std::vector<std::string> yy_alias;
	static const std::vector<std::string> gy_alias;
	static PEOPLE cur_people;
};