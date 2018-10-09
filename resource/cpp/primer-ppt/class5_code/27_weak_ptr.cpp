#include <iostream>
#include <memory>
using namespace std;

//weak_ptr用法(接口)
int main() {
	weak_ptr<int> wp1; //空指针
	shared_ptr<int> sp1 = make_shared<int>(100);
	cout << sp1.use_count() << endl; //1
 	wp1 = sp1; //不会增加 引用计数
	cout << sp1.use_count() << endl; //1

	weak_ptr<int> wp2(sp1); //用shared_ptr构造
	//use_count()是weak_ptr对应的shared_ptr的引用计数
	cout << wp2.use_count() << endl; //1

	//expired():若use_count()==0返回true,否则false
	if (!wp2.expired()) { //没有过期
		//lock():转为shared_ptr
		shared_ptr<int> sp2 = wp2.lock();
		if (sp2) cout << "转换成功！\n";
	}

	return 0;
}