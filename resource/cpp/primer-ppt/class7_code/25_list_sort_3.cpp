///模拟虚函数表
#include <iostream>
#include <cstring>
#include <string>
#include <cassert>
#include <typeinfo>
#include <vector>
using namespace std;

class IntObj;
class StrObj;
class StuObj;
class Obj { //基类
public:
	virtual ~Obj() = default;
	virtual void print()const = 0;
	virtual Obj *clone()const = 0; //虚clone
};
class IntObj :public Obj { //派生类:int数据 
public:
	IntObj(int i = 0) :data(i) { }
	~IntObj() {}
	virtual void print()const override {
		cout << data << " -> ";
	}
	virtual IntObj *clone()const override {
		return new IntObj(*this);
		//会调用本类的拷贝构造
	}
	int get_data()const { return data; }
private:
	int data;
};
class StrObj :public Obj { //派生类:str数据
public:
	StrObj(const char *str = nullptr) {
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
		cout << "\"" << data << "\" -> ";
	}
	const char *c_str()const { return data; }
private:
	char *data;
};
class StuObj :public Obj { //派生类:学生数据
public:
	StuObj(int _id, const string &_name)
		:id(_id), name(_name) {
	}
	~StuObj() {}
	virtual void print()const override {
		cout << id << ":" << name << " -> ";
	}
	virtual StuObj *clone()const override {
		return new StuObj(*this);
		//会调用本类的拷贝构造
	}
	int get_id()const { return id; }
	const string& get_name()const { return name; }
private:
	int id;
	string name;
};

bool Int_Int(const Obj &lhs, const Obj &rhs) {
	const IntObj &llhs = dynamic_cast<const IntObj&>(lhs);
	const IntObj &rrhs = dynamic_cast<const IntObj&>(rhs);
	if (llhs.get_data() < rrhs.get_data()) return true;
	return false;
}
bool Int_Str(const Obj &lhs, const Obj &rhs) {
	const IntObj &llhs = dynamic_cast<const IntObj&>(lhs);
	const StrObj &rrhs = dynamic_cast<const StrObj&>(rhs);
	string tmp = std::to_string(llhs.get_data());
	if (strcmp(tmp.c_str(), rrhs.c_str()) < 0) return true;
	return false;
}
bool Int_Stu(const Obj &lhs, const Obj &rhs) {
	const IntObj &llhs = dynamic_cast<const IntObj&>(lhs);
	const StuObj &rrhs = dynamic_cast<const StuObj&>(rhs);
	string tmp1 = std::to_string(llhs.get_data());
	string tmp2 = std::to_string(rrhs.get_id());
	tmp2 += rrhs.get_name();
	if (tmp1 < tmp2) return true;
	return false;
}
bool Str_Int(const Obj &lhs, const Obj &rhs) {
	//直接调用 Int_Str
	return !(Int_Str(rhs, lhs));
}
bool Str_Str(const Obj &lhs, const Obj &rhs) {
	const StrObj &llhs = dynamic_cast<const StrObj&>(lhs);
	const StrObj &rrhs = dynamic_cast<const StrObj&>(rhs);
	if (strcmp(llhs.c_str(), rrhs.c_str()) < 0) return true;
	return false;
}
bool Str_Stu(const Obj &lhs, const Obj &rhs) {
	const StrObj &llhs = dynamic_cast<const StrObj&>(lhs);
	const StuObj &rrhs = dynamic_cast<const StuObj&>(rhs);
	string tmp = std::to_string(rrhs.get_id());
	tmp += rrhs.get_name();
	if (strcmp(llhs.c_str(), tmp.c_str())<0) return true;
	return false;
}
bool Stu_Int(const Obj &lhs, const Obj &rhs) {
	//直接调用 Int_Stu
	return !(Int_Stu(rhs, lhs));
}
bool Stu_Str(const Obj &lhs, const Obj &rhs) {
	//直接调用 Str_Stu
	return !(Str_Stu(rhs, lhs));
}
bool Stu_Stu(const Obj &lhs, const Obj &rhs) {
	const StuObj &llhs = dynamic_cast<const StuObj&>(lhs);
	const StuObj &rrhs = dynamic_cast<const StuObj&>(rhs);
	string tmp1 = std::to_string(llhs.get_id());
	tmp1 += llhs.get_name();
	string tmp2 = std::to_string(rrhs.get_id());
	tmp2 += rrhs.get_name();
	if (tmp1<tmp2) return true;
	return false;
}
typedef bool(*LESS_FUNC_PTR)(const Obj &, const Obj &);
struct LessVecNode {
	LessVecNode() {}
	LessVecNode(const char* s1, const char* s2, LESS_FUNC_PTR p) :
		obj1(s1), obj2(s2), ptr(p) {}
	string obj1 = "";
	string obj2 = "";
	LESS_FUNC_PTR ptr = nullptr;
};
vector<LessVecNode> init_vec() { //初始化 函数表
	vector<LessVecNode> vec;
	vec.push_back(LessVecNode("class IntObj", "class IntObj", Int_Int));
	vec.push_back(LessVecNode("class IntObj", "class StrObj", Int_Str));
	vec.push_back(LessVecNode("class IntObj", "class StuObj", Int_Stu));
	vec.push_back(LessVecNode("class StrObj", "class IntObj", Str_Int));
	vec.push_back(LessVecNode("class StrObj", "class StrObj", Str_Str));
	vec.push_back(LessVecNode("class StrObj", "class StuObj", Str_Stu));
	vec.push_back(LessVecNode("class StuObj", "class IntObj", Stu_Int));
	vec.push_back(LessVecNode("class StuObj", "class StrObj", Stu_Str));
	vec.push_back(LessVecNode("class StuObj", "class StuObj", Stu_Stu));
	return vec;
}
LESS_FUNC_PTR lookup(const Obj &lhs, const Obj &rhs) {//查找 函数表
	static vector<LessVecNode> func_vec = init_vec();
	//可以用 map 来替代vector,效率高
	vector<LessVecNode>::iterator end = func_vec.end();
	vector<LessVecNode>::iterator it = func_vec.begin();
	for (; it != end; ++it) {
		if ((*it).obj1 == typeid(lhs).name() && (*it).obj2 == typeid(rhs).name())
			break;
	}
	if (it != func_vec.end())
		return (*it).ptr;
	return nullptr;
}
bool operator<(const Obj &lhs, const Obj &rhs) {
	LESS_FUNC_PTR f = lookup(lhs, rhs);
	if (f) return f(lhs, rhs);
	throw "No Match Type!";
}

class List;
class Node {
	friend List;
public:
	Node(Obj *p_obj = nullptr)
		:p_data(p_obj), next(nullptr) {}
	~Node() { delete p_data; }
private:
	Obj *p_data;
	Node *next;
};

class List {
public:
	List() :head(nullptr), tail(nullptr), size(0) {}
	~List() {
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
		for (; i < size - 1; ++i, p = p->next) {
			for (j = i + 1, q = p->next; j<size; ++j, q = q->next) {
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
	L.push_back(new StuObj(2001, "张三"));
	L.push_back(new IntObj(4));
	L.push_back(new StrObj("1001abc"));
	L.push_back(new StuObj(3002, "李四"));
	L.push_back(new IntObj(5));
	L.print_list();
	L.sort();
	L.print_list();
	return 0;
}