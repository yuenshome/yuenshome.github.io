[![header](../../../assets/header08.jpg)](https://yuenshome.github.io)

# 系统升级导致MySQL数据库崩溃

昨天发生了一个失误，主要是因为我对系统进行了升级，然后没有想到这牵扯到了后台数据库以及相关的其他组件，就比方说phpmyadmin，在登录界面就出现了异常。虽然最后算半解决，但是还有点问题，可能以后需要手动带sudo权限的方式来启动mysql服务了。<!--more-->

后来我理清了思路，首先再次登陆服务器，我发现在我自己的机器上无法登陆服务器，所以索性就到阿里云上登录服务器，登录服务器之后，我首先检查的就是数据库，先尝试看数据库是什么状态通过service mysql status这个命令，不出所料，目前的状态果然数据库服务是挂掉的。

其实在此之前，我也尝试登录数据库使用mysql -uroot -p命令尝试使用最高权限去登录数据库，但是也失败了，所以我就检查数据库当前服务的状态。

因为本身考虑到和系统升级有关系，我就使用命令sudo apt install -f，去完成修复，由于网络中断而导致升级过程安装某些软件中断的问题。这样修复完成之后，我又使用了刚刚数据库的状态查询还是有问题的。

问题报错如下：
<pre class="lang:sh decode:true">$ service mysql start
Failed to start mysql.service: The name org.freedesktop.PolicyKit1 was not provid
ed by any .service files
See system logs and 'systemctl status mysql.service' for details.</pre>
最后通过不断的百度，还有谷歌，主要是谷歌，虽然没有检查到搜索到与我一样的错误信息，但找到下面几个较为有用的信息：
<ul>
	<li>【没啥用】小心！Ubuntu14.04 升级到16.04 的几个坑 - 阿里云
https://cn.aliyun.com/jiaocheng/132986.html</li>
	<li>【尝试了里面的命令】The MySQL server fails to start: mysql.service failed because the control process exited with error code - Ask Ubuntu
https://askubuntu.com/questions/914946/the-mysql-server-fails-to-start-mysql-service-failed-because-the-control-proces</li>
</ul>
但是我用到了这样一行命令sudo service mysql start，没想到这句话执行后没什么反应，但我再次执行service mysql status查看当前状态时，没想到竟然好了：
<pre class="lang:sh decode:true ">$ service mysql status
● mysql.service - LSB: Start and stop the mysql database server daemon
   Loaded: loaded (/etc/init.d/mysql; bad; vendor preset: enabled)
   Active: active (running) since Mon 2018-07-02 21:51:41 CST; 45min ago
     Docs: man:systemd-sysv-generator(8)
  Process: 6891 ExecStart=/etc/init.d/mysql start (code=exited, status=0/SUCCESS)
   CGroup: /system.slice/mysql.service
           ├─6925 /bin/sh /usr/bin/mysqld_safe
           └─7270 /usr/sbin/mysqld --basedir=/usr --datadir=/var/lib/mysql --plu</pre>
我怀疑是因为上次升级过程中由于网络中断，导致sudo命令出现问题，目前sudo命令执行都会出现下面的反馈信息：
<pre class="lang:sh decode:true">$ sudo abc.sh
sudo: /var/lib/sudo owned by uid 105, should be uid 0</pre>
