#include <iostream>
using namespace std;
void f1(int a, int b = 0); //第一次声明
//void f1(int a, int b = 0); //错误
void f1(int a, int b); //OK
void f1(int a = 0, int b); //OK

//void f1(int a, int b = 0) {} //错误
void f1(int a, int b) {} //OK