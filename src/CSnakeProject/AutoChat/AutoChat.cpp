#include "../include/Query.h"
#include "../include/String_convert.h"
#include "AutoChat.h"


using namespace std;

void AutoChat::generate_train_set() {
	ifstream infile("聊天记录.txt");
	ofstream outfile("trainset.txt");
	string line;
	while (getline(infile, line)) {
		if (line.size() > 10 && line[0] == '2' && line[1] == '0') continue;	// 2019, 2020 xxx 跳过
		if (line.empty() || line == u8"[图片]" || line == u8"[表情]") continue;
		if (line.size() < 15) continue;
		outfile << line << "\n";
	}
}

vector<string> AutoChat::read_train_set() {
	ifstream infile("trainset.txt");
	vector<string> vec;
	string line;
	while (getline(infile, line)) {
		vec.push_back(std::move(line));
	}
	return vec;
}

Documents AutoChat::docs;

void AutoChat::init() {
	//Documents::set_stop_words("../dict/stop_words.utf8");
	docs.set_doc_vec(read_train_set());
	docs.calculate();
}

std::string AutoChat::get_reply(std::string str) {
	Query query(docs);
	auto result = query.query(str);
	/*for (auto p : result) {
		cout << String_convert::utf8_to_string(docs.get_doc_contents()[p.first]) << endl;
		docs.print_doc_info(p.first, 1);
		cout << "-------------------------" << endl;
	}*/
	int i;
	switch (rand() % 10) {
		case 0: case 1: case 2: i = 0; break;
		case 3: case 4: i = 1; break;
		case 5: case 6: i = 2; break;
		case 7: i = 3; break;
		case 8: i = 4; break;
		default: i = 5; break;
	}
	int next = min((int)docs.get_docs_num() - 1, result[i].first + 1);
	return docs.get_doc_contents()[next];
}

//int main() {
//	generate_train_set(); 
//	return 0;
//}
	