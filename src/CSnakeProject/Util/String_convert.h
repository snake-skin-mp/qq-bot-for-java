#pragma once

#include <locale>
#include <codecvt>

class String_convert {
public:
	static std::string string_to_utf8(const std::string &str);
	static std::string utf8_to_string(const std::string &str);
};

