///fstream
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
	fstream fs1("1.txt", fstream::in | fstream::out | fstream::app);
	//读写,追加,每次写数据都写在文件的最后面

	//判断文件打开是否正常
	if (!fs1) { cout << "打开文件失败!\n"; return -1; }
	if (fs1) { cout << "文件打开ok\n"; }

	cout << "fs1文件是否打开: "<<fs1.is_open() << endl;
	fs1.close(); //显式关闭,假如不显式关闭,fs1对象在析构时会自动关闭文件
	
	fstream fs2;
	fs2.open("2.txt", fstream::in|fstream::out|fstream::trunc);//open打开文件
	//读写，截断(文件存在先清空,文件不存在创建)
	if (!fs2) { cout << "open error\n"; return - 1; }

	//用 operator<< 输出到文件
	fs2 << 1 << " " << 2 << "\n" << "You like c++" << endl;
	//用 operator>> 从文件中读数据
	int i1, i2; string s1;
	fs2.seekg(0, fstream::beg); //移动读指针到文件头 beg cur end
	fs2 >> i1 >> i2 >> s1;
	cout << i1 << " " << i2 << " " << s1 << endl; //输出 1 2 You
	//问题： << 读数据,在读字符串的时候,遇到 空格 会截断
	//【用<< >> 输出到文件很方便，但是从文件读可能会有问题】
	cout << "1--------------\n";

	fs2.seekg(0, fstream::beg);
	char ch;
	while ((ch = fs2.get()) != EOF) //【char get()】 读入一个字符并返回
		cout.put(ch);
	fs2.clear(); //状态位复位
	fs2.seekg(0, fstream::beg);
	while (fs2.get(ch))	 //【fstream& get(char&)】 
		cout.put(ch);
	cout << "2--------------\n";

	fs2.clear(); //状态位复位
	fs2.seekg(0, fstream::beg);
	char buf[1024];
	fs2 >> buf; cout << buf << endl; //读字符串,遇到空格截断
	fs2.seekg(0, fstream::beg);

	//【fstream& get(char*,int,char)】字符数组/指针,个数,终止字符
	//但是,该成员函数不会将 终止字符取走,所以后面加ignore(1),忽略1个字符
	while (fs2.get(buf, 1024, '\n')) {
		cout << buf << endl;
		fs2.ignore(1);
	}
	cout << "3--------------\n";
	fs2.clear();
	fs2.seekg(0, fstream::beg);
	//【istream& getline(char*, int, char)】字符数组/指针,个数,终止字符
	//但是,该成员函数不会将终止字符存入char*,但是流中越过了截止符
	while (fs2.getline(buf, 1024, '\n')) 
		cout << buf << endl;
	cout << "4--------------\n";

	fs2.clear();
	fs2.seekg(0, fstream::beg);
	//【函数模板 不是成员函数！ stream& getline(stream&, string&)】
	string tmps;
	while (getline(fs2, tmps))
		cout << tmps << endl;
	cout << "5--------------\n";
	
	fs2.clear();
	fs2.seekg(0, fstream::beg);
	fs2.ignore(100, 'Y'); //最多忽略100个字符,或者遇到'Y','Y'字符会被忽略掉！
	fs2.putback('Y'); //重新将'Y'放回到流中
	cout << "peek:" << (char)fs2.peek() << endl; //peek()窥视,但是不取数据
	getline(fs2, tmps);
	cout << tmps << endl; 
	fs2.close();
	cout << "6--------------\n";

	ifstream ifs("2.txt",ifstream::in | ifstream::binary); 
	//ifstream默认打开方式: in 
	ofstream ofs("1.txt", ofstream::out| ofstream::trunc | ofstream::binary); 
	//ofstream默认打开方式: out | trunc
	ifs.read(buf, 1024);  //二进制read, 最多读1024字节
	int cnt = ifs.gcount(); //上次读取的字节数
	ofs.write(buf, cnt); //将buf写入流,字节数为 cnt

	struct Student { char name[20];	int age; char sex;	};
	Student stu[] = { {"张三",10,'m'},{ "李四",20,'f' },{ "王五",30,'m' } };
	fstream fs_stu("stu.data",fstream::in|fstream::out|fstream::trunc|fstream::binary);
	fs_stu.write(reinterpret_cast<char*>(stu), sizeof(stu));
	fs_stu.seekg(0, fstream::beg);
	Student tmp_stu;
	for (int i = 0; i < 3; i++) {
		fs_stu.read(reinterpret_cast<char*>(&tmp_stu), sizeof(tmp_stu));
		cout << tmp_stu.name << ":" << tmp_stu.age << ":" << tmp_stu.sex << endl;
	}
	cout << "7--------------\n";

	ifstream fs("2.txt");
	if (!fs) { cout << "open error\n"; return -1; }
	//流状态判断：
	//fs.eof() 到达尾部返回true, 对于 cin 就是输入 ctrl+Z
	//fs.fail() 上次IO操作失败返回true
	//fs.bad() 返回true表示流崩溃
	//fs.good() 流处于有效状态返回true
	cout << "fs.eof(): " << fs.eof() << endl  //0
		<< "fs.fail(): " << fs.fail() << endl //0
		<< "fs.bad(): " << fs.bad() << endl   //0
		<< "fs.good(): " << fs.good() << endl;//1
	cout << "7.1-----\n";
	while (fs.get(ch)) cout << ch;
	//当前位置在文件尾部
	cout << "fs.eof(): " << fs.eof() << endl  //1
		<< "fs.fail(): " << fs.fail() << endl //1
		<< "fs.bad(): " << fs.bad() << endl   //0
		<< "fs.good(): " << fs.good() << endl;//0
	cout << "7.2-----\n";
	fs.clear(); //状态复位,流有效
	fs.seekg(0, ifstream::beg);
	cout << "fs.eof(): " << fs.eof() << endl  //0
		<< "fs.fail(): " << fs.fail() << endl //0
		<< "fs.bad(): " << fs.bad() << endl   //0
		<< "fs.good(): " << fs.good() << endl;//1
	cout << "7.3-----\n";
	int a1, a2, a3;
	fs >> a1 >> a2 >> a3; //a3会错,要求读入int,但是没有
	cout << "fs.eof(): " << fs.eof() << endl  //0
		<< "fs.fail(): " << fs.fail() << endl //1
		<< "fs.bad(): " << fs.bad() << endl   //0
		<< "fs.good(): " << fs.good() << endl;//0
	cout << "7.4-----\n";

	cout << "end--------------\n";
	return 0;
}