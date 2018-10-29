[![header](../../../assets/header19.jpg)](https://yuenshome.github.io)

# WordPress建立数据库连接时出错

早上发现打开博客时赫然出现这几个大字：建立数据库连接时出错。其余空白，大写的尴尬。

网上搜了下有关“wordpress建立数据库连接时出错”的内容，感觉不太靠谱（只是一个表象，我想可能有不同的问题导致同一个错误显示信息），需要链接MYSQL，然后修改里面某个表里的值。我把MYSQL密码给忘了==、

ssh登录服务器，再尝试各种不同密码，发现报错似乎不是密码填错。而是：
<pre class="lang:sh decode:true">yuens@iZ280qnmzdzZ:~$ mysql
ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (111)
yuens@iZ280qnmzdzZ:~$ mysql -u root -p
Enter password: 
ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (111)
yuens@iZ280qnmzdzZ:~$ mysql -u root -p
Enter password: 
ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (111)
yuens@iZ280qnmzdzZ:~$ mysql -u root -p
Enter password: 
ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (111)</pre>
搜了下这个Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (111)，发现很多关于这个问题的，但是目标太大，后面有个(111)，缩小范围发现在stackoverflow上有一个和我一样的问题，但是无解==。
再次搜，依然定位到stackoverflow，解决了方案如下：

You're getting that message because you're trying to start MySQL as your normal user, not as root as required.
<div class="post-text">
<pre class="lang:mysql decode:true">sudo service mysql start</pre>
</div>
参考：
<ul>
	<li>ubuntu - mysql functions for days as expected and then gives 'Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (111)' - Stack Overflow
http://stackoverflow.com/questions/14538119/mysql-functions-for-days-as-expected-and-then-gives-cant-connect-to-local-mysq</li>
	<li>wordpress建立数据库连接时出错解决办法 – 邵连虎博客
http://www.shaolianhu.com/14551.html</li>
</ul>
