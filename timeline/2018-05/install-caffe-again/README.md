[![header](../../../assets/header28.jpg)](https://yuenshome.github.io)

# 再次安装Caffe:cpu的经历

这次容易不少，无论是直接Google错误信息或者是再Caffe的wiki里去查，更有先前积累了一些错误处理方法的经验。下面是在安装Caffe时，有用的链接：
<ul>
	<li>【<strong><span style="color: #ff0000;">必看必follow，否则奇怪错误</span></strong>】Ubuntu 16.04 or 15.10 Installation Guide · BVLC/caffe Wiki
https://github.com/BVLC/caffe/wiki/Ubuntu-16.04-or-15.10-Installation-Guide</li>
	<li>Building Caffe on Ubuntu: make can't find Boost's include files - Stack Overflow
https://stackoverflow.com/questions/28867791/building-caffe-on-ubuntu-make-cant-find-boosts-include-files/34915365</li>
	<li>Commonly encountered build issues · BVLC/caffe Wiki
https://github.com/BVLC/caffe/wiki/Commonly-encountered-build-issues</li>
	<li>Error compiling caffe. leveldb/db.h missing
sudo apt-get install libleveldb-dev
参考：Error compiling caffe. leveldb/db.h missing - Google 网上论坛
https://groups.google.com/forum/#!topic/caffe-users/_nqvaNZk2Bw</li>
</ul>
lsnappy缺少的问题后来也解决了，忘了在哪儿搜到的了。略
