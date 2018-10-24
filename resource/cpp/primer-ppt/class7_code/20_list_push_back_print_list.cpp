///有push_back和print_list的链表
#include <iostream>
#include <cstring>
#include <string>
#include <cassert>
using namespace std;

class Obj { //基类
public:
	virtual ~Obj() = default;
	virtual void print()const = 0;
};
class IntObj :public Obj{ //派生类:int数据 
public:
	IntObj(int i=0):data(i){}
	~IntObj(){}
	virtual void print()const override{
		cout << data << " -> ";
	}
private:
	int data;
};
class StrObj :public Obj { //派生类:str数据
public:
	StrObj(const char *str=nullptr){
		if (!str) 
			data = new char[1]{ '\0' };
		else {
			data = new char[strlen(str) + 1];
			strcpy(data, str);
		}
	}
	~StrObj() { delete data; }
	virtual void print()const override {
		cout << "\""<<data << "\" -> ";
	}
private:
	char *data;
};
class StuObj :public Obj { //派生类:学生数据
public:
	StuObj(int _id,const string &_name):id(_id),name(_name){}
	~StuObj(){}
	virtual void print()const override {
		cout << id << ":" << name << " -> ";
	}
private:
	int id;
	string name;
};


class List;
class Node {
	friend List;
public:
	Node(Obj *p_obj=nullptr):p_data(p_obj),next(nullptr){}
	~Node() { delete p_data; }
private:
	Obj *p_data;
	Node *next;
};

class List {
public:
	List():head(nullptr),tail(nullptr),size(0){}
	~List(){
		Node *p = head;
		while (p) {
			Node *tmp = p->next;
			delete p;
			p = tmp;
		}
		head = tail = nullptr;
	}
	void push_back(Obj *p_obj) {
		assert(p_obj);
		Node *p_new_node = new Node(p_obj);
		if (!head) 
			head = p_new_node;
		else 
			tail->next = p_new_node;
		tail = p_new_node;
		size++;
	}
	void print_list()const {
		Node *p = head;
		while (p) {
			p->p_data->print(); //多态
			p = p->next;
		}
		cout << "NULL\n";
	}
private:
	Node *head;
	Node *tail;
	int size;
};

int main() {
	List L;
	L.push_back(new IntObj(10));
	L.push_back(new StuObj(1001, "张三"));
	L.push_back(new IntObj(2));
	L.push_back(new StrObj("abc"));
	L.push_back(new IntObj(5));
	L.push_back(new StuObj(1002, "李四"));
	L.push_back(new IntObj(4));
	L.push_back(new StrObj("123ok"));
	L.push_back(new IntObj(8));
	L.print_list();
	return 0;
}