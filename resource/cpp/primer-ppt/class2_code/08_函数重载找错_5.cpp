//(5)nullptr
void f(int a, char* p = NULL) { cout << 1; }
void f(int a, int b = 0) { cout << 2; }
int main() {
	f('a'); //正确吗？
	f('a', 0); //输出多少？
	f(10.2, NULL); //输出多少？
	f(1, nullptr); //输出多少？
	return 0; 
}