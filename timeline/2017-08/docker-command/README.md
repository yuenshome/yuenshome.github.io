[![header](../../../assets/header31.jpg)](https://yuenshome.github.io)

# Docker常用命令（不断补充中）

先前在内部做过一次关于Docker构建深度学习环境的分享，讲的比较浅，铺开的比较大。主要是一些常用的基本命令。这里做一下记录。[toc]

# 1. 基本命令
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

- 让同一个Docker容器接受多个用户的外部访问：发现创建容器后，我想对同一个容器开启两个shell，但是发现在已经attach一个容器后，再attach进入同一个容器，得到的shell是上一个的镜像（新的docker自动被同步成了旧的容器模样。如：我原本是在vim nginx.conf下编辑内容，新打开的docker容器自动也被同步到vim 编辑页面下，并且连光标位置都一样）。`docker exec -it <CONTAINER_ID> bash`。
参考：为什么docker容器一次只能接受一个用户的外部访问？ - SegmentFault
https://segmentfault.com/q/1010000008236129

```shell
docker build -t friendlyname .  # Create image using this directory's Dockerfile
docker run -p 4000:80 friendlyname  # Run "friendlyname" mapping port 4000 to 80
docker run -d -p 4000:80 friendlyname         # Same thing, but in detached mode
docker ps                                 # See a list of all running containers
docker stop <hash>                     # Gracefully stop the specified container
docker ps -a           # See a list of all containers, even the ones not running
docker kill <hash>                   # Force shutdown of the specified container
docker rm <hash>              # Remove the specified container from this machine
docker rm $(docker ps -a -q)           # Remove all containers from this machine
docker images -a                               # Show all images on this machine
docker rmi <imagename>            # Remove the specified image from this machine
docker rmi $(docker images -q)             # Remove all images from this machine
docker login             # Log in this CLI session using your Docker credentials
docker tag <image> username/repository:tag  # Tag <image> for upload to registry
docker push username/repository:tag            # Upload tagged image to registry
docker run username/repository:tag                   # Run image from a registry
```

## 2. 提交与打包

1. 下载perfxlab用户公共仓库（如我事先创建好的，名为public仓库）镜像:
docker pull perfxlab/public:镜像tag，
docker pull perfxlab/public:ubuntu-16.04
2. 下载perfxlab用户私有仓库（如我事先创建好的，名为private仓库）的镜像:
docker login，输入用户名密码，
docker pull perfxlab/private:镜像tag
3. 上传镜像到perfxlab用户的私有仓库（如private）:
需要先登陆:docker login，
另存本地镜像的名字为perfxlab:docker tag OLD_USER/OLD_REPO:OLD_TAG perfxlab/private:NEW_TAG，
将本地镜像推到DockerHub:docker push perfxlab/private:NEW_TAG
4. 上传镜像到perfxlab用户的公共仓库（如public）:
需要先登陆:docker login，
另存本地镜像的名字为perfxlab:docker tag OLD_USER/OLD_REPO:OLD_TAG perfxlab/public:NEW_TAG，
将本地镜像推到DockerHub:docker push perfxlab/private:NEW_TAG

## 3. 管理命令

- 查看所有容器的资源使用情况：docker container stats</li>

## Docker使用问题汇总
1. 镜像build前后不一致（import mxnet 出现Failed to load libGL.so error libGL.so）</h2>
Failed to load libGL.so error libGL.so: cannot open shared object file: No such file or directory：进入容器后import mxnet as mx失败，错误就是这样，发现是因为创建容器的时候，没有使用nvidia-docker，而是使用的是docker，另外还有个问题是如果没有使用nvidia-docker来创建容器，进入容器无论是使用nvidia-docker还是docker进行attach，里面都无法使用nvidia-smi以及会出现这个错误。所以要养成在docker前加nvidia-的习惯，或者直接使用别名，写到.bashrc里。

网上查到的解决方案（Failed to load libGL.so error libGL.so: cannot open shared object file: No such file or directory - 谁与争锋的专栏 - CSDN博客http://blog.csdn.net/lovexieyuan520/article/details/12033569），对我不适用。尽管装了前面的，没有libGL.so的错误，但是还是跑不了MXNet GPU版本的验证安装的代码，可以跑CPU的。

因此，个nvidia-docker设置别名很重要，在当前用户~/.bashrc或者所有用户/etc/profile里给nvidia-docker设置别名，添加如下命令到配置文件中： alias docker='nvidia-docker'。

## 参考

- [Docker容器学习梳理--基础知识（1） - 散尽浮华 - 博客园](http://www.cnblogs.com/kevingrace/p/5252929.html)
- [Docker容器学习梳理--基础知识（2） - 散尽浮华 - 博客园(http://www.cnblogs.com/kevingrace/p/6547616.html)
- [Docker run 命令参数及使用 - 龙哥的博客 - 博客频道 - CSDN.NET](http://blog.csdn.net/alen_xiaoxin/article/details/54694051)
- [Get Started, Part 2: Containers | Docker Documentation](https://docs.docker.com/get-started/part2/)
- [什么是Docker? - Docker入门教程 - docker中文社区](http://www.docker.org.cn/book/docker/what-is-docker-16.html)
- [Docker入门实战DockOne_Docker入门实战全文阅读_百度阅读](https://yuedu.baidu.com/ebook/d817967416fc700abb68fca1?fr=aladdin&key=docker)
- [Docker 学习笔记 — » Kumu's Blog[(http://blog.opskumu.com/docker.html)
- [Home · NVIDIA/nvidia-docker Wiki](https://github.com/NVIDIA/nvidia-docker/wiki#quick-start)

