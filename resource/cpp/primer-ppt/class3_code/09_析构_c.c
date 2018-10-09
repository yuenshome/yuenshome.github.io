///析构函数初步 c代码
struct Stu {
	char *name;
	int age;
};
int main() {
	//申请st1的内存
	struct Stu *st1 = 
	   (struct Stu*)malloc(sizeof(struct Stu));
	//申请st1中name的内存
	st1->name = (char*)malloc(sizeof(char)*20);
	//释放st1中name的内存
	free(st1->name);
	//释放st1本身的内存
	free(st1);
	return 0;
}