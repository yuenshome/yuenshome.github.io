# Ubuntu常用命令（不断补充中）

个人常用命令集锦，比方查看硬件信息或者文件操作，查找定位等等，由于积累的多了，太长了，所以这里列出目录方便查找。[toc]<!--more-->
<h1>性能&amp;监控</h1>
<ul>
	<li>监控CPU使用情况：watch grep "cpu MHz" /proc/cpuinfo</li>
	<li>监控GPU使用情况：watch -n .1 nvidia-smi</li>
	<li>查看GPU使用情况（安装查看：https://github.com/wookayin/gpustat）：gpustat</li>
	<li>清理未被访问的内存数据：sudo sh -c "sync; echo 3 &gt; /proc/sys/vm/drop_caches"</li>
	<li>查看内存（适用于Xen虚拟机。查看实体内存）：sudo dmidecode -t memory | grep Size</li>
	<li>查看内存：free -m（以MB为单位）或者free -g（以GB为单位）</li>
	<li>杀死用户username的所有进程：pkill -u username</li>
</ul>
<h1>硬件信息</h1>
<h2>CPU</h2>
总核数 = 物理CPU个数 X 每颗物理CPU的核数
总逻辑CPU数 = 物理CPU个数 X 每颗物理CPU的核数 X 超线程数
<ul>
	<li># 查看物理CPU个数：cat /proc/cpuinfo| grep "physical id"| sort| uniq| wc -l</li>
	<li>查看每个物理CPU中core的个数(即核数)：cat /proc/cpuinfo| grep "cpu cores"| uniq</li>
	<li>查看逻辑CPU的个数：cat /proc/cpuinfo| grep "processor"| wc -l</li>
	<li>查看CPU信息（型号）：cat /proc/cpuinfo | grep name | cut -f2 -d: | uniq -c</li>
	<li>参考：Linux查看物理CPU个数、核数、逻辑CPU个数 - emanlee - 博客园http://www.cnblogs.com/emanlee/p/3587571.html</li>
</ul>
<h2>系统信息</h2>
<ul>
	<li>查看当前硬盘分区情况：df -h</li>
	<li>查看系统位数：getconf LONG_BIT 或 uname -a ，i686表示32位</li>
	<li>查看Ubuntu版本：lsb_release -a 或 cat /etc/issue</li>
	<li>查看内核版本：uname -a
<div>Linux jumper-VPCEA48EC 2.6.35-30-generic #61-Ubuntu SMP Tue Oct 11 15:29:15 UTC 2011 i686 GNU/Linux</div>
<div>显示主机名称 内核版本号 内核发行版本  CPU类型 操作系统</div></li>
	<li><span style="color: #ff0000;">每隔1s时间</span>，查看GPU信息：<span style="color: #ff0000;">watch -n 1</span> nvidia-smi</li>
</ul>
<h2>内存</h2>
<ul>
	<li>查看内存信息：cat /proc/meminfo</li>
	<li>查看内存大小：free -m，单位是MB，也可以用top或htop
使用和更多命令参数使用参考：ubuntu 查看内存命令 - jzp12的专栏 - 博客频道 - CSDN.NEThttp://blog.csdn.net/jzp12/article/details/7560450</li>
</ul>
<h1>用户权限</h1>
<ul>
	<li>添加新用户名为zhang：sudo adduser zhang
<span style="color: #ff0000;">注意：不要使用useradd，登陆时候shell有问题(默认登录是sh)</span></li>
	<li>删除用户zhang：sudo userdel zhang</li>
	<li>给zhang用户添加密码：sudo passwd zhang</li>
</ul>
<ul>
	<li>修改文件夹的所属用户和组：sudo chown -R yuens.yuens abc</li>
	<li>给用户abc添加到sudo组：</li>
</ul>
<pre class="lang:sh decode:true">#查看当前用户所属组
groups
#查看指定用户abc所属组
groups abc
#添加用户abc到docker组
sudo usermod -a -G sudo abc</pre>
参考：Docker Got permission denied  http://www.myexception.cn/operating-system/2173364.html

删除sudo分组中的abc用户：sudo vim /etc/group，找到对应sudo分组，删掉用户名abc
<h1>程序代码安装</h1>
<ul>
	<li>ldd命令，查看动态链接库的连接情况</li>
</ul>
一、基本概念

1.1、什么是库

在 windows 平台和 linux 平台下都大量存在着库。本质上来说库是一种可执行的二进制代码(但不可以独立执行)，可以被操作系统载入内存执行。由于 windows 和 linux 的平台不同（主要是编译器、汇编器和连接器 的不同），因此二者库的二进制是不兼容的。这里仅介绍 linux下的库。

1.2、 库的种类

