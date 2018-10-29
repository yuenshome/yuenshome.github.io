[![header](../../../assets/header05.jpg)](https://yuenshome.github.io)

# 通过修改Apache2配置文件修改网站根目录

之前网站的根目录并不是ip地址，需要跳两层目录，比方ip地址是1.1.1.1但我的网站地址是1.1.1.1/a/b，即使在ip地址对应的根目录下设置跳转，但这样就很麻烦，绑定域名后，也是abc.com/a/b。所以一直想配置下网站根目录，改成真实的博客地址。

之前参考过一些Apache配置的博客，但是大部分写的都是Apache，不是Apache2，两个版本之间的差异似乎还是蛮大的。这阵子重新买了域名：yuenshome.space，一下买了十年的。打算好好弄弄博客，说不定以后访问量会很大可以弄弄广告。废话不多说，切入正题：[toc]<!--more-->
<div>
<h1>1. Apache的配置文件</h1>
<div>看了下Apache（Apache服务器一个IP多个站点的配置方法示例_Linux_脚本之家</div>
<div>http://www.jb51.net/article/69256.htm）单个IP配置多站点的方法，真是简单。（详细版本：Ubuntu下Apache多站点虚拟主机配置-linux服务器应用-黑吧安全网</div>
<div>http://www.myhack58.com/Article/sort099/sort0102/2015/67814.htm）。</div>
<div>这些关于Apache的站点配置的博文中都提到了httpd.conf文件，但是我并没有找到，我想肯定是Apache2做了很多的调整和更新：</div>
<div>Get the path of running Apache</div>
</div>
<div>
<pre class="lang:sh decode:true">$ ps -ef | grep apache
apache 12846 14590 0 Oct20 ? 00:00:00 /usr/sbin/apache2</pre>
</div>
<div></div>
<div>
<div>Append -V argument to the path</div>
</div>
<div></div>
<div>
<pre class="lang:sh decode:true">$ /usr/sbin/apache2 -V | grep SERVER_CONFIG_FILE
-D SERVER_CONFIG_FILE="/etc/apache2/apache2.conf"</pre>
以上参考：linux - Where is my httpd.conf file located apache - Stack Overflow
http://stackoverflow.com/questions/12202021/where-is-my-httpd-conf-file-located-apache

</div>
<div>
<div>记得Apache和Apache2的配置文件好像路径不太相同，通过搜索关键词“linux apache2 配置文件”，找到一篇文章（关于Ubuntu配置文件apache2 httpd.conf位置 - Linux操作系统：Ubuntu_Centos_Debian - 红黑联盟</div>
<div>http://www.2cto.com/os/201307/226374.html），文中说道这么一句话：<strong><span style="color: #ff0000;">安装完Apache后的最重要的一件事就是要知道Web文档根目录在什么地方，对于Ubuntu而言，默认的是/var/www</span></strong>。</div>
<div>我就在/etc/apache2目录下搜索包括"/var/www"文档的内容，发现：</div>
<div>
<pre class="lang:sh decode:true">yuens@iZ280qnmzdzZ:/etc/apache2$ grep -rn /var/www
apache2.conf:148:# not allow access to the root filesystem outside of /usr/share and /var/www.
apache2.conf:164:&lt;Directory /var/www/&gt;
sites-available/000-default.conf:12: DocumentRoot /var/www/html
sites-available/default-ssl.conf:5: DocumentRoot /var/www/html</pre>
严格地说，Ubuntu的Apache（或者应该说Linux下的Apache？我不清楚其他发行版的apache软件包）的配置文件是/etc/apache2/apache2.conf，Apache在启动时会自动读取这个文件的配置信息。而其他的一些配置文件，如httpd.conf等，则是通过Include指令包含进来。在apache2.conf中可以找到这些Include行：

</div>
</div>
<div>
<pre class="lang:sh decode:true "># Include module configuration:
Include /etc/apache2/mods-enabled/*.load
Include /etc/apache2/mods-enabled/*.conf
# Include all the user configurations:
Include /etc/apache2/httpd.conf
# Include ports listing
Include /etc/apache2/ports.conf
……
# Include generic snippets of statements
Include /etc/apache2/conf.d/
# Include the virtual host configurations:
Include /etc/apache2/sites-enabled/</pre>
结合注释，可以很清楚地看出每个配置文件的大体作用。当然，你完全可以把所有的设置放在apache2.conf或者httpd.conf或者任何一个配置文件中。Apache2的这种划分只是一种比较好的习惯。

<strong><span style="color: #ff0000;">安装完Apache后的最重要的一件事就是要知道Web文档根目录在什么地方，对于Ubuntu而言，默认的是/var/www。怎么知道的呢？apache2.conf里并没有DocumentRoot项，httpd.conf又是空的，因此肯定在其他的文件中。经过搜索，发现在/etc/apache2/sites-enabled/000-default中</span></strong>，里面有这样的内容：

</div>
<div>
<pre class="lang:sh decode:true">NameVirtualHost *
ServerAdmin webmaster@localhost
DocumentRoot /var/www/
……</pre>
</div>
<h1>2. 修改配置文件./sites-avaliable/000-default</h1>
<blockquote>
<div>这是设置虚拟主机的，对我来说没什么意义。所以我就把apache2.conf里的Include/etc/apache2/sites-enabled/一行注释掉了，并且在httpd.conf里设置DocumentRoot为我的用户目录下的某个目录，这样方便开发。</div>
<div></div>
<div><strong><span style="color: #ff0000;">再看看/etc/apache2目录下的东西。刚才在apache2.conf里发现了sites-enabled目录，而在/etc/apache2下还有一个sites-available目录，这里面是放什么的呢？其实，这里面才是真正的配置文件，而sites-enabled目录存放的只是一些指向这里的文件的符号链接，你可以用ls/etc/apache2/sites-enabled/来证实一下。所以，如果apache上配置了多个虚拟主机，每个虚拟主机的配置文件都放在sites-available下，那么对于虚拟主机的停用、启用就非常方便了：当在sites-enabled下建立一个指向某个虚拟主机配置文件的链接时，就启用了它；如果要关闭某个虚拟主机的话，只需删除相应的链接即可，根本不用去改配置文件。</span></strong></div>
<div></div>
<div>mods-available、mods-enabled和上面说的sites-available、sites-enabled类似，这两个目录是存放apache功能模块的配置文件和链接的。当我用apt-get installphp5安装了PHP模块后，在这两个目录里就有了php5.load、php5.conf和指向这两个文件的链接。这种目录结果对于启用、停用某个Apache模块是非常方便的。</div></blockquote>
<div></div>
<div>以上参考：apache2的目录及配置介绍_天高愉悦_新浪博客</div>
<div>http://blog.sina.com.cn/s/blog_47517dc70102v6ar.html</div>
<div></div>
<div><strong><span style="color: #00ccff;">然后，我就修改了./sites-avaliable/000-default文件，并在/etc/apache2目录下建立完软连接到sites-enabled文件夹中的同名文件后，重启服务。</span></strong></div>
<div>
<pre class="lang:sh decode:true ">yuens@iZ280qnmzdzZ:/etc/apache2$ sudo /etc/init.d/apache2 restart                       
sudo: /var/lib/sudo owned by uid 105, should be uid 0
[sudo] password for yuens: 
Sorry, try again.
[sudo] password for yuens: 
 * Restarting web server apache2
AH00558: apache2: Could not reliably determine the server's fully qualified domain name, using 10.164.24.4. Set the 'ServerName' directive globally to suppress this message
   ...done.</pre>
<h1>3. 重启apache2报错修复</h1>
此时，直接根据ip地址，就可以打开博客首页了。但是上面restart apache2的时候报了一个错，这个错误虽然不影响什么，但是看着别扭，搜了下，解决方案如下：
<pre class="lang:sh decode:true">vim /etc/apache2/apache2.conf</pre>
</div>
<div>最后加入一句 ServerName localhost:80 或者参考这个（两个都一样，一个写是localhost，另一个写的是127.0.0.1，端口都是80）：重启Apache出现警告信息Could not reliably determine the server's fully qualified domain name, - Moolight_shadow的专栏 - 博客频道 - CSDN.NET</div>
<div>http://blog.csdn.net/moolight_shadow/article/details/45066165</div>
<div></div>
<div>此时再次，重启，无错误：</div>
<div>
<pre class="lang:sh decode:true">yuens@iZ280qnmzdzZ:/etc/apache2$ sudo /etc/init.d/apache2 restart
sudo: /var/lib/sudo owned by uid 105, should be uid 0
[sudo] password for yuens: 
* Restarting web server apache2
  ...done.</pre>
<h1>3. 根目录修改导致无法登陆博客后台</h1>
但是会发现一些图片挂了，或者是点击登录后因为web根目录改变，所以都是访问不到的，现在登录进后台数据库，在wordpress的数据表中修改wordpress和站点的后台地址即可。

</div>
<div>修改wordpress在服务器上的绝对路径（似乎也不用修改把，反正我照着下面的做了）</div>
<ul>
	<li>如何修改移动wordpress的安装目录_青岛星雅艺术中心_新浪博客http://blog.sina.com.cn/s/blog_73b90591010193u4.html</li>
</ul>
此时再次打开yuenshome.space，打开静态页面的时候（比方深度学习页面）就会跳到对应页面，但是紧接不到眨眼的时间（当前页面的内容还没出来）立马会跳到121.442.47.99/yuenshome/wordpress，我之前在wordpress后台设置中设定的地址。

我想可能是浏览器端缓存的原因（我还想了可能是服务器缓存，下载了一个wp-super-cache插件，安装时候有点问题，网上也无解放弃），清除了历史记录（这个应该不需要）和缓存数据。
<ol>
	<li>clean browsing data</li>
	<li>clean images and files</li>
</ol>
<div>再次通过域名访问yuenshome.space，点击静态页面（如深度学习等页面）就正常了，不会强制跳转。最后，修改首页地址，这个需要在【外观】-&gt;【菜单】-&gt;【首页(自定义链接)】，进行修改。修改成当前域名就OK。</div>
