#include <iostream>
using namespace std;

template<typename T>
void f1(const T&) { cout << "const T&\n"; }
//f1函数模板的一个特例化, T 为 int,实现可以不同
template<> void f1(const int&) { cout << "const int &\n"; }
//特例化的本质是实例化一个模板,而不是重载。

//类模板A
template<typename T1,typename T2>
class A {
public:
	A(const T1& _t1,const T2& _t2):t1(_t1),t2(_t2){}
	inline void show()const;
private:
	T1 t1;
	T2 t2;
};
template<typename T1, typename T2>
inline void A<T1,T2>::show()const {
	cout << t1 << " " << t2 << endl;
}

//1.类模板A 的一个特例化,T1=int,T2=double,完全自己实现
template<>
class A<int, double> {
public:
	A(const int& _t1,const double& _t2):t1(_t1), t2(_t2) {}
	inline void show()const;
private:
	int t1;
	double t2;
};
//注意类外成员函数的写法(template)
inline void A<int, double>::show()const {
	cout << "特:" << t1 << " " << t2 << endl;
}
//2.类模板A 只特例化一个成员函数
template<>
inline void A<char, char>::show()const {
	cout << "特show:" << t1 << " " << t2 << endl;
}

//3.部分特例化
template <typename T1,typename T2> class B {};//类模板B
//类模板B的三个特例化
template<typename T> class B<T, T> {};
template<typename T> class B<T, int> {};
template<typename T1,typename T2> class B<T1*, T2*> {}; 

int main() {
	f1(1.2); //根据函数模板实例化 f1(const double&)
	f1(1);	 //使用特例化的 f1(const int&)
	
	A<double, char> a1(2.2, 'c'); //实例化
	a1.show(); // 2.2 c
	A<int, double> a2(1,1.2);     //使用特例化
	a2.show(); //特: 1 1.2
	A<char, char> a3('a', 'b');   //实例化,但是show函数用特例化的
	a3.show(); //特show: a b

	B<double, char> b1;    //使用 B<T1,T2>
	B<double, double> b2;  //使用 B<T,T>
	B<double, int> b3;     //使用 B<T,int>
	B<int*, double*> b4;   //使用 B<T1*,T2*>
	//B<int, int> b5; 二义性错 B<T,T>和B<T,int>同等程度匹配
	//B<int*, int*> b6; 二义性错 B<T,T>和B<T1*,T2*>同等程度匹配
	//int*,int* 可提供另外一个特例化 	template<typename T> class B<T*,T*>{};
	return 0;
}