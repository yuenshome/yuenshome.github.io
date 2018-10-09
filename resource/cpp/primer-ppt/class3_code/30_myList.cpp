/// 1.文件 StudentNode.h
#ifndef _STUDENTNODE_H
#define _STUDENTNODE_H
#include <string>
class Student_Node {
	friend class myList;
public:
	Student_Node(bool flag = false, int _age = 20,
		const std::string &_name = "")
		:id(calc_id(flag)), age(_age),
		name(_name), next(NULL) {}

	static int calc_id(bool flag) {
		if(flag) return now_id++; 
		return 0;
	}
private:
	int id;
	int age;
	std::string name;
	Student_Node *next;
private:
	static int now_id;
};
#endif
//================================================


/// 2.文件 StudentNode.cpp
#include "StudentNode.h"

int Student_Node::now_id = 100;

//================================================

/// 3.文件 myList.h
#ifndef _MYLIST_H
#define _MYLIST_H
#include <string>
#include "StudentNode.h"
using namespace std;

class myList {
public:
	myList();
	~myList();
	myList(const myList& other);
	myList &operator=(const myList& other);
	void insert_tail(int _age,const string& _name); //尾插
	bool del_node(int _id); //按id删除节点
	void print()const; //遍历打印
	inline int get_size()const;
private:
	void copy_from(const myList& other);
private:
	Student_Node *head; //头指针
	Student_Node *tail; //尾指针
	int size; //节点数量
};

inline int myList::get_size()const {
	return size;
}
#endif

//================================================

/// 4.文件 myList.cpp
#include <iostream>
#include "myList.h"
using namespace std;

myList::myList() :size(0) {
	head = new Student_Node;
	tail = head;
}

myList::~myList() {
	while (head) {
		Student_Node *tmp = head->next;
		delete head;
		head = tmp;
	}
	head = tail = NULL;
}

void myList::insert_tail(int _age, const string& _name) {
	Student_Node *newNode = new Student_Node(true, _age, _name);
	tail->next = newNode;
	tail = newNode;
	size++;
}
bool myList::del_node(int _id) { //按id删除
	Student_Node *p = head, *q = head->next;
	while (q && q->id != _id) {
		p = q;
		q = q->next;
	}
	if (!q) return false; //没找到,删除失败
	if (q->next == NULL) tail = p;
	p->next = q->next;
	delete q;
	size--;
	return true;
}
void myList::print()const { //遍历打印
	Student_Node* p = head->next;
	cout << "Head -> ";
	while (p) {
		cout << "(" << p->id << "," << p->name << "," << p->age << ") -> " ;
		p = p->next;
	}
	cout << "NULL" << endl;
}

myList::myList(const myList& other) :myList() {
	                               //委托构造
	copy_from(other);
}

myList &myList::operator=(const myList& other) {
	if (this == &other) return *this;
	Student_Node *p = head->next;
	while (p) {
		Student_Node *q = p->next;
		delete p;
		p = q;
	}
	head->next = NULL;
	tail = head;
	size = 0;
	copy_from(other);
	return *this;
}

void myList::copy_from(const myList& other) {
	Student_Node *p = other.head->next;
	while (p) {
		Student_Node *newNode = new Student_Node(*p); 
		tail->next = newNode;
		tail = newNode;
		size++;
		p = p->next;
	}
}

//================================================
///测试 5.文件 main.cpp
#include <iostream>
#include "StudentNode.h"
#include "myList.h"
using namespace std; 

int main() {
	cout << "p1----------" << endl;
	myList* p1 = new myList;
	p1->insert_tail(30, "赵一");
	p1->insert_tail(21, "张三");
	p1->print();
	p1->del_node(100);
	p1->print();
	p1->insert_tail(19, "钱五");
	p1->print();
	cout << "p2----------" << endl;
	myList* p2 = new myList(*p1);
	p2->print();
	p2->insert_tail(18, "李六");
	p2->print();
	cout << "p3----------" << endl;
	myList* p3 = new myList;
	*p3 = *p2;
	p3->print();
	p3->del_node(103);
	p3->print();
	p3->insert_tail(19, "费大");
	p3->print();
	delete p1;
	delete p2;
	delete p3;
	return 0;
}