linux 下的库有两种：静态库和共享库（动态库）。二者的不同点在于代码被载入的时刻不同：
<ul>
	<li>静态库的代码在编译过程中已经被载入可执行程序，因此生成的可执行程序体积较大。静态用.a为后缀， 例如： libhello.a</li>
	<li>共享库(动态库)的代码是在可执行程序运行时才载入内存的，在编译过程中仅简单的引用，因此生成的可执行程序代码体积较小。</li>
</ul>
动态通常用.so为后缀， 例如：libhello.so。共享库（动态库）的好处是：不同的应用程序如果调用相同的库，那么在内存里只需要有一份该共享库的实例。为了在同一系统中使用不同版本的库，可以在库文件名后加上版本号为后缀,例如： libhello.so.1.0,由于程序连接默认以.so为文件后缀名。所以为了使用这些库，通常使用建立符号连接的方式。
<pre class="lang:sh decode:true">ln -s libhello.so.1.0 libhello.so.1
ln -s libhello.so.1 libhello.so</pre>
1.3 静、动态库在linux下的生成

以下面的代码为例，生成上面用到的hello库：
<pre class="lang:c decode:true ">/* hello.c */ 
#include "hello.h" 
void sayhello() 
{ 
    printf("hello,world "); 
}</pre>
首先用gcc编绎该文件，在编绎时可以使用任何合法的编绎参数，例如-g加入调试代码等：
<pre class="lang:sh decode:true">gcc -c hello.c -o hello.o</pre>
1、生成静态库 生成静态库使用ar工具，其实ar是archive的意思：
<pre class="lang:sh decode:true">ar cqs libhello.a hello.o</pre>
2、生成动态库 用gcc来完成，由于可能存在多个版本，因此通常指定版本号：
<pre class="lang:sh decode:true">gcc -shared -o libhello.so.1.0 hello.o</pre>
1.4、库文件的命名规范

在 linux 下，库文件一般放在/usr/lib和/lib下：
<ul>
	<li>静态库的名字一般为libxxxx.a，其中 xxxx 是该lib的名称；</li>
	<li>动态库的名字一般为libxxxx.so.major.minor，xxxx 是该lib的名称，major是主版本号，minor是副版本号</li>
</ul>
<span style="font-size: 1rem;">1.5、可执行程序在执行的时候如何定位共享库(动态库)文件 ：</span>

<span style="font-size: 1rem;">当系统加载可执行代码(即库文件)的时候，能够知道其所依赖的库的名字，但是还需要知道绝对路径，此时就需要系统动态载入器 (dynamic linker/loader) </span>

对于 elf 格式的可执行程序，是由 ld-linux.so* 来完成的，它先后搜索 elf 文件的 DT_RPATH 段--&gt;环境变量LD_LIBRARY_PATH—-&gt;/etc/ld.so.cache 文件列表--&gt; /lib/,/usr/lib 目录找到库文件后将其载入内存。如： export LD_LIBRARY_PATH=’pwd’ ，即将当前文件目录添加为共享目录。

1.6、<strong><span style="color: #ff0000;">使用ldd工具</span></strong>，查看可执行程序依赖那些动态库或着动态库依赖于那些动态库：

ldd 命令可以查看一个可执行程序依赖的共享库， 例如
<pre class="lang:sh decode:true">ldd /bin/lnlibc.so.6 
=&gt; /lib/libc.so.6 (0×40021000)/lib/ld-linux.so.2 
=&gt; /lib/ld- linux.so.2 (0×40000000)</pre>
可以看到 ln 命令依赖于 libc 库和 ld-linux 库

1.7、<strong><span style="color: #ff0000;">使用nm工具</span></strong>，查看静态库和动态库中有那些函数名；

（T类表示函数是当前库中定义的，U类表示函数是被调用的，在其它库中定义的，W类是当前库中定义，被其它库中的函数覆盖）：

有时候可能需要查看一个库中到底有哪些函数，nm工具可以打印出库中的涉及到的所有符号，这里的库既可以是静态的也可以是动态的。nm列出的符号有很多， 常见的有三种::
<ul>
	<li>T类：是在库中定义的函数，用T表示，这是最常见的；</li>
	<li>U类：是在库中被调用，但并没有在库中定义(表明需要其他库支持)，用U表示；</li>
	<li>W类：是所谓的“弱态”符号，它们虽然在库中被定义，但是可能被其他库中的同名符号覆盖，用W表示。</li>
</ul>
例如，假设开发者希望知道上文提到的hello库中是否引用了 printf():
<pre class="lang:sh decode:true ">nm libhello.so | grep printf
#或者
nm -D libhello.so
</pre>
发现printf是U类符号，说明printf被引用，但是并没有在库中定义。由此可以推断，要正常使用hello库，必须有其它库支持，使用ldd工具查看hello依赖于哪些库：
<pre class="lang:sh decode:true">ldd libhello.so
libc.so.6=&gt;/lib/libc.so.6(0x400la000)
/lib/ld-linux.so.2=&gt;/lib/ld-linux.so.2 (0x40000000)</pre>
<h1>文件基本操作</h1>
<h2>文件大小</h2>
<ul>
	<li>查看当前路径下XXX文件的大小：du -sh XXX</li>
