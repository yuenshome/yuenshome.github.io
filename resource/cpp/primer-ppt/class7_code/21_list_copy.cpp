#include <iostream>
#include <cstring>
#include <string>
#include <cassert>
#include <typeinfo>
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
	StrObj(const StrObj& other) :Obj(other) {
		data = new char[strlen(other.data) + 1];
		strcpy(data, other.data); //深拷贝
	}
	virtual void print()const override {
		cout << "\""<<data << "\" -> ";
	}
private:
	char *data;
};
class StuObj :public Obj { //派生类:学生数据
public:
	StuObj(int _id,const string &_name)
		:id(_id),name(_name){}
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
	Node(Obj *p_obj=nullptr)
		:p_data(p_obj),next(nullptr){}
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
	List(const List &other):List() {
		//思路：遍历other链表,将每个元素复制一份push_back
		//但是元素是以Obj*存放的,并不知道其真实类型是啥
		//尝试这样写：
		Node * p = other.head;
		Obj * p_new_obj = nullptr;
		while (p) { //遍历other链表
			if (typeid(*(p->p_data)) == typeid(IntObj)) {
				//先将Node结点中的Obj*转为真实类型的IntObj*
				IntObj * p_int = dynamic_cast<IntObj*>(p->p_data);
				p_new_obj = new IntObj(*p_int);
			}
			else if(typeid(*(p->p_data)) == typeid(StrObj)){
				//先将Node结点中的Obj*转为真实类型的StrObj*
				StrObj * p_int = dynamic_cast<StrObj*>(p->p_data);
				p_new_obj = new StrObj(*p_int);
			}
			else if (typeid(*(p->p_data)) == typeid(StuObj)) {
				//先将Node结点中的Obj*转为真实类型的StuObj*
				StuObj * p_int = dynamic_cast<StuObj*>(p->p_data);
				p_new_obj = new StuObj(*p_int);
			}
			else {
				cout << "没有找到匹配的类型\n";
				throw other;
			}
			push_back(p_new_obj); //push_back
			p = p->next;
		}
		size = other.size;
	}
	void push_back(Obj *p_obj) {
		assert(p_obj);
		Node *p_new_node = new Node(p_obj);
		if (!head) 	head = p_new_node;
		else  tail->next = p_new_node;
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
	L.push_back(new StrObj("abc"));
	L.print_list();
	List L1(L);
	L1.print_list();
	return 0;
}