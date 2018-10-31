#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
using namespace std;

class My_lock_guard {
public:
	explicit My_lock_guard(std::mutex& _mtx):mtx(_mtx){
		mtx.lock(); //构造完成时,资源已经可以使用
	}
	~My_lock_guard() { mtx.unlock(); } //析构时释放
private:
	std::mutex& mtx; //引用
};

class Image {
public:
	Image(int i) :data(i) {
		if (i % 4 == 1) throw "Cr_Img error";
		//1,5,9...抛异常
	}
private:
	int data;
};

class ImgMenu {
public:
	ImgMenu(int i = 0)
		:img(new Image(i)),img_changes(0) {}
	~ImgMenu() { /*delete img;*/ }
	void set_img(int image); //更改图片
private:
	std::mutex mtx; 
	//Image *img;
	shared_ptr<Image> img; //改用智能指针
	int img_changes; 
};

void ImgMenu::set_img(int image) {
	My_lock_guard lck(mtx); 

	//delete img;	img = nullptr;
	//img_changes++;
	//img = new Image(image);

	//new成功才会执行 reset
	//失败的话,不会delete原来的img内容
	img.reset(new Image(image));
	img_changes++;

	cout << img_changes << endl;
}

ImgMenu img_menu;
void do_thread(int image) { //线程函数
	try {
		img_menu.set_img(image);
	}
	catch (const char * e) {
		cout << e << endl;
	}
}

int main() {
	std::thread threads[8];
	for (int i = 0; i < 8; i++)
		threads[i] = std::thread(do_thread, i);
	for (auto &th : threads) th.join();
	return 0;
}