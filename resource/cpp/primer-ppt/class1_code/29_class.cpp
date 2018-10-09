#include <iostream>
using namespace std;
class player {
public:
	player(int level=0,int hp=0)
		:level(level),hp(hp){}
	void train(int nums) {
		int killnums = hp > nums ? nums : hp;
		level += killnums;	hp -= killnums;
		cout << "练级：长了 " << killnums << " 级。";
		cout << "当前：level=" << level << ",hp=" << hp << endl;
	}
	void pk(player &another) {
		int power1 = level * 100 + hp;
		int power2 = another.level * 100 + another.hp;
		if (power1 >= power2)
			printf("You win!\n");
		else
			printf("You loss!\n");
	}
private:
	int level;	//等级
	int hp;		//hp值
};

int main() {
	player p1(1, 100); player p2(2, 50);
	p1.train(6);
	p2.train(10);
	p1.pk(p2);
	return 0;
}