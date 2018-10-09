#include <iostream>
using namespace std;
class Person {
public:
	Person(int _age) :age(_age) { sum += _age; count++;	}
	static int get_count();
	static double get_avg() { return double(sum) / count; }
private:
	int age;         //年龄
	static int count;//总人数
public:
	//不能 static int sum=0;
	static int sum;  //年龄总和
};

//此处不能加 static
int Person::get_count() { return count; }
//此处不能加 static
int Person::sum = 0;   //必须初始化
int Person::count = 0; //数据区(bss,rw?)

int main(){
	//外部直接访问方式，类名::静态变量名
	cout << Person::sum << endl; 
	//cout << Person::count; //错误,同样有权限控制
	//静态成员变量不占用对象的内存空间
	cout << sizeof(Person) << endl; //4

	Person p1(10);
	//静态变量，也可以通过 对象.变量名 来访问
	//类外需要权限，类内部都行
	cout << p1.sum << endl;
	Person p2(20);

	//静态成员函数，可以通过 类名::函数名 来访问
	cout << Person::get_avg() << endl;
	//也可以通过 对象.函数名 来访问
	cout << p1.get_count() << endl; //2
	cout << p2.get_count() << endl; //与上面输出一致
	return 0;
}