[![header](../assets/header02.jpg)](https://yuenshome.github.io)

# Oracle数据库思考题：SQL基本命令、简单查询、限定查询

内容参考《李兴华Oracle数据库》的思考题A部分（包含：SQL基本命令、简单查询、限定查询，链接见文末参考），本节主要包括：[toc]<!--more-->
<h1>1. 思考题</h1>
<h2>1.1 选择部门30中的所有员工</h2>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE deptno=30;</pre>
<h2>1.2 列出所有办事员（CLERK）的姓名，编号和部门编号</h2>
<pre class="lang:plsql decode:true ">SELECT ename, empno, deptno
FROM emp
WHERE job=’CLERK’;</pre>
<h2>1.3 找出佣金高于薪金60%的员工</h2>
需要乘以0.6：
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE comm&gt;sal*0.6;</pre>
<h2>1.4 找出部门10中所有经理（MANAGER）） 和部门20中所有办事员（CLERK）的详细资料</h2>
<ul>
	<li>条件一：部门10中所有经理，deptno=10 AND job=’MANAGER’；</li>
	<li>条件二：部门20中所有办事员，deptno=20 AND job=’CLERK’。</li>
</ul>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE (deptno=10 AND job=’MANAGER’) OR
      (deptno=20 AND job=’CLERK’);</pre>
<h2>1.5 找出部门10中所有经理（MANAGER），部门20中所有办事员（CLERK），既不是经理也不是办事员并且薪金大于等于2000的所有员工的详细资料</h2>
<ul>
	<li>条件一：部门10中所有经理，deptno=10 AND job=’MANAGER’；</li>
	<li>条件二：部门20中所有办事员，deptno=20 AND job=’CLERK’；</li>
	<li>条件三：不是经理也不是办事员或薪金等于2000，job NOT IN ('MANAGER', 'CLERK') AND sal&gt;=2000。</li>
</ul>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE (deptno=10 AND jpb=’MANAGER’) OR
      (deptno=20 AND job=’CLERK’) OR
      (job NOT IN (‘MANAGER’,  ‘CLERK’) AND sal&gt;=2000);</pre>
<h2>1.6 找出收取佣金的员工的不同工作。</h2>
佣金上存在NULL数据，所以现在需要使用IS NOT NULL来处理领取佣金，而且需要使用DISTINCT来消除重复行。
<pre class="lang:plsql decode:true">SELECT DISTINCT job
FROM emp
WHERE comm IS NOT NULL;</pre>
<h2>1.7 找出不收取佣金或收取佣金低于100的员工。</h2>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE comm IS NULL OR comm&lt;100;</pre>
<h2>1.8 显示不带有R的员工姓名</h2>
<pre class="lang:plsql decode:true">SELECT ename
FROM emp
WHERE ename NOT LIKE ‘%R%’;</pre>
<h2>1.9 显示姓名字段的任何位置包含“A”的所有员工的姓名，显示结果按照基本工资由高到低排序，如果基本工资相同，按照雇佣年限从早到晚排序</h2>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
WHERE ename LIKE ‘%A%’
ORDER BY sal DESC, hiredate ASC, job;</pre>
最后job省去默认的ASC，对职位排。
<h1>2. 总结</h1>
<h2>2.1 Oracle数据库之中的三个主要用户：</h2>
<ul>
	<li>超级管理员：sys/change_on_install，实际工作中不能是这个密码；</li>
	<li>普通管理员：system/manager，实际工作中不能是这个密码；</li>
	<li>普通用户：scott/tiger，需要解锁，此用户是一个非常经典的用户。</li>
</ul>
<h2>2.2 SQLPlus的几个基本命令；</h2>
<h2>2.3 已给出的SQL语法</h2>
<table width="659">
<tbody>
<tr>
<td width="329"><span style="color: #ff0000;"><strong>SELECT</strong></span> [DISTINCT] * | 列 [别名], 列 [别名],…</td>
<td width="331">步骤三：控制要显示的数据列</td>
</tr>
<tr>
<td width="329"><span style="color: #ff0000;"><strong>FROM</strong></span> 表名称 [别名]</td>
<td width="331">步骤一：确定数据源</td>
</tr>
<tr>
<td width="329">[<span style="color: #ff0000;"><strong>WHERE</strong></span> 条件(s)]</td>
<td width="331">步骤二：根据判断条件选择参与的数据行</td>
</tr>
<tr>
<td width="329">[<span style="color: #ff0000;"><strong>ORDER BY</strong></span> 字段 [ASC|DESC], 字段 [ASC|DESC], ..]</td>
<td width="331">步骤四：数据排序</td>
</tr>
</tbody>
</table>
清楚每个子句的作用：
<ol>
	<li>FROM：确定数据来源；</li>
	<li>WHERE：进行数据行的筛选；</li>
	<li>SELECT：进行数据列的筛选；</li>
	<li>ORDER BY：进行最终数据的排序。</li>
</ol>
ORDER BY子句永远写在最后一行。
<h1>3. 参考</h1>
<ul>
	<li>『李兴华java培训01』Oracle数据库 - 网易云课堂  http://study.163.com/course/courseLearn.htm?courseId=932016#/learn/video?lessonId=1115370&amp;courseId=932016</li>
</ul>
