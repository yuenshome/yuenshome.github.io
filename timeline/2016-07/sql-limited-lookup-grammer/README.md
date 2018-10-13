[![header](../../../assets/header15.png)](https://yuenshome.github.io)

# 限定查询的SQL语法

内容参考《李兴华Oracle数据库》的限定查询部分（链接见文末参考），本节主要包括：[toc]<!--more-->

<strong><span style="color: #ff0000;">限定查询用来控制行。</span></strong>

简单查询最大的问题在于可以显示一张数据表中的所有数据，但很多情况下并不需要所有数据，往往需要做一些过滤，而这样的查询就属于限定查询。限定查询的语法如下：
<table width="584">
<tbody>
<tr>
<td rowspan="3" width="300">SELECT [DISTINCT] * | 列 [别名], 列 [别名], …

FROM 表名称 [别名]

[WHERE 条件(s)]</td>
<td width="283">步骤三：控制要显示的数据列</td>
</tr>
<tr>
<td width="283">步骤一：确定数据源</td>
</tr>
<tr>
<td width="283">步骤二：根据判断条件选择参与的数据行</td>
</tr>
</tbody>
</table>
注：中括号都是可选的。

在使用WHERE子句操作的时候可以进行条件判断，而对于条件的判断主要使用如下的运算：
<ol>
	<li>（&gt;、&lt;、&gt;=、&lt;=、&lt;&gt;、!=）；</li>
	<li>范围运算符（BETWEEN…AND）；</li>
	<li>IN子句；</li>
	<li>LIKE子句；</li>
	<li>空判断。</li>
</ol>
而且在进行限定查询的时可使用以下的逻辑运算：
<ul>
	<li>AND（与操作，所有的判断条件都满足；</li>
	<li>OR（表示若干个判断条件只需要满足一个）；</li>
	<li>NOT（求反，TRUE变FALSE、FALSE变为TRUE）。</li>
</ul>
<h1>1.关系运算</h1>
关系运算是最简单的运算符号，使用的符号有：“&gt;、&lt;、&gt;=、&lt;=、&lt;&gt;、!=”。其中不等于有两种表示（“&lt;&gt;”或“!=”）
<h2><strong>1.1 范例</strong>：要求查询出所有的基本工资高于2000的雇员信息（sal为基本工资）</h2>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE sal&gt;2000;</pre>
<h2><strong>1.2 范例</strong>：查询工资小于等于1600的全部雇员信息。</h2>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE sal&lt;=1600;</pre>
<h2><strong>1.3 范例</strong>：查询SMITH的雇员信息。</h2>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE ename=’smith’;</pre>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE ename=’SMITH’;</pre>
在Oracle中数据是区分大小写的，像一些系统发现密码是区分大小写的情况，非常适合于Oracle保存。
<h2><strong>1.4 范例</strong>：查询所有职位不是销售人员的信息</h2>
销售人员是看职位的数据，职位的字段是job，而现在的关系是取反，不是等于。
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE job&lt;&gt;’SALESMAN’;</pre>
或
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE job!=’SALESMAN’;</pre>
两种符号都可以实现不等于，根据开发者个人决定。
<h1>2. 逻辑运算</h1>
<h2>2.1 AND连接操作</h2>
在WHERE子句中是可以同时编写多个条件的，但多个条件之间必须使用逻辑连接。
<table>
<tbody>
<tr>
<td width="83">AND</td>
<td width="246">条件AND条件AND条件</td>
<td width="240">è所有条件同时满足</td>
</tr>
<tr>
<td width="83">OR</td>
<td width="246">条件OR条件OR条件</td>
<td width="240">è所有条件只要有一个满足即可</td>
</tr>
</tbody>
</table>
<h3>2.1.1 范例：要求查询出的所有销售人员并且工资高于1200的所有雇员</h3>
<ul>
	<li>条件一：职位是销售人员，job=’SALESMAN’；</li>
	<li>条件二：工资高于1200，sal&gt;1200；</li>
	<li>这两个条件应同时满足，所以使用AND连接。</li>
</ul>
<pre class="lang:plsql decode:true ">SELECT *
FROM emp
WHERE job=’SALESMAN’ AND sal&gt;1200;</pre>
<h3><strong>2.1.2 范例</strong>：查询出工资范围在1200-3000之间的数据（包含1200和3000）</h3>
<ul>
	<li>条件一：工资大于等于1200，sal&gt;=1200；</li>
	<li>条件二：工资小于等于3000，sal&gt;=3000；</li>
	<li>两个条件同时满足，使用AND进行连接。</li>
</ul>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE sal&gt;1200 AND sal&gt;=3000;</pre>
<h3><strong>2.1.4 范例</strong>：查询出部门为10的经理</h3>
<ul>
	<li>条件一：查询出部门位10的经理数据，deptno=10；</li>
	<li>条件二：判断职位数据，job=’MANAGER’。</li>
</ul>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE job=’MANAGER’ AND deptno=10;</pre>
<h2>2.2 OR连接操作</h2>
<h3><strong>2.2.1 范例</strong>：要求查询出工资高于3000，或者职位是办事员的全部雇员信息</h3>
<ul>
	<li>条件一：工资高于3000，sal&gt;3000；</li>
	<li>条件二：职位为办事员，job=’CLERK’。</li>
</ul>
<pre class="lang:plsql decode:true ">SELECT *
FROM emp
WHERE job=’CLERK’ OR sal&gt;3000;</pre>
<h2>2.3 NOT取反操作</h2>
正常情况下，如果1&gt;0，返回结果是TRUE，若加上NOT，则结果变为FALSE，即对原本的判断结果进行取反。
<h3><strong>2.3.1 范例</strong>：观察取反——查询出所有职位不是办事员的信息</h3>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE NOT job=’CLERK’;</pre>
若觉得NOT不好用，可以用!=或者&lt;&gt;。

所有的逻辑运算可以在任何的WHERE运算之中使用，进行条件的连接。
<h2>2.4 取值范围：BETWEEN…AND</h2>
如果现在知道了某一个范围的两个值（最大，最小），那么就可以写为：
<pre class="lang:plsql decode:true">字段 BETWEEN 最小值 AND 最大值</pre>
在进行计算过程中也会包含了最大和最小的两个值的判断。
<h3><strong>2.4.1 范例</strong>：查询出所有工资范围在1200~3000之间的雇员信息（包含1200和3000）</h3>
这时是一个范围，所以面对范围不再需要复杂的AND连接，直接用BETWEEN：
<pre class="lang:plsql decode:true ">SELECT *
FROM emp
WHERE sal BETWEEN 1200 AND 3000;</pre>
<h3><strong>2.4.2 范例</strong>：观察不分数据类型的判断</h3>
但是通过之前的程序发现一个特点：所有的关系运算符是不区分数据类型的。比方下面两个判断：
<ol>
	<li>SELECT * FROM emp WHERE ename&gt;’SMITH’;</li>
	<li>SELECT * FROM emp WHERE sal&gt;1200;</li>
</ol>
无论字符串或数字类型，都可使用这些运算符判断，所以BETWEEN…AND也可以，即所有的符号都支持字符串、数字、日期型数据的操作。
<h3><strong>2.4.3 范例</strong>： 查询所有在1981年雇佣的雇员信息</h3>
范围：’1981-01-01’（’01-1月-1981’）~’1981-12-31’（’31-12月-81’）
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE hiredate BETWEEN ’01-1月-1981’ AND ’31-12月-81’;</pre>
所有的符号都不分判断的数据类型。比方字符串也可用BETWEEN…AND。
<h1>3. 参考</h1>
<ul>
	<li>『李兴华java培训01』Oracle数据库 - 网易云课堂  http://study.163.com/course/courseLearn.htm?courseId=932016#/learn/video?lessonId=1115369&amp;courseId=932016</li>
</ul>
<audio style="display: none;" controls="controls"></audio>

