#include <iostream>
#include <string>
using namespace std;
class Person {
public:
	Person(int _age,const string& _name)
		:age(_age),name(_name){}
	bool operator==(const Person& rhs)const;
	bool operator!=(const Person& rhs)const;
	bool operator<(const Person& rhs)const;
private:
	int age;
	string name;
};
bool Person::operator==(const Person& rhs)const {
	return age == rhs.age && name == rhs.name;
}
bool Person::operator!=(const Person& rhs)const {
	return !(*this == rhs);
}
bool Person::operator<(const Person& rhs)const {
	if (age < rhs.age) 
		return true;
	if (age == rhs.age && name < rhs.name) 
		return true;
	return false;
}

int main() {
	Person p1(20, "alex");
	Person p2 = p1;
	Person p3(20, "bob");
	cout << (p1 == p2) << endl; //1
	cout << (p1 != p3) << endl; //1
	cout << (p1 < p2) << endl;  //0
	cout << (p1 < p3) << endl;  //1
	return 0;
}