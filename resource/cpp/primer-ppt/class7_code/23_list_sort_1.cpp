///虚函数 + 运行时类型识别
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
	virtual Obj *clone()const = 0; //虚clone
	virtual bool operator<(const Obj&)const = 0;
};
class IntObj :public Obj{ //派生类:int数据 
public:
	IntObj(int i=0):data(i){}
	~IntObj(){}
	virtual void print()const override{
		cout << data << " -> ";
	}
	virtual IntObj *clone()const override{
		return new IntObj(*this);
		//会调用本类的拷贝构造
	}
	int get_data()const { return data; }
	virtual bool operator<(const Obj& rhs)const override;
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
	virtual StrObj *clone()const override {
		return new StrObj(*this);
		//会调用本类的拷贝构造
	}
	virtual void print()const override {
		cout << "\""<<data << "\" -> ";
	}
	const char *c_str()const { return data; }
	virtual bool operator<(const Obj& rhs)const override;
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
	virtual StuObj *clone()const override {
		return new StuObj(*this);
		//会调用本类的拷贝构造
	}
	virtual bool operator<(const Obj& rhs)const override;
	int get_id()const { return id; }
	const string& get_name()const { return name; }
private:
	int id;
	string name;
};


bool IntObj::operator<(const Obj& rhs)const {
	const type_info& obj_type = typeid(rhs);//获取rhs的type_info
	if (obj_type == typeid(IntObj)) { //IntObj < IntObj
		const IntObj& rrhs = dynamic_cast<const IntObj&>(rhs);
		if (data < rrhs.data) return true;
		return false;
	}
	else if (obj_type == typeid(StrObj)) { //IntObj < StrObj
		const StrObj& rrhs = dynamic_cast<const StrObj&>(rhs);
		string tmp = std::to_string(data);
		if (strcmp(tmp.c_str(), rrhs.c_str()) < 0) return true;
		return false;
	}
	else if (obj_type == typeid(StuObj)) { //IntObj < StuObj
		const StuObj& rrhs = dynamic_cast<const StuObj&>(rhs);
		string tmp1 = std::to_string(data);
		string tmp2 = std::to_string(rrhs.get_id());
		tmp2 += rrhs.get_name();
		if (tmp1 < tmp2) return true;
		return false;
	}
	else {
		cout << "Type error\n";
		throw "TypeError";
	}
}

bool StrObj::operator<(const Obj& rhs)const {
	const type_info& obj_type = typeid(rhs);//获取rhs的type_info
	if (obj_type == typeid(IntObj)) { //StrObj < IntObj
		const IntObj& rrhs = dynamic_cast<const IntObj&>(rhs);
		string tmp = std::to_string(rrhs.get_data());
		if (strcmp(data,tmp.c_str())<0) return true;
		return false;
	}
	else if (obj_type == typeid(StrObj)) { //StrObj < StrObj
		const StrObj& rrhs = dynamic_cast<const StrObj&>(rhs);
		if (strcmp(data, rrhs.c_str()) < 0) return true;
		return false;
	}
	else if (obj_type == typeid(StuObj)) { //StrObj < StuObj
		const StuObj& rrhs = dynamic_cast<const StuObj&>(rhs);
		string tmp = std::to_string(rrhs.get_id());
		tmp += rrhs.get_name();
		if (strcmp(data,tmp.c_str())<0) return true;
		return false;
	}
	else {
		cout << "Type error\n";
		throw "TypeError";
	}
}

bool StuObj::operator<(const Obj& rhs)const {
	const type_info& obj_type = typeid(rhs);//获取rhs的type_info
	if (obj_type == typeid(IntObj)) { //StuObj < IntObj
		const IntObj& rrhs = dynamic_cast<const IntObj&>(rhs);
		string tmp1 = std::to_string(id);
		tmp1 += name;
		string tmp2 = std::to_string(rrhs.get_data());
		if (tmp1<tmp2) return true;
		return false;
	}
	else if (obj_type == typeid(StrObj)) { //StuObj < StrObj
		const StrObj& rrhs = dynamic_cast<const StrObj&>(rhs);
		string tmp1 = std::to_string(id);
		tmp1 += name;
		if (strcmp(tmp1.c_str(), rrhs.c_str()) < 0) return true;
		return false;
	}
	else if (obj_type == typeid(StuObj)) { //StuObj < StuObj
		const StuObj& rrhs = dynamic_cast<const StuObj&>(rhs);
		string tmp1 = std::to_string(id);
		tmp1 += name;
		string tmp2 = std::to_string(rrhs.id);
		tmp2 += rrhs.name;
		if (tmp1<tmp2) return true;
		return false;
	}
	else {
		cout << "Type error\n";
		throw "TypeError";
	}
}

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
	List(const List &other) :List() {
		//虚clone 
		Node * p = other.head;
		while (p) { //遍历other链表
			push_back(p->p_data->clone()); 
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
	void sort() { //从大到小排序
		if (size <= 1) return;
		int i = 0, j = 0;
		Node *p = head, *q = nullptr;
		for (; i < size-1; ++i,p=p->next) {
			for (j=i+1,q=p->next; j<size; ++j,q=q->next){
				if (*p->p_data < *q->p_data) 
					std::swap(p->p_data, q->p_data);
			}
		}
	}
private:
	Node *head;
	Node *tail;
	int size;
};

int main() {
	List L;
	L.push_back(new IntObj(1));
	L.push_back(new IntObj(2));
	L.push_back(new StrObj("12a"));
	L.push_back(new IntObj(3));
	L.push_back(new StuObj(2001,"张三"));
	L.push_back(new IntObj(4));
	L.push_back(new StrObj("1001abc"));
	L.push_back(new StuObj(3002,"李四"));
	L.push_back(new IntObj(5));
	L.print_list();
	L.sort();
	L.print_list();
	return 0;
}