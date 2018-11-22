///自己封装的简易MyFstream
#include <cstdio>
class MyFstream {
public:
	MyFstream(const char* path, const char* mode) { //构造，打开文件
		fp = fopen(path, mode);
		if (!fp) throw "Open File error!";
	}
	~MyFstream() { 	if (fp) fclose(fp);	} //析构，关闭文件
	MyFstream(const MyFstream&) = delete; //不能拷贝
	MyFstream& operator=(const MyFstream&) = delete; //不能赋值
	int get() {	return fgetc(fp); } //读1个字符 fp --> char
	void put(int ch) { fputc(ch, fp); } //写1个字符 char --> fp
	char* getline(char* buf, int n) { return fgets(buf, n, fp);	} //读一行fp->char*
	void seek(int offset, int where) { 	fseek(fp, offset, where); } //移动文件指针
	MyFstream & operator>> (int &val) { //从fp读一个int, fp --> int
		fscanf(fp, "%d", &val); return *this;
	}
	MyFstream & operator>> (char* val) { //从fp读1个字符串,fp --> char*
		fscanf(fp, "%s", val); return *this;
	}
	MyFstream & operator<<(const int& val) { //写一个int到文件 int --> fp
		fprintf(fp, "%d", val); return *this;
	}
	MyFstream &operator<<(const char* val) { //写一个字符串到文件 const char* --> fp
		fprintf(fp, "%s", val);  return *this;
	}
private:
	FILE * fp;
};

int main() {
	MyFstream fs("1.txt", "w+");
	
	//写入三个整数 (像 cout << 123 类似)
	fs << 123 << " " << 234 << " " << 345 << "\n";
	//写入字符串
	fs << "I like C++\n";
	fs << "We all like coding...\n";

	int a1=0, a2=0, a3=0;
	//读整数到 a1,a2,a3 (像 cin >> a1 类似)
	fs.seek(0, SEEK_SET);
	fs >> a1 >> a2 >> a3;
	printf("%d %d %d\n", a1, a2, a3); //打印输出

	char buf1[100],buf2[100],buf3[100];
	//读字符串到 buf1 (像 cin >> buf1 类似)
	fs >> buf1 >> buf2 >> buf3;
	printf("%s %s %s\n", buf1,buf2,buf3);
	//上面的结果，和cin读字符串一样,遇到空格截断

	fs.get();
	//用getline读字符串到 buf1
	fs.getline(buf1, sizeof(buf1));
	printf("%s", buf1);

	return 0;
}