[![header](../assets/header37.jpg)](https://yuenshome.github.io)

# 多表查询的SQL语法

内容参考《李兴华Oracle数据库》的多表查询部分（链接见文末参考），本节主要包括：[toc]<!--more-->
<h1>1.多表查询基本概念</h1>
在之前的查询中，FROM子句后只有一张数据表，而多表查询指的是同时从多张数据表中取出数据实现的查询。<span style="color: #ff0000;">重点是修改FROM子句</span>，多表查询的语法如下：
<pre class="lang:plsql decode:true">SELECT [DISTINCT] * | 列名称 [别名], 列名称 [别名],…
FROM 数据表 [别名], 数据表 [别名],…
[WHERE 条件(s)]
[ORDER BY 字段 [ASC | DESC], 字段 [ASC | DESC], …]</pre>
那么下面先通过一个简单的查询来观察一下多表查询所带来的问题。那么，首先介绍一个COUNT()函数，用来统计一张数据表中的数据量。
<h2><strong>1.1 范例</strong>：统计emp表中的数据量</h2>
<pre class="lang:plsql decode:true">SELECT COUNT(*)
FROM emp;</pre>
<h2><strong>1.2 范例</strong>：统计dept表数据量</h2>
<pre class="lang:plsql decode:true">SELECT COUNT(*)
FROM dept;</pre>
<h2><strong>1.3 范例</strong>：将emp表和dept表实现多表查询</h2>
<pre class="lang:plsql decode:true">SELECT *
FROM emp, dept;</pre>
此时查询符合之前给出的多表查询的语法，但结果却出现了56行记录，而且56条记录出现了重复，即56行记录 = emp表的14行记录 * dept表的4行记录。

出现这样的结果是由数据库的数学公式导致的。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle8-1.png" alt="" width="751" height="304" />

以上的结果实际上就属于集合的“积”，而这种积在数据库中称为“笛卡尔积”，解决办法是使用关联字段。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle8-2.png" alt="" width="526" height="218" />

此时应用emp表中的deptno字段与dept表中的deptno字段比较后得出结果。
<h2><strong>1.4 范例</strong>：解决笛卡尔积问题</h2>
<pre class="lang:plsql decode:true">SELECT *
FROM emp, dept
WHERE emp.deptno=dept.deptno;</pre>
此时虽然过滤了笛卡尔积不符合实际的情况，但笛卡尔积仍旧存在，因为这个是有数据库本身的数学公式所决定的问题。

在进行多表查询时，一定会存在关联列，若表中有同名列，一定要用“表名称.字段”进行标记。可是，当表名称很长，那么就可以用别名代替。
<h2><strong>1.5 范例</strong>：使用别名</h2>
<pre class="lang:plsql decode:true">SELECT*
FROM emp e, dept d
WHERE e.deptno=d.deptno;</pre>
<h2><strong>1.6 范例</strong>：要求查询出每个雇员的编号、姓名、职位、工资、部门名称、部门位置</h2>
<ul>
	<li>确定要使用的数据表：
<ul>
	<li>emp表：雇员的编号、姓名、职位、工资；</li>
	<li>dept表：部门名称、部门位置；</li>
</ul>
</li>
	<li>确定已知的关联条件：
<ul>
	<li>雇员和部门：deptno=denpt.deptno。</li>
</ul>
</li>
</ul>
<strong>第一步</strong>：查询出每个雇员的编号、姓名、职位、工资，只需要emp一张数据表即可
<pre class="lang:plsql decode:true">SELECT e.empno, e.ename, e.job, e.sal
FROM emp e;</pre>
<strong>第二步</strong>：加入部门表，一旦加入了部门表，那么就表示需要增加一个消除笛卡尔积的条件
<pre class="lang:plsql decode:true">SELECT e.empno, e.ename, e.job, e.sal, d.dname, d.loc
FROM emp e, dept d
WHERE e.deptno=d.deptno;</pre>
<h2><strong>1.7 范例</strong>：查询出每个雇员的编号、姓名、职位、工资、雇佣日期、工资等级</h2>
<ul>
	<li>确定要使用的数据表：
<ul>
	<li>emp表：雇员的编号、姓名、职位、工资、雇佣日期；</li>
	<li>salgrade表：工资等级；</li>
</ul>
</li>
	<li>确定已知的关联条件：
<ul>
	<li>雇员与工资等级：sal BETWEEN salgrade.losal AND salgrade.hisal</li>
</ul>
</li>
</ul>
<strong>第一步</strong>：查询雇员数据
<pre class="lang:plsql decode:true">SELECT e.empno, e.ename, e.job, e.sal, e.hiredate
FROM emp e;</pre>
<strong>第二步</strong>：加入工资等级表，此时使用的是一个范围的查询
<pre class="lang:plsql decode:true">SELECT e.empno, e.ename, e.job, e.sal, e.hiredate, s.grade
FROM emp e, salgrade s
WHERE e.sal BETWEEN s.losal AND s.hisal;</pre>
<h2><strong>1.8 范例</strong>：查询每个雇员的编号、姓名、职位、雇用日期、工资、工资等级、所在部门名称及位置</h2>
<ul>
	<li>确定要使用的数据表：
<ul>
	<li>emp表：雇员的编号、姓名、职位、雇佣日期、工资；</li>
	<li>salgrade表：工资等级；</li>
	<li>dept表：部门名称及位置。</li>
</ul>
</li>
	<li>确定已知的关联条件：<span style="color: #ff0000;"><strong>只要是消除笛卡尔积的条件，之间都使用</strong><strong>AND</strong><strong>连接</strong></span>
<ul>
	<li>雇员和工资等级：sal BETWEEN salgrade.losal AND salgrade.hisal;</li>
	<li>雇员和部门：deptno=dept.deptno</li>
</ul>
</li>
</ul>
<strong>第一步</strong>：查询每个雇员的编号、姓名、职位、雇用日期、工资
<pre class="lang:plsql decode:true">SELECT e.empno, e.ename, e.job, e.hiredate
FROM emp e;</pre>
<strong>第二步</strong>：增加工资等级的判断
<pre class="lang:plsql decode:true">SELECT e.empno, e.ename, e.job, e.hiredate
FROM emp e, salgrade s
WHERE e.sal BETWEEN s.losal AND s.hisal;</pre>
<strong>第三步</strong>：增加部门表信息
<pre class="lang:plsql decode:true">SELECT e.empno, e.ename, e.job, e.hiredate
FROM emp e, salgrade s
WHERE e.sal BETWEEN s.losal AND s.hisal
AND e.deptno=d.deptno;</pre>
但此时一定要注意，表关联的越多，实际上产生笛卡尔积的数量也越多，就拿以上查询的来讲，实际上最终的数据量：emp表的14行 * dept表的4行 * salgrade表5行 = 280条数据（其中只有14行有用）。
<h2><strong>1.9 说明</strong>：关于数据量庞大的两个说明（以sh用户登录）</h2>
<ul>
	<li><strong>说明一：在使用NOT IN查询的时候，里面不能有NULL</strong></li>
</ul>
如果说拿到一张数据表（costs表，这张表第一次接触），如果是一位新手一般都会写出如下的一行指令：
<pre class="lang:plsql decode:true">SELECT *
FROM costs;</pre>
于是此时就悲哀了，如果此表中的数据量较大，那么最轻的后果是查看不方便，而严重的结果是数据库导致死机。但若是有经验的开发者，往往都先使用COUNT()函数确定数据量：
<pre class="lang:plsql decode:true">SELECT COUNT(*)
FROM costs;</pre>
若数据量较大，那么使用特定的操作，取出特定的几行数据，若数据量小，随便操作。

那么回到NOT IN中，不能出现NUILL的问题，若NOT IN里面有了NULL则表示不为空，实际中有些数据是不能为空的（比方姓名），那么就表示查询全部了。如果表中的数据量庞大了，那么直接造成死机。
<ul>
	<li><strong>说明二：以上的查询只是过滤了显示的笛卡尔积，但是笛卡尔积仍然存在。下面来验证一下笛卡尔积的影响：</strong></li>
</ul>
<pre class="lang:plsql decode:true">SELECT COUNT(*) FROM costs; -- 82112条记录
SELECT COUNT(*) FROM sales; -- 918843条记录
SELECT COUNT(*)
FROM costs e, sales s
WHERE e.prod_id=s.prod_id; -- 75,448,036,416参与计算，保留1,165,337,550</pre>
虽然最终的操作消除了无用显示出来的笛卡尔积，但整个运算执行过程非常的大。所以实际的工作中，尤其是在进行数据表设计时，尽可能避免查询有可能出现的笛卡尔积问题（同时在实际中，也应尽可能少用多表查询）。

<span style="color: #ff0000;"><strong>结论：在数据量大的时候绝对不要采用多表查询，而且就算是数据量小，也别用多表查询。</strong></span>

<strong>最基础的解决性能问题的方案（面试的时候别说，丢人）：</strong>

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle8-3.png" alt="" width="837" height="303" />

<strong>例如</strong>：现在要求查询出每个雇员的姓名、职位、工资、部门名称

这样的查询一定要使用emp和dept两张表共同完成，但是如果假设emp表中有200W个雇员，dept表中有5W个部门，那么此时如果直接关联产生的笛卡尔积相当庞大，所以此时最好的解决方案是在设计初期，在emp表里重复保存一个部门名称（<span style="color: #ff0000;"><strong>用重复字段避免多表查询</strong></span>）。
<h1>2. 连接方式</h1>
对于多表查询来讲有两种表的连接方式：
<ul>
	<li>内连接：也被称为等值连接，在之前的所有查询都属于内连接；</li>
	<li>外连接：左外连接、右外连接、全外连接。</li>
</ul>
为了更好观察出内连接和外；连接的区别，首先在emp表中增加一行没有部门的雇员。
<h2>2.1 内连接</h2>
<h3><strong>2.1.1 范例</strong>：增加一个没有部门的雇员信息</h3>
<pre class="lang:plsql decode:true">INSERT INTO emp(empno, ename, job, hiredate, sal)
VALUES(7777, ‘刘小忙’, ‘CLERK’, SYSDATE, 701);</pre>
该数据没有部门编号，即部门编号为NULL。

&nbsp;
<h3>2.1.2 范例：观察内连接</h3>
<pre class="lang:plsql decode:true">SELECT *
FROM emp e, dept d
WHERE e.deptno=d.deptno;</pre>
因为7777的部门编号为NULL，所以过滤条件（WHERE e.deptno=d.deptno）不满足，没有7777。

如果想让没有部门的雇员或者是没有雇员的部门的数据显示，则可使用如下方式实现<span style="color: #ff0000;"><strong>外连接</strong></span>显示：
<ul>
	<li>左外连接：字段 = 字段(+)，“(+)”在等号右边表示左外连接</li>
	<li>右外连接：字段(+) = 字段，“(+)”在等号左边表示左外连接</li>
</ul>
<h3>2.1.3 范例：观察左外连接——7777的雇员显示了</h3>
<pre class="lang:plsql decode:true">SELECT *
FROM emp e, dept d
WHERE e.deptno=d.deptno(+);</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle8-4.png" alt="" width="661" height="323" />

刘小忙的雇员信息虽然显示了，但部门信息为NULL。
<h3>2.1.4 范例：观察右外连接——40部门显示了，对于雇员信息为NULL</h3>
<pre class="lang:plsql decode:true">SELECT *
FROM emp e, dept d
WHERE e.deptno(+)=d.deptno;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle8-5.png" alt="" width="894" height="286" />

40部门显示了，但其它雇员信息为NULL。

一般而言，只要查询的数据没有出现，才会使用外连接，没有必要去强制性地分清到底是左还是右。
<h3>2.1.5 范例：查询每个雇员的编号、姓名、职位、领导姓名</h3>
实际上在emp表中存在一个MGR字段（该雇员的领导编号）。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle8-6.png" alt="" width="739" height="298" />
<ul>
	<li>确定所需的数据表：
<ul>
	<li>emp表：雇员的编号、姓名、职位；</li>
	<li>emp表：领导姓名；</li>
</ul>
</li>
	<li>确定已知的关联字段：
<ul>
	<li>雇员和领导：mgr = memp.empno（雇员的领导编号 = 雇员编号）</li>
</ul>
</li>
</ul>
<strong>第一步：</strong>查询emp表中雇员的编号、姓名、职位
<pre class="lang:plsql decode:true">SELECT e.empno, e.ename, e.job
FROM emp e;</pre>
<strong>第二步：</strong>查询出领导的姓名è内连接，等值连接，有NULL的不显示
<pre class="lang:plsql decode:true">SELECT e.empno, e.ename, e.job, m.ename
FROM emp e, emp m
WHERE e.mgr=m.empno;</pre>
<strong>第三步：</strong>加入外连接控制，让所有的雇员数据显示
<pre class="lang:plsql decode:true">SELECT e.empno, e.ename, e.job, m.ename
FROM emp e, emp m
WHERE e.mgr=m.empno(+);</pre>
在实际工作中外连接在多表查询的操作里面概念非常重要，而且以上的表属于自身表关联查询。
<h2>2.2 外连接（SQL:1999语法支持）</h2>
SQL语法标准实际上一直在进行更新，从1999年之后对于数据表的关联查询给了一个标准操作语法（因为“(+)”符号只有Oracle可以用，其它的数据库不支持此符号，只能用SQL:1999完成），此类语法定义如下：
<pre class="lang:plsql decode:true">SELECT * | 字段 [别名]
FROM 表 [CROSS JOIN 表] [NATURAL JOIN 表]
[JOIN] ] [USING(字段)] [ON(条件)]
[LEFT | RIGHT | FULL | OUTER JOIN 表</pre>
以上给出的是一个完整语法，实际上这些语法都可以进行细分。
<h3>2.2.1 交叉连接，其主要目的是产生笛卡尔积</h3>
语法：
<pre class="lang:plsql decode:true ">SELECT * | 字段 [别名]
FROM 表 CROSS JOIN 表;</pre>
范例：
<pre class="lang:plsql decode:true">SELECT *
FROM emp CORSS JOIN dept;</pre>
<h3>2.2.2 自然连接，主要是消除笛卡尔积（相当于内连接）</h3>
语法：
<pre class="lang:plsql decode:true">SELECT * | 字段 [别名]
FROM 表 NATRUAL JOIN 表</pre>
范例：
<pre class="lang:plsql decode:true">SELECT *
FROM emp NATRUAL JOIN dept;</pre>
<h3>2.2.3 使用USING子句，用户指定关联字段</h3>
语法：
<pre class="lang:plsql decode:true">SELECT * | 字段 [别名]
FROM 表 JOIN 表 USING (字段)</pre>
范例：
<pre class="lang:plsql decode:true">SELECT *
FROM emp JOIN dept USING(deptno);</pre>
<h3>2.2.4 使用ON子句指定关联条件</h3>
语法：
<pre class="lang:plsql decode:true">SELECT * | 字段 [别名]
FROM 表 JOIN 表 ON(条件);</pre>
范例：
<pre class="lang:plsql decode:true">SELECT *
FROM emp e JOIN dept d ON(e.deptno=d.deptno);</pre>
<h3>2.2.5 外连接</h3>
语法：
<pre class="lang:plsql decode:true">SELECT * | 字段 [别名]
FROM 表 LEFT | RIGHT | FULL OUTER JOIN 表;</pre>
范例：
<pre class="lang:plsql decode:true">SELECT *
FROM emp e LEFT OUTER JOIN dept d ON(e.deptno=d.deptno);</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle8-7.png" alt="" width="690" height="358" />

左外，emp的所有雇员（主要是刘小忙）全显示了。
<pre class="lang:plsql decode:true">SELECT *
FROM emp e RIGHT OUTER JOIN dept d ON(e.deptno=d.deptno);</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle8-8.png" alt="" width="656" height="371" />

右外，40部门显示出来了。
<pre class="lang:plsql decode:true">SELECT *
FROM emp e FULL OUTER JOIN dept d ON(e.deptno=d.deptno);</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle8-9.png" alt="" width="651" height="357" />

全外连接，刘小忙和40部门都显示出来了，这都全显示了

如果想用全外连接只能使用以上语法完成。
<h2>2.3 查询结果连接</h2>
在数学中存在交集、并集、差集的概念，那么此概念在SQL中同样存在，SQL提供了：UNION、UNION ALL、INTERSECT、MINUS实现结合操作，这几个符号可以连接多个SQL。使用的基本语法如下：
<pre class="lang:plsql mark:5,10 decode:true ">SELECT [DISTINCT] * | 列名称 [别名], 列名称 [别名],…
FROM 数据表 [别名], 数据表 [别名],…
[WHERE 条件(s)]
[ORDER BY 字段 [ASC | DESC], 字段 [ASC | DESC], …]

       [UNION | UNION ALL | INTERSECT | MINUS]

SELECT [DISTINCT] * | 列名称 [别名], 列名称 [别名],…
FROM 数据表 [别名], 数据表 [别名],…
[WHERE 条件(s)]
[ORDER BY 字段 [ASC | DESC], 字段 [ASC | DESC], …]

       [UNION | UNION ALL | INTERSECT | MINUS]

SELECT [DISTINCT] * | 列名称 [别名], 列名称 [别名],…
FROM 数据表 [别名], 数据表 [别名],…
[WHERE 条件(s)]
[ORDER BY 字段 [ASC | DESC], 字段 [ASC | DESC], …]
…</pre>
<h3>2.3.1 范例：验证UNION操作，不显示重复记录</h3>
<pre class="lang:plsql decode:true">SELECT * FROM emp
UNION
SELECT * FROM emp WHERE deptno=10;</pre>
<h3>2.3.2 范例：验证UNION ALL操作，显示所有数据，包含重复数据</h3>
<pre class="lang:plsql decode:true">SELECT * FROM emp
UNION ALL
SELECT * FROM emp WHERE deptno=10;</pre>
<h3>2.3.3 范例：验证INTERSECT操作，返回相同部分，交集</h3>
<pre class="lang:plsql decode:true">SELECT * FROM emp WHERE deptno=10
INTERSECT
SELECT * FROM emp;</pre>
<h3>2.3.4 范例：验证MINUS操作，返回不同的部分，是一个差集</h3>
被减集合 - 减集合 = 差集合

若被减集合是减集合的子集，那么差集合为空。
<pre class="lang:plsql decode:true">SELECT * FROM emp WHERE deptno=10
INTERSECT
SELECT * FROM emp;</pre>
该结果是空集，因为被减集合是减集合的子集。

工作中，有步伐查询会利用以上的方式以简化查询的复杂度。
<h1>3. 参考</h1>
<ul>
	<li>『李兴华java培训01』Oracle数据库 - 网易云课堂  http://study.163.com/course/courseLearn.htm?courseId=932016#/learn/video?lessonId=1118054&amp;courseId=932016</li>
</ul>
