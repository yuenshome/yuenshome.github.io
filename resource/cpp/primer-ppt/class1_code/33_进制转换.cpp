#include <iostream>
#include <vector>
#include <string>
using namespace std;
char* num2dec(int num, char* res, int length);
string num2hex(int num);

int main() {
	char res[12];
	int test = 8267100;
	cout << "10进制：" << 
		num2dec(test, res, sizeof(res)) << endl;
	string res2;
	res2 = num2hex(test);
	cout << "16进制：" << res2 << endl;
	return 0;
}

char* num2dec(int num, char* res, int length) {
	if (num == 0) {
		res[0] = '0'; res[1] = '\0'; return res;
	}
	memset(res, 0, length);
	int idx = 0;
	while (num) {
		char c = num % 10 + '0';
		res[idx++] = c;
		num /= 10;
	}
	//0 和 idx-1 的位置 对换（数组前后交换）
	for (int i = 0; i <= (idx - 1) / 2; i++) {
		char tmp = res[i];
		res[i] = res[idx - 1 - i];
		res[idx - 1 - i] = tmp;
	}
	return res;
}

string num2hex(int num) {
	if (num == 0) return "0";
	vector<char> vec;

	while (num) {
		int yu = num % 16;
		vec.push_back("0123456789ABCDEF"[yu]);
		num /= 16;
	}
	string tmp;
	for (int i = vec.size() - 1; i >= 0; i--) 
		tmp.push_back(vec[i]);
	return tmp;
}