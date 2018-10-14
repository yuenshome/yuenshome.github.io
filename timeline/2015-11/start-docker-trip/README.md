[![header](../../../assets/header27.jpg)](https://yuenshome.github.io)

# 开启Ubuntu的docker之旅

这篇教程来自docker官网（docker.com）的，在此我只是对其做简单翻译（在有关Docker Hub上的内容我都将会改为在本地实现）。此外，这篇教程不涉及复杂的深入的docker知识讲解，但可作为一篇很好的入门教程。下面是正文，那么就让我们开始！

<a href="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2015/11/logo.png"><img class="aligncenter wp-image-2919 size-full" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2015/11/logo.png" alt="logo" width="291" height="70" /></a>

这个启蒙教程没有技术性含量，尤其是只是对docker感兴趣学习的同学们。通过这个启蒙教程，在完成如下的简单任务后，您会学到基本的docker特性，通过以下任务：
<ul>
	<li>安装Docker</li>
	<li>在容器里运行软件镜像</li>
	<li>在Docker Hub上浏览所需的镜像</li>
	<li>创建自己的镜像并在容器里运行它</li>
	<li>创建一个Docker Hub账号和镜像仓库</li>
	<li>在Docker Hub中创建你自己的镜像</li>
	<li>通过Docker Hub把你制作的镜像分享给其它用户</li>
</ul>
这个启蒙教程的目的是，藉由完成教程中的任务以减少同学们在后期自己操作时出现的问题，也就是说，大家在自己探索之前，先来看看这个教程，照着捯饬一下。完成本教程的时间大概也就45分钟。
<h2>需要有预备知识嘛？</h2>
这个开始是需要在终端窗口使用Docker命令的。但即使如此，也不是说一定要十分精通命令行才行，但你至少要熟悉我们在后面使用到的命令，这没多少所以别担心。<!--more-->
<h2>安装Docker</h2>
1.登陆您的Ubuntu系统后，使用sudo命令获取root权限（或者干脆就通过su命令进入root用户）。
2.验证一下wget这个软件您是否安装。
<pre class="lang:sh decode:true">$ which wget</pre>
如果wget没有安装也不用担心，使用下面的命令先更新一下软件管理器然后将它安装了：
<pre class="lang:sh decode:true">$ sudo apt-get update
$ sudo apt-get install wget</pre>
3.获取最新的Docker包。
<pre class="lang:sh decode:true">$ wget -qO- https://get.docker.com/ | sh</pre>
系统会因为您在命令前加了sudo提示输入密码。在这句命令完成后，Docker以及它的依赖将会被下载及安装。

注意：如果您的公司的网络会过滤掉代理，您可能在通过Docker仓库安装运行apt-key命令失败。我们这里可以通过添加下面这个key，解决该问题：
<pre class="lang:sh decode:true">  $ wget -qO- https://get.docker.com/gpg | sudo apt-key add -</pre>
4.检查docker是否安装正确。
<pre class="lang:sh decode:true">$ docker run hello-world
Unable to find image 'hello-world:latest' locally
latest: Pulling from library/hello-world
535020c3e8ad: Pull complete
af340544ed62: Pull complete
Digest: sha256:a68868bfe696c00866942e8f5ca39e3e31b79c1e50feaee4ce5e28df2f051d5c
Status: Downloaded newer image for hello-world:latest


Hello from Docker.
This message shows that your installation appears to be working correctly.</pre>
为了能生成这条信息，Docker做了如下几步：
<ol>
	<li>Docker客户端连接Docker的守护进程。</li>
	<li>Docker守护进程将“hello-world”镜像从Docker Hub上拉下来。</li>
	<li>Docker守护进程通过拉下的镜像创建一个新容器，这个镜像运行了：可执行的过程，产生了您当前看到的输出。</li>
	<li>Docker守护进程将程序执行的结果输出到Docker客户端，并发送到您的终端。</li>
</ol>
分享一些有关Docker资料的官方链接：
<ul>
	<li>可以通过下面这个连接获取更多docker使用案例：https://docs.docker.com/userguide/</li>
	<li>镜像分享，自动化工作流程，以及更多的docker镜像都可以通过登陆Docker Hub获取：https://hub.docker.com</li>
	<li>可通过下面这个命令来尝试Docker，该命令运行了一个装了Ubuntu的容器: $ docker run -it ubuntu bash</li>
</ul>
<h2>下一步</h2>
到现在为止，我们已经成功地安装了Docker。请继续保持命令窗口打开，我们即将要讲到的是Docker镜像和容器的相关概念。
<h2>学习镜像和容器</h2>
在上一步的安装中，您执行了这样的命令：docker run hello-world。通过这条命令，您完成了使用Docker的核心命令。这条命令包含了三个部分。

<img class="aligncenter wp-image-2917 size-full" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2015/11/container_explainer.png" alt="container_explainer" width="305" height="203" />

我们不妨将这条命令拆解来看看：
<ul>
	<li>docker：告诉操作系统您将要使用docker程序</li>
	<li>run：docker的子命令用来创建或者执行一个Docker容器</li>
	<li>hello-world：告诉Docker要装载到容器的镜像名。这里装载的镜像名为hello-world</li>
</ul>
容器是一个只保留基本功能的linux操作系统，而镜像是一个软件。您可以将镜像装载到容器中使其运行。当您执行docker命令时，Docker软件将会：
<ol>
	<li>检查是否有名为hello-world的（软件）镜像</li>
	<li>若没有，则会从Docker Hub上将此镜像下载下到本地（稍后我们将会讲到Docker Hub）</li>
	<li>若有或者下载到本地完成，则会将此镜像装载到容器并在容器中执行它</li>
</ol>
以上就是它的创建（译者注：准确来说应该是获取。而创建是由镜像创建者所制作，再将镜像上传到Docker Hub的，这样我们才能从Docker Hub下载到），镜像在容器中执行的简单命令，以及最后的退出（在容器中你可以输入exit命令，退出容器到终端）。这就是hello-world所做的。

也正因此，Docker意味码头工人，这么说也更形象。一个镜像可以复杂到是一个数据库，等您往里添加数据以便后期使用，之后再由其它人使用添加数据。

那么，可能有的同学有个疑问，那么是谁创建的这个hello-world镜像呢？实际上，任何人或公司都可以来创建这个执行hello-world的镜像。通过使用Docker，人们创建并分享Docker镜像，您不用担心在您的电脑上这个软件镜像能否跑得起来，因为通过Docker容器，这个软件镜像怎么着都会跑起来的。
<h2>下一步</h2>
是不是讲到这里有点快，害怕吸收不好？别担心。下面我们来讲写Docker好玩儿的东西。
<h2>发现并运行whalesay镜像</h2>
1.将光标移动到终端窗口的$符号后。

2.输入命令：<span style="color: #333333;">docker run docker/whalesay cowsay boo，之后输入回车。</span>这条命令将会在容器中运行whalesay（鲸鱼说话）镜像，执行时您的终端看起来会像下面这样：
<pre class="lang:sh decode:true">$ docker run docker/whalesay cowsay boo
Unable to find image 'docker/whalesay:latest' locally
latest: Pulling from docker/whalesay
e9e06b06e14c: Pull complete
a82efea989f9: Pull complete
37bea4ee0c81: Pull complete
07f8e8c5e660: Pull complete
676c4a1897e6: Pull complete
5b74edbcaa5b: Pull complete
1722f41ddcb5: Pull complete
99da72cfe067: Pull complete
5d5bd9951e26: Pull complete
fb434121fc77: Already exists
Digest: sha256:d6ee73f978a366cf97974115abe9c4099ed59c6f75c23d03c64446bb9cd49163
Status: Downloaded newer image for docker/whalesay:latest
 _____
&lt; boo &gt;
 -----
    \
     \
      \
                    ##        .
              ## ## ##       ==
           ## ## ## ##      ===
       /""""""""""""""""___/ ===
  ~~~ {~~ ~~~~ ~~~ ~~~~ ~~ ~ /  ===- ~~~
       \______ o          __/
        \    \        __/
          \____\______/</pre>
在你第一次执行某个软件镜像时，docker命令将会在您的本地找这个镜像。如若本地没有该镜像，docker将会从Docker Hub中搜寻并获取。

3.当终端不动上述命令执行完成，输入docker images命令饼输入回车。该命令将会列出在本地的所有镜像，当然啦，也会看到我们刚下载的whalesay（鲸鱼说话）镜像在镜像列表中。
<pre class="lang:sh decode:true">$ docker images
REPOSITORY           TAG         IMAGE ID            CREATED            VIRTUAL SIZE
docker/whalesay      latest      fb434121fc77        3 hours ago        247 MB
hello-world          latest      91c95931e552        5 weeks ago        910 B</pre>
当您在容器中运行镜像时，若本地没有，Docker会从Docker Hub上搜寻并下载到本地。下载到本地的镜像可以在您以后使用的时候，省去下载的时间。Docker唯有之前下载的镜像与现在Docker Hub上的同一镜像因在Docker Hub上的源发生变化时，才会重新下载镜像。当然，若因为镜像源的改变，您可以将之前下载到的镜像删掉而用从Docker Hub上下载的最新镜像。稍后您将会了解到更多的内容。

4.让我们来说试试刚刚下载的whalesay（鲸鱼说话）镜像。试试在我们命令最后数输入一个短语，就看看效果，在下面这个例子中我们输入的是boo-boo。
<pre class="lang:sh decode:true">$ docker run docker/whalesay cowsay boo-boo
 _________
&lt; boo-boo &gt;
 ---------
    \
     \
      \     
                    ##        .            
              ## ## ##       ==            
           ## ## ## ##      ===            
       /""""""""""""""""___/ ===        
  ~~~ {~~ ~~~~ ~~~ ~~~~ ~~ ~ /  ===- ~~~   
       \______ o          __/            
        \    \        __/             
          \____\______/</pre>
<h2>下一步</h2>
在这一节中，您学会了如何在Docker Hub上搜寻您需要的镜像（译者注：其实我们并没有登陆Docker Hub的网站或者主页，但当我们docker run hello-world或docker run docker/whalesay cowsay boo-boo这样的docker命令时，docker都会先在本地搜索对应的镜像文件，本地没有的话就自动在Docker Hub上搜寻并下载）。那么现在，您是否准备好了跟我进入下一节——创建一个自己的Docker镜像呢？
<h2>创建一个镜像</h2>
实际上，刚刚我们用到的whalesay（鲸鱼说话）镜像是可以被修改的。如果你想不到非要说些什么的话，那也没啥，不妨让这只鲸鱼开个玩笑。
<pre class="lang:sh decode:true">docker run docker/whalesay cowsay boo-boo</pre>
下面，我们将会对这个whalesay（鲸鱼说话）创建一个新版本，让这只鲸鱼说出“自己想说“的话，别怕别怕，不需要改动多少代码。
<h4>第一步：创建一个Dockerfile文件</h4>
在这一步中，您可以使用您喜欢的编辑器来写一个简单的Dockerfile文件。这个Dockerfile文件其实质描述的是我们要用到的软件做（译者注：原文中说是把软件像面包一样烘焙成镜像）成镜像的一过程。这个Dockerfile文件不只是佐料，它更告诉我们需要装的软件其要用到的环境，或者需要执行的命令。这个Dockerfile就好比食谱秘诀一样，所以短短的。

1.打开命令行窗口。

2.创建一个新路径，键入：mkdir mydockerbuild命令并回车（便创建好了一个名为mydockerbuild的文件夹）。
<pre class="lang:sh decode:true">$ mkdir mydockerbuild</pre>
这个目录作为创建镜像的“背景”或“上下文”（或说是“靠山”或理解为“环境”似乎更合适。反正就是说，没这个作为大哥的文件夹，您这个创建过程就会失败）。这个“上下文”（即创建的这个目录文件夹）包含了您在创建镜像过程中所有的东西。

3.进入刚创建好的文件夹。
<pre class="lang:sh decode:true">$ cd mydockerbuild</pre>
进入mydockerbuild文件夹后能看到是空的（译者注：不信？试试命令：ls）。

4.创建一个名为Dockerfile的文件在当前的（mydockerbuild）目录下。

您可以使用任何编辑器来做创建这个文件的事儿，比方说用vi或nano来创建（译者注：其实直接命令：touch Dockerfile即可）。

5.打开这个Dockerfile文件。（译者注：当然是用编辑器打开啦，比方可以用Ubuntu自带的vi或者gedit，命令：vi Dockerfile）。

6.在这个文件中写这么一行内容：
<pre class="lang:sh decode:true">FROM docker/whalesay:latest</pre>
这个FROM关键字告诉Docker您这个镜像是基于哪个镜像做的。whalesay（鲸鱼说话）镜像挺可爱的，而且还可以通过cowsay这个关键词来进行用户制定的话让鲸鱼输出。所以，我们不妨从这里改起。

7.我们可以给这个镜像加入fortunes这个程序。
<pre class="lang:sh decode:true">RUN apt-get -y update &amp;&amp; apt-get install -y fortunes</pre>
那么这样安装完后，鲸鱼就能说（其实就是打印）出富有哲理的话。上面这段代码就是在镜像中安装这个能输出哲理名言的软件。

8.当镜像装完了所需的软件，您就可以在镜像装载的时候运行装在里面的软件。
<pre class="lang:sh decode:true">CMD /usr/games/fortune -a | cowsay</pre>
这行代码告诉fortune这个软件输出一段积极的话，并把这段话传给cowsay这个程序。

9.检查您刚刚的工作，您Dockerfile文件中的内容应该如下：
<pre class="lang:sh decode:true">FROM docker/whalesay:latest
RUN apt-get -y update &amp;&amp; apt-get install -y fortunes
CMD /usr/games/fortune -a | cowsay</pre>
10.保存后退出Dockerfile。

这时，您已经将所有需要的东西都写在了Dockerfile里，下面就是开始准备构建这一新镜像。
<h4>第二步：通过Dockerfile来构建镜像</h4>
1.现在，在终端中通过键入命令：<span style="color: #333333;">docker build -t docker-whale .，构建一个新镜像（别忘了输入命令最后有一个英文符号的点）。</span>
<pre class="lang:sh decode:true">$ docker build -t docker-whale .
Sending build context to Docker daemon 158.8 MB
...snip...
Removing intermediate container a8e6faa88df3
Successfully built 7d9495d03763</pre>
上面这句命令可能需要等待几秒钟才能跑起来并在终端上输出反馈。在您想通过新镜像做什么之前，我们不妨来看看镜像通过Dockerfile的构建过程。
<h4>第三步：通过Dockerfile构建镜像的过程</h4>
在当前路径，运行命令：docker build -t docker-whale .，将会对Dockerfile文件进行执行，在此过程中会在本地创建一个名为docker-whale的镜像。命令执行会花上大概一分钟的时间，之后输出的结果看起来又长又难懂。但是没关系，这一节我们将会细述这每条输出信息的含义。

首先，Docker将会检查确保构建的所有东西已经俱备。
<pre class="lang:sh decode:true">Sending build context to Docker daemon 158.8 MB</pre>
之后，Docker会装载whalesay镜像。因为刚刚我们成功调用过它，这也意味着我们不需要下载了。
<pre class="lang:sh decode:true">Step 0 : FROM docker/whalesay:latest
 ---&gt; fb434121fc77</pre>
Docker会执行到下一步，也就是更新包管理器apt-get这里。这将会在屏幕上输出很多行内容，这里就不罗列出它们了。
<pre class="lang:sh decode:true">Step 1 : RUN apt-get -y update &amp;&amp; apt-get install -y fortunes
 ---&gt; Running in 27d224dfa5b2
Ign http://archive.ubuntu.com trusty InRelease
Ign http://archive.ubuntu.com trusty-updates InRelease
Ign http://archive.ubuntu.com trusty-security InRelease
Hit http://archive.ubuntu.com trusty Release.gpg
....snip...
Get:15 http://archive.ubuntu.com trusty-security/restricted amd64 Packages [14.8 kB]
Get:16 http://archive.ubuntu.com trusty-security/universe amd64 Packages [134 kB]
Reading package lists...
---&gt; eb06e47a01d2</pre>
之后，Docker会安装fortunes这个软件。
<pre class="lang:sh decode:true">Removing intermediate container e2a84b5f390f
Step 2 : RUN apt-get install -y fortunes
 ---&gt; Running in 23aa52c1897c
Reading package lists...
Building dependency tree...
Reading state information...
The following extra packages will be installed:
  fortune-mod fortunes-min librecode0
Suggested packages:
  x11-utils bsdmainutils
The following NEW packages will be installed:
  fortune-mod fortunes fortunes-min librecode0
0 upgraded, 4 newly installed, 0 to remove and 3 not upgraded.
Need to get 1961 kB of archives.
After this operation, 4817 kB of additional disk space will be used.
Get:1 http://archive.ubuntu.com/ubuntu/ trusty/main librecode0 amd64 3.6-21 [771 kB]
...snip......
Setting up fortunes (1:1.99.1-7) ...
Processing triggers for libc-bin (2.19-0ubuntu6.6) ...
 ---&gt; c81071adeeb5
Removing intermediate container 23aa52c1897c</pre>
最终，Docker完成了构建和其构建过程报告的输出。
<pre class="lang:sh decode:true">Step 3 : CMD /usr/games/fortune -a | cowsay
 ---&gt; Running in a8e6faa88df3
 ---&gt; 7d9495d03763
Removing intermediate container a8e6faa88df3
Successfully built 7d9495d03763</pre>
<h4> 第四步：执行新镜像docker-whale</h4>
在这一步中，您将首先验证刚创建的新镜像是否在您本地的计算机上，之后将其运行起来。

1.首先还是打开命令终端窗口。

2.在终端中输入docker images后回车。

请您熟记这条命令，它会罗列出您本地的镜像文件有哪些。
<pre class="lang:sh decode:true">$ docker images
REPOSITORY           TAG          IMAGE ID          CREATED             VIRTUAL SIZE
docker-whale         latest       7d9495d03763      4 minutes ago       273.7 MB
docker/whalesay      latest       fb434121fc77      4 hours ago         247 MB
hello-world          latest       91c95931e552      5 weeks ago         910 B</pre>
3.执行新镜像。输入docker run docker-whale命令后回车。
<pre class="lang:sh decode:true">$ docker run docker-whale
 _________________________________________
/ "He was a modest, good-humored boy. It  \
\ was Oxford that made him insufferable." /
 -----------------------------------------
          \
           \
            \     
                          ##        .            
                    ## ## ##       ==            
                 ## ## ## ##      ===            
             /""""""""""""""""___/ ===        
        ~~~ {~~ ~~~~ ~~~ ~~~~ ~~ ~ /  ===- ~~~   
             \______ o          __/            
              \    \        __/             
                \____\______/</pre>
当您看到如上面的显示结果时，我们可以说：我们构建了自己的镜像文件，同时这条鲸鱼比之前更聪明了，它可以说出哲理名言来。它可以说出“自己想要说的”话，同时所输入的代码还是如此的短！想必您也发现了这个过程中，我们并没有下载任何东西。这是因为我们可以基于本地的镜像来构建新镜像。
<h2>下一步</h2>
到此，您已经学会了通过写一个Dockerfile文件来构建一个镜像，并在之后在容器中运行该镜像。在下一课中，我们将会学习如何在Docker Hub上分享自己的镜像。
<h2><span style="font-size: 1.28571rem; line-height: 1.6;">创建一个Docker Hub账号和镜像仓库</span></h2>
您已经构建了一个很酷的docker-whale镜像，或许您会想要分享它。在这一节中，我跳过官方教程中注册账号部分，直接从<strong>第二步</strong>在注册账号邮箱验证和在账户中增加仓库开始。
<h4>第二步：验证邮箱及在账号中添加仓库</h4>
在Docker Hub分享前，您需要验证注册账号时填写的邮箱地址。

1.打开邮箱。

2.查看标题名为<span style="color: #333333;">Please confirm email for your Docker Hub account的邮件。如果您在邮箱中没有看到，请检查垃圾邮件箱或者邮箱还没收到再等一会儿。</span>

3.打开该邮件后点击Confirm按钮。浏览器将会打开您在Dcoker Hub里的用户页面。

4.在账户中创建一个仓库。浏览器将会打开Create Repository页面。

5.起一个仓库名并写出该仓库的简短描述。

6.确定该仓库对公众Public是可见的Visibility。当您完成后，您的表单将会像下面这样：

<a href="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2015/11/add_repository.png"><img class="aligncenter wp-image-2934" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2015/11/add_repository.png" alt="add_repository" width="550" height="388" /></a>

7.点击Create按钮完成创建。
<h2>下一步</h2>
到此，您已经成功的创建了一个Docker Hub账户及一个新的仓库。在下一节中，我们将会把我们刚创建的镜像传到仓库中，饼打上标记。
<h2><span style="font-size: 1.28571rem; line-height: 1.6;">通过Docker Hub把你制作的镜像分享给其它用户</span><span style="font-size: 1.28571rem; line-height: 1.6;">
</span></h2>
在这一节中，您标记并上传docker-whale镜像到您刚创建的仓库中。当您做完了这些，您就可以试试能否从您的仓库中将之前上传的镜像下载下来，以供其它用户使用。
<h4>第一步：标记并上传镜像</h4>
1.打开到您的终端窗口。

2.列出当您已有的镜像：
<pre class="lang:sh decode:true">$ docker images
REPOSITORY           TAG          IMAGE ID            CREATED             VIRTUAL SIZE
docker-whale         latest       7d9495d03763        38 minutes ago      273.7 MB
&lt;none&gt;               &lt;none&gt;       5dac217f722c        45 minutes ago      273.7 MB
docker/whalesay      latest       fb434121fc77        4 hours ago         247 MB
hello-world          latest       91c95931e552        5 weeks ago         910 B</pre>
3.找到docker-whale镜像的镜像ID

docker-whale的id是7d9495d03763。在当前，您应该会注意到＂REPOSITORY＂表示了仓库而不是docker-whale的<strong>namespace</strong>。您需要在Docker Hub中将docker-whale的namespace与账户关联起来，这个<strong>namespace</strong>和您的账户名是一样的。

4.使用镜像ID和docker标签给您的docker-whale镜像打上标记。命令如下：

<a style="color: #0f3647;" href="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2015/11/tagger.png"><img class="aligncenter wp-image-2937" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2015/11/tagger.png" alt="" width="550" height="169" /></a>
<ul>
	<li>docker： 告诉操作系统您将用docker程序</li>
	<li>tag：docker程序的子命令，用于标记镜像</li>
	<li>7d9495d03763：您要打标记的镜像ID值</li>
	<li>maryatdocker：您在Docker Hub的账户名</li>
	<li>docker-whale：您的镜像名</li>
	<li>latest：版本标记或标签值</li>
</ul>
这里给出的账户是我们的案例中的，您的账户肯定与这里的不同，是自己的账户名。当您输入完成这一串命令后回车。
<pre class="lang:sh decode:true">$ docker tag 7d9495d03763 maryatdocker/docker-whale:latest</pre>
5.我们可以输入docker images命令查看刚新标记的镜像
<pre class="lang:sh decode:true">$ docker images
REPOSITORY                  TAG       IMAGE ID        CREATED          VIRTUAL SIZE
maryatdocker/docker-whale   latest    7d9495d03763    5 minutes ago    273.7 MB
docker-whale                latest    7d9495d03763    2 hours ago      273.7 MB
&lt;none&gt;                      &lt;none&gt;    5dac217f722c    5 hours ago      273.7 MB
docker/whalesay             latest    fb434121fc77    5 hours ago      247 MB
hello-world                 latest    91c95931e552    5 weeks ago      910 B</pre>
6.在命令行终端中输入docker login命令，登陆Docker Hub账户。输入格式如下：
<pre class="lang:sh decode:true">docker login --username=yourhubusername --password=yourpassword --email=youremail@company.com</pre>
比方我们登陆maryatdocker的账号：
<pre class="lang:sh decode:true">$ docker login --username=maryatdocker --password=uydfiad77fad --email=mary@docker.com
WARNING: login credentials saved in C:\Users\sven\.docker\config.json
Login Succeeded</pre>
7.输入docker push命令，将您的镜像推到新的仓库中。
<pre class="lang:sh decode:true">$ docker push maryatdocker/docker-whale
The push refers to a repository [maryatdocker/docker-whale] (len: 1)
7d9495d03763: Image already exists
c81071adeeb5: Image successfully pushed
eb06e47a01d2: Image successfully pushed
fb434121fc77: Image successfully pushed
5d5bd9951e26: Image successfully pushed
99da72cfe067: Image successfully pushed
1722f41ddcb5: Image successfully pushed
5b74edbcaa5b: Image successfully pushed
676c4a1897e6: Image successfully pushed
07f8e8c5e660: Image successfully pushed
37bea4ee0c81: Image successfully pushed
a82efea989f9: Image successfully pushed
e9e06b06e14c: Image successfully pushed
Digest: sha256:ad89e88beb7dc73bf55d456e2c600e0a39dd6c9500d7cd8d1025626c4b985011</pre>
8.返回您在Docker Hub上的用户页，查看新上传的镜像

<a href="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2015/11/new_image.png"><img class="aligncenter wp-image-2938" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2015/11/new_image.png" alt="new_image" width="550" height="242" /></a>
<h4> 第二步：把镜像从Docker Hub上拉下来</h4>
在这最后一节中，您将把刚刚您上传的镜像从Docker Hub上下载。在开始之前，您需要从您本地将刚上传的镜像删掉。如果您把刚上传的镜像还留在本地，因为这两个镜像是一样的，docker将不会从Docker Hub上下载您刚上传的那个镜像。

1.打开终端。

2.输入docker images命令列出您当前本地中有的镜像。
<pre class="lang:sh decode:true">$ docker images
REPOSITORY                  TAG       IMAGE ID        CREATED          VIRTUAL SIZE
maryatdocker/docker-whale   latest    7d9495d03763    5 minutes ago    273.7 MB
docker-whale                latest    7d9495d03763    2 hours ago      273.7 MB
&lt;none&gt;                      &lt;none&gt;    5dac217f722c    5 hours ago      273.7 MB
docker/whalesay             latest    fb434121fc77    5 hours ago      247 MB
hello-world                 latest    91c95931e552    5 weeks ago      910 B</pre>
为了测试从Docker Hub上下载您刚上传的镜像的过程是成功的，最好把先前的maryatdocker/docker-whale和docker-whale两个镜像从本地系统中删掉。这样接下来就可以通过docker pull命令从Docker Hub上下载到这个镜像。

3.使用docker rmi命令删除marydocker/docker-whale和docker-whale这两个镜像。您可以通过指定ID或名字来删除镜像。
<pre class="lang:sh decode:true">$ docker rmi -f 7d9495d03763
$ docker rmi -f docker-whale</pre>
4.从您的仓库中通过使用docker pull命令下载新的镜像。该命令包含了您在Docker Hub上的用户名。
<pre class="lang:sh decode:true"> docker pull yourusername/docker-whale</pre>
因为我们之前已经删掉了上传的镜像，此时在执行这条命令的时候，Docker会从Docker Hub把这个镜像下载下来。
<pre class="lang:sh decode:true"> $ docker run maryatdocker/docker-whale
  Unable to find image 'maryatdocker/docker-whale:latest' locally
  latest: Pulling from maryatdocker/docker-whale
  eb06e47a01d2: Pull complete
  c81071adeeb5: Pull complete
  7d9495d03763: Already exists
  e9e06b06e14c: Already exists
  a82efea989f9: Already exists
  37bea4ee0c81: Already exists
  07f8e8c5e660: Already exists
  676c4a1897e6: Already exists
  5b74edbcaa5b: Already exists
  1722f41ddcb5: Already exists
  99da72cfe067: Already exists
  5d5bd9951e26: Already exists
  fb434121fc77: Already exists
  Digest: sha256:ad89e88beb7dc73bf55d456e2c600e0a39dd6c9500d7cd8d1025626c4b985011
  Status: Downloaded newer image for maryatdocker/docker-whale:latest
   ________________________________________
  / Having wandered helplessly into a      \
  | blinding snowstorm Sam was greatly     |
  | relieved to see a sturdy Saint Bernard |
  | dog bounding toward him with the       |
  | traditional keg of brandy strapped to  |
  | his collar.                            |
  |                                        |
  | "At last," cried Sam, "man's best      |
  \ friend -- and a great big dog, too!"   /
   ----------------------------------------
                  \
                   \
                    \
                            ##        .            
                      ## ## ##       ==            
                   ## ## ## ##      ===            
               /""""""""""""""""___/ ===        
          ~~~ {~~ ~~~~ ~~~ ~~~~ ~~ ~ /  ===- ~~~   
               \______ o          __/            
                \    \        __/             
                  \____\______/</pre>
<h2>下一步</h2>
您现在已经完成了以上教程，涵盖了如下内容：
<ul>
	<li>安装Docker</li>
	<li>在容器里运行软件镜像</li>
	<li>在Docker Hub上浏览所需的镜像</li>
	<li>创建自己的镜像并在容器里运行它</li>
	<li>创建一个Docker Hub账号和镜像仓库</li>
	<li>在Docker Hub中创建你自己的镜像</li>
	<li>通过Docker Hub把你制作的镜像分享给其它用户</li>
</ul>
实际上，您到现在为止也只是了解了Docker的皮毛。更多Docker的内容，您可以根据自己对Docker的兴趣，从下面两个地方来获取您感兴趣的内容：
<table class="tutorial">
<tbody>
<tr>
<th class="tg-031e">如果您对下面这些感兴趣</th>
<th class="tg-031e">在这里能找到</th>
</tr>
<tr>
<td class="tg-031e">有关Docker生产线的信息</td>
<td class="tg-031e"><a href="http://www.docker.com/products">产品说明书</a></td>
</tr>
<tr>
<td class="tg-031e">在Docker Hub上设置自动构建</td>
<td class="tg-031e"><a href="https://docs.docker.com/docker-hub/">Docker Hub 文档</a></td>
</tr>
</tbody>
</table>
