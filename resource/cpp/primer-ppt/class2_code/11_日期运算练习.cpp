#include <iostream>
using namespace std;
struct Date {
	int y;	//Äê
	int m;	//ÔÂ
	int d;	//ÈÕ
};
int tab[2][12] = {
	{ 31,28,31,30,31,30,31,31,30,31,30,31 },
	{ 31,29,31,30,31,31,30,31,30,31,30,31 } };

int d2n(const Date &date);
Date n2d(int days);
bool is_leap(int year);
bool is_leap(const Date &date);
Date operator+(Date d1, int days);
Date operator+(int days, Date d1);
Date operator+(const Date &d1, const Date &d2);
void print(const Date &date);
void print(int days);
int main() {
	Date d1 = { 2019,10,10 };
	Date d2 = { 2,1,10 };
	cout << is_leap(d1) << endl;   //0
	cout << is_leap(2000) << endl; //1
	Date d3 = d1 + d2;
	Date d4 = operator+(d2, d1);
	print(d3); //2020-10-19
	print(d4); //2020-10-19
	d3 = d1 + 2;
	print(d3); //2019-10-12
	d3 = d1 + 200 + d2 + Date{ 1,1,1 };
	print(d3); //2021-5-8
	print(Date{ 2000,1,2 });		//2000-1-1
	print(n2d(730120));				//2000-1-1
	print(730120);					//2000-1-1
	print(Date{ 1999, 1, 1 } +365);//2000-1-1
	return 0;
}

int d2n(const Date &date) {
	int days = 0;
	for (int y = 1; y < date.y; y++)
		days = is_leap(y) ?
		days + 366 : days + 365;
	for (int m = 1; m < date.m; m++)
		days = is_leap(date.y) ? days +
		tab[1][m - 1] : days + tab[0][m - 1];
	days += date.d;
	return days;
}

Date n2d(int days) {
	int y = 1, m = 1;
	while (1) {
		if (is_leap(y)) {
			if (days > 366) days -= 366;
			else break;
		}
		else {
			if (days > 365) days -= 365;
			else break;
		}
		y++;
	}
	while (1) {
		if (is_leap(y)) {
			if (days > tab[1][m - 1])
				days -= tab[1][m - 1];
			else break;
		}
		else {
			if (days > tab[0][m - 1])
				days -= tab[0][m - 1];
			else break;
		}
		m++;
	}
	return Date{ y,m,days };
}

bool is_leap(int year) {
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
		return true;
	return false;
}
bool is_leap(const Date &date) {
	return is_leap(date.y);
}
Date operator+(Date d1, int days) {
	return n2d(d2n(d1) + days);
}
Date operator+(int days, Date d1) {
	return operator+(d1, days);
}
Date operator+(const Date &d1, const Date &d2) {
	return n2d(d2n(d1) + d2n(d2));
}
void print(const Date &date) {
	cout << "(" << date.y << "-" << date.m
		<< "-" << date.d << ")" << endl;
}
void print(int days) {
	print(n2d(days));
}