</ul>
<h2>文件执行权限</h2>
要让文件有能够执行的权限：chmod +x ./abc.sh
<h2>文件计数</h2>
<ul>
	<li>按照时间列出文件：ls -Stl</li>
	<li>按照友好方式列出文件大小：ls -hl</li>
	<li>统计当前文件夹下<span style="color: #ff0000;">文件的个数</span>:ls -l |grep "^-"|wc -l</li>
	<li>统计当前文件夹下<span style="color: #ff0000;">目录的个数</span>:ls -l |grep "^-"|wc -l</li>
	<li>统计当前文件夹下<span style="color: #ff0000;">文件的个数</span>，包括子文件夹里的:ls -lR|grep "^-"|wc -l</li>
	<li>统计文件夹下<span style="color: #ff0000;">目录的个数</span>，包括子文件夹里的:ls -lR|grep "^d"|wc -l</li>
</ul>
<h2>md5对比</h2>
MD5算法常常被用来验证网络文件传输的完整性，防止文件被人篡改。MD5 全称是报文摘要算法（Message-Digest Algorithm 5），此算法对任意长度的信息逐位进行计算，产生一个二进制长度为128位（十六进制长度就是32位）的“指纹”（或称“报文摘要”），不同的文件产生相同的报文摘要的可能性是非常非常之小的。

在linux或Unix上，md5sum是用来计算和校验文件报文摘要的工具程序。一般来说，安装了Linux后，就会有md5sum这个工具，直接在命令行终端直接运行。
<ul>
	<li>生成文件a的md5：md5sum ./a.txt</li>
	<li>生成a文件的md5并保存为文件：md5sum ./a.txt &gt;&gt; ./a.txt.md5</li>
	<li>md5sum [OPTION]... [FILE]...-b 或 --binary :把输入文件作为二进制文件看待。
-t 或 --text :把输入的文件作为文本文件看待（默认）。
-c 或 --check :用来从文件中读取md5信息检查文件的一致性。(不细说了参见info)
--status :这个选项和check一起使用,在check的时候，不输出，而是根据返回值表示检查结果。
-w 或 --warn :在check的时候，检查输入的md5信息又没有非法的行，如果有则输出相应信息。</li>
</ul>
ps:感觉这个-c参数用的时候有点问题
<ul>
	<li>参考：linux比较两个文件是否一样(linux命令md5sum使用方法)_LINUX_操作系统_脚本之家http://www.jb51.net/LINUXjishu/123859.html</li>
	<li>Linux下md5sum 校验文件完整性_Linux教程_Linux公社-Linux系统门户网站
http://www.linuxidc.com/Linux/2014-03/98416.htm</li>
</ul>
<h2>挂载硬盘</h2>
<ul>
	<li>拷贝数据到移动硬盘，先查看当前的设备有哪些没有mount：sudo fdisk -l</li>
	<li>注意NTFS的硬盘格式也可以挂载但需要安装其他的软件（我查了下比较麻烦），还好我的盘是exFAT格式。</li>
	<li>然后创建一个文件夹data1在根目录，并把这个设备mount到这个文件夹：
sudo mkdir /data1
sudo mount /dev/sdc1 /data1
此时，进入根目录/data1就能看到这个硬盘设备里的东西了。如果是多个分区可能会有多个/dev/sdc1，/dev/sdc2之类的，可以逐个挂载。</li>
	<li><span style="color: #ff0000;">拷贝完成数据或者操作后，需要umount硬盘</span>：sudo umount /data1
没有提示表示成功卸载设备，可以用sudo fdisk -l命令再次检查一下是否卸载成功，完成后即可拔下硬盘设备。</li>
	<li>参考：Linux mount命令 | 菜鸟教程http://www.runoob.com/linux/linux-comm-mount.html
Linux上挂载NTFS分区 - jiu~ - 博客园
http://www.cnblogs.com/jiu0821/p/6489249.html</li>
</ul>
<h2>建立软连接</h2>
ln的链接分为软链接和硬链接两种。
<ul>
	<li>1. 软链接就是ln –s ** **，它只会在你选定的位置上生成一个文件的镜像，不会占用磁盘空间</li>
	<li>2. 硬链接ln ** **，没有参数-s， 它会在你选定的位置上生成一个和源文件大小相同的文件
无论是软链接还是硬链接，文件都保持同步变化。</li>
</ul>
如果你用ls察看一个目录时，发现有的文件后面有一个@的符号，那就是一个用ln命令生成的文件，用ls –l命令去察看，就可以看到显示的link的路径了。

