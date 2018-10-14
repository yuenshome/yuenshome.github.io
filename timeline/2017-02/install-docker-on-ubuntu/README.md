[![header](../../../assets/header12.jpg)](https://yuenshome.github.io)

# Ubuntu16.04安装Docker

重装系统后在本地安装Docker（安装的版本是：Docker version 1.13.1, build 092cba3），安装过程出现问题并记录。这次参考官方教程（https://docs.docker.com/engine/installation/linux/ubuntu/#/install-using-the-repository），从<strong>1.安装</strong>开始看起。以前我是按照如下方式安装的
<pre class="lang:sh decode:true">sudo apt-get update
sudo apt-get install docker.io</pre>
[toc]

有关Docker安装Caffe或者命令使用以及错误问题，可以参考我之前写的这篇文章<a href="http://121.42.47.99/yuenshome/wordpress/?p=3164" target="_blank">Ubuntu16.04 借助 Docker 安装 Caffe</a>，另外<a href="http://121.42.47.99/yuenshome/wordpress/?p=2915" target="_blank">开启Ubuntu的docker之旅</a>这篇是对官方入门教程的译文，但是版本可能比较早，还是以docker最新的入门教程为主。
<!--more-->

下面是官方教程的其中一种安装流程：
<h1>1. 安装</h1>
允许apt可以从https来安装：
<pre class="lang:sh decode:true">$ sudo apt-get install -y --no-install-recommends \
    apt-transport-https \
    ca-certificates \
    curl \
    software-properties-common</pre>
加入docker的官方GPG公钥（注：<em>GPG</em>和PGP是两种加密软件，你可以通过公钥在网上安全的传播自己的文件）。
<pre class="lang:sh decode:true ">$ curl -fsSL https://apt.dockerproject.org/gpg | sudo apt-key add -</pre>
验证key ID是否为：58118E89F3A912897C070ADBF76221572C52609D
<pre class="lang:sh decode:true">$ apt-key fingerprint 58118E89F3A912897C070ADBF76221572C52609D
# 下面是输出结果
  pub   4096R/2C52609D 2015-07-14
        Key fingerprint = 5811 8E89 F3A9 1289 7C07  0ADB F762 2157 2C52 609D
  uid                  Docker Release Tool (releasedocker) &lt;docker@docker.com&gt;</pre>
使用如下命令使用稳定仓库。注：lsb_release -cs 这条子命令会返回你当前Ubuntu系统的发行版名，如“xenial”。
<pre class="lang:sh decode:true">$ sudo add-apt-repository \
       "deb https://apt.dockerproject.org/repo/ \
       ubuntu-$(lsb_release -cs) \
       main"</pre>
<h1>2. 问题与解决</h1>
出现的问题都是权限问题，小问题，只要每次在使用docker命令前加上sudo就不会出现报错，但毕竟比较麻烦，在2.2给出官方安装方式后报错的解决方法（添加docker的使用权限给当前用户）。
<h2>2.1 传统方式安装报错</h2>
如果第一次安装直接就是参照的上述官方安装流程，那么可以忽略本小节的内容。

使用我以往的方式（sudo apt-get install docker.io）安装docker后，会出现如下问题：使用docker images命令，查看当前是否有镜像（结果应该为空，我们什么镜像都没下载）。报错如下：
<pre class="lang:python decode:true">Cannot connect to the Docker daemon. Is the docker daemon running on this host?</pre>
此外，docker的ps pull search run命令都会报这个错 <code>Cannot connect to the Docker daemon. Is the docker daemon running on this host?</code>

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/02/docker_trad_error.png" alt="" width="693" height="333" />

查文档（截图来自参考）发现是因为权限问题，加上sudo就好了，另外需要将当前使用的用户添加到docker的权限用户中去，就可以不用再输入sudo使用docker了。

注意：因为我出现这个问题后，当初是使用传统方式安装的，所以又查官方文档按上述的官方安装流程走了一遍，不需要卸载原先的docker（其实按理说应该卸载），不卸载的话会自动替换掉原先的docker，但是配置文件可能会再写入一次。报出如下错误：
<pre class="lang:python decode:true">W: Target Packages (main/binary-amd64/Packages) is configured multiple times in /etc/apt/sources.list:54 and /etc/apt/sources.list.d/docker.list:1
W: Target Packages (main/binary-all/Packages) is configured multiple times in /etc/apt/sources.list:54 and /etc/apt/sources.list.d/docker.list:1
W: Target Translations (main/i18n/Translation-en_US) is configured multiple times in /etc/apt/sources.list:54 and /etc/apt/sources.list.d/docker.list:1
W: Target Translations (main/i18n/Translation-en) is configured multiple times in /etc/apt/sources.list:54 and /etc/apt/sources.list.d/docker.list:1
W: Target DEP-11 (main/dep11/Components-amd64.yml) is configured multiple times in /etc/apt/sources.list:54 and /etc/apt/sources.list.d/docker.list:1
W: Target DEP-11-icons (main/dep11/icons-64x64.tar) is configured multiple times in /etc/apt/sources.list:54 and /etc/apt/sources.list.d/docker.list:1</pre>
sudo vim编辑/etc/apt/sources.list文件，删除掉重复的内容。

给出参考中网友给出的解决方案，这里没有尝试不具体说明，只是贴一下网友的回复的译文：

需要把使用docker的用户加入docker的group中（你需要把用户添加到docker group. by中），通过命令：
<pre class="lang:sh decode:true">sudo gpasswd -a xxxx docker</pre>
其中xxx是用户名，之后重启电脑问题得到解决。
<h2>2.2 官方方式安装报错</h2>
与传统方式的安装（2.1）一样是权限问题，因为使用的不是root用户，如使用docker images命令的报错如下：
<pre class="lang:python decode:true">Got permission denied while trying to connect to the Docker daemon socket at unix:///var/run/docker.sock: Get http://%2Fvar%2Frun%2Fdocker.sock/v1.26/images/json: dial unix /var/run/docker.sock: connect: permission denied
</pre>
找到stackoverflow上的一个解决方案：
<pre class="lang:sh decode:true ">$ sudo usermod -a -G docker $username</pre>
重启后问题解决。
<h1>3. 下载更多的images</h1>
我们不仅可以根据从项目repo直接clone下来的代码通过dockerfile来创建镜像（下一部分讲），还可以下载已有的镜像，通过网上的docker镜像仓库（如dockerhub），里面有别人制作好的docker镜像。

比方说想要查找mxnet的docker镜像，我们可以通过如下命令查找到：
<pre class="lang:sh decode:true">$ sudo docker search mxnet
NAME                           DESCRIPTION                                     STARS     OFFICIAL   AUTOMATED
kaixhin/mxnet                  Ubuntu Core 14.04 + MXNet.                      16                   [OK]
kaixhin/cuda-mxnet             Ubuntu Core 14.04 + CUDA + MXNet.               14                   [OK]
dmlc/mxnet                     images for mxnet - http://mxnet.dmlc.ml         3                    
dsdgroup/mxnet                 mxnet                                           1                    [OK]
pottava/mxnet                  MXNet images.                                   1                    [OK]
wangjianyong/mxnet_jupyter     mxnet jupyter                                   1                    [OK]
koallen/anaconda-mxnet         anaconda + mxnet                                0                    [OK]
louishp/mxnet                  dockerfile for mxnet                            0                    [OK]
raohuaming/mxnet               mxnet with R develop environment                0                    [OK]
wangjianyong/mxnet-cuda        mxnet with GPU backend                          0                    [OK]
tsutomu7/mxnet                 MXNet and python3.5                             0                    [OK]
alfpark/mxnet                  MXNet from dmlc                                 0                    
waldon/mxnet                   Ubuntu Core 16.04 + MXNet + Jupyter notebook.   0                    [OK]
gabbagandalf/mxnet-docker      build mxnet with opencv 3                       0                    [OK]
philipz/mxnet                  Support CUDA &amp; Cudnn                            0                    [OK]
wangjianyong/mxnet             mxnet with cpu backend                          0                    [OK]
chstone/mxnet-gpu              mxnet for R and Python built on Ubuntu 16....   0                    [OK]
alsrgv/cuda-mxnet-conda3       Build of kaixhin/cuda-mxnet with Anaconda 3.    0                    [OK]
raohuaming/cuda-mxnet          Cuda mxnet with R environment                   0                    [OK]
nitnelave/mxnet                MXNet Framework with SSH server, CUDA7.5, ...   0                    [OK]
pkjg/cuda7.5-mxnet             MXNET on [nvidia/CUDA 7.5] (https://hub.do...   0                    [OK]
pkjg/cuda7.0-mxnet             MXNET on [nvidia/CUDA 7.0](https://hub.doc...   0                    [OK]
shuxin/mxnet                   notebook，cuda7.0，2016/7/26                      0                    [OK]
joe8767/web-mxnet-gpu          nginx+uwsgi+flask+mxnet web server              0                    [OK]
raohuaming/mxnet-opencv-dlib   mxnet + opencv + dlib                           0                    [OK]
</pre>
如果根据描述符合我们的要求，那就可以通过使用指定的命令进行镜像下载（比方下载dmlc/mxnet）：
<pre class="lang:sh decode:true">yuens@Spark:~$ sudo docker pull dmlc/mxnet
Using default tag: latest
latest: Pulling from dmlc/mxnet
759d6771041e: Downloading [===================================&gt;               ]  46.5 MB/65.69 MB
8836b825667b: Download complete 
c2f5e51744e6: Download complete 
a3ed95caeb02: Download complete 759d6771041e: Downloading 62.18 MB/65.69 MB
6e9a08e578bb: Downloading [====&gt;                                              ] 17.81 MB/206.4 MB
ead57e994817: Downloading [=================&gt;                                 ] 8.288 MB/23.13 MB6e9a08e578bb: Downloading 6.464 MB/188.6 MB
ead57e994817: Downloading 9.025 MB/23.13 MB
</pre>
下载完成后，就可以通过docker images命令来查看当前的镜像仓库了（比方我下载了hello-world和ubuntu这两个镜像）：
<pre class="lang:sh decode:true">yuens@Spark:~/Software/mxnet/docker$ sudo docker images
[sudo] password for yuens: 
REPOSITORY          TAG                 IMAGE ID            CREATED             SIZE
ubuntu              latest              f49eec89601e        4 weeks ago         129 MB
hello-world         latest              48b5124b2768        5 weeks ago         1.84 kB</pre>
<h1>4. 使用dockerfile创建镜像</h1>
本节不会涉及如何编写dockerfile，但是会简单记录一下如何通过已编写好的dockerfile来创建docker image。例如从github上clone下来的mxnet repo中有一个docker文件夹，里面分别有一个cpu和gpu的文件夹，分别对应着创建cpu版本和gpu版的mxnet镜像。

其实完全可以参考docker文件夹下的README.md来查看具体的安装说明（这个要参考第二部分，即要使用build命令，第一部分是直接从网上把镜像下载下来）：

在mxnet/docker路径（此docker路径下使用ls命令可以看到有名为cpu和gpu这两个文件夹）下，使用如下命令根据dockerfile文件来创建镜像（期间也会下载必要的构建镜像的组件，具体参考dockerflie里面的内容，这里不详细说明）：
<pre class="lang:sh decode:true">yuens@Spark:~/Software/mxnet/docker$ sudo docker build -t dmlc/mxnet:cpu cpu
Sending build context to Docker daemon 2.048 kB
Step 1/6 : FROM ubuntu:14.04
14.04: Pulling from library/ubuntu
c60055a51d74: Downloading 13.52 MB/65.69 MB
c60055a51d74: Downloading 52.44 MB/65.69 MB
969d017f67e6: Download complete 
37c9a9113595: Verifying Checksum 
a3d9f8479786: Download complete</pre>
上面给出的是创建mxnet cpu版的镜像命令，若创建gpu版的mxnet镜像，在同样的路径下使用如下命令：
<pre class="lang:sh decode:true">docker build -t dmlc/mxnet:cuda cuda</pre>
<h1>参考</h1>
<ul>
	<li>Example: Install Docker on Ubuntu Linux - Docker
https://docs.docker.com/engine/getstarted/linux_install_help/</li>
	<li>Install Docker and run hello-world - Docker
https://docs.docker.com/engine/getstarted/step_one/#/docker-for-linux</li>
	<li><strong>（主要参考）</strong>docker安装后出现Cannot connect to the Docker daemon. - SegmentFault
https://segmentfault.com/q/1010000005040763</li>
	<li>/var/run/docker.sock: permission denied while running docker within Python CGI script - Stack Overflow
http://stackoverflow.com/questions/40877705/var-run-docker-sock-permission-denied-while-running-docker-within-python-cgi-s</li>
</ul>
<h1>更多参考</h1>
<ul>
	<li>（<strong>写的简单明了</strong>）Docker 安装jupyter notebook - 纸鸢spring - 博客园
http://www.cnblogs.com/lutingting/p/5229573.html</li>
</ul>
