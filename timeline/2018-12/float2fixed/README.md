# 计算机的浮点表示与浮点数转定点操作

浮点数，如`float`、`double`在计算机中的表示为二进制，其分别占32位、64位（float为4字节，double为8字节，1字节=8位）。计算机来表示该数时，首先会将其表示为二进制形式的科学计数法。

## 1. 计算机中浮点的表示

![image](https://user-images.githubusercontent.com/7320657/49349891-bcc97e00-f6e7-11e8-8dd1-837f83b31c2d.png)

虽然表示的形式是二进制的科学计数法，但存储是以三部分进行，以`float`为例：

1. 符号位（sign，占1位，不存在无符号浮点）：0代表正，1代表为负；
2. 指数位（exponent，占8位）：用于存储科学计数法中的指数数据，并且采用移位存储（**存时对算出的2^m量级中的m，存m加上127的结果进这8位；取指数时对当前的8位指数的结果减去127**）。IEEE754 中规定，单精度浮点数的指数域是 8 个比特，因此它的指数偏移值为 2^(8−1)−1=127；64 位双精度浮点数的值数域是 11 个比特，它的指数偏移值为：2^(11−1)−1=1023。指数编码时需要加上一个偏移值，是因为指数可以是负数，而计算机存储浮点数的“指数部分”又是一个无符号的整数，因此，在 IEEE754 标准规定，exponent 必须减去一个偏移值而得到真实的“指数值”；
3. 尾数位（fraction，占23位）：尾数部分。用二进制的科学计数法表示二进制，其第一位都是1，因而尾数位直接从小数点后面的第一位开始表示。所以23bit的尾数部分，可以表示的精度却变成了24bit，道理就是在这里。

下图分别是`float`和`double`的二进制表示数的形式：

![image](https://user-images.githubusercontent.com/7320657/49347879-e6c97300-f6dc-11e8-9b34-b84e2085beac.png)

![image](https://user-images.githubusercontent.com/7320657/49347882-ed57ea80-f6dc-11e8-84c5-d6c727ddadb5.png)

## 2. 浮点数的表示例子

- 8.25用二进制的科学计数法表示为：1.00001*2^3；
- 按照上面的存储方式：符号位为0，表示为正；指数位为3+127=130，尾数部分为 1.00001；
- 故8.25的存储方式如下：0xbffff380，01000001000001000000000000000000 b
- 分解如下：0--10000010--00001000000000000000000  
- 符号位为0，指数部分为10000010，位数部分为 00001000000000000000000 

同理，120.5在内存中的存储格式如下：
- 0xbffff384，01000010111100010000000000000000
- 分解如下：0--10000101--11100010000000000000000

> ## 补充：整数与小数的二进制转换
> - 整数部分转二进制：短除法，最终从下向上得到结果；
> - 小数部分转二进制：乘2取整，最终从上向下取得结果；


## 3. 计算机中整数的表示

整数的表示只有两部分：

1. 符号位：占1位，若无符号，则不计；
2. 尾数位：剩下的全部用来做尾数，如int32；

那么这么看来，`float32`与`int32`尾数部分分别为23（实际表示24，因为从小数点后计算）、31，精度上`int`更高。但是`float`因为有8位的指数部分，其表示的范围比整数大。

## 4. 存储方式与大小端模式

那么知道了整数和浮点在计算机中的表示后，那它们是怎么存储的呢？下面先讲两个来自百度百科的概念。

- 大端模式：是指数据的**高字节保存在内存的低地址中，而数据的低字节保存在内存的高地址中**，这样的存储模式有点儿**类似于把数据当作字符串**顺序处理：地址由小向大增加，而数据从高位往低位放；这和我们的阅读习惯一致；
- 小端模式：是指数据的**高字节保存在内存的高地址中，而数据的低字节保存在内存的低地址中**，这种存储模式**将地址的高低和数据位权有效地结合**起来，**高地址部分权值高，低地址部分权值低**。

**一般情况，我们的计算机都是小端模式。**


## 5. 大小端例子

下面以`unsigned int value = 0x12345678`为例，分别看看在两种字节序下其存储情况，我们可以用`unsigned char buf[4]`来表示`value`。

1. Big-Endian: 低地址存放高位，如下：
```
高地址
　　---------------
　　buf[3] (0x78) -- 低位
　　buf[2] (0x56)
　　buf[1] (0x34)
　　buf[0] (0x12) -- 高位
　　---------------
　　低地址
```

2. Little-Endian: 低地址存放低位，如下：
```
高地址
　　---------------
　　buf[3] (0x12) -- 高位
　　buf[2] (0x34)
　　buf[1] (0x56)
　　buf[0] (0x78) -- 低位
　　--------------
低地址
```

内存地址 | 小端模式存放内容 | 大端模式存放内容
-- | -- | --
0x4000 | 0x78 | 0x12
0x4001 | 0x56 | 0x34
0x4002 | 0x34 | 0x56
0x4003 | 0x12 | 0x78

## 6. 浮点转定点

目的：将32bit的`float`浮点数组转为指定`quant_data_bit_len`位数表示的定点数组，如将32量化为16位，`quant_data_bit_len=16`。但实际转后的定点仍以`float`表示（因为该硬件平台没有16bit的类型，因而这里`QDTYPE`仍为`float`），这里只是做软件层面的截断。

整体计算流程如下：

1. 判断是否存在符号位`with_sign`。遍历数组中每个元素，判断是否存在负数。若有则`with_sign=1`，没有负数则`with_sign=0`；
2. 计算指数位的高位。即对浮点元素表示为二进制的科学记数法`n * 2^n`后，计算其`n`是多少。该过程遍历所有元素，得到数组中元素绝对值的最大值，并通过对数计算，用`2^n`近似原浮点数，得到该上界的`n`；
3. 计算尾数位数`quant_keep_fraction_bit_len`。用我们设定的量化策略的位数`quant_data_bit_len`，减去符号位占用（若有），再减去指数位的占用，最后得到量化可用来表示尾数的尾数位个数`quant_keep_fraction_bit_len`；
4. 将要量化的元素逐个遍历：
    1. 对该元素左移`quant_keep_fraction_bit_len`位，即对尾数左移**量化能保留的尾数个数**；
    2. 再通过`int`强制转换，强制截断去掉小数部分得到`integer_faction`；
    3. 对`integer_fraction`右移回来，右移**量化保留的尾数个数**，恢复浮点表示（此时也完成了指定位数`quant_data_bit_len`的量化操作）；

```cpp
    int    elem_bit_len_without_sign   = t->with_sign ? t->quant_data_bit_len-1 : t->quant_data_bit_len;
    int    exponent_high_bit           = calc_exponent_high_bit(t);
    int    quant_keep_fraction_bit_len = exponent_high_bit - elem_bit_len_without_sign + 1;
    DTYPE  *data                       = t->data;
    QDTYPE *qdata                      = t->quant_data;

    for(int eidx = 0; eidx < t->len; ++eidx)
    {
        DTYPE e              = data[eidx];
        int integer_fraction = (int)(e * pow(2, -quant_keep_fraction_bit_len));        // float2fixed: cutoff part of fraction by left move
        qdata[eidx]          = integer_fraction * pow(2, quant_keep_fraction_bit_len); // fixed2float: resume float by right move
    }
```

## 参考

- [浮点数在计算机中的表示](https://blog.csdn.net/jvandc/article/details/81176294)
- [整数的存储](http://blog.sina.com.cn/s/blog_3be6742c0102x5am.html)
- [C语言浮点数存储方式](https://www.cnblogs.com/fengliu-/p/7455246.html)
- [大小端模式](https://baike.baidu.com/item/大小端模式/6750542?fr=aladdin)
