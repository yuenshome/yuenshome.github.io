#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//不同的动物：不同的叫声,不同的行动方式
typedef void(*SING)();
typedef void(*MOVE)();
struct vtbl { //模拟虚函数表
	SING sing; //函数指针
	MOVE move;
};

//基类：Animal
struct Animal {
	//指向虚表的指针
	const struct vtbl *ptr_vtbl;
	//动物的名称：(公有属性)
	char name[16];
};

//派生类：Dog
struct Dog {
	struct Animal base;
	int x; //Dog私有的属性
};
//Dog的sing和move函数
void dog_sing() { printf("Dog:汪汪汪\n"); }
void dog_move() { printf("Dog:地面狂奔\n"); }
//Dog类的虚函数表vtbl
const struct vtbl dog_vtbl={dog_sing,dog_move};
//创建Dog对象
struct Animal *create_dog(const char* name) {
	assert(name);
	struct Dog *p_dog = 
		(struct Dog*)malloc(sizeof(struct Dog));
	assert(p_dog); 
	p_dog->base.ptr_vtbl = &dog_vtbl; 
	strcpy(p_dog->base.name, name);
	p_dog->x = 10;
	//上面：将Dog对象的虚函数表绑定到 虚表指针
	return (struct Animal*)p_dog;
}

//派生类：Maque (麻雀)
struct Maque {
	struct Animal base;
	int y; //Maque的私有属性
};
//Maque的sing和move函数
void maque_sing() { printf("Maque:唧唧唧\n"); }
void maque_move() { printf("Maque:低空任我飞\n"); }
//Maque的虚函数表vtbl
const struct vtbl maque_vtbl={maque_sing,maque_move};
//创建Maque对象
struct Animal *create_maque(const char* name) {
	assert(name);
	struct Maque *p_maque =
		(struct Maque*)malloc(sizeof(struct Maque));
	assert(p_maque); 
	p_maque->base.ptr_vtbl = &maque_vtbl;
	strcpy(p_maque->base.name, name);
	p_maque->y = 20;
	return (struct Animal*)p_maque;
}

int main() {
	struct Animal *p = NULL;
	//基类指针 指向派生类对象
	p = create_dog("小狗");
	p->ptr_vtbl->sing();
	p->ptr_vtbl->move();
	free(p); printf("-------\n");
	p = create_maque("麻雀");
	p->ptr_vtbl->sing();
	p->ptr_vtbl->move();
	free(p);
	return 0;
}