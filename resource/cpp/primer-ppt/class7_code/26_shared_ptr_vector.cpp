#include <iostream>
#include <memory>
#include <vector>
using namespace std;

class Base {
public:
	virtual void show()const = 0;
protected:
	virtual ~Base() =default;
//不能是private的析构,否则D1无法析构
};
class D1 : public Base {
public:
	D1(int i) :data(i) { }
	virtual void show()const override{
		cout << data << " ";
	}
private:
	int data;
};
shared_ptr<Base> createBase(int data) {
	shared_ptr<Base> p = make_shared<D1>(data);
	return p;
}

int main() {
	vector<shared_ptr<Base>> vec;
	for (int i = 0; i<10; ++i) 
		vec.push_back(createBase(i));
	
	vector<shared_ptr<Base>>::iterator end = vec.end();
	for (auto it = vec.begin(); it != end; ++it) 
		(*it)->show();
	cout << endl;

	Base* p = vec[0].get();
	//delete p; //错误,防止delete 裸指针
	//通过get获得的裸指针不能delete,因为Base的析构是protected
	//只有shared_ptr可以，unique_ptr不行。
}