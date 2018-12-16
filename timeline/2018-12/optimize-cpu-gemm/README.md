[![header](../../../assets/header17.jpg)](https://yuenshome.github.io)

# 优化CPU矩阵乘法

一步步优化GEMM。最终相比当初的naive矩阵乘法有近乎10倍的性能提升。用到的方法简单罗列如下：

1. 寄存器变量：kernel中反复用的变量声明时带上register前缀；
2. L2 cache：kernel中的数据复用；
3. 指针代替索引：减少访问开销；
4. 间接寻址代替直接寻址：减少每次for循环中指针的更新次数；
5. simd：用上intel intrinsic实现一次完成1x4个C元素的乘加操作；
6. 矩阵分块：设置分块参数，优化L2 cache的使用；
7. 对kernel中参与计算的部分A和部分B打包：优化计算C元素时对A和B的访问。

----

到第7步后，就可以对已经写成intrinsic或者neon的代码，进一步写为汇编，又会有至少10~15%的性能提升。

下面就从头开始吧！


## 1. 最原始的矩阵乘法</h1>
首先，给出最原始的矩阵乘法的C实现：

```c
/* Create macros so that the matrices are stored in column-major order */

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */

void REF_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
  int i, j, p;

  for ( i=0; i&lt;m; i++ ){        /* Loop over the rows of C */
    for ( j=0; j&lt;n; j++ ){      /* Loop over the columns of C */
      for ( p=0; p&lt;k; p++ ){    /* Update C( i,j ) with the inner
                                   product of the i-th row of A and
                                   the j-th of B */
        C( i,j ) = C( i,j ) +  A( i,p ) * B( p,j );
      }
    }
  }
}
```

其中，相乘的两个矩阵：矩阵A是m行k列，矩阵B是k行n列。
<ul>
	<li>参考：矩阵乘法的四种理解方式 - lijiankou - 博客园
http://www.cnblogs.com/lijiankou/archive/2013/07/11/3183728.html</li>
</ul>
<p style="text-align: center;"><img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com//uploads/2017/05/gemm_01.png" alt="" width="362" height="148" />矩阵A和矩阵C相乘</p>
该方法是操作的是一维数组，通过行列索引i、j、p（分别遍历m、n、p）以及lda、ldb、ldc（本身这三个就是与m、n、k是等价，相当于矩阵A、B、C的每行元素的个数）来实现对二维矩阵下标的控制。

```cc
#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]
```

这里使用了带参数的宏定义，带参宏定义的一般形式为：#define 宏名(形参列表) 字符串。对带参数的宏，在调用中，不仅要宏展开，而且要用实参去代换形参。三点说明：
<ul>
	<li>1. 带参宏定义中，形参之间可以出现空格，但是宏名和形参列表之间不能有空格出现；</li>
	<li>2. 在带参宏定义中，不会为形式参数分配内存，因此不必指明数据类型。而在宏调用中，实参包含了具体的数据，要用它们去代换形参，因此必须指明数据类型；
这一点和函数是不同的：在函数中，形参和实参是两个不同的变量，都有自己的作用域，调用时要把实参的值传递给形参；而在带参数的宏中，只是符号的替换，不存在值传递的问题。</li>
	<li>3. 在宏定义中，字符串内的形参通常要用括号括起来以避免出错。</li>
	<li>参考：C语言带参数宏定义_C语言中文网
http://c.biancheng.net/cpp/html/66.html</li>
</ul>
所以宏定义中以及i、j、p的三个循环次数m、n、k可以看出，该方法是采用<span style="color: #ff0000;"><b>列优先</b></span>存储。我是这么看的：矩阵A的宏定义是#define A(i,j) a[ (j)*lda + (i) ]，查看对应for循环的代码，第一个for循环就是用i来遍历m，那么既然
<h2>1.1 行优先与列优先</h2>
行优先或者列优先没有好坏，但其<strong><span style="color: #ff0000;">直接涉及到对内存中数据的最佳存储访问方式</span></strong>。因为在内存使用上，<span style="color: #ff0000;">程序访问的内存地址之间连续性越好</span>，程序的<span style="color: #ff0000;">访问效率</span>就越<span style="color: #ff0000;">高</span>。所以应该尽量在行优先机制的编译器，比如C/C++，CUDA等等上，采用行优先的数据存储方式；在列优先机制的编译器，比如Fortune, Matlab等等上，采用列优先的数据存储方式。但这种思想渗透到编程中之后，代码的质量就会提高一个档次。
<p style="text-align: center;"><img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com//uploads/2017/05/gemm_02.png" alt="" width="139" height="64" />现在有2行三列矩阵</p>
<p style="text-align: center;"><img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com//uploads/2017/05/gemm_03.png" alt="" width="216" height="187" /></p>
<p style="text-align: center;">该矩阵对应的行优先序（从第一行开始存储，再存储第二行以此类推）</p>
<p style="text-align: center;"><img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com//uploads/2017/05/gemm_04.png" alt="" width="209" height="178" /></p>
<p style="text-align: center;">该矩阵对应的列优先序（从第一列开始存储，再存储第二列以此类推）</p>

<ul>
	<li>参考：C语言行优先和列优先的问题深入分析_C 语言_脚本之家
http://www.jb51.net/article/101882.htm</li>
	<li>列优先与行优先 - For record - 博客频道 - CSDN.NET
http://blog.csdn.net/shenck1992/article/details/50041777</li>
</ul>


<h1>2. 第一次优化</h1>


```cc
/* Create macros so that the matrices are stored in column-major order */

#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */

void AddDot( int, double *, int, double *, double * );

void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
  int i, j;

  for ( j=0; j&lt;n; j+=1 ){        /* Loop over the columns of C */
    for ( i=0; i&lt;m; i+=1 ){        /* Loop over the rows of C */
      /* Update the C( i,j ) with the inner product of the ith row of A
	 and the jth column of B */

      AddDot( k, &amp;A( i,0 ), lda, &amp;B( 0,j ), &amp;C( i,j ) );
    }
  }
}

/* Create macro to let X( i ) equal the ith element of x */

#define X(i) x[ (i)*incx ]

void AddDot( int k, double *x, int incx,  double *y, double *gamma )
{
  /* compute gamma := x' * y + gamma with vectors x and y of length n.

     Here x starts at location x with increment (stride) incx and y starts at location y and has (implicit) stride of 1.
  */
 
  int p;

  for ( p=0; p&lt;k; p++ ){
    *gamma += X( p ) * y[ p ];     
  }
}
```

大致结构上基本不变，<span style="color: #ff0000;">由三重for循环变为两重</span>（用i、j分别遍历n、m）。减少的第三重循环原本做的是通过A的第i行和B的第j列来更新C(i,j)，当然改变后的代码使用了一个AddDot函数（取出A的一行，B的一列，点乘后加和）来代替第三重循环，不过把第三重循环放到了AddDot函数里来做。这样看起来更规整一些，改变前后性能只有很小的提升。

另外就是<span style="color: #ff0000;">针对遍历C的两重循环的顺序改变了，现在变为先遍历C的列再遍历行</span>，与之前相反。


## 3. 第二次优化

```cc
/* Create macros so that the matrices are stored in column-major order */
#define A(i,j) a[ (j)*lda + (i) ]
#define B(i,j) b[ (j)*ldb + (i) ]
#define C(i,j) c[ (j)*ldc + (i) ]

/* Routine for computing C = A * B + C */

void AddDot( int, double *, int, double *, double * );

void MY_MMult( int m, int n, int k, double *a, int lda, 
                                    double *b, int ldb,
                                    double *c, int ldc )
{
  int i, j;

  for ( j=0; j&lt;n; j+=4 ){        /* Loop over the columns of C, unrolled by 4 */
    for ( i=0; i&lt;m; i+=1 ){        /* Loop over the rows of C */
      /* Update the C( i,j ) with the inner product of the ith row of A
	 and the jth column of B */

      AddDot( k, &amp;A( i,0 ), lda, &amp;B( 0,j ), &amp;C( i,j ) );

      /* Update the C( i,j+1 ) with the inner product of the ith row of A
	 and the (j+1)th column of B */

      AddDot( k, &amp;A( i,0 ), lda, &amp;B( 0,j+1 ), &amp;C( i,j+1 ) );

      /* Update the C( i,j+2 ) with the inner product of the ith row of A
	 and the (j+2)th column of B */

      AddDot( k, &amp;A( i,0 ), lda, &amp;B( 0,j+2 ), &amp;C( i,j+2 ) );

      /* Update the C( i,j+3 ) with the inner product of the ith row of A
	 and the (j+1)th column of B */

      AddDot( k, &amp;A( i,0 ), lda, &amp;B( 0,j+3 ), &amp;C( i,j+3 ) );
    }
  }
}

/* Create macro to let X( i ) equal the ith element of x */

#define X(i) x[ (i)*incx ]

void AddDot( int k, double *x, int incx,  double *y, double *gamma )
{
  /* compute gamma := x' * y + gamma with vectors x and y of length n.

     Here x starts at location x with increment (stride) incx and y starts at location y and has (implicit) stride of 1.
  */
 
  int p;

  for ( p=0; p&lt;k; p++ ){
    *gamma += X( p ) * y[ p ];     
  }
}
```

最明显改变就是4次AddDot函数（取出矩阵A的一行，矩阵B的一列，做先点乘后加和），即每次计算得到矩阵C的四个元素的值。这样相比之前，应该有不到4倍的性能提升
