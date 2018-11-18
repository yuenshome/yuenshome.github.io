#include <iostream>
#include <string>

template<typename T>
struct remove_reference {
	typedef T type; 
};

template<typename T>
struct remove_reference<T&> {
	typedef T type;
};

template<class T>
struct remove_reference<T&&> {
	typedef T type;
};

template<typename T>
inline typename remove_reference<T>::type&& move(T&& t) {
	return (static_cast<typename remove_reference<_Ty>::type&&>(t));
}

// move(string("abc")); //右值
//1.推断出 T 的类型是 string
//2.remove_reference<string> 实例化(第1个)
//3.remove_reference<string>的type成员是 string
//4.move的返回类型是 string&&
//5.move函数的参数 t 的类型是 string&&
//此调用实例化 move<string>: 就是 string&& move(string&& t);

// string s1="abc"; move(s1); //左值
//1.推断出 T 的类型是 string&
//2.remove_reference<string&> 实例化(第2个)
//3.remove_reference<string&>的type成员是 string
//4.move的返回类型是 string&&
//5.move函数的参数 t 的类型是 string& &&,折叠为 string&
//此调用实例化 move<string&>: 就是 string&& move(string& t);

int main() {
	remove_reference<int>::type i1;
	remove_reference<int&>::type i2;
	remove_reference<int&&>::type i3;
	//上面三种类型都是 int (相当于去掉了引用)
	return 0;
}