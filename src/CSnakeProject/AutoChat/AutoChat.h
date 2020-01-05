#pragma once

#include <vector>
#include <string>
#include "../include/Documents.h"

class AutoChat {
public:
	static void init();
	static std::string get_reply(std::string str);
private:
	static void generate_train_set();
	static std::vector<std::string> read_train_set();
	static Documents docs;

};