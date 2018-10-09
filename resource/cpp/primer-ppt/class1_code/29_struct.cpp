#include <stdio.h>
typedef void(*Train)(struct player*, int);
typedef void(*Pk)(struct player*, struct player*);
struct player {
	int level;		//等级
	int hp;			//hp值
	Train f_train;	//函数指针(练级)
	Pk f_pk;		//函数指针(PK)
};

void train_fun(struct player *p1, int nums) {
	int killnums = p1->hp > nums ? nums : p1->hp;
	p1->level += killnums;	p1->hp -= killnums;
	printf("练级：长了 %d 级。", killnums);
	printf("当前：level=%d,hp=%d\n", p1->level, p1->hp);
}

void pk_fun(struct player* p1, struct player* p2) {
	int power1 = p1->level * 100 + p1->hp;
	int power2 = p2->level * 100 + p2->hp;
	if (power1 >= power2)
		printf("player1 win\n");
	else
		printf("player2 win\n");
}

int main() {
	struct player p1 =
	{ .level = 1,.hp = 100,.f_train = train_fun,.f_pk = pk_fun };
	struct player p2 =
	{ .level = 2,.hp = 50,.f_train = train_fun,.f_pk = pk_fun };
	p1.f_train(&p1, 6);
	p2.f_train(&p2, 10);
	p1.f_pk(&p1, &p2);
	return 0;
}