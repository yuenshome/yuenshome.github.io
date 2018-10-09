///循环引用
#include <iostream>
#include <memory>
using namespace std;
class Parent;
typedef std::shared_ptr<Parent> ParentPtr;
class Child;
typedef std::shared_ptr<Child> ChildPtr;
class Parent {
public:
	Parent() { cout << "Parent构造\n"; }
	~Parent() { cout << "Parent析构\n"; }
	ChildPtr child;
};
class Child {
public:
	Child() { cout << "Child构造\n"; }
	~Child() { cout << "Child析构\n"; }
	ParentPtr parent;
};
int main() {
	ParentPtr p = make_shared<Parent>();
	ChildPtr c = make_shared<Child>();
	p->child = c;
	c->parent = p;
	//观察运行结果。。。资源没有被释放
	cout<<"parent: "<<p.use_count()<<endl;//2
	cout<<"child: "<<c.use_count()<<endl; //2
	return 0;
}