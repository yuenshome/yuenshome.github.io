[![header](../../../assets/header29.jpg)](https://yuenshome.github.io)

# Makefile选项CFLAGS、LDFLAGS、LIBS

<div class="postTitle"></div>
<div id="cnblogs_post_body">
<ul>
	<li class="postTitle">转载自：<a href="http://www.cnblogs.com/wanghetao/archive/2013/10/01/3348885.html" target="_blank">Makefile选项CFLAGS,LDFLAGS,LIBS - wanghetao - 博客园</a></li>
</ul>
<em>CFLAGS 表示用于 C 编译器的选项，</em>
<div class="postBody">
<div id="cnblogs_post_body">

<em id="__mceDel">CXXFLAGS 表示用于 C++ 编译器的选项。
这两个变量实际上涵盖了编译和汇编两个步骤。</em>

<strong>CFLAGS</strong>： 指定头文件（.h文件）的路径，如：CFLAGS=-I/usr/include -I/path/include。同样地，安装一个包时会在安装路径下建立一个include目录，当安装过程中出现问题时，试着把以前安装的包的include目录加入到该变量中来。

<strong>LDFLAGS</strong>：gcc 等编译器会用到的一些优化参数，也可以在里面指定库文件的位置。用法：LDFLAGS=-L/usr/lib -L/path/to/your/lib。每安装一个包都几乎一定的会在安装目录里建立一个lib目录。如果明明安装了某个包，而安装另一个包时，它愣是说找不到，可以抒那个包的lib路径加入的LDFLAGS中试一下。

<strong>LIBS</strong>：告诉链接器要链接哪些库文件，如LIBS = -lpthread -liconv

</div>
</div>
</div>
<!--more-->
<div id="cnblogs_post_body">
<div class="postBody">
<div id="cnblogs_post_body">

简单地说，LDFLAGS是告诉链接器从哪里寻找库文件，而LIBS是告诉链接器要链接哪些库文件。不过使用时链接阶段这两个参数都会加上，所以你即使将这两个的值互换，也没有问题。

有时候LDFLAGS指定-L虽然能让链接器找到库进行链接，但是运行时链接器却找不到这个库，如果要让软件运行时库文件的路径也得到扩展，那么我们需要增加这两个库给"-Wl,R"：

LDFLAGS = -L/var/xxx/lib -L/opt/mysql/lib -Wl,R/var/xxx/lib -Wl,R/opt/mysql/lib

如果在执行./configure以前设置环境变量export LDFLAGS="-L/var/xxx/lib -L/opt/mysql/lib -Wl,R/var/xxx/lib -Wl,R/opt/mysql/lib" ，注意设置环境变量等号两边不可以有空格，而且要加上引号（shell的用法）。那么执行configure以后，Makefile将会设置这个选项，链接时会有这个参数，编译出来的可执行程序的库文件搜索路径就得到扩展了。

</div>
</div>
</div>
