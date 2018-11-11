[![header](../../../assets/header29.jpg)](https://yuenshome.github.io)

# Batch Normalization: Accelerating Deep Network Training by Reducing Internal Covariate Shift

早先如LeNet、AlexNet、VGG等经典模型都没有BatchNorm这一层，但AlexNet有LRN（一种对于同一个二维像素点跨通道的规范化方法，后来实验发现没有性能提升逐渐舍弃，BatchNorm文中的实验部分也有提到LRN没有精度提升），如果说这些经典神经网络模型中有哪些起到Norm作用的操作，那就应该是数据初始化时减去均值的操作了。

要说数据规范化的定义，来自百度百科的解释是：
- 数据规范化是将原来的度量值转换为无量纲的值。
- 通过将属性数据按比例缩放，通过一个函数将给定属性的整个值域映射到一个新的值域中，即每个旧的值都被一个新的值替代。

常见的数据规范化策略通常有3种：

1. 最小-最大(min-max)规范化：最小-最大规范化保持原有数据之间的联系，即`x_new = (x-x_min)/(x_max-x_min)`。也叫离差标准化，是对原始数据的线性变换，使结果落到`[0,1]`区间，其中max为样本数据的最大值，min为样本数据的最小值。这种方法有一个缺陷就是当有新数据加入时，可能导致max和min的变化，需要重新定义；
2. z-score规范化：当属性`x`的实际最大和最小值未知，或异常点左右了最小－最大规范化时，该方法是有用的，该方法也称为标准差标准化，**经过处理的数据符合标准正态分布，即均值为0，标准差为1，因标准差为1，此时新特征的值就介于[-1,1]之间**，其转化函数为`x_new = (x - E(x)) / std_var(x)`。这里联想到PCA白化，其白化过程的计算就是在PCA的结果上除以标准差。（补：主成分分析，即PCA其作用是考察多个变量间相关性一种多元统计方法，研究如何通过少数几个主成分来揭示多个变量间的内部结构，即从原始变量中导出少数几个主成分，使它们尽可能多地保留原始变量的信息，且彼此间互不相关。通常数学上的处理就是将原来P个指标作线性组合，作为新的综合指标。其实在PCA操作前也有z-score规范化的过程，计算出标准化的输入再作后续处理）；
3. 小数定标规范化：小数定标规范化通过移动属性A 的小数点位置进行规范化。

## 0. 作者动机

通常来说，对数据做归一化标准化这些操作，都会加快算法的收敛和计算速度。

本文主要介绍的BatchNorm也是类似的操作，下面的内容自某博文对该篇论文的翻译，我加入了一些自己的理解，更详细地说明论文作者想出这个方法的具体动机和原因：

作者认为网络训练过程中参数不断地改变会导致后续每一层输入的分布也发生变化，而学习的过程又要使每一层适应输入的分布，而一层层累计带来的结果，会让学习到最后一层再反向回传修正参数变得异常困难，因此不得不降低学习率、小心地初始化网络参数。作者将网络每层的feature map分布发生变化称之为 internal covariate shift，即内部协变量漂移，可能没有做规范化的数据经过一层层的线性或非线性变换，导致每层相关性降低，这可能也是造成网络在反向更新参数时，（即使已经跑了一定数量的iteration/epochs）也比较难更新到前面初始的参数（经过大量的迭代次数后，才会对前面几层的网络参数有效果）。

> ### 为什么预处理操作可以加快训练
> 在训练网络的时的一些预处理操作，通常会将输入减去均值（如减去ImageNet的通道均值），还有对输入做白化等操作，目的是为了加快训练。为什么减均值、白化可以加快训练呢，这里做一个简单地说明：
> ![norm-effect](https://user-images.githubusercontent.com/7320657/48112446-707a5200-e291-11e8-9af1-34de6b105b3a.jpg)
> 首先，图像数据是高度相关的，假设其分布如上图a所示（简化为2维）。由于初始化的时候，我们的参数一般都是0均值的，因此开始的拟合`y = k * x + b`，基本过原点附近，如图b红色虚线。因此，网络需要经过多次学习才能逐步达到如紫色实线的拟合，即收敛的比较慢。如果我们对输入数据先作减均值操作，如图c，显然可以加快学习。更进一步的，我们对数据再进行去相关操作，使得数据更加容易区分，这样又会加快训练，**即对原始数据做一个线性核变换，映射到一个更容易区分的空间/维度中去**，如图d。 
参考：解读Batch Normalization - shuzfan的专栏 - CSDN博客
https://blog.csdn.net/shuzfan/article/details/50723877

对于速度的提升，我的另一个理解是：计算机是怎么做乘法的？乘法也是由于二进制的加法来实现的，规范化到`[-1,1]`之间，高位大部分的二进制值为0，自然计算效率提高，有的框架底层的编译器做了优化可能就跳过输入为0的计算，自然变快了。

为了具体了解BachNorm这个过程，首先我先给出 Caffe 的代码实现（本文重点关注推理，即预测部分，关于训练和反向传播我在文中会贴出写的比较好的博文的内容），然后再给出其论文中的描述。从中我们也可以看出，学术与工程上的差异。

----

## 参考

- [规范化_百度百科](https://baike.baidu.com/item/%E8%A7%84%E8%8C%83%E5%8C%96/3193374#2)
- [数据的标准化 | 网站数据分析](http://webdataanalysis.net/data-analysis-method/data-normalization/)
- [【求教】数据规范化中，使用最小-最大规范化和z分数规范化的区别_博问_博客园](https://q.cnblogs.com/q/57003/)
- [二进制十进制间小数怎么转换_百度经验](https://jingyan.baidu.com/article/425e69e6e93ca9be15fc1626.html)
- [计算机是怎么做乘法运算的 - f905699146的博客 - CSDN博客](https://blog.csdn.net/f905699146/article/details/77171372#commentBox)
- [小数怎么以二进制表示？_百度知道](https://zhidao.baidu.com/question/85815874.html)


## 1. Caffe中的BatchNorm与Scale

论文中的BatchNorm实际分两步：

1. z-score规范化：即减均值，除以方差；
2. 放缩和平移：对z-score规范化后的结果乘以一个系数`gamma`，再加一个常数`beta`（这两个系数是模型训练过程中得到的），其实就是一个线性变换函数，即`y = k * x + b`（`gamma`是`k`，`beta`是`b`）。

但对应在Caffe中的实现这个过程被拆分成了两层：BatchNorm层与Scale层。我想既然拆成两层，那么是否有这一种情况：即某些网络只有BatchNorm层（即z-score的计算）而没有Scale层（放缩和平移的计算），或者反之？

但是大多数经典网络看过来：

- 早期的LeNet、AlexNet、VGG时代还没有BatchNorm；
- 后来的SqueezeNet没有BatchNorm和Scale，但我想是为了加速或者已经做了合并（毕竟SqueezeNet这种早期追求轻量化的网络，认为BatchNorm这样针对每个元素的操作虽然看着简单，但转化为大量的矩阵运算，在部署到嵌入式如手机设备时，还是拖慢了速度，相比卷积/池化层的重要性而言，可以舍弃），而且本身是可以将BatchNorm和Scale层的计算通过模型转换整合到卷积核参数与bias参数中（这里后文会进一步说明）；
- 但后来的经典网络，如mobilenetv1与v2、shufflenetv1与v2、densnet、yolov2、yolov3等这些基础架构都加入了BatchNorm和Scale层（但需要说明的是darknet中的BatchNorm计算并没有像Caffe拆分成两层，而是直接在一层做两个步骤的计算，后面会给出darknet的实现代码）。

也就是说，有BatchNorm层的，都有Scale层。而且本身Scale层是一个线性变换，即不加这个层，`gamma`的值也就是1，`beta`的值是0。

### 1.1 Caffe实现分析

Caffe的BatchNorm层代码位于`caffe/include/caffe/layers/batch_norm_layer.hpp`，因为头文件中包含了一些介绍信息，部分摘录如下：

```cpp
/**
 * 将输入规范化到均值为0，方差为1的结果
 *
 * 该计算对同一个通道（channel）上的值减去相同的均值并除以相同的方差，
 * 同一个通道的均值和方差的计算是基于这一个batch的样本的各自通道计算得到的
 *
 * 默认训练过程中，会借助一个滑动平均值，计算所有训练样本的方差和均值，
 * 得到均值和方差可以在测试时候被用到。
 * 可以手动设置训练中设定use_global_stats选项，来设定方差和均值的计算
 * 是使用累计得到的还是统计的计算方法（注：实际代码中use_global_stats是判断当前是否为训练阶段的）
 * 注：这三个量存储在三个blob中：mean、variance、moving average factor
 *
 * 注意：原始论文中也包括每个通道的放缩系数scaling_factor和偏置bias项的计算。
 *            但是Caffe中是通过一个名为Scale层来实现这一个过程的，
 *            当bias_term这一项为true，则会在每个BatchNormLayer后去做scale+bias的计算。
 *
 * [1] S. Ioffe and C. Szegedy, "Batch Normalization: Accelerating Deep Network
 *     Training by Reducing Internal Covariate Shift." arXiv preprint
 *     arXiv:1502.03167 (2015).
 */
template <typename Dtype>
class BatchNormLayer : public Layer<Dtype> {
 public:
  explicit BatchNormLayer(const LayerParameter& param)
      : Layer<Dtype>(param) {}
  virtual void LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Reshape(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);

 protected:
  virtual void Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Forward_gpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  virtual void Backward_cpu(const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);
  virtual void Backward_gpu(const vector<Blob<Dtype>*>& top,
     const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom);

  Blob<Dtype> mean_, variance_, temp_, x_norm_;
  bool use_global_stats_;                 // 使用全局的统计量，即使用基于整个训练集（即caffemodel存储的）的方差和均值，true表示测试/预测/inference，false表示训练（在实现部分会针对每个batch，根据滑动平均比率moving_average_factrion_，求解累积的防擦好与均值）
  Dtype moving_average_fraction_; // 在训练中，对于均值和方差的滑动平均比率：0.999000
  int channels_;                                 // 通道数，BatchNorm前后feature map维度不变
  Dtype eps_;                                    // 除以标准差时，用来稳定分母不为0，大小为：1e-5

  // extra temporarary variables is used to carry out sums/broadcasting
  // using BLAS
  Blob<Dtype> batch_sum_multiplier_;
  Blob<Dtype> num_by_chans_;
  Blob<Dtype> spatial_sum_multiplier_;
};
}  // namespace caffe
#endif  // CAFFE_BATCHNORM_LAYER_HPP_
```

通过再Docker的caffe-cpu容器中执行如下命令：

```shell
/opt/caffe/build/examples/cpp_classification/classification.bin \
./mobilenetv2.prototxt \
./mobilenetv2.caffemodel \
/opt/caffe/data/ilsvrc12/imagenet_mean.binaryproto \
/opt/caffe/data/ilsvrc12/synset_words.txt \
/home/yuanshuai/code/inferx_model/test/cat.png
```

其[caffe/batch_norm_layer.cpp](https://github.com/BVLC/caffe/blob/master/src/caffe/layers/batch_norm_layer.cpp)具体的实现（其中加入了注释）如下：

```cpp
template <typename Dtype>
void BatchNormLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  const Dtype* bottom_data = bottom[0]->cpu_data();              // input pointer
  Dtype* top_data = top[0]->mutable_cpu_data();                  // output pointer
  int num = bottom[0]->shape(0);                                 // batch size, number of images
  int spatial_dim = bottom[0]->count()/(bottom[0]->shape(0)*channels_); // variance/mean offset in output/input when switch to next channel

  //Dtype scale_factor_tmp = 0.0;
  printf("==== batchnorm input ====\n");                         // ==== batchnorm input ====
  printf("eps_:%.8f\n", eps_);                                   // eps_:0.00001000 
  printf("num:%d\n", num);                                       // num:1
  printf("channels:%d\n", channels_);                            // channels:32
  printf("moving_average_fraction_:%f\n", moving_average_fraction_); // moving_average_fraction_:0.999000
  printf("spatial_dim:%d\n", spatial_dim);                       // spatial_dim:12544

  printf("---- bottom[0] ----\n");                               // ---- bottom[0] ---- // pointer
  printf("bottom.size():%d\n", static_cast<int>(bottom.size())); // bottom.size():1
  printf("bottom[0]->count():%d\n", bottom[0]->count());         // bottom[0]->count():401408
  printf("bottom[0]->shape(0):%d\n", bottom[0]->shape(0));       // bottom[0]->shape(0):1
  printf("bottom[0]->shape(1):%d\n", bottom[0]->shape(1));       // bottom[0]->shape(1):32
  printf("bottom[0]->shape(2):%d\n", bottom[0]->shape(2));       // bottom[0]->shape(2):112
  printf("bottom[0]->shape(3):%d\n", bottom[0]->shape(3));       // bottom[0]->shape(3):112
  printf("bottom[0]->channels():%d\n", bottom[0]->channels());   // bottom[0]->channels():32
  printf("bottom[0]->height():%d\n", bottom[0]->height());       // bottom[0]->height():112
  printf("bottom[0]->width():%d\n", bottom[0]->width());         // bottom[0]->width():112

  printf("---- top[0] ----\n");                                  // ---- top[0] ---- // pointer
  printf("top.size():%d\n", static_cast<int>(top.size()));       // top.size():1 
  printf("top[0]->count():%d\n", top[0]->count());               // top[0]->count():401408
  printf("top[0]->shape(0):%d\n", top[0]->shape(0));             // top[0]->shape(0):1
  printf("top[0]->channels():%d\n", top[0]->channels());         // top[0]->channels():32
  printf("top[0]->height():%d\n", top[0]->height());             // top[0]->height():112
  printf("top[0]->width():%d\n", top[0]->width());               // top[0]->width():112

  printf("---- variance_ ----\n");                               // ---- variance_ ---- // obj
  printf("variance_.count():%d\n", variance_.count());           // variance_.count():32
  printf("variance_.shape(0):%d\n", variance_.shape(0));         // variance_.shape(0):32
  printf("variance_.channels():%d\n", variance_.channels());     // variance_.channels():1
  printf("variance_.height():%d\n", variance_.height());         // variance_.height():1
  printf("variance_.width():%d\n", variance_.width());           // variance_.height():1

  printf("---- mean_ ----\n");                                   // ---- mean_ ---- // obj
  printf("mean_.count():%d\n", mean_.count());                   // mean_.count():32
  printf("mean_.shape(0):%d\n", mean_.shape(0));                 // mean_.shape(0):32
  printf("mean_.channels():%d\n", mean_.channels());             // mean_.channels():1
  printf("mean_.height():%d\n", mean_.height());                 // mean_.height():1
  printf("mean_.width():%d\n", mean_.width());                   // mean_.width():1

  printf("use_global_stats_:%d\n", use_global_stats_);           // use_global_stats_:1
  exit(0);
  // print input data as below, same for output data(`top`)
  /*
  const Dtype *input = bottom[0]->cpu_data();
  const int nchw = bottom[0]->channels() * bottom[0]->height() * bottom[0]->width();
  printf("nchw:%d\n", nchw);
  for(int i=0; i<nchw; i++) {
    printf("input[%d]:%lf\n", i, input[i]);
  }
  exit(0);
  */

  if (bottom[0] != top[0]) { // in-place compute if output pointer equals to input pointer
    caffe_copy(bottom[0]->count(), bottom_data, top_data);
  }

  /* step1. get scale_factor, mean, variance */
  // use_global_stats == 1 for test(inference), use stored mean/variance estimates 
  // use_global_stats == 0 for train, compute mean/variance
  if (use_global_stats_) {
    // for test, use stored mean/variance and multiply with scale_factor
    //           scale_factor stored in this->blobs_[2]->cpu_data()[0], scalar
    //           mean_        stored in this->blobs_[0]->cpu_data(),    float pointer
    //           variance_    stored in this->blobs_[1]->cpu_data(),    float pointer

    // 1. scale_factor = scale_factor == 0 ? 0 : 1 / scale_factor;
    // 2. make each elements in mean_ and variance_ multiply with scale_factor;
    const Dtype scale_factor = this->blobs_[2]->cpu_data()[0] == 0 ?
        0 : 1 / this->blobs_[2]->cpu_data()[0];
    caffe_cpu_scale(variance_.count(), scale_factor,
        this->blobs_[0]->cpu_data(), mean_.mutable_cpu_data());
    caffe_cpu_scale(variance_.count(), scale_factor,
        this->blobs_[1]->cpu_data(), variance_.mutable_cpu_data());
    /*
    { // print mean as below
      printf("---- var * scale_factor result ----\n");
      const int cc = bottom[0]->channels();
      const Dtype* varr = variance_.cpu_data();
      for(int i=0; i<cc; i++) {
        printf("var[%d]:%.20f\n", i, varr[i]);
      }
    }
    */
  } else {
    // for train, compute mean
    //    caffe_cpu_gemv interface as below:
    //        void caffe_cpu_gemv(const CBLAS_TRANSPOSE TransA, const int M, const int N,
    //                            const Dtype alpha, const Dtype* A, const Dtype* x, const Dtype beta,
    //                            Dtype* y);
    //        y := alpha*A*x + beta*y,   or   y := alpha*A'*x + beta*y,   
    //           where alpha and beta are scalars, x and y are vectors and A is an   
    //           m by n matrix. 
    caffe_cpu_gemv<Dtype>(CblasNoTrans, channels_ * num, spatial_dim,
        1. / (num * spatial_dim), bottom_data,
        spatial_sum_multiplier_.cpu_data(), 0.,
        num_by_chans_.mutable_cpu_data());
    caffe_cpu_gemv<Dtype>(CblasTrans, num, channels_, 1.,
        num_by_chans_.cpu_data(), batch_sum_multiplier_.cpu_data(), 0.,
        mean_.mutable_cpu_data());
  }

  // step2. x subtract mean channel-wisely
  // 
  //   C := alpha A * B + beta * C
  // Multiplying Matrices Using dgemm
  // https://software.intel.com/en-us/mkl-tutorial-c-multiplying-matrices-using-dgemm
  caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, channels_, 1, 1,
      batch_sum_multiplier_.cpu_data(), mean_.cpu_data(), 0.,
      num_by_chans_.mutable_cpu_data());
  caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, channels_ * num,
      spatial_dim, 1, -1, num_by_chans_.cpu_data(),
      spatial_sum_multiplier_.cpu_data(), 1., top_data);

  // step3. compute variance for train/test stage
  if (!use_global_stats_) {
    // train stage, compute variance using var(X) = E((X-EX)^2)
    caffe_sqr<Dtype>(top[0]->count(), top_data,
                     temp_.mutable_cpu_data());  // (X-EX)^2

    // num_by_chans_ = (1. / (num * spatial_dim)) * temp_ * spatial_sum_multiplier_
    caffe_cpu_gemv<Dtype>(CblasNoTrans, channels_ * num, spatial_dim,
        1. / (num * spatial_dim), temp_.cpu_data(),
        spatial_sum_multiplier_.cpu_data(), 0.,
        num_by_chans_.mutable_cpu_data());

    // // variance_ = 1.0 * num_by_chans_ * batch_sum_multiplier_
    caffe_cpu_gemv<Dtype>(CblasTrans, num, channels_, 1.,
        num_by_chans_.cpu_data(), batch_sum_multiplier_.cpu_data(), 0.,
        variance_.mutable_cpu_data());  // E((X_EX)^2)

    // compute and save moving average
    this->blobs_[2]->mutable_cpu_data()[0] *= moving_average_fraction_;
    this->blobs_[2]->mutable_cpu_data()[0] += 1;

    // this->blobs_[0] = 1 * mean_ + moving_average_fraction_ * this->blobs_[0]
    caffe_cpu_axpby(mean_.count(), Dtype(1), mean_.cpu_data(),
        moving_average_fraction_, this->blobs_[0]->mutable_cpu_data());

    int m = bottom[0]->count()/channels_;
    Dtype bias_correction_factor = m > 1 ? Dtype(m)/(m-1) : 1;

    // this->blobs_[1] = bias_correction_factor * variance_ + moving_average_fraction_ * this->blobs_[1]
    caffe_cpu_axpby(variance_.count(), bias_correction_factor,
        variance_.cpu_data(), moving_average_fraction_,
        this->blobs_[1]->mutable_cpu_data());
  }

  // compute std-variance: add eps_ before sqrt
  caffe_add_scalar(variance_.count(), eps_, variance_.mutable_cpu_data()); // eps_:0.00001000
  caffe_sqrt(variance_.count(), variance_.cpu_data(),
             variance_.mutable_cpu_data());

  // replicate variance to input size
  caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, channels_, 1, 1,
      batch_sum_multiplier_.cpu_data(), variance_.cpu_data(), 0.,
      num_by_chans_.mutable_cpu_data());
  caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, channels_ * num,
      spatial_dim, 1, 1., num_by_chans_.cpu_data(),
      spatial_sum_multiplier_.cpu_data(), 0., temp_.mutable_cpu_data());
  caffe_div(temp_.count(), top_data, temp_.cpu_data(), top_data);
  // TODO(cdoersch): The caching is only needed because later in-place layers
  //                 might clobber the data.  Can we skip this if they won't?
  // this copy operation is an exception-safe method, that is, copy and swap
  caffe_copy(x_norm_.count(), top_data,
      x_norm_.mutable_cpu_data());
}
```

### 1.2 DarkNet的BatchNorm

因为DarkNet的代码纯C实现，且干净利落，所以这里也给出以供参考，该层代码位于[darknet/batchnorm_layer.c at master · pjreddie/darknet](https://github.com/pjreddie/darknet/blob/master/src/batchnorm_layer.c)：


```c
void forward_batchnorm_layer(layer l, network net)
{
    if(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.input, 1, l.output, 1);
    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    if(net.train){ // 训练时执行
        mean_cpu(l.output, l.batch, l.out_c, l.out_h*l.out_w, l.mean);
        variance_cpu(l.output, l.mean, l.batch, l.out_c, l.out_h*l.out_w, l.variance);

        scal_cpu(l.out_c, .99, l.rolling_mean, 1);
        axpy_cpu(l.out_c, .01, l.mean, 1, l.rolling_mean, 1);
        scal_cpu(l.out_c, .99, l.rolling_variance, 1);
        axpy_cpu(l.out_c, .01, l.variance, 1, l.rolling_variance, 1);

        normalize_cpu(l.output, l.mean, l.variance, l.batch, l.out_c, l.out_h*l.out_w);   
        copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    } else { // 步骤1：减去均值除以方差，即normalize
        normalize_cpu(l.output, l.rolling_mean, l.rolling_variance, l.batch, l.out_c, l.out_h*l.out_w);
    }
    // 步骤2：线性变换。对norm后的数据乘以gamma，再加上beta，其实也是复用了卷积中的bias计算
    scale_bias(l.output, l.scales, l.batch, l.out_c, l.out_h*l.out_w);
    add_bias(l.output, l.biases, l.batch, l.out_c, l.out_h*l.out_w);
}
```

### 1.3 Caffe中的BatchNorm为何拆分成两部分

可以看到darknet中这两个步骤的计算是统一在一层做的。但是Caffe为什么拆分成两层呢？我想到卷积计算完成后也有一个bias需要加，是否这个scale层是为了在Caffe中卷积计算完成后用来计算bias来复用而存在呢？

#### 1.3.1 Caffe中Conv层的Bias项计算

但是我查了代码，发现Caffe中的卷积`void ConvolutionLayer<Dtype>::Forward_cpu`中，其bias计算是如下这么算的：

```cpp
// ConvolutionLayer<Dtype>::Forward_cpu的计算如下：
template <typename Dtype>
void ConvolutionLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const Dtype* weight = this->blobs_[0]->cpu_data();
  for (int i = 0; i < bottom.size(); ++i) {
    const Dtype* bottom_data = bottom[i]->cpu_data();
    Dtype* top_data = top[i]->mutable_cpu_data();
    for (int n = 0; n < this->num_; ++n) { // 对Batch（batch_size为num_）中每一张图片进行前向计算

      // 基类的forward_cpu_gemm函数 base_conv_layer.cpp
      // 计算的是top_data[n * this->top_dim_] =
      // weights * bottom_data[n * this->bottom_dim_]
 
      // bottom_dim_, bias_term和top_dim定义在base_conv_layer.hpp中，
      // int bottom_dim_; 大小默认为 C_in*H_in*W_in
      // int top_dim_; 大小默认为 C_out*H_out*W_out
      // bool bias_term_; 是否使用偏置项

      this->forward_cpu_gemm(bottom_data + n * this->bottom_dim_, weight,
          top_data + n * this->top_dim_);
      if (this->bias_term_) { // 这里计算了bias项
        const Dtype* bias = this->blobs_[1]->cpu_data();
        this->forward_cpu_bias(top_data + n * this->top_dim_, bias);
      }
    }
  }
}

// 上面forward_cpu_bias，来自BaseConvolutionLayer类，直接调用的是Caffe封装的矩阵乘法
template <typename Dtype>
void BaseConvolutionLayer<Dtype>::forward_cpu_bias(Dtype* output,
    const Dtype* bias) {
  caffe_cpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num_output_,
      out_spatial_dim_, 1, (Dtype)1., bias, bias_multiplier_.cpu_data(),
      (Dtype)1., output);

// 可以进一步看caffe_cpu_gemm的实现，位于caffe/src/caffe/util/math_functions.cpp：
template<>
void caffe_cpu_gemm<float>(const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
    const float alpha, const float* A, const float* B, const float beta,
    float* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B,
      ldb, beta, C, N);
}

// A = bias, B = bias_multiplier_.cpu_data(), C = output, 
// M = num_output_, N = out_spatial_dim_, K = 1, 
// alpha = (Dtype)1., beta = (Dtype)1.

// 通过查找mkl的gemm接口文档，查到以下cblas_sgemm的接口参数说明：
C := alpha*op(A)*op(B) + beta*C,
where:
op(X) is one of op(X) = X, or op(X) = X^T, or op(X) = X^H,
alpha and beta are scalars,
A, B and C are matrices:
op(A) is an m-by-k matrix,
op(B) is a k-by-n matrix,
C is an m-by-n matrix.
lda: Specifies the leading dimension of a as declared in the calling (sub)program.
}
```

#### 1.3.2 Caffe中Scale层的Bias项计算

而Scale层的`ScaleLayer<Dtype>::Forward_cpu`代码中是通过`bias_layer_->Forward`来实现bias项的计算，如下：

```cpp
  if (bias_layer_) {
    bias_layer_->Forward(bias_bottom_vec_, top);
  }
```

要说`bias_layer_`的`Forward`函数，在Scale层的cpp代码中没找到，但hpp文件中找到`bias_layer_`的类型是`shared_ptr<Layer<Dtype> >`，在Scale层的`LayerSetUp`函数中初始化：`this->bias_layer_ = LayerRegistry<Dtype>::CreateLayer(layer_param);`，我想`Forward`函数应该继承自基类`public Layer<Dtype>`，查看了基类`Layer`的`layer.hpp`代码，果不其然（同一个文件中，类`Layer`内声明虚函数`Forward`，仍旧是该文件在外部实现了`Forward`函数）：

```cpp
// layer.hpp
template <typename Dtype>
class Layer {
 public:
  // 中间省略
  // 前向传播函数Forward
  inline Dtype Forward(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  // 省略
};  // class Layer

// 仍旧是在layer.hpp中的Layer类外部实现了Forward
// Forward and backward wrappers. You should implement the cpu and
// gpu specific implementations instead, and should not change these
// functions.

// 前向传播函数、后向传播函数包装。不需要修改这两个函数
// 使用时只需要在派生类中改写 Forward_cpu、Forward_gpu、Backward_cpu、Backward_gpu 函数
template <typename Dtype>
inline Dtype Layer<Dtype>::Forward(const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  Dtype loss = 0;
  Reshape(bottom, top);
  switch (Caffe::mode()) { // 判断计算设备
  case Caffe::CPU: // 在CPU上执行前向运算
    Forward_cpu(bottom, top); // 调用CPU版的Forward函数
    // 计算 loss 值（如果有的话）
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      if (!this->loss(top_id)) { continue; }
      const int count = top[top_id]->count();
      // 若为 LossLayer，则已经通过 Forward 函数计算出全局损失函数值，放在 Top Blob data 域
      const Dtype* data = top[top_id]->cpu_data(); 
      // 若 loss_weight 不为0，则已经在 SetLossWeights 函数中将 loss 权重放在 Top Blob diff 域
      const Dtype* loss_weights = top[top_id]->cpu_diff();
      loss += caffe_cpu_dot(count, data, loss_weights);
    }
    break;
  case Caffe::GPU:
    Forward_gpu(bottom, top);
#ifndef CPU_ONLY
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      if (!this->loss(top_id)) { continue; }
      const int count = top[top_id]->count();
      const Dtype* data = top[top_id]->gpu_data();
      const Dtype* loss_weights = top[top_id]->gpu_diff();
      Dtype blob_loss = 0;
      caffe_gpu_dot(count, data, loss_weights, &blob_loss);
      loss += blob_loss;
    }
#endif
    break;
  default:
    LOG(FATAL) << "Unknown caffe mode.";
  }
  return loss;
}
```

这个Forward似乎又回到了Scale层，但是我一想这岂不就无限递归下去做Scale层计算了嘛？显然不对，重新审视了一下代码（按理说里应该跑下程序，这两天机器故障了），再次回顾执行这个`Forward`函数的`shared_ptr<Layer<Dtype> > bias_layer_`的初始化：

```cpp
// 位于Scale层的`LayerSetUp`函数中初始化
this->bias_layer_ = LayerRegistry<Dtype>::CreateLayer(layer_param);
```

这一层会通过`layer_param`将参数创建出来，很有可能这一层的类型不是Scale而是Bias这一层，翻了下`/caffe/src/caffe/layers`与`/caffe/include/caffe/layers`下面的代码，有文件名为`bias_layer.cpp`和`bias_layer.hpp`，其构造函数与Forward函数实现如下：

```cpp
// bias_layer.hpp
template <typename Dtype>
class BiasLayer : public Layer<Dtype> {
 public:
  explicit BiasLayer(const LayerParameter& param)
      : Layer<Dtype>(param) {}
  // 略
}

// bias_layer.cpp
void BiasLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const Dtype* bias_data =
      ((bottom.size() > 1) ? bottom[1] : this->blobs_[0].get())->cpu_data();
  Dtype* top_data = top[0]->mutable_cpu_data();
  if (bottom[0] != top[0]) {
    const Dtype* bottom_data = bottom[0]->cpu_data();
    caffe_copy(bottom[0]->count(), bottom_data, top_data);
  }
  for (int n = 0; n < outer_dim_; ++n) {
    caffe_cpu_gemm(CblasNoTrans, CblasNoTrans, bias_dim_,
        inner_dim_, 1, Dtype(1), bias_data,
        bias_multiplier_.cpu_data(), Dtype(1), top_data);
    top_data += dim_;
  }
}
```

通过查询其它关于介绍Scale层的Caffe源码解读，验证了我的想法，`+bias`的计算是由`bias_layer`计算的。
参考：[Caffe_Scale层解析 - 圆滚滚的小峰峰 - 博客园](https://www.cnblogs.com/LaplaceAkuir/p/7811383.html)


### 1.4 Caffe中Conv层与Scale层Bias项计算的总结

我们不难发现，卷积层`conv_layer`与BatchNorm后的Scale层的**bias计算，二者最终殊途同归**，都调用了`caffe_cpu_gemm`，但是中间的过程不同：

- `conv_layer`：`caffe_cpu_gemm`计算完一张图后，直接判断`if (this->bias_term_)`，为`true`取`bias`的值计算`caffe_cpu_gemm`完成`bias`的计算；
- `scale_layer`：先在`LayerSetUp`函数中通过参数`param`初始化`this->bias_layer_ = LayerRegistry<Dtype>::CreateLayer(layer_param);`，实际初始化了一个`bias_layer`层，之后判断是否初始化成功`if (bias_layer_)`，从而执行基类的Forward函数，在其中判断是执行CPU还是GPU的计算，并完成`bias`的计算。

### 1.5 Caffe将BatchNorm拆分为BN层与Scale层的原因

其实BatchNorm的头文件已经说明了拆分的缘由：原始论文中也包括每个通道的放缩系数`scaling_factor`和偏置`bias`项的计算，但是Caffe中是通过一个名为Scale层来实现这一个过程的。当`bias_term`这一项为`true`，则会在每个`BatchNormLayer`后去做scale+bias的计算。

我感觉这个解释并不令人信服，但是我觉得`bias_layer.cpp`这一层可能是一开始认为后面会用到，比方给卷积用？但是我想卷积的实现应比`bias_layer`要早，但是后来又没有用到了，我在Caffe代码仓库以`bias_layer`为关键词搜索，也就是只有Scale层和相应的测试、文档代码中有使用。

综上，所以我认为这里的`bias_layer`层冗余了。可以单独在Scale层中做。而且Scale层也冗余，完全可以像DarkNet那样，将Scale层的计算放在BatchNorm层中。

其实，在做推理的过程中，BatchNorm、Scale层完全可以不计算，将这部分的计算在模型参数中合并到卷积参数中，后面我会马上讲到。

### 1.6 推理代码

上面简单分析了Caffe与DarkNet的BatchNorm操作代码，其实该过程在Caffe中可简化为下面这样：

```cpp
void batchnorm_impl(int *shape, int shape_size, float *input, float *output, float *mean_, float *var_, float scale_factor_)
{
    // init param
    int n = shape[0];
    int c = shape[1];
    int h = shape[2];
    int w = shape[3];
    float eps_ = 1e-5;
    float caffe_sum_of_var_and_eps = 1.0004441718993991e-10;

    // re-compute scale factor
    scale_factor_ = fabs(scale_factor) > eps_ ? 1.0 / scale_factor_ : 0 ;

    // compute scaled mean, scaled std-var (add eps_)
    for(int channel_idx = 0; channel_idx < c; ++channel_idx)
    {
        mean_[channel_idx] *= scale_factor_;
        var_[channel_idx] = (var_[channel_idx] * scale_factor_ + eps_) <FLT_MIN ? 
                                      powf(caffe_sum_of_var_and_eps, 0.5) : 
                                      powf(scale_factor_ * var_[channel_idx] + eps, 0.5); ;
    }
    // compute batchnrom: x = (x-mean)/std_var
    for(int img_idx = 0; img_idx < n; ++img_idx)
    {
        for(int channel_idx = 0; channel_idx < c; ++channel_idx)
        {
            for(int pixel_idx = 0; pixel_idx < h*w; ++pixel_idx)
            {
                output[img_idx*c*h*w + channel_idx*h*w + pixel_idx] = 
                (input[img_idx*c*h*w + channel_idx*h*w + pixel_idx] - mean_[channel_idx]) / var_[channel_idx];
            }
        }
    }
    return;
}
```

以上是BatchNorm过程的代码计算，干净利落，我这里为清楚，不写参数检查等等，带下划线的参数表示模型参数，如`mean_`、`var_`、`scale_factor_`另外有两个地方可以改进：

- 计算索引时，可以写成内联函数或者宏函数，更清晰一些；
- 外部调用该函数时，input和output都用实际的input，即做in-place操作。

`input`和`mean`等这些指针相关的变量，对应的内存开辟`malloc`在外部，调用该函数前，外部调用时记得释放和开辟需要在同一代码层级）。

下面我再给出Scale层的计算过程：

```cpp
void scale_impl(int *shape, int shape_size, float *input, float *output, float *gamma_, float *beta_, int bias_)
{
    // init param
    int n = shape[0];
    int c = shape[1];
    int h = shape[2];
    int w = shape[3];

    if(bias_)
    {
        for(int img_idx = 0; img_idx < n; ++img_idx)
        {
            for(int channel_idx = 0; channel_idx < c; ++channel_idx)
            {
                for(int pixel_idx = 0; pixel_idx < h*w; ++pixel_idx)
                {
                    output[img_idx*c*h*w + channel_idx*h*w + pixel_idx] = 
                    gamma[channel_idx] * input[img_idx*c*h*w + channel_idx*h*w + pixel_idx] + beta[channel_idx];
                }
            }
        }
    }
    else
    {
        for(int img_idx = 0; img_idx < n; ++img_idx)
        {
            for(int channel_idx = 0; channel_idx < c; ++channel_idx)
            {
                for(int pixel_idx = 0; pixel_idx < h*w; ++pixel_idx)
                {
                    output[img_idx*c*h*w + channel_idx*h*w + pixel_idx] = 
                    gamma[channel_idx] * input[img_idx*c*h*w + channel_idx*h*w + pixel_idx];
                }
            }
        }
    }
    return;
}
```

当然，这里的`scale_impl`与`batchnorm_impl`可以合并到一起做，这里我就不写了，下面我会讲到如何将BatchNorm与Scale层做合并，将其都合并到卷积的参数中去。

### 参考

- [Caffe 源码 - BatchNorm 层与 Scale 层 - 长风破浪会有时，直挂云帆济沧海 - CSDN博客](https://blog.csdn.net/zziahgf/article/details/78843350)
- [CAFFE源码学习笔记之batch_norm_layer - sinat_22336563的博客 - CSDN博客](https://blog.csdn.net/sinat_22336563/article/details/70159386)
- [Caffe: caffe::BatchNormLayer< Dtype > Class Template Reference](http://caffe.berkeleyvision.org/doxygen/classcaffe_1_1BatchNormLayer.html)
- [学习BLAS库 -- GEMV - cocoonyang的专栏 - CSDN博客](https://blog.csdn.net/cocoonyang/article/details/54999797)
- [Multiplying Matrices Using dgemm](https://software.intel.com/en-us/mkl-tutorial-c-multiplying-matrices-using-dgemm)
- [cblas_?gemv| Intel® Math Kernel Library for C](https://software.intel.com/en-us/mkl-developer-reference-c-cblas-gemv#78589C78-9C40-4846-B698-2C36E04B8869)
- [shicai/MobileNet-Caffe: Caffe Implementation of Google's MobileNets (v1 and v2)](https://github.com/shicai/MobileNet-Caffe)  
- [farmingyard/ShuffleNet: This is a fast caffe implementation of ShuffleNet.](https://github.com/farmingyard/ShuffleNet)
- [shicai/DenseNet-Caffe: DenseNet Caffe Models, converted from https://github.com/liuzhuang13/DenseNet](https://github.com/shicai/DenseNet-Caffe)

## 3. BatchNorm/Scale/ReLU合并到Conv中

这部分将会讲到如何做kernel fusion，也就是这些层的合并。我们已知如下：

- Conv+ReLU的合并可以这么做：`max(conv(x), 0)`；
- BatchNorm+Scale的合并可以这么做：`gamma * (x-mu) / sqrt(var) + beta`。

要将batchnorm与scale合并到conv中，先不考虑ReLU层的计算，有：
1. `f(x) = gamma * (conv(x) - mu) / sqrt(var) + beta`；
2. 其实，`conv(x)`的计算可以假设为`conv(x) = k * x +b`的过程，那么则带入上公式有
`f(x) = gamma * (k * x + b - mu) / sqrt(var) + beta`，即`f(x) = gamma * k / sqrt(var) * x + gamma * (b-mu) / sqrt(var) + beta`；
3. 因为输入就是`x`，那么通过化简，仍旧是线性变换，即卷积核参数由于`k`变为`gamma*k/sqrt(var)`，bias项从原本卷积的bias现在变为：`gamma * (b - mu) / sqrt(var) + beta`；
4. 那么我们现在考虑ReLU的计算，得到conv+batchnorm+scale+relu的一次计算结果：`max(0, gamma*k/sqrt(var) * x + gamma*(b-mu)/sqrt(var)+beta)`。

因为上述的说明是简化版，具体到实际中要考虑到通道的索引与对应的卷积参数。下面我贴出对Caffe模型做BN/Scale层合并代码的关键部分（来源见参考`merge_bn.py`的代码），该代码对BN/Scale层参数合并到卷积参数中，最后再写回到模型文件中，为了清楚我对原本的代码做了修改。

```python
    for key in listKeys:
        if type(nobn.params[key]) is caffe._caffe.BlobVec:
            conv = net.params[key]
            if key not in bn_maps or "bn" not in bn_maps[key]:
                for i, w in enumerate(conv):
                    nobn.params[key][i].data[...] = w.data
            else:
                print(key)

                # 取出BN/SCALE/CONV层的模型参数
                bn = net.params[bn_maps[key]["bn"]]
                scale = net.params[bn_maps[key]["scale"]]
                wt = conv[0].data

                channels = 0
                if bn_maps[key]["type"] == "Convolution": 
                    channels = wt.shape[0]
                elif bn_maps[key]["type"] == "Deconvolution": 
                    channels = wt.shape[1]
                else:
                    print("error type " + bn_maps[key]["type"])
                    exit(-1)

                # 无论之前CONV是否有bias，合并BN/SCALE后
                # CONV层都会有bias，且bias的个数和卷积核个数一样（也即卷积输出通道数）
                # 若卷积变量conv长度大于1，说明原卷积模型参数存在bias
                bias = np.zeros(channels)
                if len(conv) > 1:
                    bias = conv[1].data

                # BatchNorm层计算
                # 从BN参数中取出mean、var、scale_factor
                mean = bn[0].data
                var = bn[1].data
                scalef = bn[2].data
                if scalef != 0:
                    scalef = 1. / scalef
                mean = mean * scalef
                var = var * scalef
                rstd = 1. / np.sqrt(var + 1e-5)

                # Scale层计算
                # 从SCALE参数中取出斜率系数gamma（即scales）、偏移量常数项beta（即shift）
                # SCALE(x) = scales * x + shift
                scales = scale[0].data
                shift = scale[1].data

                # 计算合并后的卷积参数
                # 卷积核参数由于k变为：gamma * k / sqrt(var)
                if bn_maps[key]["type"] == "Convolution": 
                    rstd1 = rstd.reshape((channels,1,1,1))
                    scales1 = scales.reshape((channels,1,1,1))
                else:
                    rstd1 = rstd.reshape((1, channels,1,1))
                    scales1 = scales.reshape((1, channels,1,1))
                wt = wt * rstd1 * scales1
                # bias项从原本卷积的bias现在变为：
                # gamma * (b - mu) / sqrt(var) + beta
                bias = (bias - mean) * rstd * scales + shift

                # 将合并后的卷积权重参数放回
                nobn.params[key][0].data[...] = wt
                nobn.params[key][1].data[...] = bias
```

上述代码我加入了自己的注释，只需要在做合并时，注意清除通道不要搞错。当然还有其他的参考可以阅读，也给我很多启发。

---- 

## 参考

- [【推导错误，将conv与bn次序搞反了，但很有启发】模型优化：BatchNorm合并到卷积中 - BigCowPeking - CSDN博客](https://blog.csdn.net/wfei101/article/details/78635557)
- [【BN/SCALE与CONV层的合并】generator_introduction · qfdong/qfdong-s-blog Wiki](https://github.com/qfdong/qfdong-s-blog/wiki/generator_introduction)
- 【Caffe BatchNorm层合并，见`merge_bn.py`】[chuanqi305/MobileNet-SSD: Caffe implementation of Google MobileNet SSD detection network, with pretrained weights on VOC0712 and mAP=0.727.](https://github.com/chuanqi305/MobileNet-SSD)
- [MobileNet-SSD/merge_bn.py at master · chuanqi305/MobileNet-SSD](https://github.com/chuanqi305/MobileNet-SSD/blob/master/merge_bn.py)


## 3. 论文中的BatchNorm

- 论文标题：Batch Normalization: Accelerating Deep Network Training by Reducing Internal Covariate Shift  
- 作者：Sergey Ioffe Google Inc., sioffe@google.com，Christian Szegedy Google Inc., szegedy@google.com  
- 下载地址：https://arxiv.org/abs/1502.03167  

论文中，BatchNorm的计算一气呵成，并没像Caffe中那样拆分成两层（BatchNorm、Scale）。

### 3.1 BatchNorm的计算过程

这里给出具体公式说明，现在有一小批样本`B`（mini-batch），包含`m`张图片，这里为了解释清楚这个规范化过程的三个步骤，先忽略掉每张图的像素点`k`。那么，我们只需要关注这`m`个激活（每张图当做一个激活值）上的规范化，即`B = {x_1...x_m}`这些个样本。

该计算过程包含三个步骤：

1. 求解Batch中方差、均值；
2. 规范化（Norm）：对每个样本减去均值，除以方差；
3. 放缩和平移（Scale，Shift，即线性变换）：对每个样本规范后的结果乘系数`\gamma`放缩，再加上`\beta`平移。

![bn01](https://user-images.githubusercontent.com/7320657/48044073-f54b6a00-e1c4-11e8-93a3-93674b34844e.jpg)

规范化后的数据是定义为`{ \widehat{x_1}, ..., \widehat{x_m} }`，将线性变换的结果记为`{y_1, ..., y_m}`，则该过程描述为：`BN_{\gamma, \beta}:x_{1,...,m} \rightarrow y_{1,...,m}`。其中作用在分母中的`\epsilon`是一个常数，用来稳定分母不为零。

以上是BatchNorm最为基本的描述。

### 3.2 BatchNorm的训练与反向传播

下面这幅图同样是来自论文中，算法描述地更复杂一些：包含了推理和训练两个部分：

- 输入是K个样本的集合：`{ x^(1), ..., x^(K) }`；
- 输出是BatchNorm的推理过程的结果，即`N^{inf}_{BN}`。

![bn02](https://user-images.githubusercontent.com/7320657/48104873-36e51f00-e270-11e8-96dd-1c2c35cfe13f.jpg)

- 2~4行：逐个样本遍历，按照上文提到的BN的计算流程，得到`y^(k)`替代输入的`x^(k)`，注：可以看到每个样本所用到的`\gamma`和`\beta`参数都是不同的（这两个参数是Scale计算中用到，且是学习得到的），实际Caffe计算中同一个通道共享相同的这两个参数。

网上找到一篇介绍训练时和反向时计算的博文，写的不错，部分摘录如下：

![bn_intro01](https://user-images.githubusercontent.com/7320657/48310795-d3d8ec80-e5cf-11e8-81cb-54a4971eb1c5.png)

![bn_intro02](https://user-images.githubusercontent.com/7320657/48310794-d3d8ec80-e5cf-11e8-9458-4a89bc193549.png)

这篇博文的来源见参考。

### 3.3 BatchNorm的使用注意

论文中说到，要发挥好BN的作用，作者在训练中还做了如下几件事：

1. 增大学习率。数据规范化后，Internal Covariate Shift减小，可以采用更大学习率来学习；
2. 移除Dropout层。BatchNorm的作用是正则化，Dropout是随机失活神经元（按照一定dro比率随机乘以为0）也是正则化作用。《Deep Learning》这本书的Chapter 7 Regularization for Deep Learning中是这么定义的：
> In section 5.2.2, we deﬁned regularization as “any modiﬁcation we make to a learning algorithm that is intended to reduce its generalization error but not its training error.”，即**只要是可以减少泛化（验证集/测试集）误差，而非训练误差的任何方法都属于正则化**。
3. 减少L2正则化的系数。作者用降低损失函数中L2惩罚项的系数，精度相比没有减少L2惩罚项系数的BN-Inception损失训练的模型，在交叉验证集上有提高，说明BN对模型的正则化已经过了，再使用正则化来限制模型的学习，适得其反（模型不够学了），反而降低了泛化性；
4. 加大对学习率的衰减。因为训练更快收敛，需要更快降低学习率，否则早早就学不动了（进步变得很慢）；
5. 打乱训练集内样本顺序。防止接近的样本太紧，每个batch训练过程中，计算的方差、均值没有全局观，导致学出来的比较“偏科”，有1%的性能提升；
6. 减少扩增训练数据中扩增数据（指以光线改变原图）的占比。因为收敛更快，应该让模型着重看真实的图片，不要误导模型“走火入魔”。

![image](https://user-images.githubusercontent.com/7320657/48310499-45fb0280-e5cb-11e8-8ef0-0cc2fe18ecfd.png)

### 3.4 BatchNorm优缺点

#### 优点

加速训练更快收敛。表象是减小权重的值尺度的影响，更进一步就是作者所说的降低**internal covariate shift**所造成的影响，降低了层与层之间数据分布差异带来的副作用。本身也是一个正则化方法，文章开头也提到了可以加速训练的原因。

>  BatchNorm与梯度消失、爆炸
BN本质上解决了反向传播中的梯度问题。通过对internal covariance shift的控制，将输入信号规范化到均值为0，方差为1的结果，保证数据层层之间不会出现分布不一致带来的累积效应，这也是先前导致梯度爆炸和消失的主要原因，因为反向传播中梯度与权重直接相关，权重的大小影响了梯度的消失和爆炸，BN的做法对每一层结果规范化（均值和方差一致），消除了权重或者feature map尺度不同带来的影响，进而解决了梯度爆炸和消失的问题。

#### 缺点

1. 训练和预测时，要计算batchnorm与scale层，对应有矩阵乘法（导致训练时间增加，但相比没有BN来说，这点时间增加可忽略）；
2. 对超分辨率（估计还有图像分割问题），image-to-image等任务效果不好。2017NTIRE图像超分辨率中取得了top1的成绩，主要原因竟是去掉了网络中的batchnorm层，由此可见，BN并不是适用于所有任务的，在image-to-image这样的任务中，尤其是超分辨率上，图像的绝对差异显得尤为重要，所以batchnorm的scale并不适合。
3. 本文一直在说CNN上的BatchNorm，找到一篇讲RNN上的效果的。结论是：RNN上的BN不如CNN有效，需要会调整，如RNN有两个方向，若垂直方向加入BN但深度不够如5层，则效果不稳定，深度足够才有效；隐层节点不能做BN，因为对垂直和水平两个方向整体做BN，两个方向不能合并后做， 应该合并前做BN；水平方向做BN虽然也有类似CNN的参数共享，但是RNN是不同时间点水平展开后参数共享，所以说BN的统计量在不同的时间序列中共享，但实验这种想法不对，必须每个时间点的神经元各自维护自己的统计量和参数（具体见参考）。

----

## 参考

- [Caffe 源码 - BatchNorm 层与 Scale 层 - 长风破浪会有时，直挂云帆济沧海 - CSDN博客](https://blog.csdn.net/zziahgf/article/details/78843350)
- [caffe中的batchNorm层（caffe 中为什么bn层要和scale层一起使用） - suixinsuiyuan33的博客 - CSDN博客](https://blog.csdn.net/suixinsuiyuan33/article/details/78637412)
- [解读Batch Normalization - shuzfan的专栏 - CSDN博客](https://blog.csdn.net/shuzfan/article/details/50723877)
- [梯度消失、梯度爆炸及其解决方法 - u011734144的专栏 - CSDN博客](https://blog.csdn.net/u011734144/article/details/80165007)
- [基础 | batchnorm原理及代码详解 - Double_V的博客 - CSDN博客](https://blog.csdn.net/qq_25737169/article/details/79048516)
- [CNN和RNN中如何引入BatchNorm - 张俊林的博客 - CSDN博客](https://blog.csdn.net/malefactor/article/details/51549771)


