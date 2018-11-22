//练习：将一个文件中的单词按照规则文件要求转换输出
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
using namespace std;
map<string, string> build_map(ifstream &map_file);
const string& trans_word(const string& word, 
			const map<string, string> &trans_map);

//将转换规则文件读入map
map<string, string> build_map(ifstream &map_file) {
	map<string, string> trans_map;
	string key;   //要替换的关键字
	string value; //替换后的内容
	char ch; //中间空格
	while (map_file >> key && 
		map_file.ignore(1),getline(map_file, value)) 
		trans_map[key] = value;
	return trans_map;
}

//给定一个单词word,在trans_map中查找是否要替换,返回转换后的string
const string& trans_word(const string& word, 
				const map<string, string> &trans_map) {
	auto it = trans_map.find(word);
	if (it != trans_map.end())  //找到,要替换
		return (*it).second;
	else //没找到,还是传回原来的word
		return word;
}

int main() {
	ifstream map_file("rules.txt"); //转换规则文件
	map<string, string> trans_map = build_map(map_file);
	for (auto& item : trans_map) //输出看下转换规则
		cout << item.first << "-->"<< item.second << "\n";
	cout << endl;

	ifstream fs("need_trans.txt"); //读需要转换的文件
	string line; //每行的内容
	while (getline(fs,line)) {
		istringstream ss(line);
		string word;
		bool flag=true;
		while (ss >> word) {
			if (flag) 
				flag = false;
			else 
				cout << " "; //非首字
			cout << trans_word(word, trans_map);
		}
		cout << endl;
	}
	return 0;
}