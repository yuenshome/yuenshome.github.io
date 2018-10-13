[![header](../../../assets/header35.jpg)](https://yuenshome.github.io)

# 单行函数的SQL语法

内容参考《李兴华Oracle数据库》的单行函数部分（链接见文末参考），本节主要包括：[toc]<!--more-->
<h1>1.上次课程的主要知识点</h1>
<h2>1.1 在Oracle数据库中的四个主要用户：</h2>
<ul>
	<li>超级管理员：sys/change_on_install；</li>
	<li>普通管理员：system/manager；</li>
	<li>普通用户：scott/tiger；</li>
	<li>大数据用户：sh/sh，主要是为了测试性能使用的。</li>
</ul>
<h2>1.2 SQLPlus是Oracle操作的主要工具，其支持的命令：</h2>
<ul>
	<li>格式化命令：SETLINE、SET PAGESIZE，这两个命令只针对SQLPlus有效，程序无效；</li>
	<li>调用记事本：ed、@；</li>
	<li>切换用户：conn 用户名/密码 [AS SYSDBA]。</li>
</ul>
<h2>1.3 给出SQL格式</h2>
<ul>
	<li><strong>SELECT</strong> [DISTINCT] * | 列名称 [别名], 列名称 [别名], …</li>
	<li><strong>FROM</strong> 数据表 [别名]</li>
	<li>[<strong>WHERE</strong> 条件(s)]</li>
	<li>[<strong>ORDER BY</strong> 字段 [ASC | DESC], 字段 [ASC | DESC], …]</li>
</ul>
以上给出的格式执行的顺序：<span style="color: #ff0000;"><strong>FROM</strong><strong>、WHERE</strong><strong>、SELECT</strong><strong>、ORDER BY</strong></span>（永远都是最后一个执行）；

在编写WHERE子句之中可以设置多个判断条件：关系运算符（&lt;、&gt;、&gt;=、&lt;=、=、!=、&lt;&gt;）、逻辑运算符（AND、OR、NOT，若干个条件的连接只能使用AND或OR）、BETWEEN…AND、IN（使用NOT IN的时候不能出现NULL，否则不会有任何的数据返回）、IS NULL、LIKE（LIKE可以使用于各种数据类型，如果没有设置关键字则表示查询全部“%%”）。

ORDER BY子句是唯一可以使用SELECT子句定义的别名的语句。
<h1>2.本次内容</h1>
<ol>
	<li>单行函数（背）</li>
	<li>多表查询；</li>
	<li>分组统计查询。</li>
</ol>
<h1>3.单行函数（<span style="color: #ff0000;"><strong>重点，背</strong></span>）</h1>
单行函数的功能非常简单，就是完成某一个功能，例如：现在希望将字母变为大写，这样的功能就可以通过单行函数完成，在Oracle之中提供的单行函数非常的多，今天主要是看核心的几个。

单行函数分类：字符串函数、数字函数、日期函数、转化函数、通用函数。所有的单行函数在进行记忆的时候要记清楚单行函数的名称、返回值类型、接受的参数数据。
<h2>3.1字符串函数</h2>
字符串函数的主要功能是进行字符串数据处理，下面按照不同的类型来观察函数的使用。

在Oracle之中为方便用户验证函数，专门提供了一个dual的虚拟表，下面将用这张表讲解。
<h3>3.1.1 大小写转化函数</h3>
<ul>
	<li>转大写：字符串UPPER(字符串 | 数据列)；</li>
	<li>转小写：字符串LOWER(字符串 | 数据列)。</li>
</ul>
<h4><strong>3.1.1.1 范例</strong>：验证函数——直接在字符串数据上使用</h4>
<pre class="lang:plsql decode:true">SELECT UPPER(‘Hello’), LOWER(‘Hello’)
FROM dual;</pre>
在Oracle之中为方便用户进行函数验证，专门提供dual的虚拟表
<h4><strong>3.1.1.2 范例</strong>：验证函数——在列上使用（对所有雇员的姓名按照小写字母的方式显示）</h4>
<pre class="lang:plsql decode:true">SELECT LOWER(ename)
FROM emp;</pre>
通过以上验证，发现UPPER()和LOWER()函数操作的时候是不关心原本的字符串是大写还是小写的，都会按照统一的规则转换，那么就可以实际中利用这两个函数进行一些数据处理，某些系统上某些用户名是不区分大小写的，而这样的功能都是在数据保存的时候将保存数据变为统一的大写或小写。
<h4><strong>3.1.1.3 范例</strong>：要求用户输入一个雇员的姓名，而后查询出此雇员的详细信息</h4>
那么此时首先要解决的问题就是如何实现用户的数据输入，在Oracle数据库里提供了一种输入方法：&amp;。
<pre class="lang:plsql decode:true">SELECT *
FROM &amp;tablename; -- 要查询的数据表名称由用户输入</pre>
那么下面就是利用替代变量输入雇员姓名
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE ename=UPPER(‘&amp;name’);</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle7-1.png" alt="" width="515" height="166" />

在工作中若有些系统不区分大小写，那么就要用统一形式保存，使用LOWER()、UPPER()函数进行转换。
<h3>3.1.2 首字母大写其余字母小写：字符串INITCAP串 | 列)</h3>
<h4><strong>3.1.2.1 范例</strong>：将每位雇员姓名的首字母大写，其余字母小写</h4>
<pre class="lang:plsql decode:true">SELECT INITCAP(ename)
FROM emp;</pre>
<h3>3.1.3 取得字符串长度：数字 LENGTH(字符串 | 列)</h3>
<h4><strong>3.1.3.1 范例</strong>：计算字符串长度</h4>
<pre class="lang:plsql decode:true">SELECT LENGTH(‘Hello world’)
FROM dual;</pre>
<h4><strong>3.1.3.2 范例</strong>：查询所有雇员姓名及姓名长度</h4>
<pre class="lang:plsql decode:true">SELECT ename, LENGTH(ename)
FROM dual;</pre>
<h4><strong>3.1.3.3 范例</strong>：查询出所有雇员姓名长度为5的全部雇员信息</h4>
此时一定要加入WHERE子句来进行数据行的控制，同时还需要用LENGTH判断：
<pre class="lang:plsql decode:true">SELECT ename, LENGTH(ename)
FROM emp
WHERE LENGTH(ename)=5;</pre>
<h3>3.1.4 替换指定字符串的内容：字符串REPLACE(字符串 | 列，要替换的内容，新的内容)</h3>
<h4><strong>3.1.4.1 范例</strong>：替换字符串数据</h4>
<pre class="lang:plsql decode:true">SELECT REPLACE(‘hello world’, ‘l’, ‘_’)
FROM dual;</pre>
<h4><strong>3.1.4.2 范例</strong>：替换数据列</h4>
<pre class="lang:plsql decode:true">SELECT ename, REPLACE(ename, ‘A’, ‘_’)
FROM emp;</pre>
<h3>3.1.5 字符串截取操作</h3>
<ul>
	<li>由指定位置截取到结尾：字符串SUBSTR(字符串 | 数据列，截取开始索引)</li>
	<li>指定截取的开始和结束位置：字符串SUBSTR(字符串 |　数据列，截取开始索引，截取结束索引)</li>
