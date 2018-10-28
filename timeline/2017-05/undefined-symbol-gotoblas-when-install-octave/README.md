[![header](../../../assets/header02.jpg)](https://yuenshome.github.io)

# 安装octave时出现的undefined symbol: gotoblas错误及解决

安装octave一直出错，报错：symbol lookup error: /usr/lib/libblas.so.3: undefined symbol: gotoblas。具体如下：
<pre class="lang:sh decode:true">/usr/lib/x86_64-linux-gnu/octave/4.0.0/exec/x86_64-pc-linux-gnu/octave-gui: symbol lookup error: /usr/lib/libblas.so.3: undefined symbol: gotoblas
dpkg: error processing package octave (--configure):
 subprocess installed post-installation script returned error exit status 127
Setting up gnuplot5-data (5.0.3+dfsg2-1) ...
Setting up libwxbase3.0-0v5:amd64 (3.0.2+dfsg-1.3ubuntu0.1) ...
Setting up libwxgtk3.0-0v5:amd64 (3.0.2+dfsg-1.3ubuntu0.1) ...
Setting up gnuplot5-qt (5.0.3+dfsg2-1) ...
update-alternatives: using /usr/bin/gnuplot5-qt to provide /usr/bin/gnuplot (gnuplot) in auto mode
update-alternatives: using /usr/bin/gnuplot5-qt to provide /usr/bin/gnuplot5 (gnuplot5) in auto mode
Setting up gnuplot (4.6.6-3) ...
Processing triggers for libc-bin (2.23-0ubuntu7) ...
Processing triggers for menu (2.1.47ubuntu1) ...
Errors were encountered while processing:
 octave
E: Sub-process /usr/bin/dpkg returned an error code (1)</pre>
后来搜到github里有一个人也是在安装某软件时出现了同样的错误，他说把/usr/local/lib下的libopenblas.so.*移除掉就OK了。<!--more-->
<h1>1. 社区提出的解决方法</h1>
I had <code>libopenblas.so.0</code> in <code>/usr/local/lib</code>. That causes the error. I assume that I should remove the version of OpenBLAS I built from the source, since I had one from Ubuntu PPA.

的确如此，之前下载openblas代码自己build了一个版本，应该是版本起冲突了。社区里的人说道：

Ubuntu provided openblas is in your alternatives list. If you built your own then it is /opt/OpenBLAS/libopenblas.so.0 apt-file can help you locate source of that file, but it did not come from either of 2 sources mentioned.

也就是说ubuntu提供了一个openblas在候选列表中，但是如果你自己build了一个版本，那么/opt/OpenBLAS/libopenblas.so.0的apt文件会去找定位这个libopenblas在你机器上的地方，但找到的不是上述两个位置。
<h1>2. 个人想法</h1>
另外，错误说symbol lookup error: /usr/lib/libblas.so.3: undefined symbol: gotoblas，似乎我也可以试试创建一个软连接libblas.so.3文件在/usr/lib/目录下，软链接到上述的自己build的blas目录或者系统候选列表安装的blas目录，这个想法我没有试。
