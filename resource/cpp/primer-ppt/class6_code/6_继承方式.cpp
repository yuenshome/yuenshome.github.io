#include <iostream>
using namespace std;
class Base {
public:
	void f_pub(){}
	int i_pub;
protected:
	void f_pro(){}
	int i_pro;
private:
	void f_pri() {}
	int i_pri;
};

class D1 :public Base { //public继承
public:
	void test() {
		//测试类内访问
		f_pub();   //访问基类public成员函数,ok
		i_pub = 10;//访问基类public数据成员,ok
		f_pro();   //访问基类protected成员函数,ok
		i_pro = 20;//访问基类protected数据成员,ok
		//f_pri();   访问基类private成员函数,error
		//i_pri = 30;访问记录private数据成员,error
	}
};

class D2 :protected Base {  //protected继承
public:
	void test() {
		//测试类内访问
		f_pub();   //访问基类public成员函数,ok
		i_pub = 10;//访问基类public数据成员,ok
		f_pro();   //访问基类protected成员函数,ok
		i_pro = 20;//访问基类protected数据成员,ok
		//f_pri();   访问基类private成员函数,error
		//i_pri = 30;访问记录private数据成员,error
	}
};

class D3 :private Base {  //private继承
public:
	void test() {
		//测试类内访问
		f_pub();   //访问基类public成员函数,ok
		i_pub = 10;//访问基类public数据成员,ok
		f_pro();   //访问基类protected成员函数,ok
		i_pro = 20;//访问基类protected数据成员,ok
		//f_pri();   访问基类private成员函数,error
		//i_pri = 30;访问记录private数据成员,error
	}
};
int main() {
	//测试类外访问
	D1 d1; //D1 public 继承
	d1.f_pub();    //访问基类public成员函数,ok
	d1.i_pub = 10; //访问基类public数据成员,ok
	//d1.f_pro();    访问基类protected成员函数,error
	//d1.i_pro = 20; 访问基类protected数据成员,error
	//d1.f_pri();    访问基类private成员函数,error
	//d1.i_pri = 30; 访问记录private数据成员,error

	D2 d2; //D2 protected 继承
	//d2.f_pub();    访问基类public成员函数,error
	//d2.i_pub = 10; 访问基类public数据成员,error
	//d2.f_pro();    访问基类protected成员函数,error
	//d2.i_pro = 20; 访问基类protected数据成员,error
	//d2.f_pri();    访问基类private成员函数,error
	//d2.i_pri = 30; 访问记录private数据成员,error

	D3 d3; //D3 private 继承
	//d3.f_pub();    访问基类public成员函数,error
	//d3.i_pub = 10; 访问基类public数据成员,error
	//d3.f_pro();    访问基类protected成员函数,error
	//d3.i_pro = 20; 访问基类protected数据成员,error
	//d3.f_pri();    访问基类private成员函数,error
	//d3.i_pri = 30; 访问记录private数据成员,error
	return 0;
}