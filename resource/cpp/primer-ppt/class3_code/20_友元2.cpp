#include <iostream>
using namespace std;
class Person {
public:
	friend void set_Person(Person& person, double m, int t);
	friend double calc_Person(const Person& person);
	void set_m_p_piece(int m) {
		if (m >= 10.0 && m <= 100.0)
			m_p_piece = m;
		else m_p_piece = 10.0;
	}
	double get_m_p_piece()const { return m_p_piece; }
	void set_t_piece(int t) {
		if (t <= 0) t_piece = 0;
		else t_piece = t;
	}
	int get_t_piece()const { return t_piece; }
private:
	double m_p_piece; //单件工资
	int t_piece;      //总件数
};

void set_Person(Person& person, double m, int t) {
	person.m_p_piece = m;
	person.t_piece = t;
}
double calc_Person(const Person& person) {
	return 	person.m_p_piece * person.t_piece;
}
int main() {
	Person person1;
	for (int i = 0; i < 100000; i++) {
		set_Person(person1, i, i);
		cout << calc_Person(person1) << endl;
	}
	return 0;
}