</ul>
<h4><strong>3.1.5.1 范例</strong>：字符串截取——由指定位置截取到结尾</h4>
取得world：
<pre class="lang:plsql decode:true">SELECT SUBSTR(‘hello world’, 7)
FROM dual;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle7-2.png" alt="" width="374" height="182" />
<h4><strong>3.1.5.2 范例</strong>：字符串截取——截取部分</h4>
取得hello：
<pre class="lang:plsql decode:true">SELECT SUBSTR(‘hello world’, 0. 5)
FROM dual;</pre>
<pre class="lang:plsql decode:true">SELECT SUBSTR(‘hello world’, 1, 5)
FROM dual;</pre>
实际在Oracle中，字符串索引下标是从1开始的（程序大多是从0开始），但为程序员的使用习惯，即使设置0，也表示从1开始。这点会由Oracle自己处理。
<h4>3.1.5.3 范例：截取每一位雇员姓名的前三位字母</h4>
<pre class="lang:plsql decode:true">SELECT ename, SUBSTR(ename, 1, 3)
FROM emp;</pre>
<h4>3.1.5.4 范例：截取每一位雇员姓名的后三位字母</h4>
<ul>
	<li>传统思路：对于后三位字母，若想取得，那么必须确定截取的开始位置。由于每一位雇员的姓名不同，长度不同，那么截取的开始下表也不同，那么就只能通过计算姓名长度后减去一定的数字（3）才能得到正确的起始下标。</li>
