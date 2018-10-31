[![header](../../../assets/header20.jpg)](https://yuenshome.github.io)

<script type="text/javascript" async src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML"> </script>

# k近邻法系列1：k近邻算法、模型与 Python 实现

《统计学习方法》 第 3 章 k近邻法学习笔记摘录及算法实现

k 近邻法（ k-nearest neighbor, k-NN ）是一种基本分类与回归方法， k 近邻的输入为实例的特征向量，对应于空间中的点，输出为实例的类别，可以取多类。k 近邻法假设给定一个训练数据集，其中的实例类别已经确定，分类时对新的实例，根据其 k 个最近邻的训练实例的类别，通过多数表决等方式进行预测。

因此， k 近邻法不具有显式的学习过程， k 近邻法实际上利用训练数据集对特征向量空间进行划分，并作为其分类的“模型”，k值的选择、距离度量以及分类决策规则是 k 近邻法的三个基本要素， k 近邻法 1968 年由 Cover 和 Hart 提出。

本章内容首先叙述 k 近邻算法，然后讨论<strong><span style="color: #ff0000;"> k 近邻法的模型</span></strong>及<strong><span style="color: #ff0000;">三个基本要素（距离度量、ｋ值选择、分类决策规则）</span></strong>，这是本系列1的内容，在系列2中将会讲述 k 近邻法的一个实现—— kd树，介绍构造 kd 树和搜索 kd 树的算法。[toc]<!--more-->
<h1>1. k近邻算法</h1>
k 近邻算法简单且直观：给定一个训练数据集，对新的输入实例，在训练数据集中找到与该实例最近邻的 k 个实例，这 k 个实例多数属于某个类。就把该输入实例分为这个类，下面先叙述 k 近邻算法，然后再讨论其细节。
<h2><span style="color: #ff0000;">算法1 k近邻法</span></h2>
<ul>
	<li>输入：训练数据集 $T = \{ (x_1, y_1), (x_2, y_2), ..., (x_N, y_N) \}$ ，其中 $x_i \in \chi \subseteq \Re^n$ 为实例的特征向量， $y_i \in \gamma = \{ c_1, c_2, ..., c_K \}$ 为实例的类别， $i=1,2,...,N$ ；实例特征向量 $x$ ；</li>
	<li>输出：实例 $x$ 所属的类 $y$ 。</li>
</ul>
（1）根据给定的距离度量，在训练集 $T$ 中找出与 $x$ 最近邻的 $k$ 个点，涵盖这 $k$ 个点的 $x$ 的邻域记作 $N_k(x)$ ；
（2）在 $N_k(x)$ 中根据分类决策规则（如多数表决）决定 $x$ 的类别 $y$ ：
$$
y = \arg \max_{c_j} \sum_{x_i \in N_k(x)} I(y_i=c_j), i=1,2,...,N; j=1,2,...,K
$$
式中， $I$ 为指示函数，即当 $y_i=c_j$ 时 $I$ 为 $1$ ，否则 $I$ 为 $0$ 。

k 近邻法的特殊情况是 $k=1$ 的情形，称为最近邻法。对于输入的实例点（特征向量） $x$ ，最近邻法将训练数据集中与 $x$ 最邻近点的类作为 $x$ 的类。

<strong><span style="color: #ff0000;">k 近邻法没有显式的学习过程。</span></strong>
<h1>2. k近邻模型</h1>
k 近邻法使用的模型<strong><span style="color: #ff0000;">实际上对应于对特征空间的划分</span></strong>。<span style="color: #ff0000;"><strong>模型由三个基本要素——距离度量、k值的选择和分类决策规则决定</strong></span>。
<h2>2.1 模型</h2>
k 近邻法中，当训练集、距离度量（如欧氏距离）、 k 值及分类决策规则（如多数表决）确定后，对于任何一个新的输入实例，它所属的类唯一地确定。这相当于根据上述要素将特征空间划分为一些子空间，确定只空间里的每个点所属的类。这一事实从最近邻算法中可以看得很清楚。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2016/11/kNN_subFeatureSpace.png" alt="" width="497" height="456" />

特征空间中，对每个训练实例点 $x_i$ ，距离该点比其它点更近的所有点组成一个区域，叫做单元（cell）。每个训练实例点拥有一个单元，所有训练实例点的单元构成对特征空间的一个划分，最近邻法将实例 $x_i$ 的类 $y_i$ 作为其单元中所有点的类标记（class label）。这样，每个单元的实例点的类别是确定的，上图是二维特征空间划分的一个例子。
<h2>2.2 距离度量</h2>
特征空间中两个实例点的距离是两个实例点相似程度的反映，k 近邻模型的特征空间一般是 $n$ 维实数向量空间 $\Re^n$ 。使用的距离是欧氏距离，但也可以是其他距离，如更一般的距离 $L_p$ 距离（$L_p$ distance）或 Minkowski 距离（Minkowski distance）。

设特征空间 $\chi$ 是 n 维实数向量空间 $\Re^n, x_i,x_j \in \chi, x_i = (x_i^{(1)}, x_i^{(2)}, ... , x_i^{(n)})^T, x_j = (x_j^{(1)}, x_j^{(2)}, ... , x_j^{(n)})^T$  ， $x_i, x_j$ 的<strong><span style="color: #ff0000;"> $L_p$ 距离定义为</span></strong>：
$$
L_p(x_i, x_j) = \left ( \sum_{l=1}^{n} |x_i^{(l)} - x_j^{(l)}|^p \right ) ^{\frac{1}{p}}
$$
这里 $p \geq 1$ ，当 $p=2$ 时，称为欧氏距离（Euclidean distance），即
$$
L_p(x_i, x_j) = \left ( \sum_{l=1}^{n} |x_i^{(l)} - x_j^{(l)}|^2 \right ) ^{\frac{1}{2}}
$$
当 $p =1$ 时，称为曼哈顿距离（Manhattan distance），即
$$
L_p(x_i, x_j) = \left ( \sum_{l=1}^{n} |x_i^{(l)} - x_j^{(l)}| \right )
$$
当 $p = \infty$ 时，它是各个坐标距离的最大值，即
$$
L_{\infty} (x_i, x_j) = \max_l |x_l^{(l)} - x_j^{(l)}|
$$

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2016/11/L_p_distance.png" alt="" width="317" height="340" />

上图给出了在二维空间中 $p$ 取不同值时，与原点的 $L_p$ 距离为 $1$ （$L_p=1$）的点的图形。下面的例子说明，由不同的距离度量所确定的最近邻点是不同的。

<span style="color: #ff0000;"><strong>例1</strong></span> 已知二维空间的 $3$ 个点 $x_1 = (1,1)^T, x_2 = (5, 1)^T, x_3 = (4,4)^T$ ，试求在 $p$ 取不同值时， $L_p$ 距离下 $x_1$ 的最近邻点。
解：因为 $x_1$ 和 $x_2$ 只有第二维上值不同，所以 $p$ 为任何值时，$L_P(x_1, x_2) = 4$ ，而
$L_1(x_1, x_3) = 6, L_2(x_1, x_3) = 4.24, L_3(x_1, x_3) = 3.78, L_4(x_1, x_3) = 3.57$
于是得到： $p$ 等于 $1$ 或 $2$ 时，$x_2$ 是 $x_1$ 的最近邻点； $p$ 大于等于 $3$ 时， $x_3$ 是 $x_1$ 的最近邻点。
<h2>2.3 k 值的选择</h2>
k 值的选择会对 k 近邻法的结果产生重大影响。

如果选择<span style="color: #ff0000;"><strong>较小的 k 值</strong></span>，就相当于用较小的领域中的训练实例进行预测，”学习“的<strong><span style="color: #ff0000;">近似误差（approximation error）会减小</span></strong>，只有与输入实例较近的（相似的）训练实例才会对预测结果起作用。但缺点是”学习“的<strong><span style="color: #ff0000;">估计误差（estimation error）会增大</span></strong>，预测结果会对近邻的实例点非常敏感，如果邻近的实例点恰巧是噪声，预测就会出错，换句话说， <strong><span style="color: #ff0000;">k 值的减小就意味着整体模型变得复杂，容易发生过拟合</span></strong>。

如果选择较大的 k 值，就相当于用较大领域中的训练实例进行预测。其优点是可以减少学习的估计误差，但缺点是学习的近似误差会增大，这时与输入实例较远的（不相似的）训练实例也会对预测起作用，是预测发生错误， k 值的增大，就意味着整体的模型变得简单。

如果 $k=n$ ，那么无论输入实例是什么，都将简单地预测它属于在训练实例中最多的类。这时，模型过于简单，完全忽略训练实例中的大量有用信息，是不可取的。

在应用中，<strong><span style="color: #ff0000;"> k 值一般取一个较小的数值</span></strong>。<strong><span style="color: #ff0000;">通常采用交叉验证法来选最优的 k 值</span></strong>。
<h2>2.4 分类决策规则</h2>
k 近邻法中的分类决策规则往往是多数表决，即由输入实例的 k 个近邻的训练实例中的多数类决定输入实例的类。

多数表决规则（majority voting rule）有如下解释：如果分类的损失函数为 0-1 损失函数，分类函数为：
$$
f: \Re^n \rightarrow {c_1, c_2, ..., c_K}
$$
那么误分类的概率是
$$
P(Y \neq f(X))=1-P(Y=f(X))
$$
对给定的实例 $x \in \chi$ ，其最近邻的 k 个训练实例点构成集合 $N_k(x)$ 。如果涵盖 $N_k(x)$ 的区域的类别是 $c_j$ ，那么误分类是
$$
\frac{1}{k} \sum_{x_i \in N_k(x)} I(y_i \neq c_j) = 1-\frac{1}{k} \sum_{x_i \in N_k(x)} I(y_i = c_j)
$$
要使误分类率最小即经验风险最小，就要使 $\sum_{x_i \in N_k(x)} I(y_i = c_j)$ 最大，所以<strong><span style="color: #ff0000;">多数表决规则等价于经验风险最小化</span></strong>。
<h1>3. Python代码</h1>
代码比较有意思的地方在于——用于选择合适的 k 值的函数（chooseK）。基于所有的样本， k 从 1 到所有样本个数依次遍历，计算不同 k 值下的误分类个数，返回误分类个数最小时候的 k 值为 bestK 。
<h2>3.1 输入数据文件</h2>

```
1 1 1 1
2 5 1 1
3 4 4 2
4 4 7 2
5 -1 -3 1
6 -2 -3 1
7 8 9 2
8 0 -2 2
```

<h2>3.2 代码</h2>

```python
################################### PART0 DESCRIPTION #################################
# Filename: kNN.py
# Description:
#

# E-mail: ysh329@sina.com
# Create: 2016-11-24 23:20:10
# Last:
__author__ = 'yuens'


################################### PART1 IMPORT ######################################
import math
import matplotlib.pylab as plt

################################### PART2 CLASS &amp;&amp; FUNCTION ###########################

def readDataFrom(path, hasHeader=True):
    '''
    读取路径为path的文件，默认第一行为表头文件(hasHeader=True)，
    否则需要设置第一行不包含表头文件(hasHeader=False)。
    :param path: 读取数据的路径
    :param hasHeader: 数据文件是否有表头
    :return: 返回数据id、特征、标签
    '''
    with open(path, 'r') as f:
        rawData = map(lambda line:\
                          line.strip().split(" "),\
                      f.readlines())
        if hasHeader:
            header = rawData[0]
            print("header:{0}".format(header))
            rawDataWithoutHeader = rawData[1:]
        else:
            print("header:None")
            rawDataWithoutHeader = rawData
        cleanData = map(lambda recordList: \
                            map(int, recordList),\
                        rawDataWithoutHeader)
        idList = map(lambda r: r[0], cleanData)
        xList = map(lambda r: tuple(r[1:len(r)-1]), cleanData)
        yList = map(lambda r: r[-1], cleanData)
        return idList, xList, yList


class kNN(object):
    '''
    k最近邻模型的类。无监督模型。
    '''
    def __init__(self, sampleNum, featureNum, k=None, distancePValue=None):
        '''
        初始化模型参数
        :param sampleNum: 训练集样本个数
        :param featureNum: 每个样本的特征个数
        :param k: 分类基于最近的 k 个样本
        :param distancePValue: $L_p$ 距离参数
        '''
        # 参数检查
        if k == None:
            k = 1
        if distancePValue == None:
            distancePValue = 2

        self.sampleNum = sampleNum
        self.featureNum = featureNum
        self.k = k
        self.p = float(distancePValue)
        self.distMatrix = dict()

    def constructDistMatrix(self, xList, p=None):
        '''
        构建距离矩阵，类似
        :param xList:
        :param p:
        :return:
        '''
        if p == None:
            p = self.p
        # 初始化
        for x1Idx in xrange(len(xList)):
            x1 = xList[x1Idx]
            if not self.distMatrix.has_key(x1):
                self.distMatrix[x1] = dict()
            for x2Idx in xrange(len(xList)):
                x2 = xList[x2Idx]
                if not self.distMatrix[x1].has_key(x2):
                    self.distMatrix[x1][x2] = 0.0
       # 计算距离
        for x1Idx in xrange(len(xList)):
            for x2Idx in xrange(len(xList)):
                x1 = xList[x1Idx]
                x2 = xList[x2Idx]
                if x1Idx != x2Idx and self.distMatrix[x1][x2] == 0.0:
                    self.distMatrix[x1][x2] = self.distanceBetween(aList=x1,\
                                                                   bList=x2,\
                                                                   p=p)
                    self.distMatrix[x2][x1] = self.distMatrix[x1][x2]

    def distanceBetween(self, aList, bList, p=None):
        '''
        计算两个点，表示为 aList 与 bList，二者之间的 $L_p$ 距离。
        :param aList: 第一个点的特征
        :param bList: 第二个点的特征
        :param p: $L_p$ 距离参数
        :return: 返回两个点之间的距离
        '''
        if p == None:
             p = self.p
        sigma = sum(\
             map(lambda aa, bb:\
                     math.pow(aa-bb, p),\
                 aList, bList)\
             )
        distance = math.pow(sigma.__abs__(), 1.0/p)
        return distance

    def chooseK(self, xList, yList, p=None):
        '''
        基于训练数据，选择最合适的 k 值。会对 k 从 1 到所有样本数进行遍历，
         统计不同 k 值时分类正确的点数，最终的 k 选择分类错误最少的。
        :param xList: 训练样本特征
        :param yList: 训练样本标签
        :param p: $L_p$ 距离参数
        :return: 返回分类错误最少的 k 值
        '''
        # 参数检查默认为2
        if p == None:
            p = self.p
        kList = range(1, len(xList))
        misClassDict = dict()
        # 遍历k
        for kIdx in xrange(len(kList)):
            k = kList[kIdx]
            misClassDict[k] = 0
            # 选择当前k下，每个样本的yHat
            for xIdx in xrange(len(xList)):
                x = xList[xIdx]
                xAndDistAndYTupList = map(lambda (x2, dist):\
                                              (x2, dist, yList[xList.index(x2)]),\
                                          self.distMatrix[x].iteritems())
                xAndDistAndYTupList.sort(key=lambda (x2, dist, y): dist,\
                                         reverse=False)
                xAndDistAndYTupList = filter(lambda (x2, dist, y): x2 != x, xAndDistAndYTupList)
                # 统计当前样本的k近邻的类别
                yHatDict = dict()
                yHatList = map(lambda (x2, dist, y):\
                                   y,\
                               xAndDistAndYTupList[:k])
                for idx in xrange(len(yHatList)):
                    yHat = yHatList[idx]
                    if yHatDict.has_key(yHat):
                        yHatDict[yHat] += 1
                    else:
                        yHatDict[yHat] = 1
                yHatAndCountList = map(lambda (yHat, count):\
                                           (yHat, count),\
                                       yHatDict.iteritems())
                yHatAndCountList.sort(key=lambda (yHat, count): count,\
                                      reverse=True)
                xsYHat = yHatAndCountList[0][0]
                if yList[xIdx] != xsYHat:
                    misClassDict[k] += 1
        # 选择错误最少的k
        kAndMisNumList = map(lambda (k, misNum):\
                                 (k, misNum),\
                             misClassDict.iteritems())
        kAndMisNumList.sort(key=lambda (k, misNum): misNum,\
                            reverse=False)
        bestK = kAndMisNumList[0][0]
        return bestK, misClassDict

    def predict(self, x, xList, yList, p=None):
        '''
        预测新输入样本 x 的类别，其中 xList 与 yList 分别是训练样本的
        特征和类别标签， p 为 $L_P$ 距离的参数（默认为None，会被设定为
        2）。
        :param x: 新输入样本的特征
        :param xList: 训练样本特征
        :param yList: 训练样本标签
        :param p: $L_p$ 距离参数
        :return: 返回 x 预测出的类别
        '''
        # 检查参数
        if p == None:
            p = self.p
        # 整合距离矩阵生成所需要的数据格式
        xAndXXAndDistAndYTupList = map(lambda xx, y:\
                                           (x,\
                                            xx,\
                                            self.distanceBetween(aList=x,\
                                                                 bList=xx,\
                                                                 p=p),
                                            y),\
                                       xList, yList)
        xAndXXAndDistAndYTupList.sort(key=lambda (x, xx, dist, y): dist,\
                                      reverse=False)
        # 根据其他样本点距离 x 的远近程度统计类别数目
        yDict = {}
        for idx in xrange(self.k):
            # (x, xx, dist, y)
            y = xAndXXAndDistAndYTupList[idx][3]
            if yDict.has_key(y):
                yDict[y] += 1
            else:
                yDict[y] = 1
        yAndCountTupList = map(lambda (y, count):\
                                   (y, count),\
                               yDict.iteritems())
        yAndCountTupList.sort(key=lambda (y, count): count,\
                              reverse=True)
        yHat = yAndCountTupList[0][0]
        return yHat

    def plotScatter(self, xList, yList, saveFigPath):
        '''
        根据特征数据 xList 及其类别 yList 绘制散点图，并将绘制出的
        散点图保存在 saveFigPath 路径下。
        :param xList: 样本特征
        :param yList: 样本类别
        :param saveFigPath: 保存散点图的路径
        :return:
        '''
        # 判断特征是否大于等于二维
        # 如果样本的特征大于等于 2
        # 那么仅可视化前面 2 维度的数据
        if len(xList[0]) &gt;= 2:
            x1List = map(lambda x: x[0], xList)
            x2List = map(lambda x: x[1], xList)
        else:
            # 1 或 2 维数据都可视化为 2 维
            x1List = x2List = map(lambda x: x[0], xList)
        # 新建画布
        scatterFig= plt.figure(saveFigPath)
        # 预定义：颜色初始化
        colorDict = {-1: 'm', 1: 'r', 2: 'b', 3: 'pink', 4: 'orange'}
        # 绘制每个点
        map(lambda idx: \
                plt.scatter(x1List[idx], \
                            x2List[idx], \
                            marker='o', \
                            color=colorDict[yList[idx]], \
                            label=yList[idx]), \
            xrange(len(x1List)))
        # 给每种类别加上标注
        # ySet = set(yList)
        # map(lambda y: \
        #         plt.legend(str(y), \
        #                    loc='best'), \
        #     ySet)

        # 设定其他属性并保存图像后显示
        plt.title(saveFigPath)
        plt.xlabel(r'$x^1$')
        plt.ylabel(r'$x^2$')
        plt.grid(True)
        plt.savefig(saveFigPath)
        plt.show()

    def plotKChart(self, misClassDict, saveFigPath):
        kList = []
        misRateList = []
        for k, misClassNum in misClassDict.iteritems():
            kList.append(k)
            misRateList.append(1.0 - 1.0/k*misClassNum)

        fig = plt.figure(saveFigPath)
        plt.plot(kList, misRateList, 'r--')
        plt.title(saveFigPath)
        plt.xlabel('k Num.')
        plt.ylabel('Misclassified Rate')
        plt.legend(saveFigPath)
        plt.grid(True)
        plt.savefig(saveFigPath)
        plt.show()

################################### PART3 TEST ########################################
# 例子
if __name__ == "__main__":
    # 参数初始化
    k = 2
    distancePValue = 2
    dataPath = "./input1"
    hasHeader = True
    saveScatterFigPath = u"k-Nearest Neighbor Scatter Plot"
    saveKChartFigPath = u"k-Nearest Neighbor's K Chart"

    # 读取数据
    idList, xList, yList = readDataFrom(path=dataPath,\
                                        hasHeader=hasHeader)
    print("idList:{0}".format(idList))
    print("xList:{0}".format(xList))
    print("yList:{0}".format(yList))

    # 实例化最近邻类
    knn = kNN(sampleNum=len(idList),\
              featureNum=len(xList[0]),\
              k=k,\
              distancePValue=distancePValue)

    # 初始化距离矩阵并完成两点间距离计算
    knn.constructDistMatrix(xList=xList)

    # 预测一个新样本点的所属类别
    newX = (1, 3)
    newYHat = knn.predict(x=newX,\
                          xList=xList,\
                          yList=yList)
    print("newYHat:{0}".format(newYHat))

    # 基于数据选择一个最合适的 k 值
    bestK, misClassDict = knn.chooseK(xList=xList,\
                                      yList=yList)
    print("bestK:{0}".format(bestK))
    knn.plotKChart(misClassDict=misClassDict,\
                   saveFigPath=saveKChartFigPath)

    # 绘制散点图
    knn.plotScatter(xList=xList,\
                    yList=yList,\
                    saveFigPath=saveScatterFigPath)
```

<h2>3.3 输出结果与图像绘制</h2>

```shell
/usr/local/lib/anaconda2/bin/python /home/yuens/Projects/PyCharm/statistical-learning-methods-note/chapter_3_kNN/kNN.py
header:['id', 'x^1', 'x^2', 'y']
idList:[1, 2, 3, 4, 5, 6, 7, 8]
xList:[(1, 1), (5, 1), (4, 4), (4, 7), (-1, -3), (-2, -3), (8, 9), (0, -2)]
yList:[1, 1, 2, 2, 1, 1, 2, 2]
newYHat:1
bestK:2
{1: 3, 2: 2, 3: 4, 4: 5, 5: 2, 6: 4, 7: 8}

Process finished with exit code 0
```

<img class="aligncenter" src="./assets/k-Nearest%20Neighbor%20Scatter%20Plot.png" alt="" width="800" height="600" />
<p style="text-align: center;"><strong>k近邻散点图</strong></p>
<p style="text-align: center;"><img class="aligncenter" src="./assets/k-Nearest%20Neighbor%27s%20K%20Chart.png" alt="" width="800" height="600" /></p>
<p style="text-align: center;"><strong>不同k值下的误分类率折线图</strong></p>
