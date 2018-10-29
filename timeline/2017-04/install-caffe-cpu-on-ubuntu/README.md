[![header](../../../assets/header20.jpg)](https://yuenshome.github.io)

# Ubuntu16.04安装CPU版Caffe

本文是安装CPU版本Caffe的教程，如果你是ARM平台也可以参考，但是可能需要在编译OpenBLAS的时候需要进一步参考给出的链接，选择相应的编译设置，其它无异。该文大部分参考官方在github的wiki上的教程，另外两个参考是csdn上在遇到hdf5.h缺失和pycaffe安装的问题，下面就开始：

[toc]<!--more-->
<h1>0. 编译OpenBLAS</h1>
从openblas.net网站（http://www.openblas.net/，目前版本：0.2.19）下载openblas源码，下载完成后进入目录下make即可，或者用下面这句，直接用当前CPU的最大线程去make：
<pre class="lang:sh decode:true ">make $(nproc)</pre>
编译完成后，安装：
<pre class="lang:sh decode:true ">make install PREFIX=your_installation_directory</pre>
默认的安装路径是/opt/OpenBLAS，比方不加PREFIX．

如果是ARM或者其它平台，具体的编译文档参考：
<ul>
	<li>Installation Guide · xianyi/OpenBLAS Wiki
https://github.com/xianyi/OpenBLAS/wiki/Installation-Guide</li>
	<li>User Manual · xianyi/OpenBLAS Wiki
https://github.com/xianyi/OpenBLAS/wiki/User-Manual</li>
	<li>Home · xianyi/OpenBLAS Wiki
https://github.com/xianyi/OpenBLAS/wiki</li>
</ul>
<h1>1. 环境安装</h1>
参考caffe在github上的教程，以下安装命令来自该链接
Ubuntu 16.04 or 15.10 Installation Guide · BVLC/caffe Wiki
https://github.com/BVLC/caffe/wiki/Ubuntu-16.04-or-15.10-Installation-Guide

需要注意的是：
<ul>
	<li>我这里没有安装Python3.5而是Python2.7，下面命令包含了，注意跳过3.5</li>
	<li>需要安装opencv，sudo apt-get install python-opencv，没有在下面的命令中，这里需要提前安装下，通过该命令默认安装opencv2</li>
</ul>
<pre class="lang:sh decode:true">sudo apt-get update

sudo apt-get upgrade

sudo apt-get install -y build-essential cmake git pkg-config

sudo apt-get install -y libprotobuf-dev libleveldb-dev libsnappy-dev libhdf5-serial-dev protobuf-compiler

sudo apt-get install -y libatlas-base-dev 

sudo apt-get install -y --no-install-recommends libboost-all-dev

sudo apt-get install -y libgflags-dev libgoogle-glog-dev liblmdb-dev

# (Python general)
sudo apt-get install -y python-pip

＃ (Python 2.7 development files)
sudo apt-get install -y python-dev
sudo apt-get install -y python-numpy python-scipy

# (or, Python 3.5 development files)
sudo apt-get install -y python3-dev
sudo apt-get install -y python3-numpy python3-scipy
 
# (OpenCV 2.4)
sudo apt-get install -y libopencv-dev

(or, OpenCV 3.1 - see the instructions below)</pre>
<h1>2. 修改Makefile等配置文件</h1>
进入caffe目录下，对配置文件Makefile.config.example复制粘帖为Makefile.config：
<pre class="lang:sh decode:true ">cp Makefile.config.example Makefile.config</pre>
因为安装的是CPU版本，我们需要打开该Make.config文件并做如下修改：

找到CPU_ONLY和USE_OPENCV，删掉这两句话前面的注释符号#，并将USE_OPENCV的值从0该为1，如下：
<pre class="lang:sh decode:true">CPU_ONLY := 1
USE_OPENCV := 1</pre>
我们需要安装pycaffe，同时也取消掉下面这行的前面的注释：
<pre class="lang:sh decode:true ">WITH_PYTHON_LAYER := 1</pre>
另外，如果是Anaconda环境，可以指定Anaconda的Python，如下：
<pre class="lang:python decode:true"># ANACONDA_HOME := $(HOME)/anaconda                                                                                                                                                                                                        
ANACONDA_HOME := /usr/local/lib/anaconda2</pre>
由于使用的CPU加速库是openblas，不是默认的atlas，我们把下面的值从
<pre class="lang:sh decode:true"># BLAS choice:
# atlas for ATLAS (default)
# mkl for MKL
# open for OpenBlas
BLAS := atlas</pre>
改为：
<pre class="lang:sh decode:true"># BLAS choice:
# atlas for ATLAS (default)
# mkl for MKL
# open for OpenBlas
BLAS := open</pre>
并加入openblas编译后的路径：
<pre class="lang:sh decode:true"># Custom (MKL/ATLAS/OpenBLAS) include and lib directories.                                                                                                                                                                                 
# Leave commented to accept the defaults for your choice of BLAS                                                                                                                                                                           
# (which should work)!

BLAS_INCLUDE := /opt/OpenBLAS/include/                                                                                                                                                                                                     
BLAS_LIB := /opt/OpenBLAS/lib</pre>
<h1>3. 安装hdf5并修改相关配置文件</h1>
<h2>3.1 安装hdf5</h2>
这部分单独写出来，因为如果没有这个部分直接make的情况下会报错cannot find hdf5什么的．所以需要事先安装下：
<pre class="lang:sh decode:true">sudo apt-get install libhdf5-serial-dev</pre>
<h2>3.2 修改Makefile.config</h2>
修改Makefile.config文件，找到：
<pre class="lang:sh decode:true">INCLUDE_DIRS:=$(PYTHON_INCLUDE) /usr/local/include</pre>
然后在后面加上"serial"的包含目录，新的目录，即：
<pre class="lang:sh decode:true ">INCLUDE_DIRS:=$(PYTHON_INCLUDE) /usr/local/include/usr/include/hdf5/serial/</pre>
<h2>3.3 修改Makefile</h2>
接着需要更改相应的"Makefile"文件，找到：
<pre class="lang:sh decode:true">LIBRARIES +=glog gflags protobuf boost_system boost_filesystem m hdf5_hl hdf5</pre>
更改最后两项，修改后为：
<pre class="lang:sh decode:true">LIBRARIES +=glog gflags protobuf boost_system boost_filesystem m hdf5_serial_hl hdf5_serial</pre>
参考：搭建caffe环境时“error: hdf5.h”找不到的解决方法 - 博客频道 - CSDN.NET
http://blog.csdn.net/goofysong/article/details/52116265
<h1>4. 编译</h1>
<pre class="lang:sh decode:true">make all -j$(nproc)

make test -j$(nproc)

make runtest -j$(nproc)</pre>
参考caffe在github上的教程，以下安装命令来自该链接
Ubuntu 16.04 or 15.10 Installation Guide · BVLC/caffe Wiki
https://github.com/BVLC/caffe/wiki/Ubuntu-16.04-or-15.10-Installation-Guide
<h1>5. 安装pycaffe</h1>
编译pycaffe：
<pre class="lang:sh decode:true">make pycaffe -j$(nproc)</pre>
添加环境变量：
<pre class="lang:sh decode:true">vim ~/.bashrc</pre>
将你的caffe/python目录添加到bashrc文件中，比如：<code>export PYTHONPATH=/home/wanghh/caffe/python:$PYTHONPATH</code>添加到文件中。将该句加入到bashrc文件的末尾，之后再：
<pre class="lang:sh decode:true">source ~/.bashrc</pre>
使更改生效。 这样，在其他地方打开python，也可以import caffe了。

参考：pycaffe安装与路径配置 - 博客频道 - CSDN.NET
http://blog.csdn.net/yingyujianmo/article/details/51014440
