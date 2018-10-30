[![header](../../../assets/header12.jpg)](https://yuenshome.github.io)

<script type="text/javascript" async src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML"> </script>

# 朴素贝叶斯法系列1：学习与分类

《统计学习方法》 第4章 朴素贝叶斯法

朴素贝叶斯（naive Bayes）法是基于贝叶斯定理与特征条件独立假设的分类方法（<strong><span style="color: #ff0000;">朴素贝叶斯法与贝叶斯估计（Bayesian estimation）是不同的概念</span></strong>）。过程描述：
<ul>
	<li>对于给定的训练数据集，首先基于特征条件独立假设学习输入/输出的联合概率分布；</li>
	<li>然后基于此模型，对给定的输入 $x$ ，利用贝叶斯定理求出后验概率最大的输出 $y$ 。</li>
</ul>
朴素贝叶斯方法实现简单，学习与预测的效率都很高，是一种常用的方法。本章叙述朴素贝叶斯法，包括朴素贝叶斯的学习与分类、朴素贝叶斯法的参数估计算法。[toc]<!--more-->
<h1>1. 朴素贝叶斯法的学习与分类</h1>
<h2>1.1 基本方法</h2>
设输入空间 $\chi \subseteq \Re^n$ 为 $n$ 维向量的集合，输出空间为类标记集合 $\gamma=\{ c_1,c_2,...,c_K \}$ 。输入为特征空间 $x \in \chi$ ，输出为类标记（class label）$y \in \gamma$ 。 $X$ 是定义在输入空间 $\chi$ 上的随机向量， $Y$ 是定义在输出空间 $\gamma$ 上的随机变量。 $P(X,Y)$ 是 $X$ 和 $Y$ 的联合概率分布。训练数据集 $T = \{ (x_1,y_1), (x_2,y_2), ...,(x_N,y_N) \}$ 由于 $P(X, Y)$ 独立同分布产生。

朴素贝叶斯法通过训练数据集学习联合概率分布 $P(X,Y)$ 。具体地，学习以下先验概率分布及条件概率分布。先验概率分布

$$
P(Y=c_k), k=1,2,...,K
$$

条件概率分布

$$
P(X=x | Y=c_k) = P(X^{(l)} = x^{(l)}, ..., X^{(n)} = x^{(n)} | Y=c_k), k=1,2,...,K
$$

于是学习到联合概率分布 $P(X,Y)$ 。

条件概率分布 $P(X=x|Y=c_k)$ 有指数级量级的参数，其估计实际是不可行的。事实上，假设 $x^{(j)}$ 可取值有 $S_j$ 个， $j=1,2,...,n$ ， $Y$ 可取值有 $K$ 个，那么参数个数为 $K\prod^n_{j=1} S_j$ 。

朴素贝叶斯法对条件概率分布做了条件独立性的假设。由于这是一个较强的假设，朴素贝叶斯法也由此得名。具体地，条件独立性假设是

$$
\begin{align}
P(X=x|Y=c_k)=P(X^{(1)}&amp; = x^{(1)}, ..., X^{(1)}=x^{(1)} | Y=c_k) \\
&amp; = \prod^n_{j=1} P(X^{(j)}=x^{(j)} | Y=c_k )
\end{align}
$$

朴素贝叶斯法实际上学习到生成数据的机制，所以属于生成模型。条件独立假设等同于是说用于分类的特征在类确定的条件下都是条件独立的。这一假设使朴素贝叶斯法变得简单，但有时会牺牲一定的分类准确率。

朴素贝叶斯法分类时，对给你定的输入 $x$ ，通过学习到的模型计算后验概率分布 $P(Y=c_k|X=x)$ ，将后验概率最大的类作为 $x$ 的类输出。后验概率计算根据贝叶斯定理进行：

$$
P(Y=c_k|X=x)=\frac{P(X=x|Y=c_k)P(Y=c_k)}{\sum_k P(X=x|Y=c_k)P(Y=c_k)}
$$

将式 $P(X=x|Y=c_k)=\prod^n_{j=1} P(X^{(j)}=x^{(j)} | Y=c_k)$ 代入 上式，有：

$$
P(Y=c_k|X=x)=
\frac
{P(Y=c_k)\prod_j P(X^{(j)}=x^{(j)}|Y=c_k)}
{\sum_k P(Y=c_k) \prod_j P(X^{(j)} | Y=c_k) P(Y=c_k)}
k=1,2,...,K
$$

这是朴素贝叶斯法分类的基本公式，于是朴素贝叶斯分类器可表示为

$$
y=f(x)=\arg \ \max_{c_k}
\frac
{P(Y=c_k) \prod_j P(X^{(j)}=x^{(j)} | Y=c_k)}
{\sum_k P(Y=c_k) \prod_j P(X^{(j)}=x^{(j)}) | Y=c_k}
$$

注意到，在该式中分母对所有 $c_k$ 都是相同的，所以，

$$
y=\arg \max_{c_k} P(Y=c_k) \prod_j (X^{(j)}=x^{(j)} | Y=c_k)
$$
<h2>1.2 后验概率最大化的含义</h2>
朴素贝叶斯法将实例分到后验概率最大的类中。这等价于期望风险最小化。假设选择 0-1 损失函数：

$$
\begin{eqnarray}
L(Y,f(X)) =
\begin{cases}
1, &amp; Y \neq f(X) \\
0, &amp; Y = f(x)
\end{cases}
\end{eqnarray}
$$

式中 $f(X)$ 是分类决策函数，这时，期望风险函数为

$$
R_{\exp} (f) = E[L(Y, f(X))]
$$

期望是对联合分布 $P(X,Y)$ 取的。由此取条件期望

$$
R_{\exp} (f) = E_x \sum^{K}_{k=1}
[L(c_k, f(X))]
P(c_k|X)
$$

为了使期望风险最小化，只需对 $X=x$ 逐个极小化，由此得到：

$$
\begin{align}
f(x)
&amp;= \arg \min_{y \in \gamma} \sum^K_{k=1} L(c_k,y) P(c_k|X=x) \\
&amp;= \arg \min_{y \in \gamma} \sum^K_{k=1} P(y \neq c_k | X=x) \\
&amp;= \arg \min_{y \in \gamma} (1-P(y=c_k | X=x)) \\
&amp;= \arg \max_{y \in \gamma} P(y=c_k|X=x)
\end{align}
$$

这样一来，根据风险最小化准则就的到了后验概率最大化准则：

$$
f(x) = \arg \max_{c_k} P(c_k|X=x)
$$

即朴素贝叶斯法所采用的原理。
