[![header](../../../assets/header39.jpg)](https://yuenshome.github.io)

# 看得“深”、看得“清” —— 深度学习在图像超清化的应用（未完待续）

转载的一篇文章。下面是针对这篇文章的百度脑图：

&nbsp;
<ul>
	<li>作者：张延祥，就职于Google北京输入法团队。北航本硕，CSDN知名博主，有多篇CSDN博文流传甚广，对深度学习、自然语言处理和计算机视觉有极大的热忱。</li>
	<li>责编：王艺，CSDN AI栏目编辑/记者，目前从事【AI创新者】系列人物访谈，合作及投稿请联系wangyi@csdn.net</li>
</ul>
[toc]

<!--more-->

日复一日的人像临摹练习使得画家能够仅凭几个关键特征画出完整的人脸。同样地，我们希望机器能够通过低清图像有限的图像信息，推断出图像对应的高清细节，这就需要算法能够像画家一样“理解”图像内容。至此，传统的规则算法不堪重负，新兴的深度学习照耀着图像超清化的星空。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/super-resolution-01.png" alt="" width="394" height="420" />
<p style="text-align: center;"><strong>图1. 最新的Pixel递归网络在图像超清化上的应用。左图为低清图像，右图为其对应的高清图像，中间为算法生成结果。这是4倍超清问题，即将边长扩大为原来的4倍。</strong></p>
得益于硬件的迅猛发展，短短几年间，手机已更新了数代，<span style="color: #ff0000;">老手机拍下的照片在大分辨率的屏幕上变得模糊起来</span>。同样地，<span style="color: #ff0000;">图像分辨率的提升使得网络带宽的压力骤增</span>。如此，图像超清化算法就有了用武之地。

对于存放多年的老照片，我们使用超清算法令其细节栩栩如生；面对网络传输的带宽压力，我们先将图像压缩传输，再用超清化算法复原，这样可以大大减少传输数据量。

<span style="color: #3366ff;">传统的几何手段如三次插值</span>，<span style="color: #3366ff;">传统的匹配手段如碎片匹配</span>，在应对这样的需求上皆有心无力。

深度学习的出现使得算法对图像的语义级操作成为可能。本文即是介绍深度学习技术在图像超清化问题上的最新研究进展。

深度学习最早兴起于图像，其主要处理图像的技术是卷积神经网络，关于卷积神经网络的起源，业界公认是Alex在2012年的ImageNet比赛中的煌煌表现。虽方五年，却已是老生常谈。因此卷积神经网络的基础细节本文不再赘述。在下文中，使用CNN（Convolutional Neural Network）来指代卷积神经网络。

<span style="color: #339966;">CNN出现</span>以来，催生了很多研究热点，其中最令人印象深刻的<span style="color: #339966;">五个热点</span>是：
<ul>
	<li><span style="color: #339966;"><strong>深广探索</strong></span>：VGG网络的出现标志着CNN在搜索的深度和广度上有了初步的突破。</li>
	<li><span style="color: #339966;"><strong>结构探索</strong></span>：Inception及其变种的出现进一步增加了模型的深度。而ResNet的出现则使得深度学习的深度变得“名副其实”起来，可以达到上百层甚至上千层。</li>
	<li><strong><span style="color: #339966;">内容损失</span></strong>：图像风格转换是CNN在应用层面的一个小高峰，涌现了一批以Prisma为首的小型创业公司。但图像风格转换在技术上的真正贡献却是通过一个预训练好的模型上的特征图，在语义层面生成图像。</li>
	<li><strong><span style="color: #339966;">对抗神经网络</span></strong>（GAN）：虽然GAN是针对机器学习领域的架构创新，但其最初的应用却是在CNN上。通过对抗训练，使得生成模型能够借用监督学习的东风进行提升，将生成模型的质量提升了一个级别。</li>
	<li><strong><span style="color: #339966;">Pixel CNN</span></strong>：将依赖关系引入到像素之间，是CNN模型结构方法的一次比较大的创新，用于生成图像，效果最佳，但有失效率</li>
</ul>
。 这五个热点，在图像超清这个问题上都有所体现。下面会一一为大家道来。
<h1>CNN的第一次出手</h1>
<div> <img class="alignnone" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/super-resolution-02.png" alt="" width="863" height="312" /></div>
&nbsp;
<p style="text-align: center;"><strong>图2. 首个应用于图像超清问题的CNN网络结构.</strong></p>
<span style="color: #3366ff;"><strong>输入为低清图像，输出为高清图像.该结构分为三个步骤：低清图像的特征抽取、低清特征到高清特征的映射、高清图像的重建。</strong></span>

图像<span style="color: #3366ff;">超清问题的特点</span>在于，<span style="color: #3366ff;">低清图像和高清图像中很大部分的信息是共享的</span>，基于这个前提，在<span style="color: #3366ff;">CNN出现之前，业界的解决方案</span>是使用一些<span style="color: #3366ff;">特定的方法</span>，如<span style="color: #3366ff;">PCA、Sparse Coding等将低分辨率和高分辨率图像变为特征表示</span>，<span style="color: #3366ff;">然后将特征表示做映射</span>。

<span style="color: #3366ff;">基于传统的方法结构，CNN也将模型划分为三个部分</span>，即<span style="color: #3366ff;">特征抽取、非线性映射和特征重建</span>。由于CNN的特性，<span style="color: #3366ff;">三个部分的操作均可使用卷积完成</span>。因而，虽然针对模型结构的<span style="color: #ff0000;">解释与传统方法类似</span>，但CNN却是可以同时<span style="color: #ff0000;">联合训练</span>的<span style="color: #ff0000;">统一体</span>，在<span style="color: #ff0000;">数学上拥有更加简单的表达</span>。

不仅在模型解释上可以看到传统方法的影子，在具体的操作上也可以看到。在上述<span style="color: #3366ff;">模型中，需要对数据</span>进行<span style="color: #3366ff;">预处理</span>，<span style="color: #3366ff;">抽取出很多patch</span>，这些<span style="color: #3366ff;">patch可能互有重叠</span>，将这些<span style="color: #3366ff;">Patch取合集便是整张图像</span>。<strong><span style="color: #3366ff;">上述的CNN结构是被应用在这些Patch</span></strong>而不是整张图像上，<span style="color: #3366ff;">得到所有图像的patch后</span>，<span style="color: #3366ff;">将这些patch组合起来得到最后的高清图像</span>，<strong><span style="color: #3366ff;">重叠部分取均值</span></strong>。
<h1>更深更快更准的CNN</h1>
<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/super-resolution-03.png" alt="" width="457" height="207" />
<p style="text-align: center;"><strong>图3. 基于残差的深度CNN结构</strong></p>
该结构使<strong><span style="color: #3366ff;">用残差连接将低清图像与CNN的输出相加得到高清图像</span></strong>，即<strong><span style="color: #ff0000;">仅</span><span style="color: #3366ff;">用CNN结构学习低清图像中缺乏的高清细节部分</span></strong>。图2中（即<span style="color: #ff0000;">直接使用CNN</span>）的方法虽然效果远高于传统方法，但是却<span style="color: #ff0000;">有若干问题</span>：
<ul>
	<li><span style="color: #ff0000;">训练层数少</span>，没有足够的视野域；</li>
	<li><span style="color: #ff0000;">训练太慢</span>，导致没有在深层网络上得到好的效果；</li>
	<li><span style="color: #ff0000;">不能支持多种倍数的高清化</span>。</li>
</ul>
针对上述问题，图3（基于<span style="color: #3366ff;">残差的深度CNN结构</span>）算法提出了<span style="color: #3366ff;">采用更深</span>的网络模型。并<span style="color: #3366ff;">用三种技术解决了图2（直接使用CNN）算法的问题</span>。
<ol>
	<li>第一种技术是<span style="color: #3366ff;">残差学习</span>，CNN是端到端的学习，如果像图2（<span style="color: #3366ff;">直接使用CNN</span>）方法那样直接学习，那么CNN<span style="color: #3366ff;">需要保存图像的所有信息</span>，需要在<span style="color: #3366ff;">恢复高清细节的同时记住所有的低分辨率图像的信息</span>。如此，网络中的<span style="color: #3366ff;">每一层</span>都需<span style="color: #3366ff;">要存储所有的图像信息</span>，这就<span style="color: #3366ff;">导致</span>了<span style="color: #3366ff;">信息过载</span>，使得<span style="color: #3366ff;">网络对梯度十分敏感</span>，容易造成<span style="color: #3366ff;">梯度消失</span>或<span style="color: #3366ff;">梯度爆炸</span>等现象。而<strong><span style="color: #3366ff;">图像超清问题中，CNN的输入图像和输出图像中的信息很大一部分是共享的</span></strong>。<strong><span style="color: #3366ff;">残差学习是只针对图像高清细节</span></strong>信息进行<strong><span style="color: #3366ff;">学习</span></strong>的算法。如上图所示，CNN的输出加上原始的低分辨率图像得到高分辨率图像，即<strong><span style="color: #ff0000;">CNN学习到的是高分辨率图像和低分辨率图像的差</span></strong>。如此，<strong><span style="color: #ff0000;">CNN承载的信息量小</span></strong>，<span style="color: #ff0000;">更容易收敛</span>的同时还<span style="color: #ff0000;">可以达到比非残差网络更好的效果。</span>高清图像之所以能够和低清图像做加减法，是因为，在数据预处理时，将低清图像使用插值法缩放到与高清图像同等大小。于是虽然图像被称之为低清，但其实图像大小与高清图像是一致的。</li>
	<li>第二种技术是<span style="color: #3366ff;">高学习率</span>，在<span style="color: #3366ff;">CNN中设置高学习率通常会导致梯度爆炸</span>，因而<span style="color: #3366ff;">在使用高学习率的同时还使用了自适应梯度截断</span>。截断区间为[-θ/γ, θ/γ]，其中γ为当前学习率，θ是常数。</li>
	<li>第三种技术是<span style="color: #3366ff;">数据混合</span>，最理想化的算法是为每一种倍数分别训练一个模型，但这样极为消耗资源。因而，同之前的算法不同，本技术<span style="color: #3366ff;">将不同倍数的数据集混合在一起训练得到一个模型</span>，从而支持多种倍数的高清化。</li>
</ol>
<h1>感知损失</h1>
<span style="color: #3366ff;">在此之前</span>，使用CNN来解决高清问题时，<span style="color: #3366ff;">对图像高清化的评价方式是将CNN生成模型产生的图像和实际图像以像素为单位计算损失函数</span>（一般为<span style="color: #3366ff;">欧式距离</span>）。<span style="color: #3366ff;">此损失函数</span>得到的模型捕捉到的<span style="color: #3366ff;">只是像素级别的规律</span>，其<span style="color: #3366ff;">泛化能力</span>相对较<span style="color: #3366ff;">弱</span>。

而<span style="color: #00ccff;"><strong>感知损失</strong></span>，则是指<span style="color: #00ccff;">将CNN生成模型和实际图像都输入到某个训练好的网络中，得到这两张图像在该训练好的网络上某几层的激活值，在激活值上计算损失函数</span>。

由于<span style="color: #3366ff;">CNN能够提取高级特征</span>，那么<span style="color: #3366ff;">基于感知损失的模型</span>能够<span style="color: #3366ff;">学习到更鲁棒</span>更令人信服的<span style="color: #3366ff;">结果</span>。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/super-resolution-04.png" alt="" width="556" height="192" />
<p style="text-align: center;"><strong>图4. <span style="color: #3366ff;">基于感知损失的图像风格转换网络</span>。该网络也可用于图像超清问题。左侧是一个待训练的转换网络，用于对图像进行操作；右侧是一个已训练好的网络，将使用其中的几层计算损失。图4即为感知损失网络，该网络本是用于快速图像风格转换。在这个结构中，需要训练左侧的Transform网络来生成图像，将生成的图像Y和内容图像与风格图像共同输入进右侧已经训练好的VGG网络中得到损失值。如果<span style="color: #3366ff;">去掉风格图像</span>，<span style="color: #3366ff;">将内容图像变为高清图像，将输入改为低清图像，那么这个网络就可以用于解决图像超清问题了</span>。</strong></p>

<h1>对抗神经网络（GAN）</h1>
<img class="alignnone aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/super-resolution-05.png" alt="" width="508" height="269" />
<p style="text-align: center;"><strong>图5. 对抗训练的生成网络G和判别网络结构D。</strong>
<strong> 上半部分是生成网络G，层次很深且使用了residual block和skip-connection结构；</strong>
<strong> 下半部分是判别网络D。</strong></p>
<span style="color: #3366ff;"><strong>对抗生成网络</strong></span>称得上是近期机器学习领域最大的变革成果。其<strong><span style="color: #3366ff;">主要思想是训练两个模型生成器G和判别器D</span></strong>。G是生成网络而D是分类网络，<span style="color: #3366ff;">G和D都用D的分类准确率来进行训练</span>。<span style="color: #3366ff;">G用于某种生成任务</span>，比如图像超清化或图像修复等。<span style="color: #3366ff;">G生成图像后，将生成图像和真实图像放到D中进行分类</span>。<strong><span style="color: #00ccff;">使用对抗神经网络训练模型是一个追求平衡的过程</span></strong>：<span style="color: #00ccff;"><strong>保持G不变，训练D使分类准确率提升；保持D不变，训练G使分类准确率下降，直到平衡</strong></span>。GAN框架使得无监督的生成任务能够利用到监督学习的优势来进行提升。

基于GAN框架，只要定义好生成网络和分类网络，就可以完成某种生成任务。

而<span style="color: #3366ff;">将GAN应用到图像高清问题的这篇论文</span>，可以说是集大成之作。生成模型<span style="color: #3366ff;">层次深</span>且使<span style="color: #3366ff;">用了residual block和skip-connection</span>；在<span style="color: #3366ff;">GAN的损失函数的基础上同时添加了感知损失</span>。

GAN的生成网络和分类网络如图5，其中，生成网络自己也可以是一个单独的图像超清算法。论文中分析了GAN和non-GAN的不同，<span style="color: #3366ff;">发现GAN主要在细节方面起作用</span>，但无法更加深入地解释。“无法解释性”也是GAN目前的缺点之一。
<h1>像素递归网络(Pixel CNN)</h1>
图5中的GAN虽然能够达到比较好的效果，但是由于<span style="color: #ff0000;">可解释性差，难免有套用之嫌</span>。

其实，对于<span style="color: #00ccff;">图像超清这个问题</span>来说，存在一个关键性的问题，即<span style="color: #00ccff;">一张低清图像可能对应着多张高清图像</span>，那么问题来了。

假如我们把<span style="color: #00ccff;">低分辨率图像中需要高清化的部分分成A,B,C,D等几个部分</span>，那么<span style="color: #00ccff;">A可能对应A1,A2,A3,A4，B对应B1,B2,B3,B4</span>，以此类推。<span style="color: #00ccff;">假设A1,B1,C1,D1对应一张完美的高清图片</span>。<span style="color: #00ccff;">那么现有的算法可能生成的是A1,B2,C3,D4这样的混搭，从而导致生成的高清图像模糊</span>。

为了验证上述问题的存在，设想一种极端情况。

<img class="aligncenter" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/super-resolution-06.png" alt="" width="339" height="308" />
<p style="text-align: center;"><strong>图6. 图像超清模糊性问题分析图示。上半部分为分析问题所用数据集的构建。下半部分为现有的损失函数在这个问题上的效果。可以通过对比看出，PixelCNN能够防止这种模糊的出现。</strong></p>
为了分析图像模糊问题的成因，在图6的上半部分，基于MNist数据集生成一个新的数据集。生成方法如下：<span style="color: #3366ff;">将MNIST数据集中的图片A长宽各扩大两倍，每张图片可以生成两张图片A1和A2，A1中A处于右下角，A2中A处于左上角</span>。

把原图当做低清图片，生成的图当成高清图片。使用图6下半部分所列举的三种方法进行训练，得到的模型，在生成图像的时候，会产生图6下半部分的结果。即每个像素点可能等概率地投射到左上部分和右下部分，从而导致生成的图片是错误的。而引入PixelCNN后，由于像素之间产生了依赖关系，很好地避免了这种情况的发生。

为了解决上述问题，需要在生成图像的同时引入先验知识。画家在拥有了人脸的知识之后，就可以画出令人信服的高清细节。类比到图像超清问题中，先验知识即是告知算法该选择哪一种高清结果。

在图像超清问题中，这样的知识体现为让像素之间有相互依赖的关系。这样，就可以保证A、B、C、D四个不同的部分对于高清版的选择是一致的。
<p style="text-align: center;"><img class="alignnone" src="http://yuenshome-wordpress.stor.sinaapp.com/uploads/2017/04/super-resolution-07.png" alt="" width="442" height="301" />
<strong>图7. 基于PixelCNN的解决图像超清问题的CNN网络结构。其中先验网络(prior network)为PixelCNN；条件网络（conditioning network）为图像生成网络，其结构与作用同GAN中的生成网络、感知损失中的转换网络均类似。</strong></p>
模型架构如图7。其中条件网络是一个在低清图像的基础上生成高清图像的网络。它能以像素为单位独立地生成高清图像，如同GAN中的G网络，感知损失中的转换网络。而先验网络则是一个Pixel CNN组件，它用来增加高清图像像素间的依赖，使像素选择一致的高清细节，从而看起来更加自然。

那么Pixel CNN是如何增加依赖的呢？在生成网络的时候，Pixel CNN以像素为单位进行生成，从左上角到右下角，在生成当前像素的时候，会考虑之前生成的像素。

若加上先验网络和条件网络的混合， PixelCNN在生成图像的时候，除了考虑前面生成的像素，还需要考虑条件网络的结果。
<h1>总结</h1>
上述算法是图像超清问题中使用的较为典型的CNN结构，此外，还有很多其他的结构也达到了比较好的效果。随着CNN<span style="color: #ff0000;">网络结构层次的日益加深</span>，距离<span style="color: #ff0000;">实用场景反而越来越远</span>。譬如，基于<span style="color: #ff0000;">GAN的网络结构的训练</span>很<span style="color: #ff0000;">难稳定</span>，且<span style="color: #ff0000;">结果</span>具有<span style="color: #ff0000;">不可解释性</span>；基于<span style="color: #ff0000;">PixelCNN的网络</span>在使用中由于要在<span style="color: #ff0000;">pixel级别生成</span>，<span style="color: #ff0000;">无法并行</span>，导致<span style="color: #ff0000;">生成效率极为低下</span>。

更进一步地，从实用出发，可以在<span style="color: #ff0000;">数据方向上</span>进行进一步的<span style="color: #ff0000;">优化</span>。譬如，现在的算法输入图像都是由低清图像三次插值而来，那么，是否可以先用一个小网络得到的结果来作为初始化的值呢？再如，多个小网络串联是否能得到比一个大网络更好的结果等等。

<strong><span style="color: #00ccff;">图像超清问题是一个相对来说比较简单的图像语义问题</span></strong>，相信这只是图像语义操作的一个开始，今后越来越多的图像处理问题将会因为CNN的出现迎刃而解。
<h1>参考文献</h1>
<ul>
	<li>[1] Dong C, Loy C C, He K, et al. Image super-resolution using deep convolutional networks[J]. IEEE transactions on pattern analysis and machine intelligence, 2016, 38(2): 295-307.</li>
	<li>[2] Kim J, Kwon Lee J, Mu Lee K. Accurate image super-resolution using very deep convolutional networks[C]//Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition. 2016: 1646-1654.</li>
	<li>[3] Johnson J, Alahi A, Fei-Fei L. Perceptual losses for real-time style transfer and super-resolution[C]//European Conference on Computer Vision. Springer International Publishing, 2016: 694-711.</li>
	<li>[4] Ledig C, Theis L, Huszár F, et al. Photo-realistic single image super-resolution using a generative adversarial network[J]. arXiv preprint arXiv:1609.04802, 2016.</li>
	<li>[5] Dahl R, Norouzi M, Shlens J. Pixel Recursive Super Resolution[J]. arXiv preprint arXiv:1702.00783, 2017.</li>
</ul>