Linux/Unix 档案系统中，有所谓的连结(link)，我们可以将其视为档案的别名，而连结又可分为两种 : 硬连结(hard link)与软连结(symbolic link)，硬连结的意思是一个档案可以有多个名称，而软连结的方式则是产生一个特殊的档案，该档案的内容是指向另一个档案的位置。硬连结是存在同一个档案系统中，而软连结却可以跨越不同的档案系统。

<strong>范例：</strong>
<strong>例1：将档案 yy 产生一个 symbolic link : zz</strong>
ln -s yy zz
文件夹建立软链接（用绝对地址）
<strong>ln <span style="color: #ff00ff;">-s</span></strong> <span style="color: #ff0000;">源文件地址(当前文件地址)</span> <span style="color: #00ccff;">目的地址(待建立链接的地址)</span>
比如我把Linux文件系统rootfs_dir软链接到/home/jyg/目录下
<strong>ln <span style="color: #ff00ff;">-s</span> </strong><span style="color: #ff0000;">/opt/linux/rootfs_dir</span> <span style="color: #00ccff;">/home/jyg/rootfs_dir</span>就可以了
<strong>例2：将档案 yy 产生一个 hard link : zz</strong>
ln yy xx
更多参数使用见参考。
ubuntu文件夹建立软链接方法 - - 博客频道 - CSDN.NET
http://blog.csdn.net/jiuyueguang/article/details/9350859
<h2>文件拷贝rsync(这个命令有点诡异==，能用scp就用scp，进度通过du -sh来查看)</h2>
<ul>
	<li><strong><span style="color: #ff0000;">边拷贝边查看进度（注：cp命令无法查看命令）</span></strong>：rsync -ah --progress /home/liuyinan_power_p100.tar.gz ./
-a：选项表示归档模式，它会保持权限、所有关系和符号（软）链接。
-H：选项用来保持硬链接。注意源目录和目标目录都可以在远程主机上。
--progress：选项用来在拷贝过程中显示进度和速度以及剩余时间。
<p lang="en-US" xml:lang="en-US">–partial参数的解释是这样说的：keep <wbr />partially <wbr />transferred <wbr />files。
因此，最终实现断点续传的参数就是rsync的–partial</p>
</li>
	<li>rsync <span class="pun">-</span><span class="pln">aH </span><span class="pun">--</span><span class="kwd">delete</span> <span class="pun">--</span><span class="kwd">link</span><span class="pun">-</span><span class="pln">dest</span><span class="pun">=</span><span class="pln">yesterdaystargetdir sourcedir todaystargetdir
</span>--link-dest：使每日备份只花费很少的额外空间和很短的时间。这个选项指定前一天的备份目录，以及今天的备份目录，然后 rsync 会创建今天的新备份目录，并将昨天备份目录里的每一个文件在今天的备份目录中创建硬链接。现在我们在今天的备份目录中有一大堆指向昨天备份的硬链接。文件没有被重复创建，而是创建了一些硬链接。对于<a class="ext" href="https://en.wikipedia.org/wiki/Hard_link" target="_blank" rel="external nofollow">硬链接</a>，在 Wikipedia 中有非常详细的描述。而在用昨天的备份目录文件的硬链接创建了今天的备份之后，rsync 和平常一样进行备份，如果在文件中检测到了变化，就不会做硬链接，而是从昨天的备份目录里复制一个文件的副本，再把源文件中变化的部分复制过去。</li>
	<li>rsync <span class="pun">-</span><span class="pln">aH </span><span class="pun">--</span><span class="kwd">delete</span> <span class="pun">--</span><span class="pln">exclude </span><span class="typ">Cache</span> <span class="pun">--</span><span class="kwd">link</span><span class="pun">-</span><span class="pln">dest</span><span class="pun">=</span><span class="pln">yesterdaystargetdir sourcedir todaystargetdir
--exclude：选项排除一些不想要备份的目录或者文件。用上面的新命令来排除浏览器的缓存Cache。
</span></li>
	<li>参考：使用 rsync 来备份 Linux 系统-观点 ◆ 热议|Linux中国开源社区https://linux.cn/article-8237-1.html</li>
</ul>
<h2>文件拷贝cp</h2>
<span style="color: #ff0000;"><strong>第一种：实现同名文件重命名的拷贝
</strong></span>给出shell脚本（但实际这段脚本还是有点问题，实现的是第三种）：
<pre class="lang:sh decode:true ">#!/bin/bash
from_path="/home/enyu/222/"
to_path="/home/enyu/111/"
file=`ls ${from_path}`
for filename in $file
do
    if [ -e "${target_path}${filename}" ];then
        cp ${from_path}${filename} ${to_path}${filename}_old
    else
        cp ${from_path}${filename} ${to_path}${filename}
    fi
