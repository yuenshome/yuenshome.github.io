[![header](../../../assets/header01.jpg)](https://yuenshome.github.io)

# Active Learning: 一个降低深度学习时间，空间，经济成本的解决方案- 简书

<ul>
	<li>转载自：http://www.jianshu.com/p/42801f031cfa?utm_campaign=hugo&amp;utm_medium=reader_share&amp;utm_content=note&amp;utm_source=weibo</li>
</ul>
<div>
<div>

作者：<a href="http://www.zongweiz.com" target="_blank">Zongwei Zhou</a> | 周纵苇
邮箱：<a href="zongweiz@asu.edu" target="_blank">zongweiz@asu.edu</a>
微博：<a href="http://weibo.com/u/1060010800" target="_blank">@MrGiovanni</a>

<hr />

<div class="image-package">

<img src="//upload-images.jianshu.io/upload_images/1689929-70c4a5d4d2cc8545.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="" data-original-src="http://upload-images.jianshu.io/upload_images/1689929-70c4a5d4d2cc8545.png?imageMogr2/auto-orient/strip%7CimageView2/2" />
<div class="image-caption"></div>
</div>
下面要介绍的工作发表于<a href="http://cvpr2017.thecvf.com/" target="_blank">CVPR2017</a>，题为“<strong>Fine-tuning Convolutional Neural Networks for Biomedical Image Analysis: Actively and Incrementally</strong>”。它主要解决了一个深度学习中的重要问题：如何使用尽可能少的标签数据来训练一个效果promising的分类器。根据我的判断，当遇到两种情况的时候，这篇论文的可以非常强大的指导意义：
<ul>
	<li>一共手头有100个未标记样本，和仅仅够标记10个样本的钱，老板说，通过训练这十个标记的样本，能接近甚至达到训练100个样本的performance；</li>
	<li>手头有了一个已经在100个样本中训练完的分类器，现在又来了100个新标记的样本，老板说，只给提供够训练10个样本的计算机；或者只给你提供够训练10个样本的时间，让分类器尽快学习到新来样本的特征；</li>
</ul>
这两个情况发生的条件是完全不同的，Situation A发生在屌丝公司，没有钱拿到精标记的数据集，却也想做深度学习；Situation B一般发生在高富帅公司，有海量的精标记数据，但是由于目前即使是最牛逼的计算机也不能用深度学习在短时间内一次性地去处理完这些数据（e.g.，内存溢出，或者算上个几年都算不完）。Anyway，我想我已经说清楚应用背景了，读者可以根据实际情况判断是否往后读下去。

</div>
</div>
<!--more-->

<hr />

&nbsp;
<div>
<div>
<div class="image-package">

<img src="//upload-images.jianshu.io/upload_images/1689929-c4a920bc884b84a9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="" data-original-src="http://upload-images.jianshu.io/upload_images/1689929-c4a920bc884b84a9.png?imageMogr2/auto-orient/strip%7CimageView2/2" />
<div class="image-caption"></div>
</div>
感谢你选择继续往下阅读，那么如果觉得下文所传递的思想和方法对你有帮助，请记得一定引用这篇CVPR2017的文章。<a href="http://openaccess.thecvf.com/content_cvpr_2017/papers/Zhou_Fine-Tuning_Convolutional_Neural_CVPR_2017_paper.pdf" target="_blank">Fine-tuning Convolutional Neural Networks for Biomedical Image Analysis: Actively and Incrementally.</a>

@InProceedings{Zhou_2017_CVPR,
author = {Zhou, Zongwei and Shin, Jae and Zhang, Lei and Gurudu, Suryakanth and Gotway, Michael and Liang, Jianming},
title = {Fine-Tuning Convolutional Neural Networks for Biomedical Image Analysis: Actively and Incrementally},
booktitle = {The IEEE Conference on Computer Vision and Pattern Recognition (CVPR)},
month = {July},
year = {2017}
}
<h1>1. 为什么会想到去解决这个问题？</h1>
现在深度学习很火，做的人也越来越多，那么它的门槛可以说是很低的，Caffe，Keras，Torch等等框架的出现，让该领域的programming的门槛直接拆了。所以深度学习真正的门槛变成了很简单概念——钱。这个钱有两个很重要的流向，一是计算机的运算能力（GPU Power），二是标记数据的数量。这儿就引出一个很重要的问题：<strong>是不是训练数据集越多，深度学习的效果会越好呢？</strong>这个答案凭空想是想不出来的，能回答的人一定是已经拥有了海量的数据，如ImageNet，Place等等，他们可以做一系列的实验来回答这个问题。需要呈现的结果很简单，横坐标是训练集的样本数，纵坐标是分类的performance，如下图所示：
<div class="image-package">

<img src="//upload-images.jianshu.io/upload_images/1689929-5e34911b5cdceaee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="" data-original-src="http://upload-images.jianshu.io/upload_images/1689929-5e34911b5cdceaee.png?imageMogr2/auto-orient/strip%7CimageView2/2" />
<div class="image-caption"><strong>Fig.1 如果答案是左图，那么就没什么可以说的了，去想办法弄到尽可能多的训练数据集就ok，但是现实结果是右图的红实线，一开始，训练集的样本数增加，分类器的性能快速地在上升，当训练集的样本数达到某一个临界值的时候，就基本不变了，也就是说，当达到了这个临界的数目时，再去标注数据的ground truth就是在浪费时间和金钱。有了这个认知，接下来就是想办法让这个临界值变小，也就是用更小的训练集来更快地达到最理想的性能，如右图的红虚线所示。红实线我们认为是在随机地增加训练集，那么红虚线就是用主动学习（Active Learning）的手段来增加训练集，从而找到一个更小的子集来达到最理想的性能。</strong></div>
</div>
这里需要说明的一点是，训练样本数的临界点大小和这个分类问题的难度有关，如果这个分类问题非常简单，如黑白图像分类（白色的是1，黑色的是0），那么这个临界值就特别小，往往几幅图就可以训练一个精度很高的分类器；如果分类问题很复杂，如判断一个肿瘤的良恶性（良性是0，恶性是1），那么这个临界值会很大，因为肿瘤的形状，大小，位置各异，分类器需要学习很多很多的样本，才能达到一个比较稳定的性能。

<strong>FIG.1</strong>对于很多从事深度学习的无论是研究员还是企业家都是一个十分有启发性的认知改变。一般来讲，人的惯性思维会引领一个默认的思路，就是训练样本越多越好，如左图所示，这将直接导致许多工作的停滞不前，理由永远是“我们没有足够的数据，怎么训练网络！”进一步的思路是图二的红实线认知：要多少是多啊，先训着再说，慢慢的就会发现即便用自己有的一小部分数据集好像也能达到一个不错的分类性能，这个时候就遇到一个问题：<strong>自己的数据集样本数到底有没有到达临界值呢？</strong>这个问题也很关键，它决定了要不要继续花钱去找人标注数据了。这个问题我会在第三部分去回答它，这里先假设我们知道了它的答案，接下来的问题就是<strong>如何让这个临界值变小？</strong>
<h1>2. 如何让临界值变小？</h1>
解决方案就是主动学习（Active Learning），去主动学习那些比较<strong>“难的”</strong>，<strong>“信息量大的”</strong>样本（hard mining）。关键点是每次都挑当前分类器分类效果不理想的那些样本（hard sample）给它训练，假设是训练这部分hard sample对于提升分类器效果最有效而快速。问题是<strong>在不知道真正标签的情况下怎么去定义HARD sample？或者说怎么去描述当前分类器对于不同样本的分类结果的好坏？</strong>

<strong>定义：</strong>由于深度学习的输出是属于某一类的概率（0～1），一个很直观的方法就是用“<a href="https://en.wikipedia.org/wiki/Entropy" target="_blank">熵（entropy）</a>”来刻画信息量，把那些预测值模棱两可的样本挑出来，对于二分类问题，就是预测值越靠近0.5，它们的信息量越大。还有一个比较直观的方法是用“<a href="https://en.wikipedia.org/wiki/Diversity" target="_blank">多样性（diversity）</a>”来刻画labeled data和unlabeled data的相似性。这两个方法都是在<a href="http://ieeexplore.ieee.org/abstract/document/7006697/" target="_blank">“Active batch selection via convex relaxations with guaranteed solution bounds”</a>中被提出。是十分重要的两个Active Learning的选择指标。
<blockquote><strong>Important Observation:</strong> To boost the performance of CNNs in biomedical imaging, multiple patches are usually generated automatically for each candidate through data augmentation; these patches generated from the same candidate share the same label, and are naturally expected to have similar predictions by the current CNN before they are expanded into the training dataset.</blockquote>
有了这两个指标来选hard sample，是比较靠谱了——实验结果表明，这比随机去选已经能更快地达到临界拐点了。
<div class="image-package">

<img src="//upload-images.jianshu.io/upload_images/1689929-95bf7d0bd6942a4a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="" data-original-src="http://upload-images.jianshu.io/upload_images/1689929-95bf7d0bd6942a4a.png?imageMogr2/auto-orient/strip%7CimageView2/2" />
<div class="image-caption"><strong>FIG.2 Active Learning的结构示意图。利用深度学习所带来的优势在于，一开始你可以不需要有标记的数据集。</strong></div>
</div>
<strong>举例来讲，</strong>假设你是一个养狗的大户，你现在想做一个非常偏的（专业化的）分类问题，如<a href="http://baike.baidu.com/item/%E5%8D%B7%E6%AF%9B%E6%AF%94%E9%9B%84%E7%8A%AC" target="_blank">卷毛比雄犬</a>和<a href="http://baike.baidu.com/item/%E5%93%88%E7%93%A6%E9%82%A3%E7%8A%AC" target="_blank">哈瓦那犬</a>的分类问题，你手头有这两种狗各50条，你自己可以很轻松地区分这100条狗，现在需要做的是训练一个分类器，给那些不懂狗的人，他们拍了狗的照片然后把照片输入到这个分类器就可以很好地判断这是卷毛比雄犬还是哈瓦那犬。首先你可以给这100条狗拍照片，每条狗都有不同形态的10张照片，一共拍了1000张没有标记的照片。对于这1000张照片，你所知道的是哪10张对应的是一条狗，其他什么都不知道。

在这个实际分类问题中，你拥有绝对的数据优势，即便是Google Image也不能企及，当然一种选择是你把1000张图片从头到尾看一遍，标注好，但是你更希望是把大多数简单的分类工作交给分类器，自己尽可能少的做标记工作，并且主要是去标记那些分类器模棱两可的那几张照片来提高分类器的性能。
我们初步提供的解决方案是(参考<strong>Fig.2</strong>或者<strong>Fig.3</strong>)：
<ol>
	<li>把1000张图放到在大量自然图像中预训练的网络（LeNet，AlexNet，GoogLeNet，VGG，ResNet等等）中测试一遍，得到预测值，然后挑出来那些“最难的”，“信息量最大的”样本，你看一下，给出他们的标签（卷毛比雄犬还是哈瓦那犬）；</li>
	<li>用这些刚刚给了标签的样本训练深度网络<strong>N</strong>；</li>
	<li>把剩下那些没有标签的图像用<strong>N</strong>测试，得到预测值，还是挑出来那些“最难的”，“信息量最大的”样本，你再给出他们的标签（卷毛比雄犬还是哈瓦那犬）；</li>
	<li>刚刚给了标签的样本和原来有标签的样本组成新的训练集，继续用它们来训练深度网络<strong>N</strong>；</li>
	<li>重复步骤3，4，直到 <strong>[?]</strong></li>
</ol>
如<strong>FIG.2</strong>所示，每次循环都用不断增加的标记数据集去提升分类器的性能，每次都挑对当前分类器比较难的样本来人为标记。
<h1>3. 这个过程什么时候可以停？</h1>
<ol>
	<li>钱用光了；</li>
	<li>当前分类器对选出来的hard samples分类正确；</li>
	<li>选出来的hard samples人类也无法标记；</li>
</ol>
以上三种情况都可以让这个循环训练过程中断，第一种就很无奈了，没钱找人标记了...第二种情况和第三种情况的前提共识是如果难的样本都分类正确了，那么我们认为简单的样本肯定也基本上分类正确了，即便不知道标签。第三种情况，举例来说就是黑白图像分类，结果分类器模棱两可的图像是灰的...也就是说事实上的确分不了，并且当前的分类器居然能把分不了的样本也找出来，这时我们认为这个分类器的性能已经不错的了，所以循环训练结束。
至此，主要讲了传统的Active Learning的思想，接下来会讲讲这篇CVPR2017论文的几个工作点。

<hr />

<blockquote>The way to create something beautiful is often to make subtle tweaks to something that already exists, or to combine existing ideas in a slightly new way.
-- "Hackers &amp; Painters"</blockquote>

<hr />

上面我讲到了Active Learning的大概思路，如<strong>Fig. 3</strong>所示，是一个很有意思的概念，说实话很实用，我在Mayo Clinic实习的时候，每次遇到新的数据集，都会相当用一用这个方法，来让给数据标注的专家轻松一点...
<div class="image-package">

<img src="//upload-images.jianshu.io/upload_images/1689929-2d1ba5140db92abb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="" data-original-src="http://upload-images.jianshu.io/upload_images/1689929-2d1ba5140db92abb.png?imageMogr2/auto-orient/strip%7CimageView2/2" />
<div class="image-caption"><strong>Fig.3 暗的表示unlabeled的数据，高亮的表示labeled的数据，CNN的结构可以随便挑SOTA的无所谓，CNN0是拍 retrained from ImageNet，得到的第二列表示每个image对应的importance指标，越红的说明entropy越大，或者diversity越大，每次挑这些important的sample给专家标注，这样labeled的数据就变多了，用labeled的数据训练CNN，得到新的更强的分类器了，再在unlabeled data上测试，挑出entropy／diversity大的样本，交给专家去标注，如此循环...</strong></div>
</div>
<h1>4. 发现新问题</h1>
貌似前面讲的算法没留下什么问题啊，也就是说，只要学术视野够开阔，这个Active Learning的技术完全可以直接用啦。但是实际上并不是这样的，上面说的熵（Entropy）的确没有什么严重的问题，但是多样性（Diversity），在很多现实应用中问题就会出现。

根据<a href="http://ieeexplore.ieee.org/abstract/document/7006697/" target="_blank">“Active batch selection via convex relaxations with guaranteed solution bounds”</a>的描述，Diversity是计算labeled data和unlabeled data之间的相似度，把和labeled data比较相似的作为简单样本，每次active select难样本，也就是挑出来和labeled data不太像的出来。体现在矩阵上就是行是labeled data，列是unlabeled data，在它们组成的大矩阵中找出最优的子矩阵。这个方法在理论上是可行的，但是实际应用中，数据量（labeled和unlabeled）会非常大，这个矩阵会特别的大，导致求最优解会很慢，或者根本得不出来最优解。因此，我们并不在image-level上算diversity。

由于标记的医学影像训练数据量和自然图像的没法比，Data Augmentation是必须的环节，我们就抓住了这个点来设计Diversity这个指标。这里的假设是：<strong>经过data augmentation后的patches，从CNN出来的预测值应该相对是一致的，因为它们的truth应该还是一致的。</strong>比如一张猫的图像，经过数据扩充，得到的那些个patch所对应的truth也应该都是猫。

<strong>定义：</strong>对于来自同一幅image的patch集，如果它们的分类结果高度不统一了，那么这个image就是Important的，或者hard sample。

需要展开解释的两点：
<ol>
	<li>由于在annotation之前不知道label，所以我们不能知道网络的预测正确还是错误，但是我们可以知道预测统一还是不统一。所以比如一幅猫的图，如果网络的预测很统一都是狗，那么我们也认为这是一个easy sample，不去active select它的。</li>
	<li>结合data augmentation的优点是我们可以知道哪些patch对应什么image，比较容易控制。这样就可以在一个image內算diversity了，每个image对应一个矩阵，大小是一样的，非常的简洁，也容易控制计算量。</li>
</ol>
这样的diversity就完美了吗？并没有... 读者可以先猜猜哪儿又出问题啦，我在第五部分会指出来。
<div class="image-package">

<img src="//upload-images.jianshu.io/upload_images/1689929-5da90c25ddbacdb0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="" data-original-src="http://upload-images.jianshu.io/upload_images/1689929-5da90c25ddbacdb0.png?imageMogr2/auto-orient/strip%7CimageView2/2" />
<div class="image-caption"></div>
</div>
<h1>5. 这次是Data Augmentation的锅</h1>
问题出在上面的假设：<strong>经过data augmentation后的patches，从CNN出来的预测值应该相对是一致的，因为它们的truth应该还是一致的。</strong>

我们知道有几种经典的数据扩充方法：平移，旋转，缩放，形变，加噪声等等。但是很有可能发生的是，经过这些变化以后，得到的patch是无法分类的。举例来讲，<a href="http://imatge-upc.github.io/telecombcn-2016-dlcv/slides/D2L2-augmentation.pdf" target="_blank">Augmentation Lecture</a>讲到的平移crop，如果我们将它应用到猫的分类问题中，很有可能得到这样的训练样本：
<div class="image-package">

<img src="//upload-images.jianshu.io/upload_images/1689929-4db27c91ec98d2a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="" data-original-src="http://upload-images.jianshu.io/upload_images/1689929-4db27c91ec98d2a3.png?imageMogr2/auto-orient/strip%7CimageView2/2" />
<div class="image-caption"><strong>Fig.4 左图是原始的图像，中间的9个patches是根据平移变化的扩充得到的，restrictions: region must contain objects or part of the object <a href="http://imatge-upc.github.io/telecombcn-2016-dlcv/slides/D2L2-augmentation.pdf" target="_blank">详见这里</a>，右图是网络得到的对应patch的预测值。</strong></div>
</div>
可以看出，在这个实例中，对于一个candidate，网络预测的一致性很低，套用Diversity的公式，Diversity的值很大，也就是说，应该被认为是hard sample挑出来。但是仔细观察一下这九个patches就不难看出，即便是很好的分类器，对于中间图中的1，2，3，也是很难分出这个是猫... could be rat, rabbit, etc. 我们把像这三个patch的例子叫做从data augmentation带来的<strong>noisy label issue</strong>。 而对于4～6的patches，网络很好地做出了分类，这很合情合理。所以我们把这种像1，2，3的patches，由于随机的数据扩充（Random Data Augmentation）带来的干扰样本称为noisy labels。<strong>Fig.4</strong>只是一个很直观的例子，其实在实际的数据集中会有很多这样的案例。

我们给出的解决方法是：先计算majority的预测，然后在majority上面算diversity，只要网络的预测大方向是统一的，就是统一的。意思就是想个办法把<strong>Fig.4</strong>中的三个非主流0.1扔掉。只抓住大方向，也就是0.9——我们不希望<strong>Fig.4</strong>这种情况被当作important sample挑出来，因为当前的分类器实际上已经可以分出来这幅图的类别啦。

这个改进的数学表达也非常直观，以<strong>Fig.4</strong>为例，为了知道大方向，我们计算它们的平均值，如果大于0.5，大方向就是label 1，反之大方向是label 0，如果是前者，那么就从大到小取前25%的预测，其他的不要啦，如果是后者，就从小到大取前25%，其他的不要啦。这样<strong>Fig.4</strong>就只剩下三个0.9，它们的diversity就非常小，也就不会被当作important sample挑出来啦。成功解决了data augmentation带来的noisy label issue。

整体的算法流程可参见<strong>Fig.5</strong>
<div class="image-package">

<img src="//upload-images.jianshu.io/upload_images/1689929-856eb8ff02f72b8c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="" data-original-src="http://upload-images.jianshu.io/upload_images/1689929-856eb8ff02f72b8c.png?imageMogr2/auto-orient/strip%7CimageView2/2" />
<div class="image-caption"><strong>Fig.5 Active Learning Algorithm</strong></div>
</div>
至此，主要讲了这篇论文是怎样成功的将<code>Diversity</code>这个重要的指标引入到Active Learning中来，所有的Active Selection部分结束。

<hr />

<h1>6. 如何训练？</h1>
既然用了迁移学习，那么一开始的CNN测试的效果肯定是一团糟，因为这个CNN是从自然图像中学过来的，没有学习过CT这种医学影像，所以这个loop的启动阶段，Active Learning的效果会没有random selecting好。不过很快，随着CNN慢慢地在labeled的CT上训练，Active Learning的效果会一下子超过random selecting。

接下来讨论Continuous fine-tuning的细节，随着labeled data集变大，CNN需要一次次地被训练，有两种选择，一是每次都从ImageNet pretrained来的model来迁移，二是每次用当前的model上面迁移(Continuous Fine-tuning)。方法一的优点是模型的参数比较好控制，因为每次都是从头开始fine-tuning，但是缺点是随着labeled数据量大增加，GPU的消耗很大，相当于每次有新的标注数据来的时候，就把原来的model扔了不管，在实际应用中的代价还是很大的。第二种方法是从当前的model基础上做finetune，在某种意义上knowledge是有记忆的，而且是连续渐进式的学习。问题在于参数不好控制，例如learning rate，需要适当的减小，而且比较容易在一开始掉入local minimum。<strong>关于Continuous Finetuning的前期论文也是有的，需要更进一步的研究。</strong>

另外，如何能够真正意义上去体现出increamentally learning，只是这样一个网络结构去不断的学习吗，还是随着数据集的增加去让网络自适应的变深，便复杂，都是我比较关心的问题。

<hr />

乍一看，Active Learning的工具很直观，思想也很简洁，但随着博客讨论的技术细节越来越深入，会出现很多的open question有待研究，因此，只能说我们的论文是active learning的一个引子。

本文只是从思想的角度来阐述这篇论文，至于后续的分析，结果和结论都在论文中详细的report了。反正一句话来总结，hin好...
<ul>
	<li>Find paper <a href="http://openaccess.thecvf.com/content_cvpr_2017/papers/Zhou_Fine-Tuning_Convolutional_Neural_CVPR_2017_paper.pdf" target="_blank">here</a>.</li>
	<li>Find poster <a href="https://docs.wixstatic.com/ugd/deaea1_c21a4361487244efba9da9d84feddc72.pdf" target="_blank">here</a>.</li>
	<li>Find author <a href="http://weibo.com/u/1060010800" target="_blank">here</a>.</li>
</ul>
祝好，
<div class="image-package"><img src="//upload-images.jianshu.io/upload_images/1689929-2dbc552de9bfae0a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240" alt="" data-original-src="//upload-images.jianshu.io/upload_images/1689929-2dbc552de9bfae0a.jpg" /></div>
</div>
作者：MrGiovanni
链接：http://www.jianshu.com/p/42801f031cfa
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。

</div>
<!--more-->

<!--more-->

<!--more-->
