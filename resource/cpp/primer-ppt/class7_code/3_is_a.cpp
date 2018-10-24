#include <iostream>
#include <string>
using namespace std;
class Person {
public:
	void eat() { cout << "吃饭" << endl; }
	void sleep() { cout << "睡觉" << endl; }
};
class Teacher :public Person { //is-a
public:
	void teach() { cout << "教书" << endl; }
};
class Student :private Person { //not is-a
public:
	void study() { cout << "学习" << endl; }
};
int main() {
	//Teacher public继承自Person, Teacher is a Person
	Teacher t; //Teacher对象可以做Person的任何事情
	t.eat();
	t.sleep();
	Person *pt = &t; //is-a 关系 才能这样赋值
	Person p;  //Person对象不能做Teacher的事情
	//p.teach();//错,无法调用

	//Student private继承自Person,不存在is-a的关系
	Student s; //Student对象不能做Person的事情
	//s.eat();	//错,无法调用
	//s.sleep();//错,无法调用 
	//Person *ps = &s; //错，不是is-a关系不能这样赋值
	return 0;
}