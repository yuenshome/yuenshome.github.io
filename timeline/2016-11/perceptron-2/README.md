[![header](../../../assets/header33.jpg)](https://yuenshome.github.io)

<script type="text/javascript" async src="https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-MML-AM_CHTML"> </script>

# 感知机系列2：感知机学习算法及其 Python 实现

感知机学习问题转化为求解损失函数式 $L(w,b) = -\sum_{x_i \in M } y_i(w \cdot x_i + b)$ 的最优化问题，最优化的方法是随机梯度下降法。本节叙述感知机的具体算法。包括原始形式和对偶形式，并证明在训练数据线性可分条件下感知机学习算法的收敛性。

最重要的是由于内容完全抄录《统计学习方法》，自己使用 Python 实现了感知机算法的原始形式和对偶形式。[toc]<!--more-->
<h1>1. 感知机学习算法的原始形式</h1>
感知机学习算法是对以下最优化问题的算法，给定一个训练数据集 $T = \{(x_1,y_1), (x_2,y_2), ..., (x_N,y_N)\}$ ，其中 $x_i \in \chi = \Re^n, y_i \in \gamma = \{-1,1\}, i=1,2,...,N$ ，求参数 $w,b$ ，使其为以下损失函数极小化问题的解：

$$
\min_{w,b} L(w,b) = -\sum_{x_i \in M} y_i (w \cdot x_i + b)
$$

其中 $M$ 为误分类点的集合。

<strong><span style="color: #ff0000;">感知机学习算法是误分类驱动的，具体采用随机梯度下降法（ stochastic gradient descent ）</span></strong>。首先，任意取一个超平面 $w_0, b_0$ ，然后用梯度下降法不断地极小化上述目标函数。极小化过程中不是一次使 $M$ 中所有误分类点的梯度下降，而是一次随机选取一个误分类点使其梯度下降。

假设误分类点集合 $M$ 是固定的，那么损失函数 $L(w,b)$ 的梯度由以下公式给出：

$$
\begin{align}
&amp; \triangledown_{w} L(w,b) = -\sum_{x_i \in M} y_i x_i \\
&amp; \triangledown_{b} L(w,b) = -\sum_{x_i \in M} y_i
\end{align}
$$

随机选取一个误分类点 $(x_i, y_i)$ ，对 $w,b$ 进行更新：

$$
\begin{align}
&amp; w \leftarrow w + \eta y_i x_i \\
&amp; b \leftarrow b + \eta y_i
\end{align}
$$

式中 $\eta (0 &lt; \eta \leq 1)$ 是步长，在统计学习中又称为学习率（ learning rate ）。这样，通过迭代可以期待损失函数 $L(w,b)$ 不断减小，直到为 $0$ 。综上所述，得到如下算法：
<h2><span style="color: #ff0000;">算法1  感知机学习算法的原始形式</span></h2>
<ul>
	<li>输入：训练数据集$T = \{(x_1,y_1), (x_2,y_2), ..., (x_N,y_N)\}$ ，其中 $x_i \in \chi = \Re^n, y_i \in \gamma = \{-1,1\}, i=1,2,...,N$ ；学习率 $\eta(0 &lt; \eta \leq 1)$ ；</li>
	<li>输出： $w,b$ ；感知机模型 $f(x) = \text{sign}(w \cdot x + b)$</li>
</ul>
<ol>
	<li>选取初值 $w_0, b_0$</li>
	<li>在训练集中选取数据 $(x_i, y_i)$</li>
	<li>如果 $y_i (w \cdot x_i + b) \leq 0$ （相乘是负数，说明二者异号，预测与实际不一致，则更新参数）
$$
\begin{align}
&amp; w \leftarrow w + \eta y_i x_i \\
&amp; b \leftarrow b + \eta y_i
\end{align}
$$</li>
	<li>转至 2 ，直至训练集中没有误分类点。</li>
</ol>
这种学习算法直观上有如下解释：当一个实例点被误分类，即位于分离超平面的错误一侧时，则调整 $w,b$ 的值，使分离超平面向该误分类点的一侧移动，以减少该误分类点与超平面间的距离，直至超平面越过该误分类点使其被正确分类。

该算法是感知机学习算法的基本算法，称为原始形式，对应于后面的对偶形式。感知机学习算法简单且易于实现。
<h1>2. 感知机原始形式的 Python 代码</h1>
<h2>2.1 问题描述</h2>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2016/11/perceptron_demo.png" alt="" width="499" height="447" />
<p style="text-align: center;"><strong>数据实例展示</strong></p>
例 如图所示的训练数据集，其正实例点是 $x_1 = (3,3)^T, x_2 = (4,3)^T$ ，负实例点是 $x_3 = (1,1)^T$ ，试用感知机学习算法的原始形式求感知机模型 $f(x) = \text{sign}(w \cdot x + b)$ 。这里， $w=(w^{(1)}, w^{(2)}), x=(x^{(1)}, x^{(2)})^T$ 。
<h2>2.2 输入数据</h2>
三个实例，每个实例有两个特征（$x_1, x_2$）。
<pre class="lang:python decode:true">id x1 x2 y
1 3 3 1
2 4 3 1
3 1 1 -1</pre>
<h2>2.3 Python 代码</h2>
书中给出的计算步骤，我这里省略，给出感知机原始形式的算法 Python 代码实现：
<pre class="lang:python decode:true" title="Perception.py"># -*- coding: utf-8 -*-
################################### PART0 DESCRIPTION #################################
# Filename: Perceptron.py
# Description:
#

# E-mail: ysh329@sina.com
# Create: 2016-11-15 09:28:18
# Last:
__author__ = 'yuens'


################################### PART1 IMPORT ######################################
import random

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
            cleanData = map(lambda recordList: \
                                map(int, recordList), \
                            rawData[1:])
        else:
            print("header:None")
            cleanData = map(lambda recordList: \
                                map(int, recordList), \
                            rawData)
        idList = map(lambda r: r[0], cleanData)
        xList = map(lambda r: r[1:len(r)-1], cleanData)
        yList = map(lambda r: r[-1], cleanData)
        return idList, xList, yList


class Perceptron(object):
    '''
    原始形式-感知器类，基于随机梯度下降法训练感知器算法，遇到分类分类错误样本进行参数更新。
    '''
    def __init__(self, sampleNum, featureNum, learningRate=10E-4):
        '''
        初始化感知器。
        :param sampleNum: 训练集样本数目
        :param featureNum: 每个样本的特征数
        :param learningRate: 随机梯度下降算法中的参数学习率
        '''
        # 初始化超参数
        self.sampleNum = sampleNum
        self.featureNum = featureNum
        self.eta = learningRate

        # 随机初始化参数
        self.wList = [map(lambda wIdx:\
                             random.random(),\
                         xrange(featureNum))]
        self.bList = [random.random()]

    def train(self, xList, yList, maxEpochNum):
        '''
        基于随机梯度下降算法对感知器参数更新，训练最大轮数为 maxEpoch ，
        当训练集完全预测正确则停止训练。
        :param xList: 训练集的特征数据
        :param yList: 训练集的真实类别
        :param maxEpochNum: 最大迭代次数，若某轮 epoch 预测完全正确则
        停止迭代。
        :return:
        '''
        # costList 和 misRateList 用于记录每轮 epoch 的损失函数
        # 值和错分率
        costList = []
        misRateList = []
        for epochIdx in xrange(maxEpochNum):
            # 记录当前这一轮的预测类别，
            # 在之后判断本轮是否全部预测正确
            # 全部正确则停止迭代
            curEpochYHatList = []
            cost = 0.0
            errNum = 0.0
            print("======= epochIdx {0} =======".format(epochIdx))
            for sampleIdx in xrange(len(xList)):
                # 初始参数
                iterIdx = epochIdx * len(xList) + sampleIdx
                x = xList[sampleIdx]
                y = yList[sampleIdx]
                yHat, summation = self.predict(x)
                curEpochYHatList.append(yHat)
                cost += -summation * y
                # 打印中间结果
                print("== iterIdx:{0} ==".format(iterIdx))
                print("x:{0}".format(x))
                print("y:{0}".format(y))
                print("yHat:{0}".format(yHat))
                print("summation:{0}".format(summation))
                # 判断预测正确与否
                if yHat != y:
                    # 预测错误进行参数更新
                    w = map(lambda ww, xx:\
                                ww + self.eta * y * xx,\
                            self.wList[-1], x)
                    b = self.bList[-1] + self.eta * y
                    self.wList.append(w)
                    self.bList.append(b)
                    errNum += 1.0
                else:
                    # 预测正确追加参数占位
                    self.wList.append(self.wList[-1])
                    self.bList.append(self.bList[-1])

            # 追加保存本轮结束后的损失函数值和错分率并打印出来
            costList.append(cost)
            misRate = errNum/len(curEpochYHatList)
            misRateList.append(misRate)
            print("&gt;&gt;&gt; cost:{0}".format(cost))
            print("&gt;&gt;&gt; misRate:{0}".format(misRate))
            # 判断本轮训练后的所有预测是否完全正确
            if curEpochYHatList == yList:
                # 训练集全部正确则停止迭代
                break
            print

        parameterDict = dict()
        parameterDict['w'] = self.wList[-1]
        parameterDict['b'] = self.bList[-1]
        return parameterDict, costList, misRateList

    def predict(self, x, iterIdx=None):
        '''
        使用第 iterIdx (=epochIdx*len(xList)+xIdx)次更新的参数
        来预测样本 x 的类别。若未指明 iterIdx ，则使用最新一次的参
        数（即最后一次参数）来预测。
        :param x: 输入样本（一个样本）特征
        :param iterIdx: 使用第 i 次训练过程中得到的感知器参数进行
        训练
        :return: 返回预测得到的类别标签（对预测值进行符号转换）以及
        该样本的预测值
        '''
        if iterIdx is None:
            iterIdx = -1
        summation = sum(\
            map(lambda xx, ww:\
                    xx*ww,\
                x, self.wList[iterIdx])\
            )
        summation += self.bList[iterIdx]
        yHat = self.sign(summation)
        return yHat, summation

    def sign(self, v):
        '''
        符号函数，传入参数 v 大于 0 则为返回 1 ，小于 0 返回 -1 ，
        等于 0 则返回 0 。
        :param v: 传入参数
        :return: 返回传入参数的正负性
        '''
        if v &gt; 0.0:
            return 1
        elif v == 0.0:
            return 0
        else:
            return -1

    def plotChart(self, costList, misRateList, saveFigPath):
        '''
        绘制错分率和损失函数值随 epoch 变化的曲线。
        :param costList: 训练过程中每个epoch的损失函数列表
        :param misRateList: 训练过程中每个epoch的错分率列表
        :return:
        '''
        # 导入绘图库
        import matplotlib.pyplot as plt
        # 新建画布
        plt.figure('Perceptron Cost and Mis-classification Rate',figsize=(8, 9))
        # 设定两个子图和位置关系
        ax1 = plt.subplot(211)
        ax2 = plt.subplot(212)

        # 选择子图1并绘制损失函数值折线图及相关坐标轴
        plt.sca(ax1)
        plt.plot(xrange(1, len(costList)+1), costList, '--b*')
        plt.xlabel('Epoch No.')
        plt.ylabel('Cost')
        plt.title('Plot of Cost Function')
        plt.grid()
        ax1.legend(u"Cost", loc='best')

        # 选择子图2并绘制错分率折线图及相关坐标轴
        plt.sca(ax2)
        plt.plot(xrange(1, len(misRateList)+1), misRateList, '-r*')
        plt.xlabel('Epoch No.')
        plt.ylabel('Mis-classification Rate')
        plt.title('Plot of Mis-classification Rate')
        plt.grid()
        ax2.legend(u'Mis-classification Rate', loc='best')

        # 显示图像并打印和保存
        # 需要先保存再绘图否则相当于新建了一张新空白图像然后保存
        plt.savefig(saveFigPath)
        plt.show()

################################### PART3 TEST ########################################
# 例子
if __name__ == "__main__":
    # 初始化参数
    dataPath = "./input"
    maxEpochNum = 1000
    learningRate = 10E-2
    saveFigPath = './PerceptronPlot.png'

    # 加载训练数据
    idList, xList, yList = readDataFrom(path=dataPath)
    print("idList:{0}".format(idList))
    print("xList:{0}".format(xList))
    print("yList:{0}".format(yList))

    # 初始化感知器类
    p = Perceptron(sampleNum=len(xList),\
                   featureNum=len(xList[0]),\
                   learningRate=learningRate)

    # 打印初始时的感知器参数
    print("p.wList:{0}".format(p.wList))
    print("p.bList:{0}".format(p.bList))

    # 训练感知器
    print("====== train ======")
    parameterDict, costList, misRateList = p.train(xList=xList, \
                                                   yList=yList, \
                                                   maxEpochNum=maxEpochNum)

    # 打印结果
    print("====== result ======")
    print("parameterDict['w']:{0}".format(parameterDict['w']))
    print("parameterDict['b']:{0}".format(parameterDict['b']))

    print("yList:{0}".format(yList))
    print("yHatList:{0}".format(map(lambda x:\
                                        p.predict(x),\
                                    xList)))

    print("p.wList:{0}".format(p.wList))
    print("p.bList:{0}".format(p.bList))

    print("len(p.wList):{0}".format(len(p.wList)))
    print("len(p.bList):{0}".format(len(p.bList)))

    # 绘制损失函数和错分率随 epoch 变化的图像
    p.plotChart(costList=costList,\
                misRateList=misRateList,\
                saveFigPath=saveFigPath)</pre>
<h2>2.4 结果输出</h2>
结果输出，由于参数初始时是随机的，所以每次运行的结果不同，下面请参考：
<pre class="lang:python decode:true">/usr/local/lib/anaconda2/bin/python /home/yuens/Projects/PyCharm/statistical-learning-methods-note/chapter_2_perceptron/Perceptron.py
header:['id', 'x1', 'x2', 'y']
idList:[1, 2, 3]
xList:[[3, 3], [4, 3], [1, 1]]
yList:[1, 1, -1]
p.wList:[[0.3785288614797855, 0.9062417009132904]]
p.bList:[0.9019748422392793]
====== train ======
======= epochIdx 0 =======
== iterIdx:0 ==
x:[3, 3]
y:1
yHat:1
summation:4.75628652942
== iterIdx:1 ==
x:[4, 3]
y:1
yHat:1
summation:5.1348153909
== iterIdx:2 ==
x:[1, 1]
y:-1
yHat:1
summation:2.18674540463
&gt;&gt;&gt; cost:-7.70435651568
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 1 =======
== iterIdx:3 ==
x:[3, 3]
y:1
yHat:1
summation:4.05628652942
== iterIdx:4 ==
x:[4, 3]
y:1
yHat:1
summation:4.3348153909
== iterIdx:5 ==
x:[1, 1]
y:-1
yHat:1
summation:1.88674540463
&gt;&gt;&gt; cost:-6.50435651568
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 2 =======
== iterIdx:6 ==
x:[3, 3]
y:1
yHat:1
summation:3.35628652942
== iterIdx:7 ==
x:[4, 3]
y:1
yHat:1
summation:3.5348153909
== iterIdx:8 ==
x:[1, 1]
y:-1
yHat:1
summation:1.58674540463
&gt;&gt;&gt; cost:-5.30435651568
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 3 =======
== iterIdx:9 ==
x:[3, 3]
y:1
yHat:1
summation:2.65628652942
== iterIdx:10 ==
x:[4, 3]
y:1
yHat:1
summation:2.7348153909
== iterIdx:11 ==
x:[1, 1]
y:-1
yHat:1
summation:1.28674540463
&gt;&gt;&gt; cost:-4.10435651568
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 4 =======
== iterIdx:12 ==
x:[3, 3]
y:1
yHat:1
summation:1.95628652942
== iterIdx:13 ==
x:[4, 3]
y:1
yHat:1
summation:1.9348153909
== iterIdx:14 ==
x:[1, 1]
y:-1
yHat:1
summation:0.986745404632
&gt;&gt;&gt; cost:-2.90435651568
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 5 =======
== iterIdx:15 ==
x:[3, 3]
y:1
yHat:1
summation:1.25628652942
== iterIdx:16 ==
x:[4, 3]
y:1
yHat:1
summation:1.1348153909
== iterIdx:17 ==
x:[1, 1]
y:-1
yHat:1
summation:0.686745404632
&gt;&gt;&gt; cost:-1.70435651568
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 6 =======
== iterIdx:18 ==
x:[3, 3]
y:1
yHat:1
summation:0.556286529419
== iterIdx:19 ==
x:[4, 3]
y:1
yHat:1
summation:0.334815390898
== iterIdx:20 ==
x:[1, 1]
y:-1
yHat:1
summation:0.386745404632
&gt;&gt;&gt; cost:-0.504356515684
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 7 =======
== iterIdx:21 ==
x:[3, 3]
y:1
yHat:-1
summation:-0.143713470581
== iterIdx:22 ==
x:[4, 3]
y:1
yHat:1
summation:1.7348153909
== iterIdx:23 ==
x:[1, 1]
y:-1
yHat:1
summation:0.786745404632
&gt;&gt;&gt; cost:-0.804356515684
&gt;&gt;&gt; misRate:0.666666666667

======= epochIdx 8 =======
== iterIdx:24 ==
x:[3, 3]
y:1
yHat:1
summation:1.05628652942
== iterIdx:25 ==
x:[4, 3]
y:1
yHat:1
summation:0.934815390898
== iterIdx:26 ==
x:[1, 1]
y:-1
yHat:1
summation:0.486745404632
&gt;&gt;&gt; cost:-1.50435651568
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 9 =======
== iterIdx:27 ==
x:[3, 3]
y:1
yHat:1
summation:0.356286529419
== iterIdx:28 ==
x:[4, 3]
y:1
yHat:1
summation:0.134815390898
== iterIdx:29 ==
x:[1, 1]
y:-1
yHat:1
summation:0.186745404632
&gt;&gt;&gt; cost:-0.304356515684
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 10 =======
== iterIdx:30 ==
x:[3, 3]
y:1
yHat:-1
summation:-0.343713470581
== iterIdx:31 ==
x:[4, 3]
y:1
yHat:1
summation:1.5348153909
== iterIdx:32 ==
x:[1, 1]
y:-1
yHat:1
summation:0.586745404632
&gt;&gt;&gt; cost:-0.604356515684
&gt;&gt;&gt; misRate:0.666666666667

======= epochIdx 11 =======
== iterIdx:33 ==
x:[3, 3]
y:1
yHat:1
summation:0.856286529419
== iterIdx:34 ==
x:[4, 3]
y:1
yHat:1
summation:0.734815390898
== iterIdx:35 ==
x:[1, 1]
y:-1
yHat:1
summation:0.286745404632
&gt;&gt;&gt; cost:-1.30435651568
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 12 =======
== iterIdx:36 ==
x:[3, 3]
y:1
yHat:1
summation:0.156286529419
== iterIdx:37 ==
x:[4, 3]
y:1
yHat:-1
summation:-0.0651846091017
== iterIdx:38 ==
x:[1, 1]
y:-1
yHat:1
summation:0.786745404632
&gt;&gt;&gt; cost:0.695643484316
&gt;&gt;&gt; misRate:0.666666666667

======= epochIdx 13 =======
== iterIdx:39 ==
x:[3, 3]
y:1
yHat:1
summation:1.65628652942
== iterIdx:40 ==
x:[4, 3]
y:1
yHat:1
summation:1.7348153909
== iterIdx:41 ==
x:[1, 1]
y:-1
yHat:1
summation:0.486745404632
&gt;&gt;&gt; cost:-2.90435651568
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 14 =======
== iterIdx:42 ==
x:[3, 3]
y:1
yHat:1
summation:0.956286529419
== iterIdx:43 ==
x:[4, 3]
y:1
yHat:1
summation:0.934815390898
== iterIdx:44 ==
x:[1, 1]
y:-1
yHat:1
summation:0.186745404632
&gt;&gt;&gt; cost:-1.70435651568
&gt;&gt;&gt; misRate:0.333333333333

======= epochIdx 15 =======
== iterIdx:45 ==
x:[3, 3]
y:1
yHat:1
summation:0.256286529419
== iterIdx:46 ==
x:[4, 3]
y:1
yHat:1
summation:0.134815390898
== iterIdx:47 ==
x:[1, 1]
y:-1
yHat:-1
summation:-0.113254595368
&gt;&gt;&gt; cost:-0.504356515684
&gt;&gt;&gt; misRate:0.0
====== result ======
parameterDict['w']:[-0.12147113852021443, 0.30624170091329084]
parameterDict['b']:-0.298025157761
yList:[1, 1, -1]
yHatList:[(1, 0.25628652941850855), (1, 0.13481539089829414), (-1, -0.11325459536764423)]
p.wList:[[0.3785288614797855, 0.9062417009132904], [0.3785288614797855, 0.9062417009132904], [0.3785288614797855, 0.9062417009132904], [0.2785288614797855, 0.8062417009132904], [0.2785288614797855, 0.8062417009132904], [0.2785288614797855, 0.8062417009132904], [0.1785288614797855, 0.7062417009132904], [0.1785288614797855, 0.7062417009132904], [0.1785288614797855, 0.7062417009132904], [0.0785288614797855, 0.6062417009132904], [0.0785288614797855, 0.6062417009132904], [0.0785288614797855, 0.6062417009132904], [-0.02147113852021451, 0.5062417009132905], [-0.02147113852021451, 0.5062417009132905], [-0.02147113852021451, 0.5062417009132905], [-0.12147113852021452, 0.4062417009132905], [-0.12147113852021452, 0.4062417009132905], [-0.12147113852021452, 0.4062417009132905], [-0.22147113852021452, 0.3062417009132905], [-0.22147113852021452, 0.3062417009132905], [-0.22147113852021452, 0.3062417009132905], [-0.3214711385202145, 0.2062417009132905], [-0.021471138520214483, 0.5062417009132906], [-0.021471138520214483, 0.5062417009132906], [-0.12147113852021449, 0.4062417009132906], [-0.12147113852021449, 0.4062417009132906], [-0.12147113852021449, 0.4062417009132906], [-0.2214711385202145, 0.3062417009132906], [-0.2214711385202145, 0.3062417009132906], [-0.2214711385202145, 0.3062417009132906], [-0.32147113852021447, 0.2062417009132906], [-0.021471138520214428, 0.5062417009132907], [-0.021471138520214428, 0.5062417009132907], [-0.12147113852021443, 0.4062417009132907], [-0.12147113852021443, 0.4062417009132907], [-0.12147113852021443, 0.4062417009132907], [-0.22147113852021444, 0.3062417009132907], [-0.22147113852021444, 0.3062417009132907], [0.17852886147978558, 0.6062417009132908], [0.07852886147978558, 0.5062417009132908], [0.07852886147978558, 0.5062417009132908], [0.07852886147978558, 0.5062417009132908], [-0.021471138520214428, 0.4062417009132908], [-0.021471138520214428, 0.4062417009132908], [-0.021471138520214428, 0.4062417009132908], [-0.12147113852021443, 0.30624170091329084], [-0.12147113852021443, 0.30624170091329084], [-0.12147113852021443, 0.30624170091329084], [-0.12147113852021443, 0.30624170091329084]]
p.bList:[0.9019748422392793, 0.9019748422392793, 0.9019748422392793, 0.8019748422392793, 0.8019748422392793, 0.8019748422392793, 0.7019748422392793, 0.7019748422392793, 0.7019748422392793, 0.6019748422392793, 0.6019748422392793, 0.6019748422392793, 0.5019748422392794, 0.5019748422392794, 0.5019748422392794, 0.4019748422392794, 0.4019748422392794, 0.4019748422392794, 0.3019748422392794, 0.3019748422392794, 0.3019748422392794, 0.2019748422392794, 0.3019748422392794, 0.3019748422392794, 0.2019748422392794, 0.2019748422392794, 0.2019748422392794, 0.10197484223927938, 0.10197484223927938, 0.10197484223927938, 0.0019748422392793785, 0.10197484223927938, 0.10197484223927938, 0.0019748422392793785, 0.0019748422392793785, 0.0019748422392793785, -0.09802515776072063, -0.09802515776072063, 0.0019748422392793785, -0.09802515776072063, -0.09802515776072063, -0.09802515776072063, -0.19802515776072063, -0.19802515776072063, -0.19802515776072063, -0.29802515776072064, -0.29802515776072064, -0.29802515776072064, -0.29802515776072064]
len(p.wList):49
len(p.bList):49

Process finished with exit code 130
</pre>
那么，基于三个实例的输入训练数据的感知机为：

$$
f(x) = \text{sign} (-0.1214 x_1 + 0.3062 x_2 + -0.298)
$$

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2016/11/perceptron_cost_and_mis-classification_rate.png" alt="" width="509" height="572" />
<p style="text-align: center;"><strong>Python 代码绘制的成本函数及错分率图像</strong></p>

<h1>3. 算法的收敛性</h1>
现在证明，对于线性可分数据集感知机学习算法原始形式收敛，即经过有限次迭代可以得到一个将训练数据集完全正确划分的分离超平面及感知机模型。

为了便于叙述与推导，将偏置 $b$ 并入权重向量 $w$ ，记作 $\hat{w} = (w^T,b)^T$ ，同样也将输入向量加以扩充，加进常数 $1$ ，记作 $\hat{x}=(x^T, 1)^T$ 。这样， $\hat{x} \in \Re^{n+1}, \hat{w} \in \Re^{n+1}$ 。显然， $\hat{w} \cdot \hat{x} = w \cdot x + b$ 。
<h2><span style="color: #ff0000;">定理 1 Novikoff</span></h2>
设训练集数据 $T=\{ (x_1, y_1), (x_2, y_2), ..., (x_N, y_N) \}$ 是线性可分的，其中 $x_i \in \chi = \Re^n, y_i \in \gamma = \{-1,+1\}, i=1,2,...,N$ ，则
<ol>
	<li>存在满足条件 $||\hat{w}_{opt}|| = 1$ 的超平面 $\hat{w}_{opt} \cdot \hat{x} = w_{opt} \cdot x + b_{opt} = 0$ 将训练数据集完全正确分开；且存在 $\gamma &gt; 0$ ，对所有 $i=1,2,...,N$
$$
y_i (\hat{w}_{opt} \cdot \hat{x}_i) = y_i (w_{opt} \cdot x_i + b_{opt}) \geq \gamma
$$</li>
	<li>令 $R = \max_{1 \leq i \leq N} ||\hat{x}_i||$ ，则感知机算法在训练数据集上的误分类次数 k 满足不等式：
$$
k \leq (\frac{R}{\gamma})^2
$$</li>
</ol>
<h2>证明</h2>
（1）由于训练数据集是线性可分的，按照定义2，存在超平面可将训练数据集完全正确分开，取此时超平面为 $\hat{w}_{opt} \cdot \hat{x} = w_{opt} \cdot x + b_{opt} = 0$ ，使 $||\hat{x}_{opt}|| = 1$ 。由于对有限的 $i=1, 2, ..., N$ ，均有
$$
y_i (\hat{w}_{opt} \cdot \hat{x}_i) = y_i (w_{opt} \cdot x_i + b_{opt}) &gt; 0
$$
所以存在
$$
\gamma = \min_{i} \{  y_i(w_{opt} \cdot x_i + b_{opt}) \}
$$
使
$$
y_i(\hat{w}_{opt} \cdot \hat{x}_i) = y_i (w_{opt} \cdot x_i + b_{opt}) \geq \gamma
$$

（2）感知机算法从 $\hat{w}_0 = 0$ 开始，如果实例被误分类，则更新权重。令 $\hat{w}_{k-1}$ 是第 $k$ 个误分类实例之前的扩充权重向量，即
$$
\hat{w}_{k-1} = (w_{k-1}^T, b_{k-1})^T
$$
则第 $k$ 个误分类实例的条件是
$$
y_i (\hat{w}_{k-1} \cdot \hat{x}_i) = y_i (w_{k-1} \cdot x_i + b_{k-1}) \leq 0
$$
若 $(x_i, y_i)$ 是被 $\hat{w}_{k-1} = (w_{k-1}^T, b_{k-1}^T)$ 误分类的数据，则 $w$ 和 $b$ 的更新是
$$
\begin{align}
&amp; w_{k} \leftarrow w_{k-1} + \eta y_i x_i \\
&amp; b_{k} \leftarrow b_{k-1} + \eta y_i
\end{align}
$$
即
$$
\hat{w}_k = \hat{w}_{k-1} + \eta y_i \hat{x}_i
$$

下面推导两个不等式：

<span style="color: #ff0000;">（1）$\hat{w}_k \cdot \hat{w}_{opt} \geq k \eta \gamma$</span>
该式由 $\hat{w}_k = \hat{w}_{k-1} + \eta y_i \hat{x}_i$ 及 $y_i(\hat{w}_{opt} \cdot \hat{x}_i) = y_i (w_{opt} \cdot x_i + b_{opt}) \geq \gamma$ 得
$$
\begin{align}
\hat{w}_k \hat{w}_{opt} &amp; = \hat{w}_{k-1} \cdot \hat{w}_{opt} + \eta y_i \hat{w}_{opt} \cdot \hat{x}_i \\
&amp; \geq \hat{w}_{k-1} \cdot \hat{w}_{opt} + \eta \gamma
\end{align}
$$

由此递推即得不等式
$$
\hat{w}_k \cdot \hat{w}_{opt} \geq \hat{w}_{k-1} \cdot \hat{w}_{opt} + \eta \gamma \geq \hat{w}_{k-2} \cdot \hat{w}_{opt} + 2\eta\gamma \geq ... \geq k \eta \gamma
$$

<span style="color: #ff0000;">（2）$||\hat{w}_k||^2 \leq k \eta^2 R^2$</span>
由式 $\hat{w}_k = \hat{w}_{k-1} + \eta y_i \hat{x}_i$ 及式 $y_i (\hat{w}_{k-1} \cdot \hat{x}_i) = y_i (w_{k-1} \cdot x_i + b_{k-1}) \leq 0$ 得
$$
\begin{align}
||\hat{w}_k||^2 &amp; = ||\hat{w}_{k-1}||^2 + 2 \eta \hat{w}_{k-1} \cdot \hat{x_i} + \eta^2 ||\hat{x}_i||^2 \\
&amp; \leq ||\hat{w}_{k-1}||^2 + \eta^2 ||\hat{x}_i||^2 \\
&amp; \leq ||\hat{w}_{k-1}||^2 + \eta^2 R^2 \\
&amp; \leq ||\hat{w}_{k-2}||^2 + 2 \eta^2 R^2 \leq ... \\
&amp; \leq k \eta^2 R^2
\end{align}
$$

结合不等式 $\hat{w}_k \cdot \hat{w}_{opt} \geq k \eta \gamma$ 和 $||\hat{w}_k||^2 \leq k \eta^2 R^2$ 即得
$$
\begin{align}
&amp; k \eta \gamma \leq \hat{w}_k \cdot \hat{w}_{opt} \leq ||\hat{w}_k|| ||\hat{w}_{opt}|| \leq \sqrt{k \eta} R \\
k^2 \gamma^2 \leq k R^2
\end{align}
$$
于是
$$
k \leq \left ( \frac{R}{\gamma} \right )^2
$$
定理表明，误分类的次数 $k$ 是有上界的，经过有限次搜索可以找到将训练数据完全正确分开的分类超平面。也就是说，当训练数据集线性可分时，感知机学习算法原始形式迭代是收敛的。但是例题2.1说明感知机学习算法存在许多解，这些解<strong><span style="color: #ff0000;">既依赖初值的选择，也依赖于迭代过程中误分类点的选择顺序。为了得到唯一的超平面，需要对分离超平面增加约束条件。</span></strong>

这就是第7章将要讲述的线性支持向量机的想法，当训练集线性不可分时，感知机学习算法不收敛，迭代结果会发生震荡。
<h1>4. 感知机学习算法的对偶形式</h1>
考虑感知机学习算法的对偶形式，感知机学习算法的原始形式与对偶形式与第 7 章支持向量机算法的原始形式与对偶形式相对应。

对偶形式的基本想法是，将 $w$ 和 $b$ 表示为实例 $x_i$ 和标记 $y_i$ 的线性组合的形式，通过求解其系数而求得 $w$ 和 $b$ 。不失一般性，在原始形式的算法中可假设初始值 $w_0, b_0$ 均为 $0$ 。对误分类点 $(x_i, y_i)$ 通过

$$
\begin{align}
&amp; w \leftarrow w + \eta y_i x_i \\
&amp; b \leftarrow b + \eta y_i
\end{align}
$$

逐步修改 $w, b$ ，设修改 $n$ 次，则 $w, b$ 关于 $(x_i, y_i)$ 的增量分别是 $\alpha_i y_i x_i$ 和 $\alpha_i y_i$ ，这里 $\alpha_i = n_i \eta$ 。这样，从学习过程不难看出，最后学习到的 $w,b$ 可以分别表示为：

$$
\begin{align}
&amp; w = \sum_{i=1}^N \alpha_i y_i x_i \\
&amp; b = \sum_{i=1}^N \alpha_i y_i
\end{align}
$$

这里， $\alpha_i \geq 0 , i=1,2,..., N$ ，当  $\eta=1$ 时，表示第 $i$ 个实例点由于误分而进行更新的次数。实例点更新次数越多，意味着它距离分离超平面越近，也就越难正确分类。换句话说，这样的实例对学习结果影响最大。

下面对照原始形式来叙述感知机学习算法的对偶形式。
<h2><span style="color: #ff0000;">算法2 感知机学习算法的对偶形式</span></h2>
<ul>
	<li>输入：线性可分的数据集 $T = \{(x_1,y_1), (x_2,y_2), ..., (x_N,y_N)\}$ ，其中 $x_i \in \chi = \Re^n, y_i \in \gamma = \{-1,1\}, i=1,2,...,N$ ；学习率 $\eta(0 &lt; \eta \leq 1)$ ；</li>
	<li>输出： $\alpha,b$ ；感知机模型 $f(x) = \text{sign}\left ( \sum_{j=1}^N \alpha_j y_j x_j \cdot x + b  \right )$ ，其中 $\alpha = (\alpha_1, \alpha_2,..., \alpha_N)^T$</li>
</ul>
<ol>
	<li>选取初值 $\alpha \leftarrow 0, b \leftarrow 0$</li>
	<li>在训练集中选取数据 $(x_i, y_i)$</li>
	<li>如果 $y_i \left ( \sum_{j=1}^N \alpha_j y_j x_j \cdot x_i + b \right ) \leq 0$ （相乘是负数，说明二者异号，预测与实际不一致，则更新参数）
$$
\begin{align}
&amp;\alpha_i \leftarrow \alpha_i + \eta \\
&amp; b \leftarrow b + \eta y_i
\end{align}
$$</li>
	<li>转至 2 ，直至训练集中没有误分类点。</li>
</ol>
对偶形式中训练实例仅以内积的形式出现，为了方便，可以预先将训练集中实例间的内积计算出来并以矩阵的形式存储，这个矩阵就是所谓的 Gram 矩阵（Gram matrix）。

$$
G = [x_i \cdot x_j]_{N \times N}
$$
<h1>5. 感知机对偶形式的 Python 代码</h1>
同样还是上面的问题，输入数据这里就省略了。使用感知机学习算法对偶形式求感知机模型，直接上代码。
<h2>5.1 Python 代码</h2>
<pre class="lang:python decode:true" title="Dual-form_Perceptron.py"># -*- coding: utf-8 -*-
################################### PART0 DESCRIPTION #################################
# Filename: Dual-form_Perceptron.py
# Description:
#

# E-mail: ysh329@sina.com
# Create: 2016-11-16 21:55:59
# Last:
__author__ = 'yuens'


################################### PART1 IMPORT ######################################
import random

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
            cleanData = map(lambda recordList: \
                                map(int, recordList), \
                            rawData[1:])
        else:
            print("header:None")
            cleanData = map(lambda recordList: \
                                map(int, recordList), \
                            rawData)
        idList = map(lambda r: r[0], cleanData)
        xList = map(lambda r: r[1:len(r)-1], cleanData)
        yList = map(lambda r: r[-1], cleanData)
        return idList, xList, yList


class DualFormPerceptron(object):
    '''
    对偶形式-感知器类，基于随机梯度下降法训练感知器算法，遇到分类分类错误样本进行参数更新。
    '''
    def __init__(self, sampleNum, featureNum, learningRate=10E-4):
        '''
        初始化感知器。
        :param sampleNum: 训练集样本数目
        :param featureNum: 每个样本的特征数
        :param learningRate: 随机梯度下降算法中的参数学习率
        '''
        # 初始化超参数
        self.sampleNum = sampleNum
        self.featureNum = featureNum
        self.eta = learningRate

        # 随机初始化参数
        self.alphaList = [map(lambda i: \
                                  0.0, #random.random(),
                              xrange(self.sampleNum))\
                          ]
        self.bList = [0.0] #[random.random()]

    def constructGramMatrix(self, xList):
        '''
        构造 Gram 矩阵。该矩阵中的每个元素是每两个样本间对应特征的点积和。
        :param xList: 输入样本特征
        :return:
        '''
        self.gramMatrix = [[0 for col in xrange(self.sampleNum)] for row in xrange(self.sampleNum)]
        print self.gramMatrix
        for idx1 in xrange(self.sampleNum):
            for idx2 in xrange(self.sampleNum):
                if idx1 &lt;= idx2:
                    innerProd = sum(\
                        map(lambda xx1, xx2:\
                                xx1 * xx2,\
                            xList[idx1], xList[idx2])\
                        )
                    self.gramMatrix[idx1][idx2] = self.gramMatrix[idx2][idx1] = innerProd

    def train(self, xList, yList, maxEpochNum):
        '''
        基于随机梯度下降算法对感知器参数更新，训练最大轮数为 maxEpoch ，
        当训练集完全预测正确则停止训练。
        :param xList: 训练集的特征数据
        :param yList: 训练集的真实类别
        :param maxEpochNum: 最大迭代次数，若某轮 epoch 预测完全正确则
        停止迭代。
        :return:
        '''
        # costList 和 misRateList 用于记录每轮 epoch 的损失函数
        # 值和错分率
        costList = []
        misRateList = []
        for epochIdx in xrange(maxEpochNum):
            print("======= epochIdx {0} =======".format(epochIdx))
            curEpochCostList = []
            for sampleIdxI in xrange(len(xList)):
                x = xList[sampleIdxI]
                yHat, sigma = self.predict(x,\
                                           xList,\
                                           yList,\
                                           useGramMatrix=True,\
                                           sampleIdxI=sampleIdxI,\
                                           iterIdx=None)
                cost = yList[sampleIdxI] * sigma
                curEpochCostList.append(cost)
                # 打印cost
                iterIdx = epochIdx * len(xList) + sampleIdxI
                print("== iterIdx:{0} ==".format(iterIdx))
                print("cost:{0}".format(cost))
                # 判断是否进行参数更新
                if cost &lt;= 0:
                    nextAlpha = self.alphaList[epochIdx]
                    nextAlpha[sampleIdxI] += self.eta
                    #nextAlpha = self.alphaList[epochIdx][sampleIdxI] + self.eta
                    nextB = self.bList[epochIdx] + self.eta * yList[sampleIdxI]
                else:
                    nextAlpha = self.alphaList[-1]
                    nextB = self.bList[-1]
                self.alphaList.append(nextAlpha)
                self.bList.append(nextB)
            # 判断当前参数的预测性能
            curEpochPredictTupleList = map(lambda x:\
                                           self.predict(x, xList, yList),\
                                       xList)
            curEpochResultList = map(lambda idx, (yHat, sigma), y:\
                                         (idx, yHat, y, y == yHat, sigma),\
                                     xrange(len(curEpochPredictTupleList)), curEpochPredictTupleList, yList)
            curEpochCorrectNum = len(\
                filter(lambda (idx, yHat, y, isCorrectPredict, sigma):\
                           isCorrectPredict,\
                       curEpochResultList)\
                )
            misRate = 1.0 - float(curEpochCorrectNum) / len(curEpochResultList)
            print("misRate:{0}".format(misRate))
            misRateList.append(misRate)
            curEpochCost = sum(curEpochCostList)
            print("cost:{0}".format(curEpochCost))
            costList.append(curEpochCost)
            print
            if misRate == 0.0:
                break

        # 最终参数打包为字典
        parameterDict = dict()
        parameterDict['alpha'] = self.alphaList[-1]
        parameterDict['b'] = self.bList[-1]
        return parameterDict, costList, misRateList

    def predict(self, x, xList, yList, useGramMatrix=False, sampleIdxI=None, iterIdx=None):
        '''
        预测输入样本 x 的所属类别。由于是对偶形式，所以也需要传入训练数据集的特征和标签，
        训练过程中，需要指定 userGramMatrix , sampleIdxI 参数，
        iterIdx 可以使用模型训练完成后使用某 iterIdx 次训练得到的模型参数进行预测，
        默认使用最后一次模型参数。
        :param x: 输入样本 x 的特征
        :param xList: 训练数据集的特征
        :param yList: 训练数据集的真实标签
        :param useGramMatrix: 是否使用 Gram 矩阵，训练时需指定
        :param sampleIdxI: 当前样本位于训练集的下标，训练时需指定
        :param iterIdx: 使用哪一次的模型参数预测，默认使用最后一次
        :return: 返回输入样本 x 的预测类别和 sigma
        '''
        # 使用第几次的模型参数（默认为最后一次，即元素下标为-1）
        if iterIdx == None:
            iterIdx = -1
        # 是否使用 Gram 矩阵
        if useGramMatrix:
            sigma = sum(\
                    map(lambda sampleIdxJ:\
                            self.alphaList[iterIdx][sampleIdxJ] *\
                            yList[sampleIdxJ] *\
                            self.gramMatrix[sampleIdxI][sampleIdxJ],\
                        xrange(len(xList)))\
                    ) + self.bList[iterIdx]
        else:
            sigma = sum(\
                map(lambda sampleIdxJ:\
                        self.alphaList[iterIdx][sampleIdxJ] *\
                        yList[sampleIdxJ] *\
                        sum(\
                            map(lambda xx1, xx2:\
                                    xx1 * xx2,\
                                x, xList[sampleIdxJ])\
                            ),\
                    xrange(len(xList)))\
                ) + self.bList[iterIdx]

        yHat = self.sign(sigma)
        return yHat, sigma

    def sign(self, v):
        '''
        符号函数，传入参数 v 大于 0 则为返回 1 ，小于 0 返回 -1 ，
        等于 0 则返回 0 。
        :param v: 传入参数
        :return: 返回传入参数的正负性
        '''
        if v &gt; 0.0:
            return 1
        elif v == 0.0:
            return 0
        else:
            return -1

    def plotChart(self, costList, misRateList, saveFigPath):
        '''
        绘制错分率和损失函数值随 epoch 变化的曲线。
        :param costList: 训练过程中每个epoch的损失函数列表
        :param misRateList: 训练过程中每个epoch的错分率列表
        :return:
        '''
        # 导入绘图库
        import matplotlib.pyplot as plt
        # 新建画布
        plt.figure('Perceptron Cost and Mis-classification Rate', figsize=(8, 9))
        # 设定两个子图和位置关系
        ax1 = plt.subplot(211)
        ax2 = plt.subplot(212)

        # 选择子图1并绘制损失函数值折线图及相关坐标轴
        plt.sca(ax1)
        plt.plot(xrange(1, len(costList) + 1), costList, '--b*')
        plt.xlabel('Epoch No.')
        plt.ylabel('Cost')
        plt.title('Plot of Cost Function')
        plt.grid()
        ax1.legend(u"Cost", loc='best')

        # 选择子图2并绘制错分率折线图及相关坐标轴
        plt.sca(ax2)
        plt.plot(xrange(1, len(misRateList) + 1), misRateList, '-r*')
        plt.xlabel('Epoch No.')
        plt.ylabel('Mis-classification Rate')
        plt.title('Plot of Mis-classification Rate')
        plt.grid()
        ax2.legend(u'Mis-classification Rate', loc='best')

        # 显示图像并打印和保存
        # 需要先保存再绘图否则相当于新建了一张新空白图像然后保存
        plt.savefig(saveFigPath)
        plt.show()

################################### PART3 TEST ########################################
# 例子
if __name__ == "__main__":
    # 参数初始化
    dataPath = "./input"
    learningRate = 10E-3
    maxEpochNum = 10000
    saveFigPath = "./DualFormPerceptronPlot.png"

    # 读取训练数据
    idList, xList, yList = readDataFrom(path=dataPath,\
                                        hasHeader=True)
    print("idList:{0}".format(idList))
    print("xList:{0}".format(xList))
    print("yList:{0}".format(yList))

    # 对偶形式感知机类的实例化
    dfp = DualFormPerceptron(sampleNum=len(xList),\
                             featureNum=len(xList[0]),\
                             learningRate=learningRate)
    # 基于训练集构造 Gram 矩阵
    dfp.constructGramMatrix(xList=xList)
    # 训练模型
    parameterDict, costList, misRateList = dfp.train(xList=xList,\
                                                     yList=yList,\
                                                     maxEpochNum=maxEpochNum)
    # 打印结果
    print("====== result ======")
    print("parameterDict['alpha']:{0}".format(parameterDict['alpha']))
    print("parameterDict['b']:{0}".format(parameterDict['b']))

    print("yList:{0}".format(yList))
    print("yHatList:{0}".format(map(lambda x: \
                                        dfp.predict(x=x, \
                                                    xList=xList,\
                                                    yList=yList,\
                                                    useGramMatrix=False,\
                                                    sampleIdxI=None,\
                                                    iterIdx=None), \
                                    xList)))

    print("dfp.alphaList:{0}".format(dfp.alphaList))
    print("dfp.bList:{0}".format(dfp.bList))

    print("len(dfp.alphaList):{0}".format(len(dfp.alphaList)))
    print("len(dfp.bList):{0}".format(len(dfp.bList)))

    # 绘制模型的损失函数及错分率性能图像
    dfp.plotChart(costList=costList,\
                  misRateList=misRateList,\
                  saveFigPath=saveFigPath)</pre>
<h2>5.2 结果输出</h2>
<pre class="lang:python decode:true">/usr/local/lib/anaconda2/bin/python /home/yuens/Projects/PyCharm/statistical-learning-methods-note/chapter_2_perceptron/Dual-form_Perceptron.py
header:['id', 'x1', 'x2', 'y']
idList:[1, 2, 3]
xList:[[3, 3], [4, 3], [1, 1]]
yList:[1, 1, -1]
[[0, 0, 0], [0, 0, 0], [0, 0, 0]]
======= epochIdx 0 =======
== iterIdx:0 ==
cost:0.0
== iterIdx:1 ==
cost:0.22
== iterIdx:2 ==
cost:-0.07
misRate:0.333333333333
cost:0.15

======= epochIdx 1 =======
== iterIdx:3 ==
cost:0.11
== iterIdx:4 ==
cost:0.13
== iterIdx:5 ==
cost:-0.03
misRate:0.333333333333
cost:0.21

======= epochIdx 2 =======
== iterIdx:6 ==
cost:0.06
== iterIdx:7 ==
cost:0.07
== iterIdx:8 ==
cost:-0.02
misRate:1.0
cost:0.11

======= epochIdx 3 =======
== iterIdx:9 ==
cost:0.0
== iterIdx:10 ==
cost:0.21
== iterIdx:11 ==
cost:-0.06
misRate:0.333333333333
cost:0.15

======= epochIdx 4 =======
== iterIdx:12 ==
cost:0.1
== iterIdx:13 ==
cost:0.12
== iterIdx:14 ==
cost:-0.02
misRate:0.0
cost:0.2

====== result ======
parameterDict['alpha']:[0.02, 0.0, 0.05]
parameterDict['b']:-0.02
yList:[1, 1, -1]
yHatList:[(1, 0.03999999999999994), (1, 0.04999999999999995), (-1, -1.0408340855860843e-17)]
dfp.alphaList:[[0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05], [0.02, 0.0, 0.05]]
dfp.bList:[0.0, 0.01, 0.01, -0.01, -0.01, -0.01, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -0.02, -0.02, -0.02, -0.02]
len(dfp.alphaList):16
len(dfp.bList):16

Process finished with exit code 0
</pre>
那么，该对偶形式的感知机模型为：

$$
f(x) = \text{sign} \left ( \sum_{j=1}^N \alpha_j y_j x_j \cdot x + b \right )
$$

其中，计算出来的 $b = -0.02, \overrightarrow{\alpha} = (0.02, 0.0, 0.05)$ 。两个实例 $x$ 相乘指的是两个实例对应特征相乘然后加和。

绘制出的损失函数图像及错分率图像为：

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2016/11/DualFormPerceptronPlot.png" alt="" width="515" height="579" />
<h1>6. 本章概要</h1>
<ol>
	<li>感知机是根据输入实例的特征向量 $x$ 对其进行二分类的线性分类器：
$$
f(x) = \text{sign} (w \cdot x + b)
$$
感知机模型对应于输入空间（特征空间）中的分离超平面 $w \cdot x + b = 0$ 。</li>
	<li>感知机学习的策略是极小化损失函数：
$$
\min_{w,b} L(w,b) = -\sum_{x_i \in M} y_i (w \cdot x_i + b)
$$
损失函数对应于误分类点到分离超平面的总距离。</li>
	<li>感知机学习算法是基于随机梯度下降法的对损失函数的最优化算法，有原始形式与对偶形式。算法简单且易于实现。
原始形式：首先任意选取一个超平面，然后使用梯度下降法不断极小化目标函数，这个过程中一次随机选取一个误分类点使其梯度下降。</li>
	<li>当训练数据集线性可分时，感知机学习算法是收敛的，感知机算法在训练数据集上的误分类次数 $ k$ 满足不等式：
$$
k \leq \left (  \frac{R}{\gamma} \right ) ^2
$$
当训练数据集线性可分时，感知机学习算法存在无穷多个解，其解由于不同的初值或不同的迭代顺序而可能有所不同。</li>
</ol>
感知机最早在 1957 年由 Rosenblatt 提出， Novikoff 、 Minsky 与 Papert 等人对感知机进行了一系列理论研究，感知机的扩展学习方法包包括口袋算法（pocket algorithm）、表层感知机（voted perceptron）、边缘感知机（perceptron with margin）。关于感知机的介绍可进一步参考文献。
