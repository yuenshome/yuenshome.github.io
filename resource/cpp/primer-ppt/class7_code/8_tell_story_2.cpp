#include <iostream>
#include <string>
using namespace std;
class Book {
public:
	string getContent()const {
		return "很久很久以前，有一个...";
	}
};
class Newspaper {
public:
	string getContent()const {
		return "昨日凌晨，台风...";
	}
};
class Mother {
public:
	void tellStory(const Book& b) {
		cout << "妈妈开始讲故事: " 
			<< b.getContent() << endl;
	}
	void tellStory(const Newspaper& b) {
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