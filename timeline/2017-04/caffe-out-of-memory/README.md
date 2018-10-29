[![header](../../../assets/header11.jpg)](https://yuenshome.github.io)

# 尝试解决Caffe内存不够Check failed: *ptr host allocation of size XXX failed问题

只是做infer，在使用Caffe（C/C++ API）跑inception-v2和MXNet（Python API）跑inception-v2、v3、vgg-16、AlexNet、DeepID网络的时候都出现了内存不够的问题。这个和Spark的out of memory问题很像，Spark的问题可以参考我之前记录的<a href="http://yuenshome.space/?p=3265" target="_blank">这篇</a>（<a href="http://yuenshome.space/?p=3265" target="_blank">解决Spark数据倾斜的八种方法</a>）文章，说不定也可以得到inspire。[toc]<!--more-->
<h1>1. 机器具体配置</h1>
下面是树莓派的具体CPU和RAM配置，900M左右的RAM很鸡肋。
<pre class="lang:sh decode:true">yuens@raspberrypi:~ $ cat /proc/cpuinfo 
processor       : 0
model name      : ARMv7 Processor rev 4 (v7l)
BogoMIPS        : 76.80
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32 
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xd03
CPU revision    : 4

processor       : 1
model name      : ARMv7 Processor rev 4 (v7l)
BogoMIPS        : 76.80
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32 
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xd03
CPU revision    : 4

processor       : 2
model name      : ARMv7 Processor rev 4 (v7l)
BogoMIPS        : 76.80
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32 
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xd03
CPU revision    : 4

processor       : 3
model name      : ARMv7 Processor rev 4 (v7l)
BogoMIPS        : 76.80
Features        : half thumb fastmult vfp edsp neon vfpv3 tls vfpv4 idiva idivt vfpd32 lpae evtstrm crc32 
CPU implementer : 0x41
CPU architecture: 7
CPU variant     : 0x0
CPU part        : 0xd03
CPU revision    : 4

Hardware        : BCM2709
Revision        : a02082
Serial          : 0000000001a5ce38
yuens@raspberrypi:~ $ free -m
             total       used       free     shared    buffers     cached
Mem:           925        466        458          2         18        383
-/+ buffers/cache:         64        860
Swap:           99         47         52</pre>
<h1>2. Caffe的内存报错</h1>
<div>调用caffe的C接口（Caffe | CaffeNet C++ Classification example</div>
<div>http://caffe.berkeleyvision.org/gathered/examples/cpp_classification.html）时，会出现如下问题：</div>
<div>
<pre class="lang:sh decode:true">yuens@raspberrypi:~/code/caffe $ ./build/examples/cpp_classification/classification.bin ../caffe_inference/InceptionBN-21K-for-Caffe/deploy.prototxt ../caffe_inference/InceptionBN-21K-for-Caffe/Inception21k.caffemodel ./data/ilsvrc12/imagenet_mean.binaryproto ../caffe_inference/InceptionBN-21K-for-Caffe/synset.txt ./examples/images/cat.jpg 
---------- Prediction for ./examples/images/cat.jpg ----------
F0410 20:06:03.651886 14365 syncedmem.hpp:33] Check failed: *ptr host allocation of size 3763200 failed
*** Check failure stack trace: ***
    @ 0x76b978a4 (unknown)
    @ 0x76b9772c (unknown)
    @ 0x76b9708c (unknown)
    @ 0x76b9a9a8 (unknown)
    @ 0x76eb066c caffe::SyncedMemory::mutable_cpu_data()
    @ 0x76d750c8 caffe::Blob&lt;&gt;::mutable_cpu_data()
    @ 0x76e3039c caffe::PoolingLayer&lt;&gt;::Forward_cpu()
    @ 0x76e762cc caffe::Net&lt;&gt;::ForwardFromTo()
    @ 0x76e764a4 caffe::Net&lt;&gt;::Forward()
    @ 0x14dd8 Classifier::Predict()
    @ 0x14f58 Classifier::Classify()
    @ 0x138e4 main
    @ 0x756db294 (unknown)
Aborted</pre>
以上问题乍一看是内存不够的原因，网上搜到相关的问题如下：syncedmem.hpp: Check failed: *ptr host allocation of size XXXX failed。通过find -name syncedmem.hpp命令查到build目录下的代码。打开可以看到：
<pre class="lang:c++ decode:true">#ifdef USE_MKL
  *ptr = mkl_malloc(size ? size:1, 64);
#else
  *ptr = malloc(size);
#endif
  *use_cuda = false;
  CHECK(*ptr) &lt;&lt; "host allocation of size " &lt;&lt; size &lt;&lt; " failed";
}</pre>
在这里因为主机端内存不够任务执行failed了。

</div>
<div>
<h1>3. 解决思路总结</h1>
</div>
在caffe的issue搜到如下：
<ul>
	<li>syncedmem.hpp: Check failed: *ptr host allocation of size 158297088 failed · Issue #2474 · BVLC/caffe
