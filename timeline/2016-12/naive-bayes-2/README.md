[![header](../../../assets/header17.jpg)](https://yuenshome.github.io)

<script type="text/javascript" async src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML"> </script>

《统计学习方法》 第4章 朴素贝叶斯法

[toc]<!--more-->
<h1>2 朴素贝叶斯法的参数估计</h1>
<h2>2.1 极大似然估计</h2>
在朴素贝叶斯法中，学习意味着估计 $P(Y=c_k)$ 和 $P(X^{(j)} = x^{(j)} | Y=c_k)$ 。可以应用极大似然法估计相应的概率。先验概率 $P(Y=c_k)$ 的极大似然估计是

$$
P(Y=c_k)=
\frac
{\sum^N_{i=1} I(y_i = c_k)}
{N}, k=1,2,...,K
$$

设第 $j$ 个特征 $x^{(j)}$ 可能取值的集合为 $\{ a_{j1}, a_{j2}, ..., a_{jS_j} \}$ ，条件概率 $P(X^{(j)} = a_{jl} | Y=c_k)$ 的极大似然估计是

$$
P(X^{(j)} = a_{jl} | Y=c_k)=
\frac
{\sum^N_{i=1} I(x^{(j)}_i = a_{ji}, y_i=c_k)}
{\sum^N_{i=1} I(y_i = c_k)} \\
j=1,2,...,n; l=1,2,...,S_j; k=1,2,...,K
$$

式中， $x^{(j)}_i$ 是第 $i$ 个样本的第 $j$ 个特征； $a_{jl}$ 是第 $j$ 个特征可能取得第 $i$ 个值； $I$ 为指示函数。
<h2>2.2 学习与分类算法</h2>
下面给出朴素贝叶斯法的学习与分类算法。

算法 4.1 （朴素贝叶斯算法(naive Bayes algorithm)）
<ul>
	<li>输入：训练数据 $T=\{(x_1, y_1), (x_2, y_2), ..., (X_N, y_N)\}$ ，其中 $x_i=(x^{(1)}, x^{(2)}, ..., x^{(n)})^T$ ， $x^{(j)}_i$ 是第 $i$ 个样本的第 $j$ 个特征，$x^{(j)}_i \in \{a_{j1},a_{j2},...,a_{jS_j} \}$ ， $a_{jl}$ 是第 $j$ 个特征可能取的第 $l$ 个值， $j=1,2,...,n; l=1,2,...,S_{j}; y_i \in \{c_1, c_2, ..., c_K\}$ ，实例 $x$ 。</li>
	<li>输出：实例 $x$ 的分类。</li>
</ul>
<ol>
	<li> 计算先验概率及条件概率
$$
\begin{align}
&amp;P(Y=c_k)=
\frac
{\sum^{N}_{i=1} I(y_i=c_k) }
{N}
k=1,2,...,K \\
&amp; P(X^{(j)}= a_{jl} | Y=c_k)
=\frac{\sum^N_{i=1} I(x^{(j)}_{i} = a_{jl}, y_i=c_k) }
{\sum^N_{i=1} I(y_i=c_k)} \\
&amp; j=1,2,...,n; l=1,2,...,S_j;k=1,2,...,K
\end{align}
$$</li>
	<li>对于给定的实例 $x=(x^{(1)}, x^{(2)}, ..., x^{(n)})^T$ ，计算
$$
<span style="line-height: 1.71429; font-size: 1rem;">P(Y=c_k) \prod^n_{j=1} P(X^{(j)}|Y=c_k),k=1,2,...,K
</span><span style="line-height: 1.71429; font-size: 1rem;">$$</span></li>
	<li>确定实例 $x$ 的类
$$
<span style="line-height: 1.71429; font-size: 1rem;">y=\arg \max_{c_k} P(Y=c_k) \prod^n_{j=1} P(X^{(j)} = x^{(j)} | Y=c_k)
</span><span style="line-height: 1.71429; font-size: 1rem;">$$</span></li>
</ol>
<h2>例1</h2>
试由表1的训练数据学习一个朴素贝叶斯分类器并确定 $x=(2,S)^T$ 的类标记 $y$ 。表中 $X^{(1)}, X^{(2)}$ 为特征，取值的集合分别为 $A_1 = \{1,2,3\}, A_2 = \{S, M, L\}$ ，$Y$ 为类标记， $Y \in C = \{1,-1\}$ 。
<table>
<thead>
<tr>
<th style="text-align: center;"></th>
<th style="text-align: center;">1</th>
<th style="text-align: center;">2</th>
<th style="text-align: center;">3</th>
<th style="text-align: center;">4</th>
<th style="text-align: center;">5</th>
<th style="text-align: center;">6</th>
<th style="text-align: center;">7</th>
<th style="text-align: center;">8</th>
<th style="text-align: center;">9</th>
<th style="text-align: center;">10</th>
<th style="text-align: center;">11</th>
<th style="text-align: center;">12</th>
<th style="text-align: center;">13</th>
<th style="text-align: center;">14</th>
<th style="text-align: center;">15</th>
</tr>
</thead>
<tbody>
<tr>
<td style="text-align: center;"><strong><span id="MathJax-Element-83-Frame" class="MathJax"><span id="MathJax-Span-2010" class="math"><span id="MathJax-Span-2011" class="mrow"><span id="MathJax-Span-2012" class="msubsup"><span id="MathJax-Span-2013" class="mi">X</span><span id="MathJax-Span-2014" class="texatom"><span id="MathJax-Span-2015" class="mrow"><span id="MathJax-Span-2016" class="mo">(</span><span id="MathJax-Span-2017" class="mn">1</span><span id="MathJax-Span-2018" class="mo">)</span></span></span></span></span></span></span></strong></td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">2</td>
<td style="text-align: center;">2</td>
<td style="text-align: center;">2</td>
<td style="text-align: center;">2</td>
<td style="text-align: center;">2</td>
<td style="text-align: center;">3</td>
<td style="text-align: center;">3</td>
<td style="text-align: center;">3</td>
<td style="text-align: center;">3</td>
<td style="text-align: center;">3</td>
</tr>
<tr>
<td style="text-align: center;"><strong><span id="MathJax-Element-84-Frame" class="MathJax"><span id="MathJax-Span-2019" class="math"><span id="MathJax-Span-2020" class="mrow"><span id="MathJax-Span-2021" class="msubsup"><span id="MathJax-Span-2022" class="mi">X</span><span id="MathJax-Span-2023" class="texatom"><span id="MathJax-Span-2024" class="mrow"><span id="MathJax-Span-2025" class="mo">(</span><span id="MathJax-Span-2026" class="mn">2</span><span id="MathJax-Span-2027" class="mo">)</span></span></span></span></span></span></span></strong></td>
<td style="text-align: center;"><span id="MathJax-Element-85-Frame" class="MathJax"><span id="MathJax-Span-2028" class="math"><span id="MathJax-Span-2029" class="mrow"><span id="MathJax-Span-2030" class="mi">S</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-86-Frame" class="MathJax"><span id="MathJax-Span-2031" class="math"><span id="MathJax-Span-2032" class="mrow"><span id="MathJax-Span-2033" class="mi">M</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-87-Frame" class="MathJax"><span id="MathJax-Span-2034" class="math"><span id="MathJax-Span-2035" class="mrow"><span id="MathJax-Span-2036" class="mi">M</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-88-Frame" class="MathJax"><span id="MathJax-Span-2037" class="math"><span id="MathJax-Span-2038" class="mrow"><span id="MathJax-Span-2039" class="mi">S</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-89-Frame" class="MathJax"><span id="MathJax-Span-2040" class="math"><span id="MathJax-Span-2041" class="mrow"><span id="MathJax-Span-2042" class="mi">S</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-90-Frame" class="MathJax"><span id="MathJax-Span-2043" class="math"><span id="MathJax-Span-2044" class="mrow"><span id="MathJax-Span-2045" class="mi">S</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-91-Frame" class="MathJax"><span id="MathJax-Span-2046" class="math"><span id="MathJax-Span-2047" class="mrow"><span id="MathJax-Span-2048" class="mi">M</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-92-Frame" class="MathJax"><span id="MathJax-Span-2049" class="math"><span id="MathJax-Span-2050" class="mrow"><span id="MathJax-Span-2051" class="mi">M</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-93-Frame" class="MathJax"><span id="MathJax-Span-2052" class="math"><span id="MathJax-Span-2053" class="mrow"><span id="MathJax-Span-2054" class="mi">L</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-94-Frame" class="MathJax"><span id="MathJax-Span-2055" class="math"><span id="MathJax-Span-2056" class="mrow"><span id="MathJax-Span-2057" class="mi">L</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-95-Frame" class="MathJax"><span id="MathJax-Span-2058" class="math"><span id="MathJax-Span-2059" class="mrow"><span id="MathJax-Span-2060" class="mi">L</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-96-Frame" class="MathJax"><span id="MathJax-Span-2061" class="math"><span id="MathJax-Span-2062" class="mrow"><span id="MathJax-Span-2063" class="mi">M</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-97-Frame" class="MathJax"><span id="MathJax-Span-2064" class="math"><span id="MathJax-Span-2065" class="mrow"><span id="MathJax-Span-2066" class="mi">M</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-98-Frame" class="MathJax"><span id="MathJax-Span-2067" class="math"><span id="MathJax-Span-2068" class="mrow"><span id="MathJax-Span-2069" class="mi">L</span></span></span></span></td>
<td style="text-align: center;"><span id="MathJax-Element-99-Frame" class="MathJax"><span id="MathJax-Span-2070" class="math"><span id="MathJax-Span-2071" class="mrow"><span id="MathJax-Span-2072" class="mi">L</span></span></span></span></td>
</tr>
<tr>
<td style="text-align: center;"><strong><span id="MathJax-Element-100-Frame" class="MathJax"><span id="MathJax-Span-2073" class="math"><span id="MathJax-Span-2074" class="mrow"><span id="MathJax-Span-2075" class="mi">Y</span></span></span></span></strong></td>
<td style="text-align: center;">-1</td>
<td style="text-align: center;">-1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">-1</td>
<td style="text-align: center;">-1</td>
<td style="text-align: center;">-1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">1</td>
<td style="text-align: center;">-1</td>
</tr>
</tbody>
</table>
解：根据算法1，由表1，容易计算下列概率：

$$
\begin{align}
&amp;
P(Y=1) = \frac{9}{15},
P(Y=-1) = \frac{6}{15} \\
&amp;
P(X^{(1)}=1 | Y=1) = \frac{2}{9},
P(X^{(1)}=2 | Y=1) = \frac{3}{9},
P(X^{(1)}=3 | Y=1) = \frac{4}{9} \\
&amp;
P(X^{(2)}=S | Y=1) = \frac{1}{9},
P(X^{(2)}=M | Y=1) = \frac{4}{9},
P(X^{(2)}=L | Y=1) = \frac{4}{9} \\
&amp;
P(X^{(1)}=1 | Y=-1) = \frac{3}{6},
P(X^{(1)}=2 | Y=-1) = \frac{2}{6},
P(X^{(1)}=3 | Y=-1) = \frac{1}{6} \\
&amp;
P(X^{(2)}=S | Y=-1) = \frac{3}{6},
P(X^{(2)}=M | Y=-1) = \frac{2}{6},
P(X^{(2)}=L | Y=-1) = \frac{1}{6}
\end{align}
$$

对于给定的 $x=(2,S)^T$ 计算：

$$
\begin{align}
&amp;
P(Y=1)
P(X^{(1)} | Y=1)
P(X^{(2)}=s | Y=1)
=
\frac{9}{15}
\frac{3}{9}
\frac{1}{9}
=
\frac{1}{45}
\\
&amp;
P(Y=-1)
P(X^{(1)} | Y=-1)
P(X^{(2)}=s | Y=-1)
=
\frac{6}{15}
\frac{1}{6}
\frac{3}{6}
=
\frac{1}{15}
\end{align}
$$

因为 $P(Y=-1) P(X^{(1)} | Y=-1) P(X^{(2)}=s | Y=-1)$ 最大，所以 $y=-1$ 。
<h2>2.3 贝叶斯估计</h2>
用极大似然估计可能会出现所要估计的概率值为 $0$ 的情况。这时会影响到后验概率的计算结果，使分类产生偏差。解决这一问题的方法是采用贝叶斯估计。具体地，条件概率的贝叶斯估计是

$$
P_{\lambda} (X^{(j)}=a_{jl} | Y=c_k)
=
\frac
{\sum^N_{i=1} I(x^{(j)}_i = a_{jl}, y_i=c_k) + \lambda}
{\sum^N_{i=1} I(y_i=c_k) + S_j \lambda}
$$

式中 $\lambda \geq 0$ 。等价于在随机变量各个取值的频数上赋予一个正数 $\lambda &gt; 0$ 。当 $\lambda = 0$ 时就是极大似然估计。常取 $\lambda=1$ ，这时称为拉普拉斯平滑（Laplace smoothing）。显然，对任何 $l=1,2,...,S_j, k=1,2,...,K$ ，有

$$
\begin{align}
&amp;
P_\lambda (X^{(j)} = a_{jl} | Y=c_k) &gt; 0 \\
&amp;
\sum^{S_j}_{l=1}P(X^{(j)} = a_{jl} | Y=c_k) = 1
\end{align}
$$

表明本小节一开始讲的公式 $P_{\lambda} (X^{(j)}=a_{jl} | Y=c_k)
=
\frac
{\sum^N_{i=1} I(x^{(j)}_i = a_{jl}, y_i=c_k) + \lambda}
{\sum^N_{i=1} I(y_i=c_k) + S_j \lambda}$ 确实为一种概率分布。同样，先验概率的贝叶斯估计是

$$
P_\lambda (Y=c_k)
=
\frac
{\sum^N_{i=1} I(y_i=c_k) + \lambda}
{N + K \lambda}
$$
<h2>例2</h2>
问题同例1，按照拉普拉斯平滑估计概率，即取 $\lambda = 1$ 。

解：$A_1 = \{1,2,3\}, A_2=\{S,M,L\}, C=\{1,-1\}$ 。按照本小节前面讲到的两个公式（条件概率的贝叶斯估计、先验概率的贝叶斯估计）计算下列概率：

$$
\begin{align}
&amp;
P(Y=1)=\frac{10}{17}, P(Y=-1)=\frac{7}{17}
\\
&amp;
P(X^{(1)}=1|Y=1)=\frac{3}{12},
P(X^{(1)}=2|Y=1)=\frac{4}{12},
P(X^{(1)}=3|Y=1)=\frac{5}{12}
\\
&amp;
P(X^{(2)}=S|Y=1)=\frac{2}{12},
P(X^{(2)}=M|Y=1)=\frac{5}{12},
P(X^{(2)}=L|Y=1)=\frac{5}{12}
\\
&amp;
P(X^{(1)}=1|Y=-1)=\frac{4}{9},
P(X^{(1)}=2|Y=-1)=\frac{3}{9},
P(X^{(1)}=3|Y=-1)=\frac{2}{9}
\\
&amp;
P(X^{(2)}=S|Y=-1)=\frac{4}{9},
P(X^{(2)}=M|Y=-1)=\frac{3}{9},
P(X^{(2)}=L|Y=-1)=\frac{2}{9}
\end{align}
$$

对于给定的 $x=(2,S)^T$ 计算：

$$
\begin{align}
&amp;
P(Y=1)P(X^{(1)}=2|Y=1)P(X^{(2)}=S|Y=1)
=\frac{10}{17}\frac{4}{12}\frac{2}{12}
=\frac{5}{153}
=0.0327
\\
&amp;
P(Y=-1)P(X^{(1)}=2|Y=-1)P(X^{(2)}=S|Y=-1)
=\frac{7}{17}\frac{3}{9}\frac{4}{9}
=\frac{28}{459}
=0.0610
\end{align}
$$

由于 $P(Y=-1)P(X^{(1)}=2|Y=-1)P(X^{(2)}=S|Y=-1)$ 最大，所以 $y=-1$ 。
<h1>3. 本章概要</h1>
<ol>
	<li>朴素贝叶斯法是典型的生成学习方法。生成方法由训练数据学习联合概率分布 $P(X,Y)$ ，然后求得后验概率分布 $P(Y|X)$ 。具体地说，利用训练数据学习 $P(X|Y)$ 和 $P(Y)$ 的估计，得到联合概率分布：
$$
P(X,Y) = P(Y)P(X|Y)
$$</li>
	<li>朴素贝叶斯法的基本假设是条件独立性，
$$
\begin{align}
P(X=x|Y=c_k) &amp; =P(X^{(1)}, ..., X^{(n)}=x^{(n)}|Y=c_k) \\
&amp; = \prod^{n}_{j=1} P(X^{(j)}=x^{(j)}|Y=c_k)
\end{align}
$$
这是一个较强的假设，由于这一假设，模型包含的条件概率的数量大为减少，朴素贝叶斯法的学习与预测大为简化。因而朴素贝叶斯法高效，且易于实现。其缺点是分类的性能不一定很高。</li>
	<li>朴素贝叶斯法利用贝叶斯定理与学到的联合概率模型进行分类预测。
$$
P(Y|X)
=\frac{P(X,Y)}{P(X)}
=\frac{P(Y)P(X|Y)}{\sum_rP(Y)P(X|Y)}
$$
将输入 $x$ 分到后验概率最大的类 $y$ 。
$$
y = \arg \max_{c_k} P(Y=c_k) \prod^n_{j=1} P(X_j=x^{(j)}|Y=c_k)
$$
后验证概率最大等价于 0-1 损失函数时的期望风险最小化。</li>
</ol>
朴素贝叶斯法中假设输入变量都是条件独立的，如果假设它们之间存在概率依存关系，模型就变成了贝叶斯网络。
