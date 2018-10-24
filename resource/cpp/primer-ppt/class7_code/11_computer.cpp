#include <iostream>
using namespace std;
class Cpu { //Cpu接口(抽象基类)
public:
	virtual void work() = 0;
	virtual ~Cpu(){}
};
class Mem { //Mem接口
public:
	virtual void work() = 0;
	virtual ~Mem() {}
};
class HDisk { //HDisk接口
public:
	virtual void work() = 0;
	virtual ~HDisk() {}
};
class IntelCpu :public Cpu {
public:
	virtual void work() override { cout << "Intel Cpu working..." << endl;}
};
class AmdCpu :public Cpu {
public:
	virtual void work() override { cout << "AMD Cpu working..." << endl;}
};
class KSTMem :public Mem {
public:
	virtual void work() override { cout << "KST Mem working..." << endl;}
};
class SamsungMem :public Mem {
public:
	virtual void work() override { cout << "SamSung Mem working..." << endl;}
};
class WDHDisk :public HDisk {
public:
	virtual void work() override { cout << "WD HDisk working..." << endl; }
};

class Computer { 
public:
	Computer(Cpu *pcpu,Mem *pmem,HDisk *phdisk)
	:cpu(pcpu),mem(pmem),hdisk(phdisk){}
	void work() {
		cpu->work();
		mem->work();
		hdisk->work();
	}
private:
	Cpu *cpu;
	Mem *mem;
	HDisk *hdisk;
};
int main() {
	IntelCpu intel;
	KSTMem kst;
	WDHDisk wd;
	Computer pc(&intel,&kst,&wd);
	pc.work();
	return 0;
}