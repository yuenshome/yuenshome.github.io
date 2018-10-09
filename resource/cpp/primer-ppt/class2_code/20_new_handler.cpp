#include <iostream>
using namespace std;
int * p1, *p2;
void my_new_heandler() {
	cout << "haha" << endl;
	delete[] p1;
	p1 = NULL;
}
int main() {
	set_new_handler(my_new_heandler);
	p1 = new int[336870912];
	cout << "p1 ok" << endl;
	p2 = new int[336870912];
	cout << "p2 ok" << endl;
	getchar(); //µÈ´ý¹Û²ì
	delete[] p1;
	delete[] p2;
	return 0;
}