</ul>
<pre class="lang:plsql decode:true ">SELECT ename, SUBSTR(ename, LENGTH(ename)-2)
FROM emp;</pre>
改善思路：在Oracle设计之初，SUBSTR()函数已经考虑到了右后向前的截取。
<pre class="lang:plsql decode:true ">SELECT ename, SUBSTR(ename, -3)
FROM emp;</pre>
<strong>面试题：请问Oracle之中的SUBSTR()函数进行截取的时候是从0开始还是从1开始？</strong>
<ul>
	<li>实际上从0开始还是从1开始没有任何区别，Oracle之中的字符串是从1开始的，但是即使设置了0，也表示从1开始（所以两者一样）。此外，SUBSTR()函数中的下表参数也可设置为负数，表示由后的指定下表开始。</li>
</ul>
<h3>3.1.6 去掉左右空格函数：字符串 TRIM(字符串 | 列)</h3>
使用TRIM函数可以去掉的只是左右空格，但中间的空格是无法取消的。
<pre class="lang:plsql decode:true">SELECT TRIM(‘   hello world   ’), TRIM(‘   hello world   ’)
FROM dual;</pre>
这类操作在进行数据接受的时候非常有用处。
<h2>3.2 数字函数</h2>
数字函数主要是针对数字进行的一些小处理。
<h3>3.2.1 四舍五入函数：数字ROUND(数字 | 列 [, ])</h3>
<h4><strong>3.2.1.1 范例</strong>：验证ROUND函数</h4>
<table>
<tbody>
<tr>
<td rowspan="5" width="206">SELECT ROUND(9615.7652),

ROUND(9615.7652, 2),

ROUND(9615.7652, -2),

ROUND(-15.5)

FROM dual;</td>
<td width="362">结果：9616</td>
</tr>
<tr>
<td width="362">结果：9615.77</td>
</tr>
<tr>
<td width="362">结果：<strong><span style="color: #ff0000;">9600</span></strong></td>
</tr>
<tr>
<td width="362">结果：<strong><span style="color: #ff0000;">16</span></strong></td>
</tr>
<tr>
<td width="362"></td>
</tr>
</tbody>
</table>
如果没有编写保留小数位数，那么就会将小数点之后的数据直接进行进位，不保留小数位。
<h3>3.2.2 截取小数函数：数字 TRUNC(数字 | 列 [, 保留小数位])</h3>
<h4><strong>3.2.2.1 范例</strong>：验证TRUNC()函数</h4>
<table>
<tbody>
<tr>
<td rowspan="5" width="284">SELECT TRUNC(9615.7652),

TRUNC(9615.7652, 2),

TRUNC(9615.7652, -2),

TRUNC(-15.5)

FROM dual;</td>
<td width="284">è9615</td>
</tr>
<tr>
<td width="284">è9615.76</td>
</tr>
<tr>
<td width="284">è9600</td>
</tr>
<tr>
<td width="284">è-15</td>
</tr>
<tr>
<td width="284"></td>
</tr>
</tbody>
</table>
<h3>3.2.3 取模：数字 MOD(数字1 | 列1, 数字2 | 列2)</h3>
<h4><strong>3.2.3.1 范例</strong>：求余数</h4>
<pre class="lang:plsql decode:true">SELECT MOD(10,3)
FROM dual;</pre>
<span style="color: #ff0000;"><strong>以后再工作中，只要见到ROUND</strong><strong>这个单词，一定要清楚其在四舍五入。</strong></span>
<h2>3.3 日期函数</h2>
如果要想操作日期函数，实际上有一个首要的问题需要解决。Oracle里提供两个<span style="color: #ff0000;"><strong>伪列（实不存在表中的列，但可直接使用的列）</strong>：<strong>SYSDATE</strong></span>、SYSTIMESTAMP。
<h3><strong>3.3.0.1 范例</strong>：取得当前系统日期和系统时间</h3>
<pre class="lang:plsql decode:true">SELECT SYSDATE
FROM dual;</pre>
<pre class="lang:plsql decode:true">SELECT SYSTIMESTAMP
FROM dual;</pre>
当有了系统日期之后就可以通过以下的三个公式来进行一些简单的计算：
<ul>
	<li><span style="color: #ff0000;">日期 + 数字 = 日期，表示若干天之后的日期；</span></li>
	<li><span style="color: #ff0000;">日期 – 数字 = 日期，表示若干天之前的日期；</span></li>
	<li><span style="color: #ff0000;">日期 – 日期 = 数字，表示两个日期之间的间隔天数。</span></li>
