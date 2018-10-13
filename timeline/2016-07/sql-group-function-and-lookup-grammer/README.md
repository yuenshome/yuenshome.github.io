[![header](../../../assets/header36.jpg)](https://yuenshome.github.io)

# 分组函数与分组统计查询的SQL语法

内容参考《李兴华Oracle数据库》的SQL分组函数与分组统计查询部分（链接见文末参考），本节主要包括：[toc]<!--more-->
<h1>1.组函数</h1>
COUNT函数主要功能是表中数据量统计，属于一种统计函数。常见的五个统计函数：COUNT()、SUN()、AVG()、MAX()、MIN()。
<h2>1.2 <strong>范例</strong>：统计所有雇员的人数、支付的总工资、平均工资、最高工资、最低工资</h2>
<pre class="lang:plsql decode:true">SELECT COUNT(*), SUM(sal), AVG(sal), MAX(sal) MIN(sal)
FROM emp;</pre>
<h2>1.2 <strong>范例</strong>：统计公司支付的总年薪与平均年薪</h2>
<pre class="lang:plsql decode:true ">SELECT SUM((sal+NVL(comm, 0)) * 12), AVG((sal+NVL(comm, 0) * 12)
FROM emp;</pre>
<h2>1.3 <strong>范例</strong>：求出公司最早雇佣雇员的日期和最晚雇佣雇员的日期</h2>
<pre class="lang:plsql decode:true">SELECT MAX(hiredate), MIN(hiredate)
FROM emp;</pre>
<h2>1.4 <strong>面试题</strong>：请解释“COUNT(*)”、“COUNT(字段)”、“COUNT(DISTINCT 字段)”</h2>
<pre class="lang:plsql decode:true">SELECT COUNT(*), COUNT(empno), COUNT(comm), COUNT(DISTINCT job)
FROM emp;</pre>
<ul>
	<li>COUNT(*)：会实际地统计出表中的数据量；</li>
	<li>COUNT(字段)：
<ul>
	<li>如果统计的字段不包含有NULL，那么与COUNT(*)结果相同；</li>
	<li>如果统计的字段包含有NULL，NULL不参与统计；</li>
</ul>
</li>
	<li>COUNT(DISTINCT 字段)：消除重复数据后统计。</li>
</ul>
<h1>2. 分组统计（<span style="color: #ff0000;"><strong>难点</strong></span>）</h1>
在讲解具体的分组统计操作前必须要先解决一个问题，什么情况下有可能分组？
<ul>
	<li>例如：全班，男生一组，女生一组，互相拔河；</li>
	<li>例如：全班，戴眼镜一组，不戴眼镜一组，互相比裸眼视力；</li>
	<li>例如：25岁前一组，之后一组，比年轻。</li>
</ul>
实际上所谓的分组，指的某些群体具备共同的特征。现在回到emp表中，可以发现job和deptno因为存在重复数据，所以可分组，

但一定要记住，一条记录也可分组。只不过这类的做法没有意义。如果要实现统计查询，那么可以采用如下的语法完成，如果想实现统计查询，那么可采用如下语法完成：
<table width="593">
<tbody>
<tr>
<td rowspan="5" width="432">SELECT [DISTINCT] * | 列名称 [别名], 列名称 [别名], … | 统计函数

FROM 数据表 [别名], 数据表 [别名], …

[WHERE 条件(s)]

<span style="color: #ff0000;">[<strong>GROUP BY </strong><strong>分组字段, </strong><strong>分组字段, …</strong>]</span>

[ORDER BY 字段 [ASC | DESC], 字段 [ASC | DESC], …]</td>
<td width="161">步骤四：确定查询列</td>
</tr>
<tr>
<td width="161">步骤一：确定数据源</td>
</tr>
<tr>
<td width="161">步骤二：过滤数据行</td>
</tr>
<tr>
<td width="161"><span style="color: #ff0000;"><strong>步骤三：执行分组操作</strong></span></td>
</tr>
<tr>
<td width="161">步骤五：数据排序</td>
</tr>
</tbody>
</table>
<h2><strong>2.1 </strong><strong>范例</strong>：按职位分组，统计出每个职位的平均工资、最高和最低工资、人数</h2>
<pre class="lang:plsql decode:true">SELECT job, AVG(sal), MAX(sal), MIN(sal), COUNT(*)
FROM emp
GROUP BY job;</pre>
<h2><strong>2.2 </strong><strong>范例</strong>：按照部门编号分组，统计出每个部门的人数、平均工资、平均服务年限</h2>
<pre class="lang:plsql decode:true">SELECT COUNT(*), AVG(sal), AVG(MONTHS_BETWEEN(SYSDATE, hiredate)/12)
FROM emp
GROUP BY deptno;</pre>
<h2><span style="color: #ff0000;"><strong>提示：关于统计查询的几个重要说明</strong></span></h2>
以上的代码只是根据基础语法实现了统计操作，但在整个操作中仍有三个限制：
<ul>
	<li><span style="color: #ff0000;"><strong>限制一：统计函数在单独使用的时候（没有GROUP BY</strong><strong>子句）只能够出现统计函数，不能够出现其它字段。</strong></span></li>
</ul>
<table>
<tbody>
<tr>
<td width="284">正确代码</td>
<td width="284"><span style="color: #339966;"><strong>错误代码</strong></span></td>
</tr>
<tr>
<td width="284">SELECT COUNT(empno)
FROM emp;</td>
<td width="284"><span style="color: #339966;">SELECT COUNT(empno), <span style="color: #ff0000;"><strong>ename</strong></span></span>
<span style="color: #339966;"> FROM emp;</span></td>
</tr>
</tbody>
</table>
<ul>
	<li><span style="color: #ff0000;"><strong>限制二：使用统计查询时（存在GROUP BY</strong><strong>子句），SELECT</strong><strong>子句之中只能出现统计函数的分组字段，其它字段都不允许出现。</strong></span></li>
</ul>
<table>
<tbody>
<tr>
<td width="284">正确代码</td>
<td width="284"><span style="color: #339966;"><strong>错误代码</strong></span></td>
</tr>
<tr>
<td width="284">SELECT deptno, COUNT(empno)
FROM emp
GROUP BY detpno;</td>
<td width="284"><span style="color: #339966;">SELECT deptno, COUNT(empno),<strong><span style="color: #ff0000;">ename</span></strong></span>
<span style="color: #339966;"> FROM emp</span>
<span style="color: #339966;"> GROUP BY detpno;</span></td>
</tr>
</tbody>
</table>
<ul>
	<li><span style="color: #ff0000;"><strong>限制三：统计函数在分组之中可以嵌套使用，但是嵌套之后的统计查询之中，不能使用任何字段，包括统计分组字段。</strong></span></li>
</ul>
<table>
<tbody>
<tr>
<td width="284">正确代码</td>
<td width="284"><span style="color: #339966;"><strong>错误代码</strong></span></td>
</tr>
<tr>
<td width="284">SELECT MAX(COUNT(empno))
FROM emp
GROUP BY detpno;</td>
<td width="284"><span style="color: #339966;">SELECT <strong><span style="color: #ff0000;">deptno</span></strong>, MAX(COUNT(empno))</span>
<span style="color: #339966;"> FROM emp</span>
<span style="color: #339966;"> GROUP BY detpno;</span></td>
</tr>
</tbody>
</table>
在整个分组操作之中，以上的三个限制可以是最难理解的。一定要搞清楚。

以上的操作都是针对单张表一个字段的分组，那么通过先前给出的语法格式可发现，分组可设置多个字段，那么这就要求这多个字段必须同时重复。
<h2><strong>2.3 </strong><strong>范例（开始）</strong>：查询出每个部门的名称、部门人数、平均工资</h2>
<ul>
	<li>确定所需数据表：
<ul>
	<li>dept表：找到部门名称；</li>
	<li>emp表：统计部门人数、平均工资。</li>
</ul>
</li>
	<li>确定已知的关联字段：
<ul>
	<li>雇员和部门关联：deptno=dept.deptno</li>
</ul>
</li>
</ul>
<pre class="lang:plsql decode:true">SELECT d.dname, e.empno, e.sal
FROM emp e, dept d
WHERE e.deptno=d.deptno;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle9-1.png" alt="" width="411" height="311" />

第二步：在上面查询结果中发现部门名称是重复的，但此时并不是一张实际表操作，而是针对一张临时查询结果进行分组操作，由于GROUP BY在WHERE之后执行，所以可直接分组。
<pre class="lang:plsql decode:true">SELECT d.dname, COUNT(e.empno), AVG(e.sal)
FROM emp e, dept d
WHERE e.deptno=d.dpetno
GROUP BY d.dname;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle9-2.png" alt="" width="380" height="145" />

第三步：现在引入dept表，其中表示有四个部门，而此时只有三个部门，那么此时就需要外连接进行40部门的数据显示。
<pre class="lang:plsql decode:true">SELECT d.dname, COUNT(e.empno), AVG(e.sal)
FROM emp e, dept d
WHERE e.deptno(+)=d.dpetno
GROUP BY d.dname;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle9-3.png" alt="" width="390" height="159" />
<h2><strong>2.4 </strong><strong>范例（完结）</strong>：查询出每个部门的编号、名称、位置、部门人数、平均工资</h2>
<ul>
	<li>确定所需数据表：
<ul>
	<li>dept表：找到部门编号、名称、位置；</li>
	<li>emp表：统计部门人数、平均工资。</li>
</ul>
</li>
	<li>确定已知的关联字段：
<ul>
	<li>雇员和部门关联：deptno=dept.deptno</li>
</ul>
</li>
</ul>
<strong>第一步</strong>：首先不去考虑分组，先实现多表查询，查询部门编号、名称、位置、雇员编号
<pre class="lang:plsql decode:true">SELECT d.deptno, d.dname, e.loc, d.empno, e.ename
FROM emp e, dept d
WHERE e.deptno=d.dpetno;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle9-4.png" alt="" width="507" height="244" />

现在发现部门的数据三个字段都是在整体重复着，那么针对三个字段实现分组：
<pre class="lang:plsql decode:true">SELECT d.deptno, d.dname, d.loc, COUNT(e.empno), AVG(e.ename)
FROM emp e, dept d
WHERE e.deptno(+)=d.dpetno
GROUP BY d.deptno, d.dname, d.loc;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle9-5.png" alt="" width="457" height="139" />

<strong>2.5 </strong><strong>范例</strong>：按照职位分组，统计每个职位的平均工资，要求显示的是平均工资高于2000的职位信息

现在唯一可以想到的条件过滤只有WHERE子句一个。于是现在编写出如下代码：
<pre class="lang:plsql decode:true">SELECT
FROM emp
WHERE AVG(sal)&gt;2000
GROUP BY job;</pre>
意思是WHERE子句中不能够使用分组（即统计）函数，通过先前的分析也该知道，WHERE是在GROUP BY之前使用的，而此时的条件明显是在GROUP BY后的过滤，这时只能使用HAVING子句完成，语法如下：
<table>
<tbody>
<tr>
<td rowspan="6" width="423">SELECT [DISTINCT] * | 列名称 [别名], 列名称 [别名], … | 统计函数

FROM 数据表 [别名], 数据表 [别名], …

[WHERE 条件(s)]

[GROUP BY 分组字段, 分组字段,…]

<span style="color: #ff0000;">[<strong>HAVING </strong><strong>分组后过滤</strong>]</span>

[ORDER BY 字段 [ASC | DESC], 字段 [ASC | DESC], …]</td>
<td width="145">5.确定数据列</td>
</tr>
<tr>
<td width="145">1.数据源</td>
</tr>
<tr>
<td width="145">2.过滤数据行</td>
</tr>
<tr>
<td width="145">3.执行分组</td>
</tr>
<tr>
<td width="145"><span style="color: #ff0000;"><strong>4.</strong><strong>针对分组过滤</strong></span></td>
</tr>
<tr>
<td width="145">6.数据排序</td>
</tr>
</tbody>
</table>
<h2><strong>2.6 </strong><strong>范例</strong>：使用HAVING修改程序</h2>
<pre class="lang:plsql decode:true">SELECT job, AVG(sal)
FROM emp
GROUP BY job
HAVING AVG(sal) &gt; 2000;</pre>
<h2>解释：关于WHERE和HAVING的区别</h2>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle9-6.png" alt="" width="448" height="327" />
<ul>
	<li><span style="color: #ff0000;"><strong>WHERE</strong><strong>子句：是在分组前使用，而且不能够使用统计函数进行验证，经过WHERE</strong><strong>筛选后的数据才可分组；</strong></span></li>
	<li><span style="color: #ff0000;"><strong>HAVING</strong><strong>子句：必须结合GROUP BY</strong><strong>子句一起出现，是在分组后过滤，可以使用统计函数。</strong></span></li>
</ul>
<h2><strong>2.7 </strong><strong>思考题一</strong>：统计公司每个工资等级的人数、平均工资</h2>
<ul>
	<li>确定要使用的数据表：
<ul>
	<li>salgrade表：工资等级；</li>
	<li>emp表：统计人数、平均工资；</li>
</ul>
</li>
	<li>确定已知的关联字段：
<ul>
	<li>工资与工资等级：sal BETWEEN salgrade.losal AND salgrade.hisal。</li>
</ul>
</li>
</ul>
第一步：查询出工资等级，以及每个雇员的编号、工资，直接实现了多表查询。
<pre class="lang:plsql decode:true ">SELECT
FROM emp e, salgrade s
WHERE e.sal BETWEEN s.losal AND hisal;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle9-7.png" alt="" width="357" height="337" />

第二步：针对临时数据表中的数据进行分组
<pre class="lang:plsql decode:true ">SELECT s.grade, COUNT(e.empno), AVG(e.sal)
FROM emp e, salgrade s
WHERE e.sal BETWEEN s.losal AND hisal;
GROUP BY s.grade;</pre>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads%2F2016%2F07%2Foracle9-8.png" alt="" width="361" height="215" />
<h2><strong>2.8 </strong><strong>思考题二</strong>：统计出领取佣金和不领取佣金的人数、平均工资</h2>
现在很容易联想到，针对comm字段分组，写出如下语句：
<pre class="lang:plsql decode:true">SELECT comm, COUNT(empno), AVG(sal)
FROM emp
GROUP BY comm;</pre>
所以此时这个查询结果发现并不理想，不如换个思路。

<strong>第一步</strong>：统计出所有领取佣金的人数
<pre class="lang:plsql decode:true ">SELECT ‘领取佣金’, COUNT(empno), AVG(sal)
FROM emp
WHERE comm IS NOT NULL;</pre>
<strong>第二步</strong>：统计出所有不领取佣金的人数
<pre class="lang:plsql decode:true">SELECT ‘不领取佣金’, COUNT(empno), AVG(sal)
FROM emp
WHERE comm IS NULL;</pre>
<strong>第三步</strong>：以上两个查询返回的结构完全相同，合并为一个结果
<pre class="lang:plsql decode:true ">SELECT ‘领取佣金’, COUNT(empno), AVG(sal)
FROM emp
WHERE comm IS NOT NULL
       UNION
SELECT ‘不领取佣金’, COUNT(empno), AVG(sal)
FROM emp
WHERE comm IS NULL;</pre>
很多时候都可以直接进行分组，但是在进行分组的时候，必须保证数据列上有重复，但如果是针对一些条件，只能利用以上的方式完成。
<h1>3. 参考</h1>
<ul>
	<li>『李兴华java培训01』Oracle数据库 - 网易云课堂 http://study.163.com/course/courseLearn.htm?courseId=932016#/learn/video?lessonId=1116382&amp;courseId=932016</li>
</ul>
