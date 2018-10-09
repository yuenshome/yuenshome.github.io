void reverse() {
		if (*count > 1) { //除了自己外，还有人在用
			//重新复制一份以后，再处理
			(*count)--; //先把自己的计数去掉
			count = new int(1); //重新开辟新的计数空间
			char* tmp = ps; //保留原来的ps指针指向的位置
			ps = new char[strlen(tmp) + 1];
			strcpy(ps, tmp);
			//此时，本对象已经复制了一份拷贝，并新开了计数器
		}
		int len = strlen(ps) - 1;
		for (int i = 0; i < len / 2; i++) {
			char c = ps[i];
			ps[i] = ps[len - i];
			ps[len - i] = c;
		}
	}