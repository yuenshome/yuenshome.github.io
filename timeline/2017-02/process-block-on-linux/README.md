[![header](../../../assets/header09.jpg)](https://yuenshome.github.io)

# Unable to lock the administration directory (/var/lib/dpkg/)，is another process using it?

转载的，ubuntu安装软件时候出现的一个锁问题。通常是在安装某个软件时候有进行另一个软件的安装时候碰到，但是重启后该问题仍然没有得到解决，搜下得到如下解决方案。[toc]
<h1>1. 描述</h1>
无法获得锁 /var/lib/dpkg/lock - open (11 Resource temporarily unavailable)

Unable to lock the administration directory (/var/lib/dpkg/), is another process using it?
<pre class="lang:python decode:true ">root@spark:/home/yuens# apt-get install gdebi
apt-get: /usr/local/lib/anaconda2/lib/liblzma.so.5: no version information available (required by /usr/lib/x86_64-linux-gnu/libapt-pkg.so.5.0)
E: Could not get lock /var/lib/dpkg/lock - open (11: Resource temporarily unavailable)
E: Unable to lock the administration directory (/var/lib/dpkg/), is another process using it?</pre>
<pre class="lang:python decode:true  ">root@spark:/home/yuens# apt-get install -f
apt-get: /usr/local/lib/anaconda2/lib/liblzma.so.5: no version information available (required by /usr/lib/x86_64-linux-gnu/libapt-pkg.so.5.0)
E: Could not get lock /var/lib/dpkg/lock - open (11: Resource temporarily unavailable)
E: Unable to lock the administration directory (/var/lib/dpkg/), is another process using it?
</pre>
<!--more-->
<h1>2. 解决</h1>
<pre class="lang:sh decode:true">sudo rm /var/cache/apt/archives/lock
sudo rm var/lib/dpkg/lock</pre>
<h1>参考</h1>
<ul>
	<li>Unable to lock the administration directory (/var/lib/dpkg/)，is another process using it? - 风采之子的专栏 - 博客频道 - CSDN.NET
http://blog.csdn.net/wjl_hdu/article/details/6980584</li>
</ul>
