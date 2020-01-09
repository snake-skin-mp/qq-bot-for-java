#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <set>
#include <iostream>
#include "../include/String_convert.h"
using namespace std;

using PEOPLE = enum { _6g3y, _yxy, _mp, _system, _mc2, _zhuzi, _other };

void generate_train_set() {
	ifstream infile("trainset.txt");
	ofstream outfile("train.txt");
	istream_iterator<string> iit(infile), eof;
	set<string> sss;
	copy(iit, eof, inserter(sss, sss.begin()));
	for (const string &s : sss) {
		outfile << s << endl;
	}return;
	//ofstream outfile("trainset.txt", ofstream::app);
	string line;
	PEOPLE prev = _system, curr = _system;
	string prev_time, cur_time;
	ostringstream oss;
	while (getline(infile, line)) {
		if (line.substr(0, 11) == u8"@机器人 ") {
			outfile << line.substr(11) << "\n";
		}continue;
		if (line.empty()) continue;
		if (line.size() > 10 && line[0] == '2' && line[1] == '0') {
			prev = curr;
			prev_time = cur_time;
			if (line.find(u8"372091066") != string::npos) curr = _6g3y;
			else if (line.find(u8"yizhulovejiayi") != string::npos) curr = _zhuzi;
			else if (line.find(u8"2667165349") != string::npos) curr = _mp;
			else if (line.find(u8"2414898646") != string::npos) curr = _yxy;
			else if (line.find(u8"10000") != string::npos) curr = _system;
			else if (line.find(u8"136538498") != string::npos) curr = _mc2;
			else curr = _other;
			cur_time = line.substr(0, 15);
			continue;
		}
		//cout << "prev" << prev << " curr" << curr << ": " << String_convert::utf8_to_string(line) << endl;
		//cin.get();
		if (prev == curr && prev_time == cur_time) {
			if (line == u8"[图片]" || line == u8"[表情]") continue;
			if (line.find(u8"撤回了一条消息") != string::npos) continue;
			oss << line << u8", ";
		} else {
			if (oss.str().size() > 15)
				outfile << (int)prev << oss.str() << "\n";
			oss.str("");
			if (line == u8"[图片]" || line == u8"[表情]") continue;
			if (line.find(u8"撤回了一条消息") != string::npos) continue;
			oss << line;
		}
	}
}

int main() {
	generate_train_set();
	return 0;
}