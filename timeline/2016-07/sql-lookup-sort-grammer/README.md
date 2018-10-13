[![header](../../../assets/header13.jpg)](https://yuenshome.github.io)

# 查询排序的SQL语法

内容参考《李兴华Oracle数据库》的查询排序部分（链接见文末参考），本节主要包括：[toc]<!--more-->
<h1>1. 查询排序（重点）</h1>
若希望查询出的数据可按升序或降序排列，可使用ORDER BY子句，此子句的语法如下：
<table width="584">
<tbody>
<tr>
<td rowspan="4" width="291">SELECT [DISTINCT] * | 列 [别名], 列 [别名], …

FROM 表名称 [别名]

[WHERE 条件(s)];

[ORDER BY 字段 [ASC|DESC], 字段 [ASC|DESC], ..]</td>
<td width="293">步骤三：控制要显示的数据</td>
</tr>
<tr>
<td width="293">步骤一：确定数据源</td>
</tr>
<tr>
<td width="293">步骤二：根据判断条件选择参与的数据行数</td>
</tr>
<tr>
<td width="293">步骤四：数据排序</td>
</tr>
</tbody>
</table>
在以上的语法之中最重要的一点：<strong><span style="color: #ff6600;">ORDER BY子句是写在最后行的，而且永恒在最后一行</span></strong>，同时ORDER BY也是在SELECT子句之后执行的，那么也就可以得出一个结论：ORDER BY可以使用SELECT子句里面定义的别名。

在使用ORDER BY排序的时候有两种排序的模式：
<ul>
	<li>ASC：升序，是默认方式；</li>
	<li>DESC：降序，必须由用户设置。</li>
</ul>
<h2><strong>1.1 范例</strong>：对工资按降序排序。</h2>
<pre class="lang:plsql decode:true">SELECT *
FROM emp
ORDER BY sal DESC;</pre>
<h2><strong>1.2 范例</strong>：对工资降序排序，若工资相同，按雇佣日期由早到晚排序</h2>
在日期的表示中，日期越早，数字越小，由早到晚意味着升序。
<pre class="lang:plsql decode:true">SELECT *
FROM emp
ORDER BY sal DESC, hiredate ASC;</pre>
<h2><strong>1.3 范例</strong>：按年薪排序，因为ORDER BY是在SELECT后执行</h2>
&nbsp;
<pre class="lang:plsql decode:true">SELECT empno, ename, (sal*12) income
FROM emp
ORDER BY income;</pre>
<h2><strong>1.4 范例</strong>：查询出所有办事员的信息，按工资降序排序</h2>
<pre class="lang:plsql decode:true">SELECT empno, ename, sal
FROM emp
WHERE job=’CLERK’
ORDER BY sal DESC;</pre>
目前为止最重要的SQL基本语法就以上知识。
<h1>2. 参考</h1>
<ul>
	<li>『李兴华java培训01』Oracle数据库 - 网易云课堂  http://study.163.com/course/courseLearn.htm?courseId=932016#/learn/video?lessonId=1116380&amp;courseId=932016</li>
</ul>
