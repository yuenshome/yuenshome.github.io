[![header](../../../assets/header28.jpg)](https://yuenshome.github.io)

# Ubuntu16.04 借助 Docker 安装 Caffe

最近在 Ubuntu 上安装 Caffe 总是有各种各样的问题，尽管因为系统版本的问题没有安装 CUDA ，少了不少麻烦，但在 make 的时候总是缺这个.o文件或者少那个.h文件。<strong><span style="color: #ff0000;">Docker 常用命令请查看参考部分的第一个</span></strong>。[toc]<!--more-->
<h1>0. 安装 Docker</h1>
在 Ubuntu 上安装 Docker 的命令十分简单：
<pre class="lang:sh decode:true">sudo apt-get install docker.io</pre>
<h1>1. 获取 Caffe 的 Docker 镜像</h1>
恰巧有群友说到 Docker，以前看过，现在还依稀记得。现在提供两种方法来获取 Docker 镜像，大家二选一（1.1 或者 1.2）即可。考虑到有的同学没有 git 下 caffe 的源码，我们这里推荐第二种方法（跳过1.1，开始1.2 搜索并下载 Caffe 的 Docker 镜像）。
<h2>1.1 通过 Dockerfile 来创建 Docker 镜像</h2>
发现 Caffe 从 Github 上下载下来的压缩包中，根目录就有一个名为 Docker 的文件夹，可以进去后选择 CPU 文件夹或者 GPU 文件夹，根据Dockerfile来 build 不同版本的 Docker Caffe 镜像。使用如下命令来 build 镜像：
<pre class="lang:sh decode:true">docker build -t repo_name/tag_name .</pre>
镜像保存的地方我也不知道在哪儿，查书也没查到，不过到找到了 Docker 安装的部分文件在用户根目录的某个文件夹里面。反正镜像在我们的镜像仓库里（后文会说到）。
<h2>1.2 搜索并下载 Caffe 的 Docker 镜像</h2>
使用如下命令，可以在 Docker Hub 上搜索所有用户上传的镜像，一般来说选择点星星（Star）最多的镜像。输入命令（<strong><span style="color: #ff0000;">docker search caffe</span></strong>）后，显示如下：
<pre class="lang:sh decode:true ">root@spark:/home/yuens# docker search caffe
NAME                               DESCRIPTION                                     STARS     OFFICIAL   AUTOMATED
tleyden5iwx/caffe-cpu-master                                                       42                   [OK]
tleyden5iwx/caffe-gpu-master                                                       27                   [OK]
kaixhin/caffe                      Ubuntu Core 14.04 + Caffe.                      14                   [OK]
kaixhin/cuda-caffe                 Ubuntu Core 14.04 + CUDA + Caffe.               14                   [OK]
nvidia/caffe                       Caffe images from github.com/NVIDIA/nvidia...   4                    
neowaylabs/caffe-cpu               Caffe CPU based on:  https://hub.docker.co...   2                    [OK]
mbartoli/caffe                     Caffe, CPU-only                                 1                    [OK]
ruimashita/caffe-gpu-with-models   ubuntu 14.04 cuda 7.0 caffe  bvlc_referenc...   0                    [OK]
tingtinglu/caffe                   caffe                                           0                    [OK]
kavolorn/caffe                     Caffe image based on OpenCV image.              0                    [OK]
suyongsun/caffe-gpu                Caffe image with gpu mode.                      0                    [OK]
chakkritte/docker-caffe            Docker caffe                                    0                    [OK]
ruimashita/caffe-gpu               ubuntu 14.04 cuda 7 (NVIDIA driver version...   0                    [OK]
kaixhin/caffe-deps                 `kaixhin/caffe` dependencies.                   0                    [OK]
ruimashita/caffe-cpu-with-models   ubuntu 14.04 caffe  bvlc_reference_caffene...   0                    [OK]
kaixhin/cuda-caffe-deps            `kaixhin/cuda-caffe` dependencies.              0                    [OK]
haoyangz/caffe-cnn                 caffe-cnn                                       0                    [OK]
ruimashita/caffe-cpu               ubuntu 14.04 caffe                              0                    [OK]
alfakini/caffe-cpu                 Docker container with Ubuntu Core 14.04, C...   0                    [OK]
ypandit/caffe                      BLVC Caffe with PyCaffe                         0                    [OK]
mtngld/caffe-gpu                   Ubuntu + caffe (gpu ready)                      0                    [OK]
suyongsun/caffe-cpu                Caffe image with cpu mode only.                 0                    [OK]
nitnelave/caffe                    Master branch of BVLC/caffe, on CentOS7 wi...   0                    [OK]
elezar/caffe                       Caffe Docker Images                             0                    [OK]
patdiscvrd/caffe                                                                   0                    [OK]
</pre>
我们选择星星最多的镜像并下载，命令如下：
<pre class="lang:sh decode:true">docker pull tleyden5iwx/caffe-cpu-master</pre>
我们这里选择 cpu 版本。下载完成后镜像就会在我们的镜像仓库里，可以通过命令（<strong><span style="color: #ff0000;">docker images</span></strong>）来查看，显示如下：
<pre class="lang:sh decode:true">root@spark:/home/yuens# docker images
REPOSITORY             TAG                 IMAGE ID            CREATED             SIZE
build_repo/caffe-cpu   latest              e740a499ed4b        46 hours ago        1.307 GB
docker-caffe-cpu       latest              e740a499ed4b        46 hours ago        1.307 GB
ubuntu                 14.04               90d5884b1ee0        11 days ago         188 MB</pre>
<h1>2. 查看当前运行的容器</h1>
首先，我们先来使用命令（<strong><span style="color: #ff0000;">docker ps</span></strong>）看看当前有哪些容器正在运行：
<pre class="lang:sh decode:true ">root@spark:/home/yuens# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES</pre>
可以看到，当前没有正在运行的容器，这个命令还可以带一个参数 -a （代表全部，<strong><span style="color: #ff0000;">docker ps -a</span></strong>），查看所有的容器状态，可以看到有的容器我们退出了的时间，创建了的时间等等信息：
<pre class="lang:sh decode:true">root@spark:/home/yuens# docker ps -a
CONTAINER ID        IMAGE                  COMMAND             CREATED             STATUS                         PORTS               NAMES
a27e67f5757a        docker-caffe-cpu       "/bin/bash"         36 minutes ago      Exited (137) 29 minutes ago                        happy_roentgen
0665dbc54249        docker-caffe-cpu       "/bin/bash"         36 minutes ago      Exited (0) 36 minutes ago                          lonely_yalow
8d3f3fed703c        e74                    "/bin/bash"         37 minutes ago      Exited (0) 37 minutes ago                          sad_wescoff
49363061600e        docker-caffe-cpu       "/bin/bash"         37 minutes ago      Exited (0) 37 minutes ago                          high_lamarr
b1e3c1784a49        docker-caffe-cpu       "-itb"              38 minutes ago      Created                                            jovial_dubinsky
e46fc2828e48        docker-caffe-cpu       "-ibt"              38 minutes ago      Created                                            reverent_dubinsky
bbf7ff03611f        docker-caffe-cpu       "/bin/bash"         39 minutes ago      Exited (0) 39 minutes ago                          happy_chandrasekhar
15aa31c5ad8f        docker-caffe-cpu       "/bin/bash"         About an hour ago   Exited (130) 58 minutes ago                        condescending_brattain
2fe7b65c69c7        docker-caffe-cpu       "/bin/bash"         About an hour ago   Exited (0) About an hour ago                       small_meninsky
a025ec63fe9f        build_repo/caffe-cpu   "/bin/bash"         28 hours ago        Exited (130) 28 hours ago                          big_keller
f89e0ebf7f2d        docker-caffe-cpu       "/bin/bash"         28 hours ago        Exited (0) 28 hours ago                            cranky_rosalind
fddf20df7d91        docker-caffe-cpu       "/bin/bash"         28 hours ago        Exited (0) 28 hours ago                            stupefied_shaw
</pre>
<h1>3. 由 Docker 镜像创建并运行一个容器</h1>
既然没有正在运行的容器，那么我们根据镜像来创建一个容器吧！使用命令（<strong><span style="color: #ff0000;">docker run -tdi REPOSITORY</span></strong>）来创建，之后再查看当前运行的容器有哪些。
<pre class="lang:sh decode:true ">root@spark:/home/yuens# docker run -tdi docker-caffe-cpu
7b3bf6742debcf59f583d6bc61e32b5f692b41cc5ec72a12a5bec8644718fd7b
root@spark:/home/yuens# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
7b3bf6742deb        docker-caffe-cpu    "/bin/bash"         3 seconds ago       Up 2 seconds                            silly_euclid
</pre>
创建容器并运行的命令（<strong><span style="color: #ff0000;">docker run -tdi REPOSITORY</span></strong>）中，<strong><span style="color: #ff0000;">REPOSITORY </span></strong>名称是镜像仓库中镜像的名字，可以通过使用命令 （<strong><span style="color: #ff0000;">docker images</span></strong>）来查看当前本地镜像仓库有哪些镜像，并根据要创建镜像的 <strong><span style="color: #ff0000;">REPOSITORY </span></strong><span style="color: #000000;">名来创建容器</span>。
<h1>4. 进入容器内</h1>
在创建运行了容器后，我们进入容器。进入容器的命令（<strong><span style="color: #ff0000;">docker attach CONTAINER_ID</span></strong>，其中CONTAINER_ID只要开头一点可以和其他的CONTAINER_ID区分就可以），执行结果如下：
<pre class="lang:sh decode:true ">root@spark:/home/yuens# docker ps -a
CONTAINER ID        IMAGE                  COMMAND             CREATED             STATUS                           PORTS               NAMES
7b3bf6742deb        docker-caffe-cpu       "/bin/bash"         7 seconds ago       Up 6 seconds                                         silly_euclid
673de646bf70        docker-caffe-cpu       "/bin/bash"         21 seconds ago      Exited (0) 20 seconds ago                            furious_mclean
a27e67f5757a        docker-caffe-cpu       "/bin/bash"         38 minutes ago      Exited (137) 31 minutes ago                          happy_roentgen
0665dbc54249        docker-caffe-cpu       "/bin/bash"         38 minutes ago      Exited (0) 38 minutes ago                            lonely_yalow
8d3f3fed703c        e74                    "/bin/bash"         39 minutes ago      Exited (0) 39 minutes ago                            sad_wescoff
49363061600e        docker-caffe-cpu       "/bin/bash"         39 minutes ago      Exited (0) 39 minutes ago                            high_lamarr
b1e3c1784a49        docker-caffe-cpu       "-itb"              40 minutes ago      Created                                              jovial_dubinsky
e46fc2828e48        docker-caffe-cpu       "-ibt"              40 minutes ago      Created                                              reverent_dubinsky
bbf7ff03611f        docker-caffe-cpu       "/bin/bash"         41 minutes ago      Exited (0) 41 minutes ago                            happy_chandrasekhar
15aa31c5ad8f        docker-caffe-cpu       "/bin/bash"         About an hour ago   Exited (130) About an hour ago                       condescending_brattain
2fe7b65c69c7        docker-caffe-cpu       "/bin/bash"         About an hour ago   Exited (0) About an hour ago                         small_meninsky
a025ec63fe9f        build_repo/caffe-cpu   "/bin/bash"         28 hours ago        Exited (130) 28 hours ago                            big_keller
f89e0ebf7f2d        docker-caffe-cpu       "/bin/bash"         28 hours ago        Exited (0) 28 hours ago                              cranky_rosalind
fddf20df7d91        docker-caffe-cpu       "/bin/bash"         28 hours ago        Exited (0) 28 hours ago                              stupefied_shaw
root@spark:/home/yuens# docker attach 7b3
root@7b3bf6742deb:/workspace#</pre>
一般在执行进入容器前，我们都会输入查看当前正在运行的容器的命令（<strong><span style="color: #ff0000;">docker ps</span></strong> 或 <span style="color: #ff0000;"><strong>docker ps -a</strong></span>），看着 CONTAINER_ID 来进入指定的容器。

有时候会出现命令行伪假死状态（进入命令输入完成后，等半天命令行不动，似乎卡死了），其实没有，你输入键盘上的回车敲击几下，发现好着呢（一般而言，当输入docker attach命令完成后，瞬间就进入了容器，输入回车键就跳出命令行伪假死状态）。
<h1>5. 退出并停止容器</h1>
进入容器后，我们进行我们需要的作业，完成后或者设置完成后我们可以退出当前的容器，<strong><span style="color: #ff0000;">在容器里使用命令exit（或者直接键盘Ctrl+D）即可退出并暂停当前容器</span></strong>。

退出后，当我们需要再次启动该容器，我们同样是先查看刚刚退出暂停的容器的CONTAINER_ID（可以用命令docker ps -a来查看），然后容器开始执行的命令（<strong><span style="color: #ff0000;">docker start CONTAINER_ID</span></strong>）。
<pre class="lang:sh decode:true ">root@spark:/home/yuens# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
root@spark:/home/yuens# docker start 74
74
root@spark:/home/yuens# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
74f5856efa76        docker-caffe-cpu    "/bin/bash"         30 minutes ago      Up 1 seconds                            silly_davinci
</pre>
对于正在运行中的容器，我们停止其运行，可以使用停止命令（<strong><span style="color: #ff0000;">docker stop CONTAINER_ID</span></strong>），结果如下：
<pre class="lang:sh decode:true">root@spark:/home/yuens# docker images
REPOSITORY             TAG                 IMAGE ID            CREATED             SIZE
build_repo/caffe-cpu   latest              e740a499ed4b        47 hours ago        1.307 GB
docker-caffe-cpu       latest              e740a499ed4b        47 hours ago        1.307 GB
ubuntu                 14.04               90d5884b1ee0        11 days ago         188 MB
root@spark:/home/yuens# docker run -idt ubuntu
Unable to find image 'ubuntu:latest' locally
root@spark:/home/yuens# docker run -idt 90d5
9ce93c6664f3d936704949b8ded91b158492088c3727e77f5cccf9c65fc5407d
root@spark:/home/yuens# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
9ce93c6664f3        90d5                "/bin/bash"         9 seconds ago       Up 8 seconds                            naughty_rosalind
74f5856efa76        docker-caffe-cpu    "/bin/bash"         51 minutes ago      Up 21 minutes                           silly_davinci
root@spark:/home/yuens# docker stop 9ce
9ce
root@spark:/home/yuens# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
74f5856efa76        docker-caffe-cpu    "/bin/bash"         52 minutes ago      Up 22 minutes                           silly_davinci</pre>
可发现，我们在创建并运行某个容器的时候，可以用其 IMAGE ID （镜像ID）来创建。
<h1>6. 进入 Caffe 容器尝试使用 Python</h1>
下面是在Shell里操作，大致含义：
<ul>
	<li>首先查看现在是否有正在运行的容器，发现没有，那我们由 docker-caffe-cpu 的镜像 ID 创建一个容器，然后观察当前是否增加了一个运行中的容器，发现有的，那么我们就连接 attach 进这个容器中。</li>
	<li>进去后，我们到根目录的opt文件夹下，看到 caffe 安装文件在这里。</li>
	<li>输入 Python 进入，尝试 import caffe，第一次初始化中，第二次成功。</li>
</ul>
<pre class="lang:sh decode:true">root@spark:/home/yuens# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
root@spark:/home/yuens# docker images
REPOSITORY             TAG                 IMAGE ID            CREATED             SIZE
build_repo/caffe-cpu   latest              e740a499ed4b        47 hours ago        1.307 GB
docker-caffe-cpu       latest              e740a499ed4b        47 hours ago        1.307 GB
ubuntu                 14.04               90d5884b1ee0        11 days ago         188 MB
root@spark:/home/yuens# docker run -idt e74
f8dd391d23a9096f03813486fac51e255bbc7828b72ab33eef8e9eaee3e7174e
root@spark:/home/yuens# docker ps
CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS               NAMES
f8dd391d23a9        e74                 "/bin/bash"         4 seconds ago       Up 3 seconds                            compassionate_leakey
root@spark:/home/yuens# docker attach e74
Error: No such container: e74
root@spark:/home/yuens# docker attach f8
Error response from daemon: Multiple IDs found with provided prefix
root@spark:/home/yuens# docker attach f8d
root@f8dd391d23a9:/workspace# 
root@f8dd391d23a9:/workspace# cd /
root@f8dd391d23a9:/# ls
bin  boot  dev  etc  home  lib  lib64  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var  workspace
root@f8dd391d23a9:/# cd opt/
root@f8dd391d23a9:/opt# ls
caffe
root@f8dd391d23a9:/opt# cd caffe/
root@f8dd391d23a9:/opt/caffe# ls
CMakeLists.txt   CONTRIBUTORS.md  LICENSE   Makefile.config.example  build       cmake  docker  examples  matlab  python   src
CONTRIBUTING.md  INSTALL.md       Makefile  README.md                caffe.cloc  data   docs    include   models  scripts  tools
root@f8dd391d23a9:/opt/caffe# python 
Python 2.7.6 (default, Jun 22 2015, 17:58:13) 
[GCC 4.8.2] on linux2
Type "help", "copyright", "credits" or "license" for more information.
&gt;&gt;&gt; import caffe
libdc1394 error: Failed to initialize libdc1394
/usr/local/lib/python2.7/dist-packages/matplotlib/font_manager.py:273: UserWarning: Matplotlib is building the font cache using fc-list. This may take a moment.
  warnings.warn('Matplotlib is building the font cache using fc-list. This may take a moment.')
&gt;&gt;&gt; import caffe
&gt;&gt;&gt; print caffe
&lt;module 'caffe' from '/opt/caffe/python/caffe/__init__.py'&gt;</pre>
若发现输入 python 后，尝试 <span style="color: #ff0000;"><strong>import caffe</strong></span> 失败，提示如下：
<pre class="lang:sh decode:true">[root@0b98cab20445 home]# python
Python 2.7.5 (default, Jun 24 2015, 00:41:19) 
[GCC 4.8.3 20140911 (Red Hat 4.8.3-9)] on linux2
Type "help", "copyright", "credits" or "license" for more information.
&gt;&gt;&gt; import caffe
Traceback (most recent call last):
  File "&lt;stdin&gt;", line 1, in &lt;module&gt;
ImportError: No module named caffe</pre>
那是因为<strong><span style="color: #ff0000;">没有编译 pycaffe</span> </strong>，那就切换到 root 目录的 caffe 下（<strong><span style="color: #ff0000;">cd ~/caffe</span></strong> 或 <strong><span style="color: #ff0000;">cd /root/caffe</span></strong>），然后编译 pycaffe（<strong><span style="color: #ff0000;">貌似编译有点问题，后面import caffe不行，这里先Mark一下</span></strong>）。
<h1>7. 启动和其它问题</h1>
<h2>7.1 Ubuntu14.04启动失败</h2>
后来我在Ubuntu14.04上安装后，运行时出现如下信息的报错：
<pre class="lang:sh decode:true">post http:///var/run/docker.sock/v1.18/containers/service/start: dial unix /var/run/docker.sock: no such file or directory. Are you trying to connect to a TLS-enabled daemon without TLS?</pre>
执行如下命令后，问题解决：
<pre class="lang:sh decode:true ">sudo apt-get install apparmor
service docker restart</pre>
<h2>7.2 无法从仓库下载到镜像</h2>
使用 docker pull 命令报错信息如下：
<pre class="lang:sh decode:true">FATA[0025] Get https://index.docker.io/v1/repositories/kaixhin/caffe/images: dial tcp: lookup index.docker.io on 127.0.1.1:53: read udp 127.0.1.1:53: i/o timeout</pre>
目前就是反复使用该命令，总会有一次成功的。但这不是个办法，所以可以考虑翻墙或者国内的一些仓库，比方说使用 DaoCloud 的仓库或者是阿里的。我们这里提供一个阿里的镜像（专门做DeepLearning和HPC，里面已经集成了CUDA7.0，Caffe，Thean，Torch7等）。

可以使用该命令直接把镜像 pull 下来：
<pre class="lang:sh decode:true">docker pull registry.aliyuncs.com/alicloudhpc/toolkit</pre>
由于阿里提供的镜像比较大，将近9个G，下载下来后，用 docker images 命令查看当前的镜像信息：
<pre class="lang:sh decode:true ">REPOSITORY                                  TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
&lt;none&gt;                                      &lt;none&gt;              99527b47b472        5 days ago          1.1 GB
registry.aliyuncs.com/alicloudhpc/toolkit   latest              4f6e0348746d        4 months ago        8.589 GB
</pre>
其中，&lt;none&gt;是笔者尚未下载完被强制暂停的（只包含） caffe 的镜像。
<h2>7.3 容器内无法访问外网</h2>
注：本小节是2017年02月20日追加的博客内容。重新安装系统后把docker按照官方教程安装了，但是从网上docker pull下来的镜像，进入容器无法访问外网导致无法安装软件。下面给出解决方案和参考链接。

根据后文的参考，我属于的问题是默认的docker配置文件没有修改，需要改dns server就好了。
<pre class="lang:sh decode:true">vi /etc/default/docker</pre>
去掉“docker_OPTS="--dns 8.8.8.8 --dns 8.8.4.4"”前的#号。

参考的这个，总结了Docker容器内不能联网的6种解决方案：http://blog.csdn.net/yangzhenping/article/details/43567155
<h1>参考</h1>
<ol>
	<li>Docker 4 -- 总结（<strong>Docker常用命令</strong>）：https://blog.tankywoo.com/docker/2014/05/08/docker-4-summary.html</li>
	<li>linux - FATA[0000] Get http:///var/run/docker.sock/v1.17/version: dial unix /var/run/docker.sock - Stack Overflow  http://stackoverflow.com/questions/29294286/fata0000-get-http-var-run-docker-sock-v1-17-version-dial-unix-var-run-doc</li>
	<li>阿里开发者平台（<strong>DL+HPC的docker镜像</strong>）  https://dev.aliyun.com/detail.html?spm=5176.1972343.2.20.C16rVe&amp;repoId=2</li>
	<li>Pulling Docker images: i/o timeout - Open Source Projects / Open Source Registry API - Docker Forums （<strong>用处可能不大</strong>） https://forums.docker.com/t/pulling-docker-images-i-o-timeout/740/11</li>
	<li>（<strong>写的很详细</strong>）Docker容器内不能联网的6种解决方案 - 每一天都有新的希望 - 博客频道 - CSDN.NEThttp://blog.csdn.net/yangzhenping/article/details/43567155</li>
</ol>
<h1>更多参考</h1>
<ol>
	<li>
<p id="docker-%E2%80%94%E2%80%94-%E4%BB%8E%E5%85%A5%E9%97%A8%E5%88%B0%E5%AE%9E%E8%B7%B5">Docker —— 从入门到实践：http://udn.yyuap.com/doc/docker_practice/index.html</p>
</li>
	<li>国内首个 Docker Hub 镜像服务 DaoCloud 上线 - 开源中国社区  http://www.oschina.net/news/57894/daocloud</li>
</ol>