</ul>
<h3><strong>3.3.</strong><strong>0</strong><strong>.2 范例</strong>：计算若干天之后的日期</h3>
<pre class="lang:plsql decode:true">SELECT SYSDATE + 3, SYSDATE + 120
FROM dual;</pre>
<h3><strong>3.3</strong><strong>.</strong><strong>0</strong><strong>.3 范例</strong>：计算若干天之前的日期</h3>
<pre class="lang:plsql decode:true">SELECT SYSDATE - 120
FROM dual;</pre>
计算的时候是按照自然日的天数进行计算，是不会考虑那个月由30天或28天等。
<h3><strong>3.3.0.4 范例</strong>：统计出每位雇员到今天为止的雇佣日期</h3>
<pre class="lang:plsql decode:true ">SELECT ename, SYSDATE – hiredate
FROM emp;</pre>
如果在一个很长的时间内，通过天计算年，并不准确（不确定365、366），所以想准确地计算出天数，需要计算相距不要超过30天。

如果想准确地进行日期时间的操作，那么需要提供的日期函数。
<h3>3.3.1 计算两个日期之间经历的月数：数字MONTHS_BETWEEN(日期1 | 列1, 日期2 | 列2)</h3>
<h4>3.3.1.1 范例：计算每位雇员到目前的雇佣年数</h4>
年数的计算最准确地一定是通过月数完成的，一年是不会被改变的。
<pre class="lang:plsql decode:true ">SELECT ename, MONTHS_BETWEEN(SYSDATE, HIREDATE)
MONTHS_BETWEEN(SYSDATE, hiredate)/12
FROM emp;</pre>
<pre class="lang:plsql decode:true ">SELECT ename, MONTHS_BETWEEN(SYSDATE, hiredate),
TRUNC(MONTHS_BETWEEN(SYSDATE, hiredate)/12)
FROM emp;</pre>
<h4><strong>3.3.1.2 范例</strong>：查询出所有已经被雇佣了33年以上的雇员</h4>
<pre class="lang:plsql decode:true ">SELECT *
FROM emp
WHERE MONTHS_BETWEEN(SYSDATE, hiredate)/12&gt;33;</pre>
<h3>3.3.2 加上指定月之后的日期：日期 ADD_MONTHS(日期 | 列, 月数)</h3>
<h4><strong>3.3.2.1 范例</strong>：计算若干月后/前（正/负）的日期</h4>
<pre class="lang:plsql decode:true">SELECT SYSDATE + 120, ADD_MONTHS(SYSDATE, 4), ADD_MONTHS(SYSDATE, 500)
FROM dual;</pre>
在进行计算的时候，数字的大小是不受限制的。

<strong>思考题</strong>：要求计算出每一位雇员到今天为止雇佣的年限（理解）
<ul>
	<li>A、例如：今天是2014年08月12日，CLARK的雇佣日期是：1981年06月09日</li>
	<li>B、到今天CLARK已经在公司服务了：33年、02月、03天</li>
