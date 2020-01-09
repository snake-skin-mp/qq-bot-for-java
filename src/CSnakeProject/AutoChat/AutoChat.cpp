#include "../include/Query.h"
#include "../include/String_convert.h"
#include "AutoChat.h"


using namespace std;

vector<string> AutoChat::read_train_set() {
	ifstream infile("trainset.txt");
	vector<string> vec;
	string line;
	while (getline(infile, line)) {
		if (line.empty()) continue;
		vec.push_back(std::move(line));
	}
	infile.close();
	infile.open("learn.txt");
	while (getline(infile, line)) {
		istringstream iss(line);
		string q, r;
		iss >> q;
		vector<string> rs;
		while (iss >> r) {
			rs.push_back(r);
		}
		if (!rs.empty()) {
			learn_query.push_back(std::move(q));
			learn_replys.push_back(std::move(rs));
		}
	}
	return vec;
}

std::vector<std::string> AutoChat::learn_query;

std::vector<std::vector<std::string>> AutoChat::learn_replys;

Documents AutoChat::docs;

Documents AutoChat::docs_learn;

std::string AutoChat::replace_people(std::string str) {
	for (const auto &s : mp_alias) {
		auto pos = str.find(s);
		while (pos != string::npos) {
			str.replace(pos, s.size(), u8"#");
			cur_people = _mp;
			pos = str.find(str, pos + 1);
		}
	}
	for (const auto &s : yy_alias) {
		auto pos = str.find(s);
		while (pos != string::npos) {
			str.replace(pos, s.size(), u8"#");
			cur_people = _yxy;
			pos = str.find(str, pos + 1);
		}
	}
	for (const auto &s : gy_alias) {
		auto pos = str.find(s);
		while (pos != string::npos) {
			str.replace(pos, s.size(), u8"#");
			cur_people = _6g3y;
			pos = str.find(str, pos + 1);
		}
	}
	return str;
}

std::string AutoChat::replace_symbol(std::string str) {
	auto pos = str.find(u8"#");
	while (pos != string::npos) {
		switch (cur_people) {
			case _6g3y: str.replace(pos, 1, gy_alias[rand() % gy_alias.size()]); break;
			case _mp: str.replace(pos, 1, mp_alias[rand() % mp_alias.size()]); break;
			case _yxy: str.replace(pos, 1, yy_alias[rand() % yy_alias.size()]); break;
			default:
				break;
		}
		pos = str.find(str, pos + 1);
	}
	return str;
}

const std::vector<std::string> AutoChat::mp_alias = { u8"mp4", u8"MP", u8"Mp", u8"mp", u8"嗯批儿", u8"七海浅夏" };
const std::vector<std::string> AutoChat::yy_alias = { u8"yy", u8"YY", u8"ppy", u8"PPY", u8"皮皮歪", u8"歪歪", u8"蛇皮Y", u8"蛇皮y" };
const std::vector<std::string> AutoChat::gy_alias = { u8"6g3y", u8"6gay" };

AutoChat::PEOPLE AutoChat::cur_people;

//std::vector<AutoChat::PEOPLE> AutoChat::people;

//AutoChat::PEOPLE AutoChat::get_people(char num) {
//	return (PEOPLE)(num - '0');
//}

void AutoChat::init() {
	//Documents::set_stop_words("../dict/stop_words.utf8");
	docs.set_doc_vec(read_train_set());
	docs_learn.set_doc_vec(learn_query);
	docs.calculate();
	docs_learn.calculate();
}

std::string AutoChat::get_reply(std::string str) {
	Query query_learn(docs_learn);
	auto result_learn = query_learn.query(replace_people(str));
	Query query(docs);
	auto result = query.query(str);
	/*for (auto p : result) {
		cout << String_convert::utf8_to_string(docs.get_doc_contents()[p.first]) << endl;
		docs.print_doc_info(p.first, 1);
		cout << "-------------------------" << endl;
	}*/
	int i;
	switch (rand() % 10) {
		case 0: case 1: case 2: case 3: i = 0; break;
		case 4: case 5: case 6: i = 1; break;
		case 7: case 8: i = 2; break;
		default: i = 3; break;
	}
	string reply;
	//reply = u8"这是";
	//int num[7] = {};
	//for (int i = 0; i < 10; ++i) {
	//	num[(int)people[result[i].first]]++;
	//}
	//switch (get_people(max_element(num, num + 6) - num + '0')) {
	//	case _6g3y: reply += u8"6g3y"; break;
	//	case _yxy: reply += u8"yyxxyy"; break;
	//	case _mp: reply += u8"mp"; break;
	//	case _system: reply += u8"系统消息"; break;
	//	case _mc2: reply += u8"mc的方"; break;
	//	case _zhuzi: reply += u8"竹子"; break;
	//	default: reply = u8"我无法推断出这是谁问我的问题。"; break;
	//}
	//reply += u8"在问我？";
	int next = min((int)docs.get_docs_num() - 1, result[i].first + 1);
	if (result[i].second > result_learn[i].second + 0.2) {
		cout << reply + docs.get_doc_contents()[next] << endl;
		return reply + docs.get_doc_contents()[next];
	} else {
		const auto &v = learn_replys[result_learn[i].first];
		cout << reply + v[rand() % v.size()] << endl;
		return reply + replace_symbol(v[rand() % v.size()]);
	}
}