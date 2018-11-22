//文本查询程序: 在一个给定的文件中查询单词
//查询结果以及输出:显示所有出现该单词的行号和行内容,以及出现行的次数。
//如果一个单词在一行中多次出现,那么该行只显示一次,也只计数一次。
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <sstream>
#include <fstream>
#include <algorithm>
using namespace std;

class QueryResult;
ostream& print(ostream& out, const QueryResult &qr);

class TextQuery { 
public:
	//根据文件生成相关需要的数据:按行保存文件到vector,建立单词到行号集合的映射
	TextQuery(ifstream& ifs):file(make_shared<vector<string>>()) { //根据文件,构造
		string text;
		while (getline(ifs, text)) { //一行一行读到 text中
			(*file).push_back(text); //存入vector
			int n = (*file).size() - 1; //当前行号
			istringstream line(text); //准备读该行的每个单词
			string word;
			while (line >> word) { //该行中的每个单词,存入map(wm中)
				transform(word.begin(), word.end(), word.begin(), ::tolower); //转小写
				shared_ptr<set<int>> &lines = wm[word]; //没有该单词,也会自动插入,但是此时value是空指针
				if (!lines)
					lines.reset(new set<int>); //创建一个新的 set<int>
				(*lines).insert(n); //将此行号插入 set中
			}
		}
	}
	void show()const {
		int i = 1;
		for (const auto& item : *file) //显示vector:
			cout << "(" << i++ << ") " << item << endl;
		for (const auto& item : wm) { //显示map:
			cout << item.first << "-->";
			for (const auto& item1 : *item.second) cout << item1 << " "; cout << endl;
		}
	}
	QueryResult query(const string& sought)const; 
private:
	shared_ptr<vector<string>> file; //文件内容,每个元素都是一行,并且下标和行号对应
	map<string, shared_ptr<set<int>>> wm; //单词 --> 保存行号的set 的映射
};

class QueryResult { 
	friend ostream& print(ostream& out, const QueryResult &qr);
public:
	QueryResult(const string& s,
		const shared_ptr<vector<string>>& f,
		const shared_ptr<set<int>> &p):
				sought(s),file(f),lines(p){}
private:
	string sought; //查询的单词
	shared_ptr<vector<string>> file; //文件内容
	shared_ptr<set<int>> lines; //行号集合
};
//查询函数,输入单词,返回一个QueryResult对象
QueryResult TextQuery::query(const string& sought)const { 
	//如果没有找到该单词,则返回下面这个 空set<int>
	static shared_ptr<set<int>> nodata = make_shared<set<int>>();
	auto it = wm.find(sought);
	if (it != wm.end()) //找到了
		return{ sought,file, (*it).second };
	else
		return{ sought,file,nodata };
}
ostream& print(ostream& out, const QueryResult &qr) {
	out << qr.sought << " 共在 " << (*qr.lines).size() << " 行中出现:\n";
	for (const auto& item : *qr.lines) 
		cout << " (" << item + 1 << ") " << (*qr.file)[item] << endl;
	return out;
}

int main() {
	ifstream ifs("word_query.txt");
	if (!ifs) { cout << "open error!\n"; return -1; }
	
	TextQuery tq(ifs);
	tq.show();
	cout << "------------\n";
	while (true) {
		cout << "\n输入要查找的单词 (输入q退出): ";
		string s;
		if (cin >> s && s != "q") {
			transform(s.begin(), s.end(), s.begin(), ::tolower); //转小写
			print(cout, tq.query(s));
		}
		else
			break;
	}
}