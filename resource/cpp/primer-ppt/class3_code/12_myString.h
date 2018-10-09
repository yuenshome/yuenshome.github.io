///myString.h
#ifndef MYSTRING_H
#define MYSTRING_H
#include <iostream>
class myString {
public:
	myString(const char *str = NULL);
	~myString();
	const char* c_str()const;
private:
	char *ps;
};
#endif // !MYSTRING_H