</ul>
<strong>第一步：计算出每一位雇员到今天为止的雇佣年份，直接利用月数除以12</strong>
<pre class="lang:plsql decode:true">SELECT ename, hiredate,
TRUNC(MONTHS_BETWEEN(SYSDATE, hriedate)/12) year
FROM emp;</pre>
<strong>第二步：计算月，在进行年计算的时候发现存在有小数，这些小数就包含了月</strong>
<pre class="lang:plsql decode:true">SELECT ename, hiredate,
TRUNC(MONTHS_BETWEEN(SYSDATE, hriedate)/12) year,
TRUNC(MOD(MONTHS_BETWEEN(SYSDATE, hiredate), 12)) months
FROM emp;</pre>
<strong>第三步：计算天数，对于天数的计算实际上只学会一个公式：“日期1 – 日期2 = 天数”，于是分析日期：</strong>

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle7-3.png" alt="" width="392" height="112" />
<ul>
	<li>日期1：应该是当前日期，SYSDATE;</li>
	<li>日期2：应该是剔除了经过月后的日期。</li>
</ul>
<pre class="lang:plsql decode:true">SELECT ename, hiredate,
TRUNC(MONTHS_BETWEEN(SYSDATE, hriedate)/12) year,
TRUNC(MOD(MONTHS_BETWEEN(SYSDATE, hiredate), 12)) months，
TRUNC(SYSDATE – ADD_MONTHS(hiredate, MONTHS_BETWEEN(SYSDATE, hriedate))) day
FROM emp;</pre>
【<strong><span style="color: #ff0000;">为何ADD_MONTHS的结果不TRUNC</span></strong>？】

只有日期函数可准确计算出日期，其它直接操作无法进行。
<h3>3.3.3 求出指定的下一个一周时间数的日期：日期 NEXT_DAY(日期 | 列, 一周时间数)</h3>
一周时间数：周一，周二等等
<h4><strong>3.3.3.1 范例</strong>：计算下一个星期二</h4>
<pre class="lang:plsql decode:true ">SELECT NEXT_DAY(SYSDATE, ‘星期二’)
FROM dual;</pre>
<h3>3.3.4 求出指定日期所在月的最后一天日期：日期 LAST_DAY(日期 | 列)</h3>
<h4><strong>3.3.4.1 范例</strong>：求出本月的最后一天</h4>
<pre class="lang:plsql decode:true">SELECT LAST_DAY(SYSDATE)
FROM dual;</pre>
<h4><strong>3.3.4.2 范例</strong>：查询出每个雇员雇佣所在月倒数第二天雇佣的全部雇员信息</h4>
<ul>
	<li>每一个雇员雇用日期是不同的，那么雇佣所在月也一定是不同的；</li>
	<li>倒数第二天应该使用：“雇佣所在月最后一天-2”，可以通过LAST()函数计算。</li>
</ul>
<pre class="lang:plsql decode:true">SELECT ename, hiredate, LAST_DAY(hiredate)-2
FROM emp
WHERE LAST_DAY(hiredate)-2=hiredate;</pre>
所有的日期函数实际上在进行一些统计操作的时候还是有用处的。
<h2>3.4 转换函数</h2>
目前为止，我们已经接触到Oracle提供的三类数据：字符串、数字、日期，那么这三种数据可通过转换函数实现类型转换。
<h3>3.4.1 转字符串数据：字符串 TO_CHAR(数字 | 日期 | 列, 转换格式)</h3>
在进行转换格式设置的时候，要根据不同的数据类型进行格式标记的定义：
<ul>
	<li>日期：年（yyyy）、月（mm）、日（dd）、时（HH、HH24）、分（mi）、秒（ss）</li>
	<li>数字：一位任意数字（9）、本地货币（L）。</li>
