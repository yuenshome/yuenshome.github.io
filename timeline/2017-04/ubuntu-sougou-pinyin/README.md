[![header](../../../assets/header28.jpg)](https://yuenshome.github.io)

# ubuntu 16.04下搜狗输入法不能输入中文解决

因为想在Ubuntu上装个语音识别输入法，结果搞坏了，尴尬，最后删除干净<strong><span style="color: #ff0000;">重装就好了</span></strong>。我的问题是由于安装了新的输入法，搜狗无法启用了，就算切换到搜狗但是仍旧是那个新安装的输入法，而且只能输入英文，和转载博文作者还不一样。

[toc]

这篇文章转载自jilijelon@csdn，也有个副标题：
<h1>linux下常见软件崩溃问题解决方案</h1>
之前一段时间正常使用的搜狗输入法突然无法输出中文（<strong>具体现象是，可以呼出搜狗输入法界面，但是候选词列表无显示</strong>），解决之后记录下来，希望能为同样遇到这个问题的人提供参考

我的系统是 ubuntu 16.04 LTS，
搜狗版本： sogoupinyin_2.1.0.0082_amd64，
fcitx版本：1:4.2.9.1-1ubuntu1.16.04.1<!--more-->
<h1>方法一：重启输入法</h1>
通过下面的命令重启搜狗输入法，看重启后是否可以正常使用：
<pre class="lang:sh decode:true">killall fcitx 
killall sogou-qinpanel
fcitx</pre>
<h1>方法二：检查修复安装依赖</h1>
因为我之前是可以正常使用的，所以应该不是安装依赖的问题。刚安装好的朋友如果无法使用，可以用下面的命令排除或修复安装依赖问题：
<pre class="lang:sh decode:true">sudo apt-get install -f</pre>
<h1>方法三：删除配置文件并重新安装</h1>
ubuntu下搜狗的配置文件在 ~/.config下的3个文件夹里：
<ol>
	<li>SogouPY、</li>
	<li>SogouPY.users、</li>
	<li>sogou-qimpanel</li>
</ol>
删除这3个文件夹，然后重启搜狗。<strong>我使用这种方法后解决了无法输入中文问题</strong>

<span style="color: #ff0000;">然而，我是通过把下面有关Sogou的全部删掉。然后重新安装，解决的（我的问题是由于安装了新的输入法，搜狗无法启用了，就算切换到搜狗但是仍旧是那个新安装的输入法，而且只能输入英文）</span>。
<h1 id="小记">小记</h1>
Linux下软件经常会因为配置问题而崩溃，最直接的反应就是无法正常使用。之前使用MySQL workbench的时候也遇到过这个问题，输入一个连接配置后导致软件崩溃，重启便闪退，最终也是删除了对应的配置文件解决了问题。

因此，<span style="color: #ff0000;">如果linux下正常使用的软件，突然崩溃无法使用，可以尝试删除或修改配置文件的方式尝试解决（其实就是第三种方法，删了重装，哈哈哈）</span>。
<h1>参考</h1>
<ol>
	<li>（方法三）ubuntu 16.04下搜狗输入法不能输入中文解决（linux下常见软件崩溃问题解决方案） - 吉客工作室 - 博客频道 - CSDN.NET
http://blog.csdn.net/jilijelon/article/details/53759965</li>
	<li>搜狗输入法 for linux 安装指南
http://pinyin.sogou.com/linux/help.php</li>
</ol>