done</pre>
<ul>
	<li>参考：求助，shell中文件cp时自动重命名问题-CSDN论坛-CSDN.NET-中国最大的IT技术社区
<div class="line number1 index0 alt2">

<code class="cpp plain"><code class="cpp plain"> http://bbs.csdn.net/topics/330057847</code></code>

</div>
<span style="color: #ff0000;">第二种：实现同名文件不覆盖的拷贝</span>：shell&gt;awk 'BEGIN { cmd="cp -ri a1/* a2/"; print "n" |cmd; }'
注：a1为原文件夹，a2为目标文件夹
<span style="color: #ff0000;">第三种：实现同名文件覆盖的拷贝</span>：shell&gt;cp -Rf a1 a2
基本用法：
cp [-adfilprsu] 来源档(source) 目的檔(destination)
cp [options] source1 source2 source3 .... directory
参数：
-a   ：相当于 -pdr 的意思；
-d   ：若来源文件为连结文件的属性(link file)，则复制连结文件属性而非档案本身；
-f   ：为强制 (force) 的意思，若有重复或其它疑问时，不会询问使用者，而强制复制；
-i   ：若目的檔(destination)已经存在时，在覆盖时会先询问是否真的动作！
-l   ：进行硬式连结 (hard link) 的连结档建立，而非复制档案本身；
-p   ：连同档案的属性一起复制过去，而非使用预设属性；
-r   ：递归持续复制，用于目录的复制行为；
-s   ：复制成为符号连结文件 (symbolic link)，亦即『快捷方式』档案；
-u   ：若 destination 比 source 旧才更新 destination ！
最后需要注意：
1)如果来源档有两个以上，则最后一个目的文件一定要是『目录』才行！
2)由于 cp 有种种的档案属性与权限的特性，所以，在复制时，您必须要清楚的了解到：
• 是否需要完整的保留来源档案的信息？
• 来源档案是否为连结档 (symbolic link file)？
• 来源档是否为特殊的档案，例如 FIFO, socket 等？
• 来源文件是否为目录？2.cp覆盖目标文件
shell&gt;cp -Rf a1 a2
3.cp不覆盖目标文件
shell&gt;awk 'BEGIN { cmd="cp -ri a1/* a2/"; print "n" |cmd; }'
注：a1为原文件夹，a2为目标文件夹
参考：cp命令实现不覆盖 - xiaoxiaozhu2010的专栏 - 博客频道 - CSDN.NEThttp://blog.csdn.net/xiaoxiaozhu2010/article/details/17164361</li>
</ul>
<h1>Qt5安装</h1>
sudo apt install -y qt-default qtcreator qtdeclarative5-dev
<h1>软件压缩与解压</h1>
<h2>7z格式</h2>
<ul>
	<li><span style="color: #ff0000;">压缩为7z格式</span></li>
	<li>首先安装：sudo apt-get install p7zip p7zip-full p7zip-rar</li>
	<li>将yajiu.jpg和yajiu.png压缩成一个7z包：7z a yajiu.7z yajiu.jpg yajiu.png</li>
	<li>将所有.jpg的文件压缩成一个7z包：# 7z a yajiu.7z *.jpg</li>
	<li>将文件夹yajiu压缩成一个7z包：7z a yajiu.7z yajiu</li>
	<li>参考：Linux 下压缩与解压.zip和.rar及.7z文件-kowems-ChinaUnix博客http://blog.chinaunix.net/uid-27666459-id-3993688.html</li>
	<li><span style="color: #ff0000;">解压7z</span></li>
	<li>解压到当前目录下：7z x ./xxx.7z</li>
	<li>解压到指定目录下：7z x ./xxx.7z ~/Software</li>
	<li>参考：Ubuntu Linux 安装 .7z 解压和压缩文件 - zqlovlg的专栏 - 博客频道 - CSDN.NEThttp://blog.csdn.net/zqlovlg/article/details/8033456</li>
</ul>
<h2>tar.gz</h2>
<ul>
	<li><span style="color: #ff0000;">解压tar.gz</span></li>
	<li>比方下载aliedit.tar.gz，首先解压：tar -zxvf aliedit.tar.gz，或者 tar -xvf aliedit.tar.gz</li>
	<li>之后安装：./aliedit.sh</li>
</ul>
<h2>.bz2</h2>
据说这种压缩率很高：
<ul>
	<li>解压abc.tar.bz2：tar -xjf abc.tar.bz2</li>
	<li>压缩当前路径下所有jpg文件为bz2格式的压缩包abc.tar.bz2：tar -cjf abc.tar.bz2 *.jpg</li>
