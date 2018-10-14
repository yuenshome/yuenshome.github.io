[![header](../../../assets/header31.jpg)](https://yuenshome.github.io)

# Docker常用命令（不断补充中）

先前在内部做过一次关于Docker构建深度学习环境的分享，讲的比较浅，铺开的比较大。主要是一些常用的基本命令。这里做一下记录。[toc]

<!--more-->
<h1>Docker常用命令汇总</h1>
<h2>让同一个Docker容器接受多个用户的外部访问</h2>
发现创建容器后，我想对同一个容器开启两个shell，但是发现在已经attach一个容器后，再attach进入同一个容器，得到的shell是上一个的镜像（新的docker自动被同步成了旧的容器模样。
如：我原本是在vim nginx.conf下编辑内容，新打开的docker容器自动也被同步到vim 编辑页面下，并且连光标位置都一样）。

使用attach命令不行，所以搜了下有人在segmenetfault上问了一样的问题，
<ul>
	<li>attach方式进入容器是进入相同的bash环境，</li>
	<li>如果需要进入独立的环境，可以用另一种方式进入容器：<span style="color: #ff0000;"><code>docker exec -it &lt;id&gt; bash</code></span></li>
</ul>
参考：php - 为什么docker容器一次只能接受一个用户的外部访问？ - SegmentFault
https://segmentfault.com/q/1010000008236129
<h2>1. 基本命令</h2>
<ul>
	<li>启动命令
nvidia-docker run -v /home/rentingting:/home/rentingting -tdi bvlc/caffe:gpu</li>
	<li>连接命令
nvidia-docker attach CONTAINER_ID</li>
	<li>退出命令
键盘ctrl+Q+P，同时摁住多摁几次查询CONTAINER_ID
nvidia-docker ps</li>
	<li>查询所有CONTAINER_ID（包括死掉的）
nvidia-docker ps -a</li>
	<li>让死掉的CONTAINER重启
nvidia-docker start CONTAINER_ID</li>
	<li>将容器打包为镜像：docker commit &lt;container_id&gt; &lt;new_owner/new_image:tag&gt;</li>
	<li>提交镜像到DockerHub（前提先登陆docker login）：docker push &lt;image_owner/repo&gt;</li>
</ul>
<h2>2. 管理命令</h2>
<ul>
	<li>查看所有容器的资源使用情况：docker container stats</li>
</ul>
<h1>Docker使用问题汇总</h1>
<h2>1. 镜像build前后不一致（import mxnet 出现Failed to load libGL.so error libGL.so）</h2>
Failed to load libGL.so error libGL.so: cannot open shared object file: No such file or directory：进入容器后import mxnet as mx失败，错误就是这样，发现是因为创建容器的时候，没有使用nvidia-docker，而是使用的是docker，另外还有个问题是如果没有使用nvidia-docker来创建容器，进入容器无论是使用nvidia-docker还是docker进行attach，里面都无法使用nvidia-smi以及会出现这个错误。所以要养成在docker前加nvidia-的习惯，或者直接使用别名，写到.bashrc里。

所以网上查到的解决方案（Failed to load libGL.so error libGL.so: cannot open shared object file: No such file or directory - 谁与争锋的专栏 - CSDN博客http://blog.csdn.net/lovexieyuan520/article/details/12033569），对我不适用。尽管装了前面的，没有libGL.so的错误，但是还是跑不了MXNet GPU版本的验证安装的代码，可以跑CPU的。

因此，个nvidia-docker设置别名很重要，在当前用户~/.bashrc或者所有用户/etc/profile里给nvidia-docker设置别名，添加如下命令到配置文件中： alias docker='nvidia-docker'。

<audio style="display: none;" controls="controls"></audio>
