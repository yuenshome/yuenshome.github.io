[![header](../../../assets/header33.jpg)](https://yuenshome.github.io)

# Real-Time Grasp Detection Using Convolutional Neural Networks

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-01.Gif" alt="" width="1000" height="562" />

借助百度脑图（naotu.baidu.com）画出本文大致结构，建议去百度查看高清大图-&gt;<a href="http://naotu.baidu.com/file/7067316d366df78ad1eda60e8dd6ede5?token=e6c255eaedc427ab" target="_blank">点击这里查看</a>。

<a href="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/Real-Time+Grasp+Detection+Using+Convolutional+Neural+Networks.png" target="_blank"><img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/Real-Time+Grasp+Detection+Using+Convolutional+Neural+Networks.png" alt="" width="319" height="206" /></a>
<h1>使用卷积神经网络做实时抓取检测<del></del></h1>
<ul>
	<li>题目：Real-Time Grasp Detection Using Convolutional Neural Networks</li>
	<li>作者：Joseph Redmon1 , Anelia Angelova2</li>
	<li>原文链接：https://pjreddie.com/media/files/papers/grasp_detection_1.pdf</li>
	<li>Slides：https://docs.google.com/presentation/d/1Zc9-iR1eVz-zysinwb7bzLGC2no2ZiaD897_14dGbhw/edit#slide=id.g3b4b0f6b2_0154</li>
</ul>
[toc]<!--more-->
<h2>摘要</h2>
我们提出一种<span style="color: #ff0000;">准确</span>而且可以<span style="color: #ff0000;">达到实时性能</span>要求基于<span style="color: #ff0000;">卷积神经网络</span>的，机器人抓取检测方法。

我们的抓取方法没有使用常见的滑动窗口或者区域选择技术，我们的网络<strong><span style="color: #339966;">只是使用single-stage回归去找到可抓取的包围检测框</span></strong>。模型的表现性能比当前最先进的方法也要高出十四个百分点（我想这里应该说的是准确率）。同时可以在<span style="color: #ff0000;">单张GPU上能达到每秒十三帧图像的性能</span>。我们的网络可以在对物体进行<strong><span style="color: #339966;">分类的同时</span></strong>，也可以做到找出可抓取部位的<strong><span style="color: #339966;">检测</span></strong>矩形框。在这个模型上我们也做了一些修改，使它<strong><span style="color: #339966;">可以对单个物体上预测多个抓取部位</span></strong>，实现的方法是<strong><span style="color: #339966;">基于一个局部受限预测机制</span></strong>，基于该机制的模型表现更好，尤其是物体有多个抓取方式的时候。
<h2>第一部分：介绍</h2>
感知器，是一种传感器，或者也可理解为机器人的一些传感器，通过传感器采集得到的数据来对你当前环境进行理解。

视觉感知可以让像素值以及光亮的信息建模，从而全局的推断出周围的环境。一般的场景理解需要复杂的视觉任务来完成比方说，将当前的这个场景分割成不同的区域部分，进而识别这些部分是什么。同时，在视觉层面上消除相似的物体，也就是视觉二义性。由于该任务异常复杂，所以对于真实环境下建立的机器人系统，视觉感知也是一个巨大的瓶颈。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-02.Gif" alt="" width="405" height="228" />

通用目的的机器人在设计时，我们就希望他有能力可以在这个物理世界中对不同的物体进行交互，也就是说操纵他们，<span style="color: #3366ff;"><strong>人类可以看新物体，可以迅速的理解而且本能地与他们交互，比方人类可以很自然的知道如何去抓取拿起之前没见到的物体。机器人对于检测物体的能力远远落后于人类的表现</strong></span>，<strong><span style="color: #3366ff;">我们专注于给机器人RGB-D的图像数据让机器人可以很好地去抓取事物</span></strong>。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-03.Gif" alt="" width="399" height="224" />

基于康奈尔抓取检测数据集，我们进行算法的评估，同时还有一个更大的有着更多物体和对应抓取标签的数据集。在该数据上的相关（查看第一和第二个参考文献）工作，可以达到75%的准确率，性能表现则是13.5秒/帧。这期间<span style="color: #ff0000;">13.5秒的延迟主要在来自机器人查看该场景并用手去抓住其中的位置</span>。最常用的抓取检测方法是通过滑动窗口在单帧图像进行检测。<strong><span style="color: #339966;">滑动窗口方法，是一种使用分类器来确定图像中的小块是否构成了该图像中的物体潜在可能的抓取部位</span></strong>。

我们采取与以往不同的方法。我们使用一个神经网络在单张图像上<span style="color: #339966;"><strong>预测抓取的坐标</strong></span>，网络相当大，但由于<strong><span style="color: #339966;">对单张图像只应用一次</span></strong>。得到了很好的<span style="color: #ff0000;">性能提升</span>。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/grasp-detection-01.png" alt="" width="330" height="376" /><strong>图1这个是<span style="color: #339966;">康奈尔大学抓取数据集</span>，该数据集包含了各种物体，<span style="color: #339966;">每一个物体都有多个抓取标签。抓取标签形式是以2D矩形方框的形式给出的</span>。</strong>

除了<span style="color: #339966;">考虑局部</span>的图像信息，网络也结合<span style="color: #339966;">考虑全局</span>的信息来<span style="color: #339966;">做抓取位置预测</span>。我们的网络达到了<span style="color: #ff0000;">88%的准确率以及实时性能</span>，也就是说<span style="color: #ff0000;">每秒十三帧图像</span>。我们也重新定义了使用RGB-D实现抓取检测的最新技术。
<h2>第二部分：相关工作</h2>
过去的方法中使用<span style="color: #339966;">三维模拟</span>去寻找好的抓取位置，其中比较重要的工作有第三个到第七个参考文献，这些方法都是非常有用的，但是是<span style="color: #339966;">借助全3d模型以及其他关于当前物体的物理信息来找到一个合适的抓取位置</span>。但这种方法并不适用于所有物体，也就是说要抓取的物体不能作为一个先验。通用目的的机器人也许需要抓取新的物体，也就是说他之前并未见到过的物体，没有建立过该物体的3d模型。

机器人系统越来越多地利用RGB-D传感器等数据进行诸如物体识别[8]，检测[9] [10]和映射[11] [12]等任务。 像Kinect这样的<span style="color: #339966;">RGB-D传感器很便宜</span>，而且与3-D环境交互的机器人的额外<span style="color: #339966;">深度信息是无价的</span>。

抓取检测的最新工作集中在仅从RGB-D数据中获取抓取的问题[13]。 这些技术依靠机器学习来查找特征，从数据中得到好的抓取预测。 抓取视觉模型对于新物体的泛化能力更强，只需单张物体的图像，而不是一个完整的物理模型[14] [2]，就能预测出好的抓取位置。

<span style="color: #339966;"><strong>卷积网络的学习能里很强，作为特征提取器和视觉模型</strong></span>[15] [16]效果很好。 Lenz et al在滑动窗口检测流程中，成功使用卷积网络进行抓取检测作为分类器[1]。 我们也解决了与Lenz等人相同的问题。 但使用不同的网络架构和处理流程，能够以更快的速度获得更高的精度。
<h2>第三部分：问题描述</h2>
通过一张无题的图片我们想要找到如何握住这个物体的方法。我们<strong><span style="color: #339966;">用五个维度的数据表示机器人的抓取</span></strong>，该方法是Lenz et al.提出来的[1]。这种表示方法给出了<span style="color: #339966;"><strong>两组平行的位置和方向信息（一个矩形框）</strong></span>，最终的数据可以绘制成一个封闭的图形，也就是刚好能变成一个矩形。真实地抓取是一个有着某种姿态位置，尺寸，和方向的矩形。可用如下数学符号描述：

$$
g = \{x, y, \theta, h, w\}
$$

其中$x$和$y$是这个矩形的中心，$\theta$是矩形相对于水平方向的角度，$h$则是高度（注：这里应该说的是矩形的长），$w$是矩形宽度，图2展示使用抓取表示的例子。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/grasp-detection-02.png" alt="" width="305" height="203" />

图2五维度的抓取数据表示，其中包含位置，尺寸，方向等信息。蓝色的线条是尺寸，水平线的方向，红色线条的距离是执行抓取时候的宽度。

这是七个维度抓取方法（该方法由Jiang et al提出 ）的一种简化，我们没有使用完整的物体3d抓取的位置和方向信息，而是假设一个机器人查看这个场景的视角，假设二维图像的抓取，可以映射回到三维数据上。第二个参考文献等人描述了实现这一方法的过程，并且与此同时，他们并没有直接评估该方法，不过在他们的实验中，他们的方法表现很好。

使用了五个维度的数据表现形式，可以使这个抓取检测任务与计算机视觉的物体检测变得非常相像。有一点不同，就在于加入了抓取的方向。
<h2>第四部分：使用神经网络进行抓取检测</h2>
卷积神经网络比起传统的或者其他的，计算机视觉的模型来说，有着更好的表现效果，尤其是对于分类或者检测问题。卷积神经网络也已经在抓取检测问题当中得到了很好的应用，它作为滑动窗口过程中的一个分类器。

我们想要使<strong><span style="color: #339966;">用一个小的分类器</span></strong>，<strong><span style="color: #339966;">多次的在同一张图像上的不同的小块图像上面执行</span></strong>，希望这样可以<strong><span style="color: #339966;">避免计算量过大</span></strong>的问题。我们利用大卷积网络的大容量，可以使得在整幅图像上进行<strong><span style="color: #339966;">考虑全局的抓取</span></strong>预测。
<h3>a. 模型架构</h3>
当构建一个抓取检测系统，是我们希望从一个比较鲁莽的原始模型开始搭建，我们使用的模型是<strong><span style="color: #339966;">基于Krizhevsky et al.的物体识别任务的AlexNet卷积网络模型</span></strong>。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/grasp-detection-03.png" alt="" width="653" height="332" />

我们的网络有<span style="color: #339966;"><strong>五个卷积层</strong></span>，<strong><span style="color: #339966;">紧随其后的是三个全连接层</span></strong>，卷积层的不同层中使用<span style="color: #339966;">normalization</span>技术，以及<span style="color: #339966;">maxpooling</span>，关于这个架构的完整描述可以<span style="color: #339966;">在图3</span>中找到。
<h3>b. 直接回归抓取（Direct regression to grasps）</h3>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-04.Gif" alt="" width="405" height="227" />

我们发现的<span style="color: #339966;">最简单模型，是基于对原始的RGBD图像数据直接回归去做坐标抓取</span>。原始图像给卷积层后，卷积层会对原始图像进行特征提取。之后是<span style="color: #339966;">六个神经元的输出（其中角度信息占2个，所以是6个输出）</span>分别对应抓取的坐标信息等等，其中四个神经元对应着位置和高度信息，抓取的角度信息是双重旋转对称性，所以我们通过两个附加的坐标对角度参数化，也就是两倍角度的正弦和余弦。(Grasp angles are two-fold rotationally symmetric so we parameterize by using the two additional coordinates: the sine and cosine of twice the angle.)

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-05.Gif" alt="" width="493" height="276" />

这个<span style="color: #339966;">模型有一个很强的假设，那就是每张图像只包含一个可以抓取的物体，并且对于那个物体，只需要预测一个抓取</span>。这一较强的假设不能考虑到实验条件以外的情况，但是在实际中模型将会以一种流水线的形式执行，<span style="color: #339966;">第一步对图像进行分割成不同的小块区域</span>，这些小块区域包含了不同的物体。使用这一强假设的好处是<span style="color: #339966;">不需要使用传统的滑动窗口的方法</span>——对每一个小图像进行分类。我们<span style="color: #339966;">只需要让模型观察一次图像，并做一个全局的预测就可以了</span>。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-06.Gif" alt="" width="542" height="305" />

在模型在训练中，对于看到的物体，<span style="color: #ff0000;">每一次只随机选择该物体所有可抓取中的一次真实地抓取信息进行训练</span>。因为抓取也是经常会发生变动的，这样模型就不会在一个物体上过拟合（注：<span style="color: #ff0000;">相当于一种防止过拟合的策略，或者可以称为正则策略</span>）。我们对物体抓取的平方误差进行最小化，最终的效果也就是<span style="color: #ff0000;">模型对一个物体所有可能抓取的平均情况的一种拟合</span>。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-07.Gif" alt="" width="440" height="248" />

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-09.Gif" alt="" width="440" height="247" />
<p style="text-align: center;">然后直接回归的平均抓取在圆的物体上就表现不好了</p>

<h3>c. 回归+分类</h3>
机器人要进行抓取物体的动作，第一步就需要对物体进行辨识。通过对我们模型的扩展，可以对识别和抓取检测的动作流程化。

我们<span style="color: #339966;">将上节（直接回归模型）中所提到的模型架构进行修改，在输出层加入了额外的神经元，新加入的神经元对应着物体的类别</span>。剩下的架构与我们先前的模型保持一致，这样我们就可以使用与前文中所述一样的架构，<span style="color: #339966;">既可以识别也可以检测</span>。

结合之后的模型在处理单张图片时，只需要一次正向传播，就可以预测出图像中物体的类别，以及对于该物体的一次抓取动作。因为模型的<span style="color: #339966;">架构与前文中模型的架构并没有太大的改变</span>， <span style="color: #339966;">模型执行速度与直接回归模型相同</span>。
<h3>d. 多抓取检测, MultiGrasp Detection</h3>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-15.Gif" alt="" width="472" height="266" />
<p style="text-align: center;">直接回归相当于在NxN的path上预测一次</p>
我们的<span style="color: #339966;">第三种模型是对第一种模型的一种泛化</span>，之前的模型都是假设对每张图像只有一个正确的抓取，并且预测那个抓取。而多抓取检测模型是<span style="color: #339966;">将图像分成了NxN的网格</span>，并且假设<span style="color: #339966;">每一个网格内预测一个抓取（<strong><span style="color: #ff0000;">我的疑问：如何确定N的大小，后来想了想根据单张图像的ground truth label可以做到，但是作者并没有说这个N是固定的还是根据label的信息动态的调整的</span></strong>）</span>，并且预测物体上抓取是否可行，<span style="color: #339966;">即预测抓取在网格中间的可能性</span>。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-16.Gif" alt="" width="465" height="262" />
<p style="text-align: center;">加入额外输出（热力图，即抓取概率图）的网络</p>
模型的<span style="color: #339966;">输出结果是一个NxNx7的形式</span>。<span style="color: #339966;">第一个通道是一个热力图，这个热力图描述这个区域是否包含了一个正确抓取的可能</span>。另外<span style="color: #339966;">六通道包含了这次预测的抓取的坐标等具体信息</span>。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-17.Gif" alt="" width="453" height="255" />
<p style="text-align: center;">对于热力图的误差反向传播</p>
在康奈尔数据集上，我们的实验中使用了7×7的网格，输出层的形式是7×7×7（343个神经元）。<span style="color: #339966;"><strong>第一个模型（直接回归模型）可以被视为一个网格尺寸是1×1的例子</strong></span>，<span style="color: #339966;"><strong>各单元内抓取存在的概率是1</strong></span>。(Our first model can be seen as a specific case of this model with a grid size of 1x1 where the probability of the grasp existing in the single cell is implicitly one.)

<strong><span style="color: #339966;">训练多抓取模型需要一些额外考虑</span></strong>，<span style="color: #339966;">每一次多次抓取需要观察一张图像的随机五个抓取作为真实抓取</span>。<span style="color: #339966;">热力图的构建是在这5个小格子内标注成1，剩下的格子填补0</span>，此外也需要计算抓取会落在哪些格子，并且把真实抓取的其余具体数据信息填入适当的列中。因为大多数列都是空白的（即网格内没有真实抓取），所以即使有误差对应的7×7×7格子在训练过程也不会进行<span style="color: #339966;">数值上</span>的反向传播。反而对于<span style="color: #339966;">整个热力图通道以及含有<strong>真实抓取的格子中，才进行误差的回传</strong></span>。

我们提出的物体检测模型是基于前人的一些工作的，但在一些重要的层面来说，这个模型也是新颖的。Szegedy et al在一张图像上使用深度神经网络，预测二进制的物体的掩码，并用预测出的掩码生成边界框[17]。我们预测出的热力图和物体掩码的方法非常相似，但是我们也预测完整的边界框，并用热力图去平衡预测的结果(only use the heatmap for weighting our predictions)。我们的整个系统不借助于后端处理或者用启发式的方法去得到边界框，而是直接预测。

Erhan et al提出的多边界框预测以及对应打分[18]，该方法和我们的方法非常相似，我们也是预测多个边界框和对应分数来，<span style="color: #339966;">最重要的不同在于我们的预测结构——每个单元格只对其图像局部区域预测</span>。
<h2>Ⅴ.实验和评估</h2>
<span style="color: #339966;">康奈尔抓取数据集包含了240种不同的物体，共计885张图片</span>。它们都被标记了真实的抓取标记，<span style="color: #339966;">每一张图片都包含多个抓取标记</span>，对应抓取该物体的不同方式，这个数据集是专门为机器人的平行板夹机器手设计的。这些标记在方向、位置和抓取规模方面都是全面且多样的，但并不是详尽无遗的。只是包含了不同样式的恰当合适的抓取。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/grasp-detection-04.png" alt="" width="467" height="203" />

图4. 多抓取模型在测试图像（飞碟）上的可视化。多抓取模型将一幅完整的图像切分成NxN的网格，对于每一个网格，模型在网格的中心预测是否有抓取包围边界框的概率。可视化时是以该边界框的粗细来表示它的概率。例如，模型选择概率最高的值的边界框作为最终抓取。
<h3>A. 抓取检测</h3>
之前的工作是<span style="color: #339966;">用两种不同的方法</span>在康奈尔数据集上进行抓取工作的评估。<strong><span style="color: #339966;">点测量</span></strong>的评估方法是在计算距离，<span style="color: #339966;">计算预测的抓取的中心距离真实抓取的中心的距离</span>，如<span style="color: #339966;">果这些距离中任意一个小于某一个阈值，那么这次抓取就被被视为一次成功抓取</span>。

然而对于这种测量方法也有很多的问题。因为它<span style="color: #339966;">没有考虑到抓取的角度或者大小</span>，进一步说之前的工作也并没有说明采取的阈值是多少，这也是为什么我们没有采用这种点来测量进行评估的方法。

<strong><span style="color: #339966;">第二种测量方法考虑的是一个完整的抓取矩形</span></strong>，一次成功抓取需要保证以下是正确的：
<ol>
	<li>预测抓取角度与真实抓取的角度在30度以内</li>
	<li>预测抓取比真实抓取的Jaccord index大25%</li>
</ol>
其中，计算公式如下：

$$
J(A,B) = \frac{|A \cap B|}{|A \cup B|}
$$

矩形测量方法在对于好的和不好的抓取时，它的效果要比点测量的估计方法要更好一些，这种测量方法相比相似的物体检测的评估方法，设定的Jaccord index是25%，还是比较低的。这里的25%，并不是计算机视觉中更标准常见的50%。这也是因为真实的抓取并不是非常详尽的缘故导致的。<strong><span style="color: #339966;">如果预测的矩形框与实际正确抓取的方向有25%的重叠，那么我们也认为这次预测的抓取仍是一个好的抓取</span></strong>结果。我们在所有的实验中都使用了矩形测量评估的方法。

与之前的工作一样，我们在实验的结果计算上也使用了<span style="color: #339966;">五折交叉验证</span>集。我们使用两种不同的方法对数据进行split。
<ol>
	<li>逐图片split，将图片随机split。</li>
	<li>逐个物体split，对一个问题事例随机分开，将所有图像中相同的物体分到相同的交叉验证集的split中。</li>
</ol>
逐图像的split在测试的时候可以验证模型的泛化能力（对于<span style="color: #339966;">见过的物体但不同角度的时候）；</span>逐个物体的split在做交叉验证可以更进一步地测试网络对于<strong><span style="color: #339966;">新物体的泛化能力</span></strong>。在实际应用中，上述的两种方法具有一定的等价性，可能是由于数据集中的一些物体具有一定的相似性。（例如不同的太阳镜在形状和颜色上也只是稍许不同）
<h3>B. 物体分类</h3>
我们<strong><span style="color: #339966;">人工地对康奈尔抓取数据集的图像分成了十六种不同的类别</span></strong>，比方说有瓶子，鞋子，运动器械等等。康奈尔原始数据集中<span style="color: #339966;"><strong>各种类的图像张数的分布差距很大</strong></span>，即每类样本数量并不是相同的，但是每一种类别的图像都有足够的数目表示本身的含义。图像张数最小的那个种类只有20张，但最多图像的种类有156张。

我们使用这些类别标记训练并测试了结合了回归和分类的模型。在测试时间上，结合后的回归分类模型可以预测最佳抓取的同时预测物体种类。我们在上述相同的交叉验证的分割方法中，已经报告了分类的准确率。
<h3>C. 预训练</h3>
在抓取数据集上训练我们的网络之前，我们先在ImageNet的分类任务上进行了预先的训练[20]。现有文献中的经验表明，通过对大的卷积神经网络进行<span style="color: #339966;"><strong>预训练</strong></span>，可以大大<strong><span style="color: #339966;">减少训练时间</span></strong>，<strong><span style="color: #339966;">有助于避免过拟合</span></strong>[21][22]。Krizevsky et al.设计了<span style="color: #339966;"><strong>针对标准RGB图像的AlexNet</strong></span>，像Kinect这样的低成本立体视觉系统可以使RGB-D数据在机器人系统中不断增加。但是如果要<strong><span style="color: #339966;">使用AlexNet和RGB-D数据</span></strong>，我们只需要<strong><span style="color: #339966;">用深度信息替换图像中原本的蓝色通道</span></strong>，这样修改后的架构可以<strong><span style="color: #339966;">具有另一个输入通道，但是无法预训练完整的网络</span></strong>。

当<span style="color: #339966;">领域数据很限时</span>，<span style="color: #339966;">预训练</span>是非常<span style="color: #339966;">重要</span>的。通过对模型进行<span style="color: #339966;">预先训练</span>，可以<span style="color: #339966;">使网络找到更有用，更具泛化能力的滤波器</span>，这样可以更好的将原始数据转化为特定应用[22]。<strong><span style="color: #339966;">即使在数据格式和实际的输入有变化的情况下，仍然发现预训练的滤波器的性能还不错</span></strong>，这可能是因为训练过的<span style="color: #00ccff;"><strong>视觉滤波器也是深度空间中好的滤波器</strong></span>。
<h3>D. 训练</h3>
我们针对每个城市的模型进行类似的训练方案。对于交叉验证的每个折叠，每个模型都是用<span style="color: #339966;">25个epoch，学习率0.0005，权重衰减0.001，在全链接层的隐含层中，使用0.5的dropout概率</span>作为正则化的附加形式。

在对模型进行<span style="color: #339966;">训练和测试</span>时候，我们使用<span style="color: #339966;">Tesla K20显卡和cuda-convet2包</span>计算。GPU提供了强大的计算能力，其运算到时间也是我们整个流程的一部分。虽然这GPU在机器人平台方面不是主流，但由于其在视觉任务中的实用性，它越来越受欢迎。
<h3>E. 数据预处理</h3>
在数据给网络之前，我们需要对数据进行少量的预处理操作，如前文中所述<span style="color: #339966;">深度信息，被放入了图像中的蓝色通道</span>，深<span style="color: #339966;">度信息同时也被重新映射在0到255</span>之间，由于一些像素缺少深度信息，他们被遮挡在立体图像中，所以用0来代替这些像素值，之后再对每张<span style="color: #339966;">图像减去像素平均中心(approximately mean-center the image)144</span>。

在这之后我们对数据平移和旋转，从而扩增图像数据。我们取出原始图像中心320x320像素大小的区域。在$x$和$y$方向上，将其旋转50个像素，并再旋转一个随机像素数。之后把图像重新缩放成224×224的大小，这样就可以适用于网络的输入层。<span style="color: #339966;">对于原始图像，我们生成了三千个训练样本（比例是1：3000）</span>，在测试图像上，我们只是简单取出原始图像中心的320×320区域，并缩放到网络的输入尺寸，没有平移和旋转操作。
<h2>Ⅵ. 结果</h2>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/grasp-detection-t-01.png" alt="" width="463" height="181" />

我们的模型无论是在准确性还是速度方面，是目前最先进的。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/grasp-detection-05.png" alt="" width="316" height="374" />

图5 直接回归模型的正确（顶部）和不正确（底部）示例。一些不正确的抓取（例如罐头开瓶器）可能实际上是可行的，而其他的（例如碗）显然是不可行的。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-06.Gif" alt="" width="501" height="282" />

我们将结果和以前的工作使用矩形度量法得到的准确率相比较。<span style="color: #ff0000;">直接回归模型</span>在抓取检测任务中，得到了一个新的基准线，在逐个图像和逐个物体的交叉验证上达到了大约<span style="color: #ff0000;">85%的准确率</span>，比之前版本的最高准确率要高出十个百分点。在测试时，也就是计算一次正向的直接回归模型，对于每一个批次的处理在<span style="color: #ff0000;">76毫秒</span>，每一个批次（batchsize）有128张图像。也就是说，每秒钟会处理超过1600张的图像，延迟在整个抓取检测的过程所占用的时间非常的多，以至于每一秒钟只能处理13帧（每次使用1张图像和128张所需要的时间相同）。得到加速的主要是由原本的扫描滑动窗口的分类模型，转换为我们的单次传播模型，并且使用GPU硬件进行计算加速。对于在GPU上的一个完整的批次计算，这里只需要原本1/128的浮点计算操作，CPU无疑至少需要76毫秒（这里没太理解这句话，76 milliseconds per frame is certainly achievable on a CPU because it would require only 1/128th of the ﬂoating point operations required for processing a full batch on a GPU）。

在<span style="color: #ff0000;">直接回归模型的训练</span>过程当中，我们每一次针对每张图像都是用<span style="color: #ff0000;">随机的真实抓取</span>，而<span style="color: #ff0000;">不是该图像的所有真实抓取</span>。因为这个缘故，所以<span style="color: #ff0000;">模型学到的预测是物体的一个平均真实抓取</span>。<span style="color: #ff0000;">对于某些种类的物体预测平均抓取非常有效</span>，例如，形状上比较长且细的物体如马克笔。<span style="color: #ff0000;">该模型大多数失败的主要原因是平均抓取并不能转换成一种切实可行的抓取</span>，例如一些圆形的物体，如飞碟。图5展示了一些正确和不正确的抓取，这些都是使用直接回归模型进行预测的。
<p style="text-align: justify;">结合了回归和分类的模型展示了在检测模型的基础之上，还可在不牺牲检测精度的同时进行分类；表2是该模型的分类结果。当我们的模型已经<span style="color: #ff0000;">看到该物体9次或10次时，他就可以正确地预测出物体的</span>类别，但是当给出一个<span style="color: #ff0000;">新的物体时，准确率只有60%</span>。相比最常见的类别占总数据集的比例来说，用占比最多的类别进行预测，准确率有17.7%。</p>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/grasp-detection-t-02.png" alt="" width="389" height="111" />
<p style="text-align: center;">表2. 在康奈尔数据集上的分类准确率</p>
即使加入了分类任务结合之后的模型，仍然保持高检测准确率。之前逐对象的交叉验证集上有同样的性能，比之前逐图像交叉验证事后的性能还要稍微好一些，这个结合了抓取检测和物体分类后的模型，在康奈尔数据集上建立了更好的性能基准线。

<span style="color: #ff0000;">多抓取模型要比直接回归模型的基准线要好一大截</span>。多抓取模型在对于大多数物体与直接回归模型得到的结果非常相似，然而<span style="color: #ff0000;">多抓取模型并没有直接回归模型的较差的平均抓取的问题</span>（<span style="color: #339966;">这也是为什么多抓取模型能够减少大多数错误的原因</span>）。图6展示了多抓取模型比直接回归模型要好的几个例子，以及在这两个模型上都出现失败的例子。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/grasp-detection-06.png" alt="" width="274" height="373" />
<p style="text-align: left;">图6 直接回归模型和MultiGrasp的性能比较。前两行显示了直接回归模型由于平均效应而失败的示例，但MultiGrasp预测了可行的掌握。底部的两行显示了两个模型都无法预测好抓握的示例。直接回归模型图像上的蓝色和红色展示出真实的抓握。</p>
<span style="color: #339966;">多次抓取模型与直接回归模型有相似的架构，以及相似的实时处理速度</span>。多抓取模型在机器人抓取检测方面保持世界领先，检测<span style="color: #ff0000;">准确率为88%，性能为13帧/秒</span>。
<h2>Ⅶ. 讨论</h2>
我们提出了在保证速度和高准确率下的模型。计算显卡提供了强大的计算能力，尤其是基于卷积神经网络的。在视觉任务上，卷积神经网络一直在统治着相关的技术。这也是计算显卡成为任何高性能机器人系统的一个重要组件的原因。计算显卡在模型训练中非常重要以及整个过程中都有优化，然而却有着很大的延迟。在测试时，CPU可以少于一秒的处理单张图像，这可以使机器人应用实时化。

从模型层面上考虑对实现高性能来说是非常重要。为了图像在模型中单次上处理就可以完成准确的抓取预测，我们对数据进行强约束。

<span style="color: #339966;">与滑动窗口的方法不同，直接回归模型使用了图像上的全局信息进行预测。滑动窗口分类器只能看到较小的局部的图像，因此他们不能在一堆候选抓取结果中做出有效的决定，而且很容易被假阳性的样本所迷惑</span>。Lenz et al.报告他们的分类器有很高的识别率，94%，然而对于<span style="color: #ff0000;">假阳性样本来说，其检测精度要低很多</span>，在这一方面与只能看到局部信息的模型相比，考虑全局的模型具有很大的优势。

考虑全局的模型也有其缺点，值得注意的是，直接回归模型通常会在几个好的抓取之间进行差异化（注：这里我理解不是差异化，而是平均化），很最后预测出不正确的抓取，然<span style="color: #339966;">而滑动窗口方法永远不会造成在圆形物体上预测中心位置的错误抓取</span>，比方飞盘。
<p style="text-align: center;"><img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/gdf-ppt-26.Gif" alt="" width="574" height="322" />性能比较</p>
我们的多抓取模型结合了只考虑全局和局部信息这两种模型的优势。他会观察整幅图像并找到最佳抓取，并忽略假阳性样本，然而因为图像上每个单元格会做局部预测，这也可以避免陷入不好抓取的预测陷阱。

考虑局部的模型也有在单幅图像上预测多个抓取的能力。因为目前没有哪些数据集可以对单张图像预测多抓取的合适评估，所以我们没能给出一种定量评估该模型的方法。

另一个重点考虑是在于构建大规模卷积神经网络时，预训练的重要性。如果没有在ImageNet数据集上进行预训练，模型很快的会在训练数据集上过拟合，更不会学习出好的抓取的特征表示。有趣的是，<span style="color: #00ccff;">预训练在不同的领域以及特征类型上都有效果</span>。我们对原本的模型中的蓝色通道，用深度信息将其替换，仍然得到了不错的结果。重要的是，使用深度信息的通道比起原本只用三种颜色的图像通道来说，得到了更好的结果。
<h2>Ⅷ. 总结</h2>
我们提出了一种快速，高准确的系统，用来预测机器人处理RGB-D的图像数据进行物体抓取。模型达到世界领先水平，并比先前的方法提高了150倍。我们也展现了在不损失精度和性能的前提下，可以将抓取检测和物体分类的模型进行结合。多抓取模型在康奈尔抓取数据集上，通过结合全局的信息，可以进行局部预测，并达到当前已知的最佳性能。
