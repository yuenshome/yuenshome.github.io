#include <iostream>
#include <string>
using namespace std;

class Book {
public:
	string getContent()const {
		return "很久很久以前，有一个...";
	}
};

class Mother {
public:
	void tellStory(const Book& b) {
		cout << "妈妈开始讲故事: " 
			<< b.getContent() << endl;
	}
};

int main() {
	Book book;
	Mother m;
	m.tellStory(book);
	return 0;
}