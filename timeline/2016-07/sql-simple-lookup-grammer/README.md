[![header](../../../assets/header14.png)](https://yuenshome.github.io)

# 简单查询的SQL语法

内容参考《李兴华Oracle数据库》的简单查询部分（链接见文末参考），本节主要包括：[toc]

<!--more-->
<h1>1. scott用户的主要数据表（重点，背）</h1>
传统数据库中使用的是表作为存储的基本单元，在一个数据库中有大量的数据表。而在整个学习中，scott用户是主要的数据来源，所以首先观察此用户下的四张数据表及其表结构。
<h2><strong>1.1 范例：查询一个用户的所有表，使用如下命令</strong></h2>
<pre class="lang:plsql decode:true">SELECT * FROM tab;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-1.png" alt="" width="614" height="144" />

此时显示有四张表：dept、emp、salgrade、bonus，观察四张表的结构（查看表结构语法：DESC 表）。
<h3>1.1.1 部门表：dept</h3>
<table>
<tbody>
<tr>
<td width="54"><strong>No.</strong></td>
<td width="132"><strong>字段</strong></td>
<td width="104"><strong>类型</strong></td>
<td width="277"><strong>描述</strong></td>
</tr>
<tr>
<td width="54">1</td>
<td width="132">DEPTNO</td>
<td width="104">NUMBER(2)</td>
<td width="277">部门编号，最多只能由两位数字组成</td>
</tr>
<tr>
<td width="54">2</td>
<td width="132">DNAME</td>
<td width="104">VARCHAR<strong><span style="color: #ff0000;">2</span></strong>(14)</td>
<td width="277">部门名称，由14个字节长度组成</td>
</tr>
<tr>
<td width="54">3</td>
<td width="132">LOC</td>
<td width="104">VARCHAR<span style="color: #ff0000;"><strong>2</strong></span>(13)</td>
<td width="277">部门位置</td>
</tr>
</tbody>
</table>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-2.png" alt="" width="393" height="137" />
<h3>1.1.2 雇员表：emp</h3>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-3.png" alt="" width="453" height="282" />
<table>
<tbody>
<tr>
<td width="54"><strong>No.</strong></td>
<td width="132"><strong>字段</strong></td>
<td width="104"><strong>类型</strong></td>
<td width="277"><strong>描述</strong></td>
</tr>
<tr>
<td width="54">1</td>
<td width="132">EMPNO</td>
<td width="104">NUMBER(4)</td>
<td width="277">雇员编号，最多由4位数字组成</td>
</tr>
<tr>
<td width="54">2</td>
<td width="132">ENAME</td>
<td width="104">VARCHAR<strong><span style="color: #ff0000;">2</span></strong>(10)</td>
<td width="277">雇员姓名</td>
</tr>
<tr>
<td width="54">3</td>
<td width="132">JOB</td>
<td width="104">VARCHAR<span style="color: #ff0000;"><strong>2</strong></span>(9)</td>
<td width="277">职位信息，多个雇员的职位信息一定会重复</td>
</tr>
<tr>
<td width="54">4</td>
<td width="132">MGR</td>
<td width="104">NUMBER(4)</td>
<td width="277">领导标号，领导也一定是公司雇员</td>
</tr>
<tr>
<td width="54">5</td>
<td width="132">HIREDATE</td>
<td width="104">DATE</td>
<td width="277">雇佣日期，DATE包含年月日时分秒数据</td>
</tr>
<tr>
<td width="54">6</td>
<td width="132">SAL</td>
<td width="104">NUMBER(7,2)</td>
<td width="277">基本工资，由2位小数位和5位整数位组成</td>
</tr>
<tr>
<td width="54">7</td>
<td width="132">COMM</td>
<td width="104">NUMBER(7,2)</td>
<td width="277">佣金，销售人员才会有佣金</td>
</tr>
<tr>
<td width="54"><span style="color: #ff0000;"><strong>8</strong></span></td>
<td width="132"><span style="color: #ff0000;"><strong>DEPTNO</strong></span></td>
<td width="104"><span style="color: #ff0000;"><strong>NUMBER(2)</strong></span></td>
<td width="277"><span style="color: #ff0000;"><strong>雇员所在的部门编号，与dept</strong><strong>表对应</strong></span></td>
</tr>
</tbody>
</table>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-4.png" alt="" width="396" height="174" />

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-5.png" alt="" width="403" height="208" />
<h3>1.1.3 工资等级表：salgrade</h3>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-6.png" alt="" width="310" height="154" />

每位雇员都有一个工资数据，工资在公司中会划分等级。
<table>
<tbody>
<tr>
<td width="54"><strong>No.</strong></td>
<td width="132"><strong>字段</strong></td>
<td width="104"><strong>类型</strong></td>
<td width="277"><strong>描述</strong></td>
</tr>
<tr>
<td width="54">1</td>
<td width="132">GRADE</td>
<td width="104">NUMBER</td>
<td width="277">等级编号</td>
</tr>
<tr>
<td width="54">2</td>
<td width="132">LOSAL</td>
<td width="104">NUMBER</td>
<td width="277">此等级的最低工资</td>
</tr>
<tr>
<td width="54">3</td>
<td width="132">HISAL</td>
<td width="104">NUMBER</td>
<td width="277">此等级的最高工资</td>
</tr>
</tbody>
</table>
<h3>1.1.4 工资表：bonus</h3>
<table>
<tbody>
<tr>
<td width="54"><strong>No.</strong></td>
<td width="132"><strong>字段</strong></td>
<td width="104"><strong>类型</strong></td>
<td width="277"><strong>描述</strong></td>
</tr>
<tr>
<td width="54">1</td>
<td width="132">ENAME</td>
<td width="104">VARCHAR<span style="color: #ff0000;"><strong>2</strong></span>(10)</td>
<td width="277">雇员姓名</td>
</tr>
<tr>
<td width="54">2</td>
<td width="132">JOB</td>
<td width="104">VARCHAR<span style="color: #ff0000;"><strong>2</strong></span>(9)</td>
<td width="277">职位</td>
</tr>
<tr>
<td width="54">3</td>
<td width="132">SAL</td>
<td width="104">NUMBER</td>
<td width="277">工资</td>
</tr>
<tr>
<td width="54">4</td>
<td width="132">COMM</td>
<td width="104">NUMBER</td>
<td width="277">佣金</td>
</tr>
</tbody>
</table>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-7.png" alt="" width="248" height="68" />

其中三张数据表（emp、dept、salgrade）都存在数据，但是只有bonus表不包含任何数据（类似sh用户）。
<h1>2. SQL简介（了解）</h1>
SQL是一个功能强大的数据库语言。SQL通常用于与数据库的通讯，SQL是关系数据库管理系统的标准语言。

SQL是在70年代由IBM研发出来的，目的是统一数据库的操作方式（最早的是visual foxpro）。实际上最初的数据库厂商都有自己的操作标准。后来80年代，Oracle公司提供了SQL支持（Oracle是最早支持SQL标准的大型数据库）。到了今天，几乎所有的数据库都支持SQL，大大简化程序开发人员的负担（学习只学习标准SQL）。

PS：SQL的操作属于长期以来的历史发展产物，同时也是现有可能会被延续下去的产物，不过在SQL产生的部分人员认为SQL并不方便（传统的基于行、列的数据库的存储结构不方便），于是产生了一个NOSQL技术（最早的翻译是不使用SQL），但到现在为止NOSQL翻译为：“Not OnlySQL”，其中代表作：MongoDB（Node.JS，taobao），采用的是一种文档的方式进行存储的。

&nbsp;

SQL功能强大，概括起来，它可以分成以下几组：
<ul>
	<li><span style="color: #ff0000;">DML（Data Manipulation Language，数据库操作语言）——实现数据库的查询与更新操作，根据库的查询有部分人员将其称为DQL，因为最麻烦的部分就是查询。</span></li>
	<li><span style="color: #ff0000;">DDL（Data Definition Language，数据定义语言）——定义数据库对象，例如：在一些系统设计人员的工作里需要进行数据表的定义。</span></li>
	<li>DCL（Data Control Language，数据控制语言）——控制用户的操作权限，一般由DBA负责</li>
</ul>
<h1>3. 简单查询（<span style="color: #ff0000;">重点，控制列</span>）</h1>
对于查询一共分为四类：
<ol>
	<li>简单查询；</li>
	<li>限定查询；</li>
	<li>多表查询；</li>
	<li>统计查询。</li>
</ol>
本节主要讲简单查询。
<table style="height: 100px;" width="722">
<tbody>
<tr>
<td rowspan="2" width="310"><strong><span style="color: #ff0000;">SELECT</span></strong> [DISTINCT]  * | 列 [别名], 列[别名], …

<strong><span style="color: #ff0000;">FROM</span></strong> 表名称 [别名]</td>
<td width="259">步骤二：控制要显示的数据列</td>
</tr>
<tr>
<td width="259">步骤一：确定查询的数据源</td>
</tr>
</tbody>
</table>
数据表的组成是行与列，而简单查询的最大特征在于会将一张数据表之中的全部数据行进行显示，而用控制SELECT来控制显示的数据列。

其中几个参数的作用如下：
<ul>
	<li>DISTINCT：表示去掉重复行数据；</li>
	<li>“*”：表示查询所有的列，如果不想查询所有列，则编写具体的列名称。</li>
</ul>
<h2><strong>3.1 范例</strong>：查询查询所有雇员的信息（所有雇员一定包含所有数据）</h2>
<pre class="lang:plsql decode:true">SELECT *
FROM emp;</pre>
<h2><strong>3.2 范例</strong>：查询每个雇员的编号（empno）、姓名（ename）、职位（job）、基本工资（sal）</h2>
<pre class="lang:plsql decode:true">SELECT empno, ename, job, sal 
FROM emp;
</pre>
<span style="line-height: 1.71429; font-size: 1rem;">通过以上的查询可以清楚发现，数据表所有行都显示，而列只显示需要的部分。除了这样的基本查询在进行查询的过程之中，SELECT子句之中也可以进行各种数学计算（+、-、×、÷）。</span>
<h2><strong>3.3 范例</strong>：查询每个雇员的编号、姓名、基本年薪</h2>
<pre class="lang:plsql decode:true">SELECT empno, ename, sal*12
FROM emp;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-8.png" alt="" width="279" height="297" />

但是现在有问题出现，发现计算处的列名称比较奇怪，并没有其它列的意思明确，所以此时可以起一个别名。
<h2><strong>3.4 范例</strong>：为查询列起别名</h2>
<pre class="lang:plsql decode:true">SELECT empno 编号, ename 姓名, sal*12 年薪
FROM emp;</pre>
或
<pre class="lang:plsql decode:true">SELECT empno, ename, sal*12 income
FROM emp;</pre>
此处的程序是为了演示才采用中文的处理方式，但是在开发之中一定要记住：回避中文。
<h2><strong>3.5 范例</strong>：现在查询每个雇员的编号、姓名、职位、年薪，而且每个雇员，每个月有200元的饭食补助、200元汽车补助、夏天有4个月发每月300元的防暑补贴，年底的时候可以领到15个月的基本工资。</h2>
<pre class="lang:plsql decode:true">SELECT empno, ename, job,
      (sal + 400)*12 + 300*4 + sal * 3 income
FROM emp;</pre>
四则运算的过程中，也有一定的优先顺序，先乘除后加减。
<h2><strong>3.6 范例</strong>：查询所有雇员的职位</h2>
<pre class="lang:plsql decode:true ">SELECT job FROM emp;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-9.png" alt="" width="207" height="352" />

发现此时的查询结果中，有重复数据，因为多个雇员必然有重复职位，若想消除重复列，可使用DISTINCT关键字处理，此关键字只能出现在SELECT子句中。
<h2><strong>3.7 范例</strong>：去除重复列</h2>
<pre class="lang:plsql decode:true">SELECT DISTINCT job
FROM emp;</pre>
虽然此时可以消除重复数据，但对于重复数据数据有一个原则，只有在所有列数据都重复的情况下才叫重复数据。
<h2><strong>3.8 范例</strong>：关于消除重复的一个小问题——查询每个雇员的姓名、职位</h2>
职位会有重复，但姓名不会重复，只有所有列都重复的时候才会消除重复。
<pre class="lang:plsql decode:true">SELECT DISTINCT ename, job
FROM emp;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-10.png" alt="" width="343" height="345" />

以上操作都数据SQL的基本操作，但Oracle中，增加了一个双竖线的连接符“||”，可进行数据的连接。
<h2><strong>3.9 范例</strong>：观察连接符</h2>
<pre class="lang:plsql decode:true">SELECT empno, ename
FROM emp;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-11.png" alt="" width="240" height="268" />

若在empno和ename字段中间将逗号替换为双竖线，那么两列将会合成为一列。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-12.png" alt="" width="247" height="277" />

但这样的合并意义不大，所以下面可以增加一些描述文字，例如现在有这样的数据显示：
<ul>
	<li><span style="color: #ff0000;">编号：</span><span style="color: #00ccff;">7369</span><span style="color: #ff0000;">，姓名：</span><span style="color: #00ccff;">SMITH</span><span style="color: #ff0000;">， 工资：</span><span style="color: #00ccff;">800</span></li>
	<li>蓝色字体应该是灵活可变的，而红色的数据是固定的。</li>
</ul>
<pre class="lang:plsql decode:true">SELECT ‘编号：’ || enpno || ‘、姓名：’ || ename || ‘、工资：’ || sal
     info
FROM emp;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-13.png" alt="" width="776" height="181" />

<span style="color: #ff0000;"><strong>注意：别名没有单引号，字符有单引号</strong>。</span>

提示：关于数据的表示问题
<ul>
	<li>在进行查询的时候如果起别名，那么可以直接编写，不需要使用任何的修饰符；</li>
	<li>在进行查询的时候如果直接编写字符串，则字符串要使用“’”（英文单引号括起来）声明，如果是数字，则直接编写（无需单引号）；</li>
</ul>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle2-14.png" alt="" width="776" height="181" />
<ul>
	<li>在SELECT子句中出现的字符串和数字都会固定的显示，有多少行就重复出现多少行。</li>
</ul>
<h1>4. 参考</h1>
『李兴华java培训01』Oracle数据库 - 网易云课堂  http://study.163.com/course/courseLearn.htm?courseId=932016#/learn/video?lessonId=1116379&amp;courseId=932016
