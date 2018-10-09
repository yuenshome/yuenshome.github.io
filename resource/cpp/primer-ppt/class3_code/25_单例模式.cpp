#include <iostream>
using namespace std;
class A {
private:
	A() {}; //构造和析构私有
	~A() {};
public:
	static A * GetInstance() {//公有静态方法，可以获取该唯一实例
		if (NULL == m_pInstance) m_pInstance = new A; //(多线程需要加锁)
		return m_pInstance;
	}
	static void DeleteInstance() {//公有静态方法，可以删除该实例
		if (m_pInstance != NULL)
			delete m_pInstance;
		m_pInstance = NULL;
	}
private:
	static A *m_pInstance; //私有静态指针变量：指向类的唯一实例
	int count; //其他成员变量...
};

A *A::m_pInstance = NULL;    //懒汉式
//A *A::m_pInstance = new A; //饿汉式

int main() {
	//A a; //错误，外部无法创建该类对象
	A *pa = A::GetInstance(); //通过调用 类静态成员函数 来获取类对象
	A *pb = A::GetInstance(); //可多次调用
	cout << pa << pb << endl; //地址相同，pa,pb指向的是同一个对象
	//delete pa; //错误，外部无法直接销毁该类对象
	A::DeleteInstance(); //通过调用 类静态成员函数 来析构类对象
	return 0;
}