</ul>
<h2>.tar后缀</h2>
解压：tar -xvf xxxxx.tar
<h2>rar</h2>
<ul>
	<li>需先安装：sudo apt-get install unrar</li>
	<li>压缩/etc目录为etc.rar：rar a etc.rar /etc</li>
	<li>解压etc.rar：
<ul>
	<li>rar x etc.rar</li>
	<li>unrar -e etc.tar</li>
</ul>
</li>
</ul>
<h2>gz解压</h2>
<ul>
	<li>方法一：gzip -b java.gz</li>
	<li>方法二：使用zcat 命令，然后将标准输出 保存文件 zcat java.gz &gt; java.java</li>
	<li>参考：https://www.cnblogs.com/wangshouchang/p/7748527.html</li>
</ul>
<h1>查找与定位</h1>
<ul>
	<li>统计某文档行数：wc -l /etc/httpd/conf/httpd.conf</li>
	<li>查找当前路径下的所有<strong>文件内容中</strong>包含XXX名字的文件：<b>grep -rn "XXX"或<b>grep -rn XXX（不需要前后加上*）</b></b></li>
	<li>查找当前路径下的所有<strong>文件名</strong>包含XXX的文件：<b>find . -name "*XXX*"</b></li>
	<li>查询软件安装位置（这里以sphinxsearch为例）：whereis sphinxsearch</li>
	<li>在vim中查找关键词xxx，进入vim后输入?然后输入xxx回车，输入小写n（跳到上一个关键词的位置），输入大写N，跳到下一个关键词的位置</li>
	<li>more命令，功能类似 cat ，cat命令是整个文件的内容从上到下显示在屏幕上。 more会以一页一页的显示方便使用者逐页阅读，而最基本的指令就是按空白键（space）就往下一页显示，按 b 键就会往回（back）一页显示，而且还有搜寻字串的功能 。more命令从前向后读取文件，因此在启动时就加载整个文件。（该命令还有更多用法，见参考）
参考：linux more命令的使用 - Linux操作系统：Ubuntu_Centos_Debian - 红黑联盟
http://www.2cto.com/os/201310/250257.html</li>
</ul>
<h1>任务管理进程</h1>
<ul>
	<li>查询进程是否开启（这里以search为例）：ps -aux | grep searchd</li>
	<li>强行停止正在运行的进程：先查看需要停下进程的pid（如22222），然后用命令kill 22222</li>
	<li>查看当前运行的所有进程：top或htop</li>
	<li>查看当前用户正在进行的任务：ps -a</li>
	<li>通过sub-shell让进程在后台运行HUP（hangup），例如：(python ./test.py &amp;)
更多详细Linux 技巧：让进程在后台可靠运行的几种方法
http://www.ibm.com/developerworks/cn/linux/l-cn-nohup/
<strong>hangup 名称的来由</strong>：在 Unix 的早期版本中，每个终端都会通过 modem 和系统通讯。当用户 logout 时，modem 就会挂断（hang up）电话。 同理，当 modem 断开连接时，就会给终端发送 hangup 信号来通知其关闭所有子进程。</li>
	<li>将脚本在后台执行，并将控制台的输出结果在新建的log文件中保存：(./test.sh &gt; ./test.log &amp;)，如果是追加方式，则将&gt;改为&gt;&gt;，即：(./test.sh &gt;&gt; ./test.log &amp;)</li>
	<li>挂起正在执行的任务：ctrl+z，此时进程会在后台挂起并停下。再用fg可以将后台挂起的进行重新放回前台，也可以用bg将挂起的进程放在后台运行（但其实这样会占用当前的终端），fg则是将挂起的进程放在前台执行。</li>
	<li>杀死命令中带有LOCAL=NO的命令：ps -ef|grep <span style="color: #ff6600;">LOCAL=NO</span>|grep -v grep|cut -c 9-15|xargs kill -9
参考：Linux下批量Kill多个进程的方法_LINUX_操作系统_脚本之家
http://www.jb51.net/LINUXjishu/43534.html
<h2 id="N100C0">灵活运用 CTRL-z</h2>
在我们的日常工作中，我们可以用 CTRL-z 来将当前进程挂起到后台暂停运行，执行一些别的操作，然后再用 fg 来将挂起的进程重新放回前台（也可用 bg 来将挂起的进程放在后台，也可用fg将挂起的进程放到前台，比方之前挂起的是vim）继续运行。这样我们就可以在一个终端内灵活切换运行多个任务，这一点在调试代码时尤为有用。因为将代码编辑器挂起到后台再重新放回时，光标定位仍然停留在上次挂起时的位置，避免了重新定位的麻烦。</li>
</ul>
<h1>软件安装下载卸载</h1>
<ul>
	<li>apt-get install因缺少依赖包安装失败：sudo apt-get -f install</li>
	<li>删除（yuenshome）文件夹（首先获取权限）：<span style="color: #454545;">sudo chmod 777 yuenshome; </span>rm -rf yuenshome</li>
	<li>Linux端点续传后台下载命令wget -c http://123.com/abc.deb</li>
	<li>彻底卸载某软件softname1（见后文）</li>
	<li>从根目录查找包含cuda的文件夹或文件：find / | grep cuda</li>
	<li>查看本机ip地址：ifconfig -a</li>