</ul>
<h4><strong>3.4.1.1 范例</strong>：将日期格式化为字符串</h4>
<table>
<tbody>
<tr>
<td width="347">
<pre class="lang:plsql decode:true">SELECT TO_CHAR(SYSDATE, ‘yyyy-mm-dd’) FROM dual;</pre>
</td>
<td width="221">2014-08-12</td>
</tr>
<tr>
<td width="347">
<pre class="lang:plsql decode:true">SELECT TO_CHAR(SYSDATE, ‘fmyyyy-mm-dd’) FROM dual;</pre>
</td>
<td width="221">2014-8-12</td>
</tr>
</tbody>
</table>
<h4><strong>3.4.1.2 范例</strong>：在Oracle的SYSDATE里，包含有时间数据，也可继续显示时间</h4>
<pre class="lang:plsql decode:true">SELECT TO_CHAR(SYSDATE, ‘yyyy-mm-dd hh24:mi:ss’) FROM dual;</pre>
<span style="line-height: 1.71429; font-size: 1rem;">那么，实际上用户也可利用TO_CHAR()函数进行年、月、日数据的拆分。</span>
<h4><span style="color: #ff0000;"><strong>3.4.1.3 范例</strong>：拆分日期数据</span></h4>
<pre class="lang:plsql decode:true">SELECT TO_CHAR(SYSDATE, ‘yyyy’) year,
TO_CHAR(SYSDATE, ‘mm’) months,
TO_CHAR(SYSDATE, ‘dd’) day
FROM dual;</pre>
<h4><strong>3.4.1.4 范例</strong>：查询出所有在2月份雇佣的雇员</h4>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE TO_CHAR(hiredate, ‘mm’)=’02’;</pre>
使用TO_CHAR()还可以格式化数字显示。
<h4>3.4.1.5 范例：格式化数字显示</h4>
23567823651 = 23,567,823,651，格式化数字任意一位用9表示（多了也没事），L代表本地货币。
<pre class="lang:plsql decode:true">SELECT TO_CHAR(23567823651, ‘L999,999,999,999,999’)
FROM mep;</pre>
<h3>3.4.2 转日期数据：日期TO_DATE(字符串, 转换格式)</h3>
<h4><strong>3.4.2.1 范例</strong>：将字符串变为日期</h4>
<pre class="lang:plsql decode:true">SELECT ‘1981-09-15’, TO_DATE(‘1981-09-15’, ‘yyyy-mm-dd’)
FROM dual;</pre>
但是在Oracle之中提供有自动的转换方式，如果字符串按照日期的格式编写，那么可以自动由字符串变为日期。
<h3>3.4.3 转数字数据：数字 TO_NUMBER(字符串, 转换格式)</h3>
<pre class="lang:plsql decode:true">SELECT TO_NUMBER(‘1’) + TO_NUMBER(‘2’)
FROM dual;</pre>
或
<pre class="lang:plsql decode:true">SELECT ‘1’ + ‘2’
FROM dual;</pre>
在Oracle中这些自动的数据类型转换功能是非常方便的，包括日期和字符串、字符串和数字。
<h2>3.5 通用函数</h2>
通用函数主要是Oracle的特色函数，有两个：NVL()、DECODE()。
<h3>3.5.1 处理NULL数据：数字 NVL(列 | 数据, 默认值)</h3>
<h4>3.5.1.1 范例：遇到NULL</h4>
<pre class="lang:plsql decode:true">SELECT ename, sal, comm, (sal+comm)*12
FROM emp;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle7-4.png" alt="" width="464" height="266" />

此时，发现有一部分人没有年薪，是因为佣金为NULL。

在数据库中，NULL与任何数据计算都是NULL。那么如果遇到了NULL应按照数字0来处理，那么我们可用NULL来处理。
<pre class="lang:plsql decode:true">SELECT ename, sal, comm, (sal+NVL(comm, 0))*12
FROM emp;</pre>
<h3>3.5.2 多数据判断：数据 DECODE(字段 | 数据, 判断数据1, 显示数据1, 判断数据2, 显示数据2, …[默认数据])</h3>
<h4><strong>3.5.2.1 范例</strong>：将所有雇员的职位数据变为中文</h4>
<pre class="lang:plsql decode:true">SELECT ename, job,
DECODE(job, ‘CLERK’, ‘办事员’, ‘SALESMAN’, ‘销售’, ‘MANAGER’, ‘经理’, ‘ANALYST’, ‘分析员’, ‘PRESIDENTIAL’, ‘总裁’, ‘未处理’)
FROM emp;</pre>
非常类似IF…ELSE…IF语句（或者是SWICTH...CASE）。

