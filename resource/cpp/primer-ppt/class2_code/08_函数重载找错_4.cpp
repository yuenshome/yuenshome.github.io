//(4):char 向 int 的提升.
void f(int a) { cout << 1; }
void f(short a) { cout << 2; }
int main() {
	f('a'); //输出是多少？
	return 0; 
}