</ul>
<!--more-->
<h2>Linux端点续传后台下载命令wget</h2>
命令wget -c -b http://abc.com/file.zip，断点续传下载（-c参数表示继续下载，-b代表后台下载，产生下载日志wget-log文件）。

参考
<ul>
	<li>wget 使用的15个震撼例子 - Ant - 博客频道 - CSDN.NET  http://blog.csdn.net/mathgeophysics/article/details/9399119</li>
</ul>
<h2>彻底卸载某软件softname1</h2>
找到此软件名称，然后sudo apt-get purge softname1，purge参数为彻底删除文件，然后sudo apt-get autoremove，sudo apt-get clean和dpkg -l |grep ^rc|awk '{print $2}' | sudo xargs dpkg -P 两条命令来清除残余的配置文件。再来一次sudo apt-get remove --purge softname1,然后用sudo apt-get update来更新一新软件信息。

参考
<ul>
	<li>ubuntu彻底卸载软件_百度知道</li>
	<li>http://zhidao.baidu.com/link?url=An0KLuefJv8-DUbQ7VbLF1i_i5SjT3NFd0IF2DjK4-Ou-w2I8XRXoKpHHYjfE-hZyapQBtG2baY8sa5cbK4ywq</li>
	<li>Terminal下查看Ubuntu操作系统位数及版本_Linux教程_Linux公社-Linux系统门户网站
http://www.linuxidc.com/Linux/2014-05/101121.htm</li>
</ul>
<h1>服务器运维</h1>
<ul>
	<li>要封停一个IP，使用下面这条命令：
iptables -I INPUT -s ***.***.***.*** -j DROP
要解封一个IP，使用下面这条命令:
iptables -D INPUT -s ***.***.***.*** -j DROP
参考：安全组规则的设置|云服务器 ECS - 开发者论坛
https://bbs.aliyun.com/read/285602.html</li>
	<li>关机命令，30小时后自动关机：sudo shutdown -t $[30*60]</li>
	<li>取消关机命令：sudo shutdown -c</li>
	<li>立刻关机/重启：sudo shutdown now，sudo reboot now</li>
	<li>远程连接服务器，带端口：ssh -p 4499 yuens@127.0.0.1</li>
	<li>不带端口：ssh yuens@127.0.0.1</li>
	<li>将远端的localhost:8888端口映射到本地8008端口：ssh -L8008:localhost:8888 yuens@121.121.121.121</li>
	<li>跨过跳板机连接服务器：ssh yuens@目标服务器IP -p 22 -o ProxyCommand='ssh -p 22 yuens@跳板机IP -W %h:%p'
目前的问题是密码需要输入两次，==
参考：ssh如何通过跳板机直接访问到后端服务器(Mac&amp;Linux&amp;Windows解决方案) - 永福的博客https://my.oschina.net/foreverich/blog/657075</li>
	<li><span style="color: #ff0000;">注：scp不支持断点续传</span></li>
	<li>发送文件到服务器：scp ./caffe.zip yuens@111.111.111.111:~/download/</li>
	<li>指定端口发送（-P，p大写）：scp -P 4500 ./caffe.zip yuens@111.111.111.111:~/download/</li>
	<li>从服务器下载文件到本地：scp <strong>yuens@111.111.111.111:~/download/caffe.zip ./</strong></li>
	<li>跨服务器拷贝文件abc.zip：
<ul>
	<li>先执行：scp -P 4510 abc.zip yuens@1.1.1.1:~/</li>
	<li>然后键盘输入：ctrl + z，挂载任务；</li>
	<li>然后执行：bg，后台继续执行，或者bg &gt; log，将日志输出到log文件里。退出shell也不会停掉scp。</li>
</ul>
</li>
</ul>
有两个命令用来上传、下载文件（适合中小文件的上传和下载）
<ul>
	<li>rz：在命令行里输入rz，则会出现弹窗选择本地的文件，可以上传到服务器</li>
	<li>sz：在命令行里输入sz ./bbb/abc.log，则会将bbb/abc.log文件下载到本地</li>
</ul>
如果输入rz和sz提示没有这两个命令，就是没安装：sudo apt install lrzsz
<h1>网络问题</h1>
<ul>
	<li>有一个可能是dns配置不正确：sudo vim /etc/resolv.conf，修改dns配置文件</li>
	<li>和windows一样，linux也有hosts文件，可以在这里编辑：sudo vim /etc/hosts</li>
	<li>ping前需要安装：sudo apt-get install inetutils-ping</li>