若没有给出对应的职位信息的解码信息，那么遇到这条数据时候会给默认输出，若没有给出默认输出，则为NULL。
<h1>4. 单行函数总结</h1>
<table>
<tbody>
<tr>
<td width="54">No.</td>
<td width="324">方法名称</td>
<td width="189">类型</td>
</tr>
<tr>
<td width="54">1</td>
<td width="324">字符串 UPPER(字符串 | 数据列)</td>
<td width="189">将特定的字符串或者指定的列数据变为大写</td>
</tr>
<tr>
<td width="54">2</td>
<td width="324">字符串 LOWER(字符串 | 数据列)</td>
<td width="189">将特定的字符串或者指定的列数据变为小写</td>
</tr>
<tr>
<td width="54">3</td>
<td width="324">字符串 INITCAP(字符串 | 列)</td>
<td width="189">将特定字符串或者指定列的数据的首字母大写，其余字母小写</td>
</tr>
<tr>
<td width="54">4</td>
<td width="324">数字 LENGTH(字符串 | 列)</td>
<td width="189">计算出指定字符串或者数据列的数据长度</td>
</tr>
<tr>
<td width="54">5</td>
<td width="324">字符串 REPLACE(字符串 | 列，要替换的内容，新的内容)</td>
<td width="189">将指定字符串或者数据列中的数据按照指定的新内容</td>
</tr>
<tr>
<td width="54">6</td>
<td width="324">字符串 SUBSTR(字符串 | 数据列，截取开始索引)</td>
<td width="189">由指定位置截取到结尾</td>
</tr>
<tr>
<td width="54">7</td>
<td width="324">字符串 SUBSTR(字符串 | 数据列，截取开始索引，截取结束索引)</td>
<td width="189">指定截取的开始和结束位置</td>
</tr>
<tr>
<td width="54">8</td>
<td width="324">字符串 TRIM(字符串 | 列)</td>
<td width="189">去掉左右空格函数</td>
</tr>
<tr>
<td width="54">9</td>
<td width="324">数字 ROUND(数字 | 列 [，保留小数位])</td>
<td width="189">四舍五入操作</td>
</tr>
<tr>
<td width="54">10</td>
<td width="324">数字 TRUNC(数字 | 列 [，保留小数位])</td>
<td width="189">截取小数</td>
</tr>
<tr>
<td width="54">11</td>
<td width="324">数字 MOD(数字1 | 列1，数字2 | 列2)</td>
<td width="189">求模（余数）</td>
</tr>
<tr>
<td width="54">12</td>
<td width="324">数字MONTHS_BETWEEN(日期1 | 列1，日期2 | 列2)</td>
<td width="189">计算两个日期之间所经历的月数</td>
</tr>
<tr>
<td width="54">13</td>
<td width="324">ADD_MONTHS(日期 | 列，月数)</td>
<td width="189">在指定日期上增加若干月之后的日期</td>
</tr>
<tr>
<td width="54">14</td>
<td width="324">日期 NEXT_DAY(日期 | 列，一周时间数)</td>
<td width="189">求出指定的下一个一周时间数的日期</td>
</tr>
<tr>
<td width="54">15</td>
<td width="324">日期 LAST_DAY(日期 | 列)</td>
<td width="189">求出指定日期所在月的最后一天日期</td>
</tr>
<tr>
<td width="54">16</td>
<td width="324">字符串 TO_CHAR(数字 | 日期 | 列，转换格式)</td>
<td width="189">转字符串数据</td>
</tr>
<tr>
<td width="54">17</td>
<td width="324">日期 TO_DATE(字符串，转换格式)</td>
<td width="189">转日期数据</td>
</tr>
<tr>
<td width="54">18</td>
<td width="324">数字 TO_NUMER(字符串，转换格式)</td>
<td width="189">转数字数据</td>
</tr>
<tr>
<td width="54">19</td>
<td width="324">数字NVL(列 | 数据，默认值)</td>
<td width="189">处理NULL数据</td>
</tr>
<tr>
<td width="54">20</td>
<td width="324">数据DECODE(字段 | 数据，判断数据1，显示数据1，判断数据2，显示数据2，…[默认显示])</td>
<td width="189">多数据判断</td>
</tr>
</tbody>
</table>
<h1>5. 参考</h1>
<ul>
	<li>『李兴华java培训01』Oracle数据库 - 网易云课堂  http://study.163.com/course/courseLearn.htm?courseId=932016#/learn/video?lessonId=1115371&amp;courseId=932016</li>
</ul>
