[![header](../../../assets/header32.jpg)](https://yuenshome.github.io)

<script type="text/javascript" async src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML"> </script>

# 感知机系列1：感知机模型和学习策略

学习《统计学习方法》中第二章 感知机学习笔记。更多的是摘抄，基于例子对算法的实现等。
<blockquote>感知机（感知器， perceptron ）是二分类的线性模型，输入为样本（实例）的特征向量，输出为样本类别（ $-1$ 或 $+1$ ）。由于其会对输入空间（特征空间）中将实例划分为正负两类的分离超平面，故<strong><span style="color: #ff0000;">属于判别模型</span></strong>。

感知机学习的过程是求出训练数据进行线性划分的分离超平面，做法是通过<strong><span style="color: #ff0000;">基于错分类样本</span></strong>的损失函数，利用梯度下降法对损失函数极小化，求得模型参数。

特点是简单且易于实现，<strong><span style="color: #ff0000;">分为原始形式和对偶形式</span></strong>。感知机 1957 年由 Rosenblatt 提出，<strong><span style="color: #ff0000;">是神经网络与支持向量机的基础</span></strong>。</blockquote>
[toc]

<!--more-->
<h1>1. 感知机模型</h1>
<strong>定义1 （感知机）</strong> 假设输入空间（特征空间）是 $\chi \subseteq \Re^n$ ，输出空间是 $\gamma = \{+1,-1\}$ 。输入 $x \in \chi$ 表示实例（样本）的特征向量，对应于输入空间（特征空间）的点；输出 $y \subseteq \gamma$ 表示实例的类别。由输入空间到输出空间的如下函数

$$
f(x) = \text{sign}(w \cdot x + b)
$$

称为感知机。其中， $w$ 和 $b$ 为感知机模型参数， $w \in \Re^n$ 叫做权值（ weight ）或权值向量（ weight vector ）， $b \in \Re$ 叫做偏置（bias）， $w \cdot x$ 表示 $w$ 和 $x$ 的内积。 $\text{sign}$ 是符号函数，即

$$
\begin{eqnarray}
\text{sign(x)} =
\begin{cases}
+1, &amp; x \geq 0 \\
-1, &amp; x &lt; 0
\end{cases}
\end{eqnarray}
$$

感知机是一种线性分类模型，属于判别模型。其假设空间是定义在特征空间中的所有线性分类模型（ linear classification model ）或线性分类器（ linear classifier ），即函数集合 $\{f|f(x)=w \cdot x + b\}$ 。

感知机有如下几何解释：线性方程

$$
w \cdot x + b = 0
$$

对应于特征空间 $\Re^n$ 中的一个超平面 $S$ ，其中 $w$ 是超平面的法向量， $b$ 是超平面的截距。这个超平面将特征空间划分为两个部分，位于两部分的点（特征向量）分别被分为正、负两类。因此，超平面 $S$ 称为分离超平面（ separating hyperplane），如下图所示。

<img class="aligncenter" src="./assets/perceptron%20model.png" alt="" width="389" height="323" />

感知机学习基于训练数据集（实例样本的特征向量及真实类别） $T = \{(x_1, y_1), (x_2, y_2), ..., (x_N, y_N)\}$ ，其中 $x_i \in \chi = \Re^n, y_i \in \gamma = \{+1, -1\} , i=1,2,...,N$ 。求得感知机模型（即 $f(x) = \text{sign}(w \cdot x + b)$），即求得模型参数 $w,b$ 。此时对新输入实例样本即可给出对应的输出类别。
<h1>2. 感知机学习策略</h1>
<h2>2.1 数据集的线性可分性</h2>
定义2 （数据集的线性可分性） 给定一个数据集 $T=\{(x_1, y_1), (x_2, y_2), ..., (x_N, y_N)\}$ ，其中， $x_i \in \chi = \Re^n , y_i \in \gamma = \{+1, -1\}, i=1,2,...,N$ ，如果存在某个超平面 $S$ ，即

$$
w \cdot x + b = 0
$$

能够将数据集的正实例点和负实例点完全正确地划分到超平面的两侧，即对所有 $y_i=+1$ 的实例 $i$ ，有 $w \cdot x_i + b &gt; 0$ ，对所有 $y_i = -1$ 的实例 $i$ ，有 $w \cdot x_i + b &lt; 0$ ，则数据集 $T$ 为线性可分数据集（ linearly separable data set）；否则，称数据集 $T$ 线性不可分。
<h2>2.2 感知机学习策略</h2>
假设训练数据集是线性可分的，感知机学习的目标是求得一个能够将训练集正实例点和负实例点完全正确分开的分离超平面。为了找出这样的超平面，即确定感知机模型参数 $w,b$ ，需要确定一个学习策略，即定义（经验）损失函数并将损失函数极小化。

损失函数的一个自然选择是误分类点的总数。但是，这样的损失函数不是参数 $w,b$ 的连续可导函数，不易优化。损失函数的另一个选择是误分类点到超平面 $S$ 的总距离，这是感知机所采用的。为此，首先写出<strong><span style="color: #ff0000;">输入空间 $\Re^n$ 中一点 $x_0$ 到超平面 $S$ 的距离：</span></strong>

$$ \frac{1}{||w||} |w \cdot x_0 + b| $$

这里，$||w||$ 是 $w$ 的 $L_2$ 范数。

其次，对于误分类的数据$(x_i, y_i)$ 来说，

$$
-y_i (w \cdot x_i + b) &gt; 0
$$

成立，因为当 $w \cdot x_i + b &gt; 0 $ 时， $y_i = -1$ ，而当 $w \cdot x_i + b &lt; 0$ 时， $y_i = +1$ ，因此，误分类点 $x_i$ 到超平面 $S$ 的距离是

$$
-\frac{1}{||w||} \sum_{x_i \in M } y_i (w \cdot x_i + b)
$$

不考虑 $\frac{1}{||w||}$ ，就得到感知机学习的损失函数。

给定训练数据集 $T=\{(x_1, y_1), (x_2, y_2), ..., (x_N, y_N)\}$ ，其中 $x_i \in \chi = \Re^n , y_i \in \gamma = {+1, -1}, i=1,2,..., N$ 。感知机 $\text{sign}(w \cdot x + b)$ 学习的损失函数定义为：

$$
L(w,b) = -\sum_{x_i \in M } y_i(w \cdot x_i + b)
$$

其中 $M$ 为误分类点的集合，这个损失函数就是感知机学习的经验风险函数。

显然，损失函数 $L(w,b)$ 是非负的。如果没有误分类点，损失函数值是 $0$ 。而且，误分类点越少，误分类点离超平面越近，损失函数值就越小。一个特定的样本点的损失函数： 在分类时的参数 $w,b$ 的线性函数，在正确分类时是 $0$ 。因此，给定训练数据集 $T$ ，损失函数 $L(w,b)$ 是 $w,b$ 的连续可导函数。

感知机学习的策略是在假设空间中选取使损失函数 $L(w,b) = -\sum_{x_i \in M } y_i(w \cdot x_i + b)$ 最小时的模型参数 $w,b$ ，即感知机模型。
<h1></h1>
