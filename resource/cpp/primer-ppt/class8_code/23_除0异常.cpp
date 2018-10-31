///除0
#include <iostream>
using namespace std;

int main() {
	int a, b;
	cout << "输入2个整数: ";
	while (cin >> a >> b) {
		try {
			if (b == 0) throw runtime_error("divisor is 0");
			cout << static_cast<double>(a) / b << endl;
			cout << "输入2个整数: ";
		}
		catch (runtime_error err) {
			cout << err.what();
			cout << "\nTry Again? Enter y or n:" << endl;
			char c;
			cin >> c;
			if (!cin || c == 'n' || c == 'N')
				break;
			else
				cout << "输入2个整数: ";
		}
	}
	
	return 0;
}