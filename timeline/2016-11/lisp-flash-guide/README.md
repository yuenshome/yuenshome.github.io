[![header](../../../assets/header11.jpg)](https://yuenshome.github.io)

<script type="text/javascript" async src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML"> </script>

# Lisp 光速上手指北

本文来自 hackernoon.com 的 《Learn you a Lisp in 0 minutes》，是该文译文，因本人水平有限，如有错误请留言，不胜感激。[toc]
<h1>1. 为什么会有本文？</h1>
学习一门语言并不需要非常专业地去学习，好比游览一个国家一样，不一定要住进去。这门语言吸引人的地方在于对当今编程界的影响。你也许看到过该门语言的技术轨迹在意想不到的技术上出现，如 <a href="http://webassembly.org/getting-started/js-api/" target="_blank">WebAssembly</a> （译注：网络汇编语言）或者 <a href="https://gcc.gnu.org/onlinedocs/gccint/RTL.html#RTL" target="_blank">GCC internal representation</a> 。<!--more-->
<h1>2. 怎么上手？</h1>
如果你正在读本篇文章，那就说明你懂英文，甚至还会懂一点法语，你会知道的词语比方有"concept"、“culture”、“action”、“instinct”、“machine“、“science”以及其他两种语言所共享的词语。比方说“cognates”，英文和法文在这个单词上的写法是一样的。

这是因为曾经有一段时间，法语对英语产生过一些影响。在本文中的语言学习上也有同样的情况。某些思想和概念，已经在现代编程语言中所广泛传播。如果你对编程有少许经验，那你将会很快的上手本文所要讲的 Lisp 这门语言。
<h1>3. 所以呢？</h1>
我将给你做了一些测试，证明你对这门语言有所了解。正如大多数的语言和方言属于这门 Lisp 语言的家族一样，我将会为你说明这个测试是基于 <a href="http://www.wescheme.org/openEditor" target="_blank">WebScheme</a> 。

这个测试起源于很简单的事物但慢慢不被人们所重视。如果你们没能答对所有问题也没关系，测试问题中的一些题目是需要有函数式编程背景的人或许才答对的。

来一起看看测试吧！
<blockquote><span style="color: #3366ff;">如何在 Ubuntu 上搭建 Lisp 开发环境：</span>
<pre class="lang:sh decode:true ">sudo apt-get install clisp</pre>
参考：http://blog.csdn.net/cxroom/article/details/8262891
启动 cLisp ：
<pre class="lang:sh decode:true ">yuens@spark:~$ clisp
  i i i i i i i       ooooo    o        ooooooo   ooooo   ooooo
  I I I I I I I      8     8   8           8     8     o  8    8
  I  \ `+' /  I      8         8           8     8        8    8
   \  `-+-'  /       8         8           8      ooooo   8oooo
    `-__|__-'        8         8           8           8  8
        |            8     o   8           8     o     8  8
  ------+------       ooooo    8oooooo  ooo8ooo   ooooo   8

Welcome to GNU CLISP 2.49 (2010-07-07) &lt;http://clisp.cons.org/&gt;

Copyright (c) Bruno Haible, Michael Stoll 1992, 1993
Copyright (c) Bruno Haible, Marcus Daniels 1994-1997
Copyright (c) Bruno Haible, Pierpaolo Bernardi, Sam Steingold 1998
Copyright (c) Bruno Haible, Sam Steingold 1999-2000
Copyright (c) Sam Steingold, Bruno Haible 2001-2010

Type :h and hit Enter for context help.
</pre>
</blockquote>
<h1>4. 来看看题目</h1>
<strong><span style="color: #ff0000;">1. 这个值等于多少？</span></strong>
<pre class="lang:lisp decode:true">(+ 2 2)
4
</pre>
因为前面说了是加法，所以答案是 4 。

<strong><span style="color: #ff0000;">2. 这是 true 还是 false？</span></strong>
<pre class="lang:lisp decode:true">(= (+ 2 2) (* 2 2))
T</pre>
这是 true，因为 2+2 等于 2*2 。

<strong><span style="color: #ff0000;">3. 这是 `a 、`b 还是 `c？</span></strong>
<pre class="lang:lisp decode:true">(first (list 'a 'b 'c))
A</pre>
列表 (a, b, c) 的第一个元素是 a ，经过 ' 元素每个英文字母变为大写。

<strong><span style="color: #ff0000;">4. 会打印 Apples! 还是 Oranges! ？</span></strong>
<pre class="lang:lisp decode:true ">(define apples 5)
(define oranges 6)
(if (&lt; apples oranges)
  (printf "Apples!")
  (printf "Oranges!"))</pre>
Apples 是一个值被设定为 5 的变量， Oranges 则被设为 6 。因为 5 &lt; 6 ，所以应该打印 Apples! 。

<strong><span style="color: #ff0000;">5. 下面将会得到什么结果？</span></strong>
<pre class="lang:lisp decode:true">(define (dbl x)
  (* 2 x))
(dbl 2)</pre>
函数 db1 的功能被定义为传入参数的两倍的值。所以当传入值为 2 时，返回的结果是 4 。

<strong><span style="color: #ff0000;">6. 下面的结果是什么？</span></strong>
<pre class="lang:lisp decode:true ">(define (fact x)
  (if (&lt; x 1)
      1
      (* x (fact (- x 1)))))
(fact 3)</pre>
对于任何小于 1 的元素，函数 fact 会返回 1 ，否则执行乘法，即 x 元素与 fact(x-1) 相乘。所以， fact(0) = 1 ，fact(1) = 1 * 1，fact(2) = 2 * 1 * 1 而 fact(3) = 3*2*1*1 = 6 。所以传入 3 则表示计算的是 3 的阶乘。

<strong><span style="color: #ff0000;">7. 这个函数将会对这个列表做什么？</span></strong>
<pre class="lang:lisp decode:true ">(define (qs xs)
  (if (empty? xs)
      (list )
      (let (
        (middle (first xs))
        (others (rest xs)))
        (let (
          (left (filter (lambda (x) (&lt;= x middle)) others))
          (right (filter (lambda (x) (&gt; x middle)) others)))
            (append (qs left) (cons middle (qs right)))))))
(qs (list 4 5 6 1 2 3))</pre>
这是快速排序算法的一种简单实现，该代码将一个列表分成多个部分：中间的元素、列表左边比中间元素值要小的元素们，以及右边比中间元素要大的元素们。让这个列表中的每一个元素，即其左边和右边都满足上述关系，这样如此的进行递归执行快速排序直到完成，最终函数返回正确的结果。所以，未经排序的列表 (4,5,6,1,2,3) 最终会变成有序列表 (1,2,3,4,5,6) 。

<hr />

<ul>
	<li>如果本篇文章你读到了这里那么恭喜你，你现在已经知道了有关 Lisp 语言的你所知道的内容。</li>
	<li>如果你回答了五个正确的问题，那么你将可能写出一些简单的类似于 <a href="http://blog.aaronbieber.com/2015/07/05/a-gentle-introduction-to-emacs-configuration.html" target="_blank">Emacs</a> 配置的 <a href="https://docs.gimp.org/en/gimp-using-script-fu-tutorial.html" target="_blank">Gimp plugin</a> 这样的小插件。</li>
	<li>如果这些问题你都回答正确了，那么说明你在函数式编程上有一定的诀窍，或者很熟练，如果你之前从未在工作当中使用过函数式编程语言，那么也可尝试一下。</li>
</ul>
<h1>5. 现在呢！</h1>
当然这个测试包括了这门语言的最基本语法。但它不涉及到元编程，元编程指的是这门语言的特性。但学习这些最基本的并不是这门语言的全部。本测试只是告诉你，这门语言的学习并非止步于此。尝试去学习更多的语言，以及更多的语言特性，这样才能熟悉。

写到这里，你可能在工作当中会用到很多不同的语言。其实对于这些语言的学习，类似本文所讲的这门语言。

正如 Eric Raymond 曾经所写到的：
<blockquote>当你掌握了 Lisp 时，你会发现在学习 Lisp 的过程中这门语言给你的启示和经验，即使你今后不再使用这门语言， Lisp 也会在你的余生给你启示，让你变成一个更好的程序员。</blockquote>
但是我不认为，本文所讲的语言有多么的神奇，我只是觉得，学习任何一门语言都会增进我们的经验。最重要的在于每一天做的事情的不同使我们变得更好，以及我们知道了更多的想法来完成新的任务，这才是一门语言所应该带给你的旅行。

如果学习一门新的语言，新的概念或者想法可以使你变为一个更好的程序员，那么在去学习它的过程当中并不是在浪费时间，而是对你的事业的一种投资。

难道不是吗？
