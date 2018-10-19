#include <iostream>
#include <string>
using namespace std;
class Person {
public:
	Person(int _age,const string& _name)
		:age(_age),name(_name){}
	virtual void show()const{
		cout << "我是" << name << "," 
			<< age << "岁。" << endl;
	}
protected:
	int age;
	string name;
};
class Student:public Person {
public:
	Student(int _age,const string& _name,const string& _course)
		:Person(_age,_name),stu_course(_course){}
	void show()const {
		cout << "我是" << name << "," << age << 
			"岁。我在学: " << stu_course << endl;
	}
private:
	string stu_course;
};
class Teacher :public Person {
public:
	Teacher(int _age,const string& _name,const string& _course)
		:Person(_age, _name), tea_course(_course) {}
	void show()const {
		cout << "我是" << name << "," << age << 
			"岁。我在教: " << tea_course << endl;
	}
private:
	string tea_course;
};

void show(const Person& pp) {
	pp.show();
}
int main() {
	Student stu1(22, "张三", "C++");
	show(stu1);
	Teacher tea1(32, "李四", "C语言");
	show(tea1);
	return 0;
}