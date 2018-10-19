#include <iostream>
using namespace std;
class Base {
public:
	Base(int _b) :b(_b), pb(&b) { }
	Base(const Base& other)
		:b(other.b),pb(other.pb) {
		cout << "Base¿½±´¹¹Ôì" << endl;
	}
	Base& operator=(const Base &other) {
		if (this != &other) {
			b = other.b;
			pb = other.pb;
		}
		cout << "Base¸³ÖµÔËËã·ûÖØÔØ" << endl;
		return *this;
	}
	int b;
	int *pb;
};
class D1 :public Base {
public:
	D1(int _b, int _d) :Base(_b), d(_d), pd(&d) { }
	int d;
	int *pd;
};
int main() {
	D1 d1(1, 2);
	D1 d2(d1); //¿½±´¹¹Ôì  d1 --> d2
	cout << d2.b << " " << d2.d << endl; // 1 2
	cout << d1.pb << " " << d1.pd << endl;
	cout << d2.pb << " " << d2.pd << endl; //Ç³¿½±´ 
	D1 d3(0, 0);
	d3 = d2;   //¸³ÖµÔËËã·û d2 --> d3
	cout << d3.b << " " << d3.d << endl; // 1 2
	cout << d2.pb << " " << d2.pd << endl;
	cout << d3.pb << " " << d3.pd << endl; //Ç³¿½±´ 
	return 0;
}