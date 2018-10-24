#include <iostream>
#include <string>
using namespace std;
class List {
public:
	void push_back(const string& item){}	//插入链表
	bool find_item(const string& item)const{//查找链表
		return true; } 
	void remove(const string& item){}    //删除链表元素
	int size()const {return 1;}          //求链表元素个数
	//其他接口函数
private:
	//...
};

class Set {
public:
	bool is_member(const string& item)const;//item是否在集合中
	void insert(const string& item);   //将item插入集合
	void remove(const string& item);   //将item从集合中删除
	int size()const;				   //求集合中元素个数
private:
	List LS;
};

bool Set::is_member(const string& item)const {
	return LS.find_item(item);
}
void Set::insert(const string& item) {
	if(is_member(item))
		LS.push_back(item);
}
void Set::remove(const string& item) {
	LS.remove(item);
}
int Set::size()const {
	return LS.size();
}
int main() {
	return 0;
}