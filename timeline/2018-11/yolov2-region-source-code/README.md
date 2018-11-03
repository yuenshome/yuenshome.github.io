[![header](../../../assets/header32.jpg)](https://yuenshome.github.io)

# YOLOv2 region层 Forward 部分 DarkNet 源码分析

- 代码仓库：[pjreddie/darknet](https://github.com/pjreddie/darknet)
- 代码版本：61c9d02ec461e30d55762ec7669d6a1d3c356fb2
- 代码日期：2018-09-14

本文以`yolov2-tiny`这个小网络为例进行分析，在此先给出网络执行流程图。


## 1. yolov2-tiny 网络结构与 Region 层参数

下面先给出其执行流程图和对应的`region`层在网络结构`cfg`文件中的定义，后面会对检测 `region` 层代码一步一步分析。

### 1.1 YOLOV2 网络结构

```
$ ./darknet detect cfg/yolov2-tiny.cfg yolov2-tiny.weights data/dog.jpg 
layer     filters    size              input                output
    0 conv     16  3 x 3 / 1   416 x 416 x   3   ->   416 x 416 x  16  0.150 BFLOPs
    1 max          2 x 2 / 2   416 x 416 x  16   ->   208 x 208 x  16
    2 conv     32  3 x 3 / 1   208 x 208 x  16   ->   208 x 208 x  32  0.399 BFLOPs
    3 max          2 x 2 / 2   208 x 208 x  32   ->   104 x 104 x  32
    4 conv     64  3 x 3 / 1   104 x 104 x  32   ->   104 x 104 x  64  0.399 BFLOPs
    5 max          2 x 2 / 2   104 x 104 x  64   ->    52 x  52 x  64
    6 conv    128  3 x 3 / 1    52 x  52 x  64   ->    52 x  52 x 128  0.399 BFLOPs
    7 max          2 x 2 / 2    52 x  52 x 128   ->    26 x  26 x 128
    8 conv    256  3 x 3 / 1    26 x  26 x 128   ->    26 x  26 x 256  0.399 BFLOPs
    9 max          2 x 2 / 2    26 x  26 x 256   ->    13 x  13 x 256
   10 conv    512  3 x 3 / 1    13 x  13 x 256   ->    13 x  13 x 512  0.399 BFLOPs
   11 max          2 x 2 / 1    13 x  13 x 512   ->    13 x  13 x 512
   12 conv   1024  3 x 3 / 1    13 x  13 x 512   ->    13 x  13 x1024  1.595 BFLOPs
   13 conv    512  3 x 3 / 1    13 x  13 x1024   ->    13 x  13 x 512  1.595 BFLOPs
   14 conv    425  1 x 1 / 1    13 x  13 x 512   ->    13 x  13 x 425  0.074 BFLOPs
   15 detection
mask_scale: Using default '1.000000'
Loading weights from yolov2-tiny.weights...Done!
dog: 82%
car: 74%
bicycle: 59%
```

### 1.2 Region 层

这里列出最后一个卷积层和 Region 检测层，列出 Region 层的上一层（卷积）的原因是因为输出个数，与检测结果直接相关。

最后卷积层的输出维度为`13 x  13 x 425 = 71825`，输出元素个数与检测层一样，检测层的维度需要这么看：`13 x 13 x 5 x (4 + 1 + 80)`：

1. 对应将 resize 过后宽高为`416 x 416`的原图，在经过 5 个`stride = 2`的`max-pooling`后为`13 x 13`，即将原图最后切分为`13 x 13`个区域；
2. 5 个 anchors ，对应每个区域有 5 种比例，那么会得到`13 x 13 x 5`个检测框；
3. 4 个坐标参数（检测框的二维中心点坐标x、y、该检测框的高与宽h、w），1 个框的置信度 confidence 分数，以及 80 个概率值对应当前检测框 80 类物体中的存在概率。

所以，检测层对最后一个卷积的结果没有做变换，只是以最初训练预设的方式将最后一层卷积的结果解析出来。

```
[convolutional]
size=1
stride=1                                                                                                         
pad=1
filters=425
activation=linear

[region]
anchors =  0.57273, 0.677385, 1.87446, 2.06253, 3.33843, 5.47434, 7.88282, 3.52778, 9.77052, 9.16828 # anchors的值
bias_match=1
classes=80                  # 类别个数
coords=4                    # 坐标值: x,y,w,h
num=5                       # anchors的个数
softmax=1
jitter=.2
rescore=0
 
object_scale=5              # loss中栅格中是目标的系数
noobject_scale=1            # loss中栅格中非目标的系数
class_scale=1               # loss中栅格中类别损失的系数
coord_scale=1               # loss中栅格中坐标损失的系数
 
absolute=1
thresh = .6
random=1
```



## 参考

1. [【YOLO v3】Series: YOLO object detector in PyTorch - Hello Paperspace](https://blog.paperspace.com/tag/series-yolo/)  
2. [YOLO v3算法笔记 - AI之路 - CSDN博客](https://blog.csdn.net/u014380165/article/details/80202337)  
3. [【v1到v3详解】目标检测网络之 YOLOv3 - 康行天下 - 博客园](https://www.cnblogs.com/makefile/p/YOLOv3.html)  
4. [【v2、v3】YOLOv2与YOLOv3学习笔记基本思路模型训练YOLOv3 - 云+社区 - 腾讯云](https://cloud.tencent.com/developer/article/1156245)  
5. [darknet/region_layer.c at master · hgpvision/darknet](https://github.com/hgpvision/darknet/blob/master/src/region_layer.c)    
6. [YOLO配置文件理解 - hrsstudy的博客 - CSDN博客](https://blog.csdn.net/hrsstudy/article/details/65447947)  