</ul>
<h1>环境变量</h1>
<strong>Linux的变量种类</strong>按变量的生存周期来划分，Linux变量可分为两类：
<ol>
	<li>永久的：需要修改配置文件，变量永久生效。</li>
	<li>临时的：使用export命令声明即可，变量在关闭shell时失效。</li>
</ol>
<strong>设置变量有三种方法：</strong>

<strong>2.1 在/etc/profile文件中添加变量【<span style="color: #ff0000;">对所有用户生效(永久的)</span>】</strong>
<ul>
	<li>用VI在文件<span style="color: #ff0000;">/etc/profile文件中增加变量</span>，该变量将会对Linux下<span style="color: #ff0000;">所有用户有效</span>，并且是“<span style="color: #ff0000;">永久</span>的”。</li>
	<li>例如：编辑/etc/profile文件，添加CLASSPATH变量</li>
	<li># vi /etc/profile</li>
	<li>export CLASSPATH=./JAVA_HOME/lib;$JAVA_HOME/jre/lib</li>
	<li>注：修改文件后要想<span style="color: #ff0000;">马上生效</span>还要运行# <span style="color: #ff0000;">source /etc/profile</span>不然只能在下次重进此用户时生效。</li>
</ul>
<strong>2.2 在用户目录下的.bash_profile文件中增加变量【<span style="color: #ff0000;">对单一用户生效(永久的)</span>】</strong>

用VI在用户目录下的.bash_profile文件中增加变量，改变量仅会对当前用户有效，并且是“永久的”。例如：编辑guok用户目录(/home/guok)下的.bash_profile（现在叫做~/.bashrc）
<ul>
	<li>$ vi /home/guok/.bash.profile</li>
</ul>
添加如下内容：
<ul>
	<li>export CLASSPATH=./JAVA_HOME/lib;$JAVA_HOME/jre/lib</li>
</ul>
注：修改文件后要想马上生效还要运行$ source /home/guok/.bash_profile不然只能在下次重进此用户时生效。

<strong>2.3 直接运行export命令定义变量【<span style="color: #ff0000;">只对当前shell(BASH)有效(临时的)</span>】</strong>

在shell的命令行下直接使用[export 变量名=变量值] 定义变量，该变量只在当前的shell(BASH)或其子shell(BASH)下是有效的，shell关闭了，变量也就失效了，再打开新shell时就没有这个变量，需要使用的话还需要重新定义。

<strong>3.环境变量的查看</strong>

<strong>3.1 使用echo命令查看单个环境变量。</strong>例如：
<ul>
	<li>echo $PATH</li>
</ul>
<strong>3.2 使用env查看所有环境变量。</strong>例如：
<ul>
	<li>env</li>
</ul>
<strong>3.3 使用set查看所有本地定义的环境变量。</strong>
<ul>
	<li>unset可以删除指定的环境变量。</li>
</ul>
<strong>4.常用的环境变量</strong>
<ul>
	<li>PATH 决定了shell将到哪些目录中寻找命令或程序</li>
	<li>HOME 当前用户主目录</li>
	<li>HISTSIZE　历史记录数</li>
	<li>LOGNAME 当前用户的登录名</li>
	<li>HOSTNAME　指主机的名称</li>
	<li>SHELL 　　当前用户Shell类型</li>
	<li>LANGUGE 　语言相关的环境变量，多语言可以修改此环境变量</li>
	<li>MAIL　　　当前用户的邮件存放目录</li>
	<li>PS1　　　基本提示符，对于root用户是#，对于普通用户是$</li>
</ul>
<strong>别名的设置与变量设置方法相同，在不同文件中设置可以使其永久生效或者临时生效，</strong><strong>如：可以在/etc/profile文件中添加 alias ls='ls --color=auto'   alise ll='ls -alF' 设置命令ll 和ls</strong>
<h1>shell算术运算</h1>
加减乘除：
<ul>
	<li>$[100+1]</li>
	<li>$[-20-10]</li>
	<li>$[10*10]</li>
	<li>$[120/10]</li>
</ul>
直接执行上面会给出一个数字外加command not found，那个数字就是运算的结果，如果不想看到这个数字可以在前面加上echo命令，如echo $[10+10]，就不会有命令找不到的报错了。

bc为任意精度算数语言提供解释器，即binary calculator，往往用于实现高精度的计算。
<pre class="lang:sh decode:true ">echo "3.14*999." | bc

# 把1000转为2进制
echo 'obase=2;1000' | bc

# 把1000转为8进制
eccho 'obase=8;1000' | bc</pre>
当然直接在命令行输入bc进入交互模式也可以用。

&nbsp;

<audio style="display: none;" controls="controls"></audio>

<audio style="display: none;" controls="controls"></audio>
