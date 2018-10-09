#include <stdio.h>
#include <string.h>
#include <malloc.h>
struct Stu {	//单个学生结构体
	int id;
	char name[20];
};
struct Stu_arr {//学生数组结构体
	struct Stu *pstu;
	int size;
};
void init(struct Stu_arr *p_ss);
void destroy(struct Stu_arr *p_ss);
void push_back(struct Stu_arr *p_ss, struct Stu *p_stu);
void print(struct Stu_arr *p_ss);
int main() {
	struct Stu_arr ss;	
	init(&ss);	//初始化 
	struct Stu stu1;
	stu1.id = 1; strcpy(stu1.name, "张三");
	push_back(&ss, &stu1);
	print(&ss);
	stu1.id = 5; strcpy(stu1.name, "mike");
	push_back(&ss, &stu1);
	print(&ss);
	destroy(&ss); //清理内存
	return 0;
}

void init(struct Stu_arr *p_ss) {
	p_ss->size = 0;
	p_ss->pstu = NULL;
}
void destroy(struct Stu_arr *p_ss) {
	if (p_ss->pstu)
		free(p_ss->pstu);
}
void push_back(struct Stu_arr *p_ss, struct Stu *p_stu) {
	struct Stu* p_new = (struct Stu*)malloc((p_ss->size + 1)
									* sizeof(struct Stu));
	memcpy(p_new, p_ss->pstu, p_ss->size * sizeof(struct Stu));
	memcpy(p_new + p_ss->size, p_stu, sizeof(struct Stu));
	free(p_ss->pstu);
	p_ss->pstu = p_new;
	p_ss->size++;
}
void print(struct Stu_arr *p_ss) {
	for (int i = 0; i < p_ss->size; i++) {
		printf("(%d,%s)\t", (p_ss->pstu)[i].id, (p_ss->pstu)[i].name);
	}
	printf("\n");
}