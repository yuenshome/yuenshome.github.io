#include <iostream>
using namespace std;
class A {
public:
	A(int i = 0) :ci(i),num(i) {  }
	int get_ci()const {
		return ci;
	}
	void show()const {
		//set_num(0); //错误，调用非const函数
		//num = 0; //错误，修改数据成员
		cout << num*ci << endl; //正确,调用不修改
		get_ci(); //正确，调用const函数
		cout << "show() const" << endl;
	}
	void show() { //重载
		cout << "show()" << endl;
	}
	int set_num(int i) { num = i; }
private:
	const int ci;
	int num;
};

int main(){
	A a; //普通对象
	const A ca; //const对象
	//ca.set_num(100); //错误，const对象只能调用const函数
	ca.show(); // show() const
	a.show(); //show() 非const [非const对象也可调用const函数]
	return 0;
}