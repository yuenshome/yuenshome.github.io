///自定义的hash函数
///
#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

class Position {
public:
	Position(int x=0,int y=0):pos_x(x),pos_y(y){}
	int get_x()const { return pos_x; }
	int get_y()const { return pos_y; }
private:
	int pos_x;
	int pos_y;
};
//作为key的Position必须要实现 == 运算符
bool operator==(const Position& lhs, const Position& rhs) {
	return (lhs.get_x() == rhs.get_x()) && (lhs.get_y() == rhs.get_y());
}

class Person {
public:
	Person(const string& n="",int a=0):name(n),age(a){}
private:
	string name;
	int age;
};

//创建unordered_map: key: Positoin --> value: Person

template<typename T> //boost中的一种写法
inline void hash_combine(std::size_t &seed, const T& v) {
	seed ^= std::hash<T>()(v) + 0x9e3779b9 +(seed<<6) +(seed>>2);
}
//方法1：自己定义一个hash
class MyHash {
public:
	std::size_t operator()(const Position& p)const {
		//return p.get_x() + p.get_y();
		//return hash<int>()(p.get_x()) ^ hash<int>()(p.get_y());
		std::size_t hash_result = hash<int>()(p.get_x());
		hash_combine(hash_result, p.get_y());
		return hash_result;
	}
};

//方法2：模板特例化
namespace std {
	template<> struct hash<Position> {
		std::size_t operator()(const Position& p)const {
			std::size_t hash_result = hash<int>()(p.get_x());
			hash_combine(hash_result, p.get_y());
			return hash_result;
		}
	};
}


int main() {
	//方法1: 使用自己构建的 MyHash
	unordered_map<Position, Person, MyHash> g1;

	//方法2: 使用hash<Position>, hash<T>的模板特例化 
	unordered_map<Position, Person> g2 = { {Position(10,20),Person("张三",20)} };

}