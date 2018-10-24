#include <iostream>
#include <string>
using namespace std;
class IReader {
public:
	//纯虚函数，接口
	virtual string getContent()const = 0;
};
class Book:public IReader {
public:
	virtual string getContent()const override{
		return "很久很久以前，有一个...";
	}
};
class Newspaper:public IReader {
public:
	virtual string getContent()const override{
		return "昨日凌晨，台风...";
	}
};
class Mother {
public:
	void tellStory(const IReader& b) {
		cout << "妈妈开始讲故事: " 
			<< b.getContent() << endl;
	}
};
int main() {
	Book book;
	Mother m;
	m.tellStory(book);
	Newspaper paper;
	m.tellStory(paper);
	return 0;
}