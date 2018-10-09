#include <iostream>
using namespace std;
typedef int Type; //using Type = int;
Type** create(int row, int col);
void destroy(Type** p, int row);
int main() {
	int row, col;
	cin >> row >> col;
	Type ** arr = create(row, col);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			arr[i][j] = i * 10 + j + 1;
			cout << arr[i][j] << "\t";
		}
		cout << endl;
	}
	destroy(arr, row);
	return 0;
}


Type** create(int row, int col) {
	if (row <= 0 || col <= 0) return NULL;
	Type **p = new Type*[row];
	for (int i = 0; i < row; i++)
		p[i] = new Type[col];
	return p;
}
void destroy(Type** p, int row) {
	if (p == NULL || row <= 0) return;
	for (int i = 0; i < row; i++)
		delete[] p[i];
	delete[] p;
}