https://github.com/BVLC/caffe/issues/2474</li>
</ul>
<div>根据该链接，总结出如下几个方案：</div>
<h2>3.1 换成64 bit操作系统</h2>
换成64bit的操作系统解决。不同位数的操作系统对支持的内存大小不同（<span style="color: #ff0000;">其实这里写的有问题，应该是CPU每个线程支持4G内存，实际和系统位数的影响不大，<strong>每个线程支持4GB内存</strong></span>）。有如下两种情况：
<ol>
	<li>当前内存大于操作系统CPU线程数支持的内存。这样有些内存资源就用不上了。这时候换成64 bit的操作系统，或许就能解决内存不够的问题；</li>
	<li>当前内存小于操作系统CPU线程数的内存。让内存达到当前系统的最大支持内存，买更多内存条，或者用更大内存条替换现有的小内存条。</li>
</ol>
这样，cover住了test或者train时候需要的内存，问题就得到了解决。
<h2>3.2 增加swap area</h2>
<div>

通过增加swap area解决。操作系统的交换分区（swap area）是作为内存不足（或内存较低）时，将部分不常用的内存交换出来的备用区域。如果说内存是汽油，内存条就相当于油箱，交换区域则相当于备用油箱。Ubuntu Linux安装时可以在磁盘上划出一个分区用作内存交换区域。文件则介绍如何使用文件作为内存交换区域。这两种方法在使用效果上没有太大区别，但文件可以在分区之后创建，且调整大小更容易，所以这种方案更加灵活。

参考：如何在Linux上使用文件作为内存交换区(Swap Area) - Linux系统教程
http://www.linuxdiyf.com/linux/26411.html

</div>
1、Windows 下有两个超大文件，分别是虚拟内存和休眠缓存文件。而linux下只用一个swap分区（文件）兼备这两个功能，如果你内存够大、不想要休眠，可以考虑不要这个分区（文件）；
2、用不着那么纠结什么时候会用到swap，在Ubuntu下有个明确的比例：60%，即实际内存使用60%的时候开始使用swap。并且这个比例可以自己调节。

休眠时必须。其他，以防万一，程序加载到内存，或者运行时申请内存，说不定就直接崩溃了。再大的内存都没用。比率是不要参考的。大内存的话，随便给点swap就够。

参考：我就不明白了，为什么还要swap分区？ - 查看主题 • Ubuntu中文论坛
http://forum.ubuntu.org.cn/viewtopic.php?t=465065
<div>关于如何设置swap space（即swap area），有用gparted的，以及命令的，具体参考该链接：</div>
<ul>
	<li>How to increase swap space? - Ask Ubuntu
http://askubuntu.com/questions/178712/how-to-increase-swap-space</li>
</ul>
<h2>3.3 用等效的小模型替代大模型</h2>
比方能达到同样或近似性能（准确率、实时性）下，用多个更小的模型替代大模型，或者用多个小卷积核替代大卷积核等等。
<h2>3.4 压缩图像文件</h2>
对图像压缩，比方H.264压缩，缩小图像文件的大小。说白了就是在可接受范围内降低质量。比方用格式工厂或者在线的一些图像压缩软件。
<div>
<h2>3.5 减少batch size</h2>
在train和test的时候可以减少batch size，因为默认情况下，优化的方法是随机梯度下降（SGD，按理说SGD做每次参数更新是基于一个样本，但实际上在很多框架中都是基于大于一个样本的batch来实现的，严格来说不应该叫做SGD，而是mini-batch Gradient Descent），每次做参数更新的时候是基于这个batch的所有样本做iteration的，所以调小这次加载到内存的数据就少（我想的，也有人提到了）。

但这也不能保证，如果是预测单张图像还好说，因为不知道当前框架一次会加载多少图像，是每个batch加载，还是整个训练集或测试集完整加载。不过一般来说，调小batch size还是有作用的。
<h2>3.6 用内存利用率高的框架</h2>
社区里有人说用Keras替代caffe后，跑同样的模型该问题得到解决。或使用MXNet等框架，某些框架底层做了数学或者工程上的特别优化。
<h2>3.7 加更大更多内存</h2>
在保证系统支持的情况下插更多更大的内存条。有人反应该问题可以解决，但当数据量再次增大也会出现同样的问题。
<h2>3.8 特别的数据格式</h2>
某些框架（keras）在使用前需要将input的数据转换为该框架的特别数据格式，如果能事先将数据转换好，或许就不会因为没有转换而导致将全部未转换的数据载入出现内存不够的问题了。

当然，这个首先是要确认出现内存不够的问题是在数据转换阶段产生的。

使用HDF5数据格式。HDF5格式的功能之一是能够使用大型数据集直接从硬盘不需要加载所有数据到内存中。我想使用相同的数据集有不同的框架，目前hdf5大多是支持的格式。
<h2>3.9 模型压缩</h2>
<ul>
	<li>SqueezeNet模型，减少模型的大小，计算量并没有多大影响</li>
	<li>二值化网络，减少模型大小和计算量</li>
	<li>xnor-net，减少模型大小和计算量</li>
</ul>
<h2>3.10 降低精度</h2>
将模型的格式由默认精度float32，转换为int8/float16。如果对infer速度有要求，可以在训练模型的时候可以用高精度比方默认的float32甚至更高，而在infer的时候用低精度如int8，可以减少计算量和模型大小。

</div>
