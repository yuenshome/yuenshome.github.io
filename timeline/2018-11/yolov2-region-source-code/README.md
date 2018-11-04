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

最后卷积层的输出维度为`13 x  13 x 425 = 71825`，输出元素个数与检测层一样，检测层的维度需要这么看：`13 x 13 x 5 x (4 + 1 + 80)`：

1. 对应将 resize 过后宽高为`416 x 416`的原图，在经过 5 个`stride = 2`的`max-pooling`后为`13 x 13`，即将原图最后切分为`13 x 13`个区域；
2. 5 个 anchors ，对应每个区域有 5 种比例，那么会得到`13 x 13 x 5`个检测框；
3. 4 个坐标参数（检测框的二维中心点坐标x、y、该检测框的高与宽h、w），1 个框的置信度 confidence 分数，以及 80 个概率值对应当前检测框 80 类物体中的存在概率。

所以，检测层对最后一个卷积的结果没有做变换，只是以训练预设的方式将最后一层卷积的结果解析出来。

## 2.锁定网络主干与检测起始位置

### 2.1 锁定网络主干

先看网络主干部分，按我们的理解，整个网络的计算应该会有循环来逐层计算，经过在代码里加入打印信息，可以定位在`src/network.c`中的`forward_network`函数，计算了每层的结果（找到这一层可以看命令的调用，所出发的代码，一步步加入打印信息，在这之前可以先看看`darknet.h`这一头文件，该头文件定义了`darknet`对层、网络的定义，看完后会对框架有一个大概的了解），也是在这里可以每一层的的结果打印出来。其`forward_network`函数内容如下：

```c
void forward_network(network *netp)
{
#ifdef GPU
    if(netp->gpu_index >= 0){
        forward_network_gpu(netp);
        return;
    }
#endif
    network net = *netp;
    int i;
    for(i = 0; i < net.n; ++i){
        net.index = i;
        layer l = net.layers[i];
        if(l.delta){
            fill_cpu(l.outputs * l.batch, 0, l.delta, 1);
        }
        l.forward(l, net);
        net.input = l.output;

        // print input here

        if(l.truth) {
            net.truth = l.output;
        }
    }
    calc_network_cost(netp);
}
```

需要注意的是，打印的第`i`层的结果其实都是第`i+1`层的输入。因而打印最后卷积的结果，即检测的输入，即第`15`层的input，所以在上面代码打印处，需要判断`i == 15`。实际上面最后一层`detection`的计算并不在这个循环里，是一个后处理的过程。

取到最后一层卷积的结果，根据前面的网络执行结构图的输入输出信息，我们也可以最后一个卷积结果的元素个数是否是`echo "13*13*425" | bc`，从而确认是否执行到了最后一个卷积层。下一步就是看看检测这一后处理的计算过程。

### 2.2 锁定检测起始位置

仍旧在`src/network.c`中查找调用了`forward_network`的函数，发现是`network_predict`，仍旧是在该文件，发现调用`network_predict`的函数有多个：`network_predict_image`，`network_predict_data_multi`，`network_predict_data`，感觉这样找反而越来越多，那么反着麻烦，看正向的代码执行过程。

由于调用的命令是`./darknet detect cfg/yolov2-tiny.cfg yolov2-tiny.weights data/dog.jpg`，那么看看`example/darknet.c`中`main`函数的`detect`这部分代码（别急，再看一眼if-else结束，发现是`return 0`，那就可以放心了）：

```c
else if (0 == strcmp(argv[1], "detect")){
        float thresh = find_float_arg(argc, argv, "-thresh", .5);
        char *filename = (argc > 4) ? argv[4]: 0;
        char *outfile = find_char_arg(argc, argv, "-out", 0); 
        int fullscreen = find_arg(argc, argv, "-fullscreen");
        test_detector("cfg/coco.data", argv[2], argv[3], filename, thresh, .5, outfile, fullscreen);
    }
```

深入`test_detector`探究，找到该函数位于`examples/detector.c:562`，这个函数很长，不过条理还算清晰，可以看到里面有调用`forward_network`的`network_predict`函数：

```c
void test_detector(char *datacfg, char *cfgfile, char *weightfile, char *filename, float thresh, float hier_thresh, char *outfile, int fullscreen)
{
    list *options = read_data_cfg(datacfg);
    char *name_list = option_find_str(options, "names", "data/names.list");
    char **names = get_labels(name_list);

    image **alphabet = load_alphabet();
    network *net = load_network(cfgfile, weightfile, 0);
    set_batch_network(net, 1);
    srand(2222222);
    double time;
    char buff[256];
    char *input = buff;
    float nms=.45;
    while(1){
        if(filename){
            strncpy(input, filename, 256);
        } else {
            printf("Enter Image Path: ");
            fflush(stdout);
            input = fgets(input, 256, stdin);
            if(!input) return;
            strtok(input, "\n");
        }
        image im = load_image_color(input,0,0);
        image sized = letterbox_image(im, net->w, net->h);
        //image sized = resize_image(im, net->w, net->h);
        //image sized2 = resize_max(im, net->w);
        //image sized = crop_image(sized2, -((net->w - sized2.w)/2), -((net->h - sized2.h)/2), net->w, net->h);
        //resize_network(net, sized.w, sized.h);
        layer l = net->layers[net->n-1];


        float *X = sized.data;
        time=what_time_is_it_now();
        network_predict(net, X); // 该函数调用了forward_network
        printf("%s: Predicted in %f seconds.\n", input, what_time_is_it_now()-time);
        int nboxes = 0;
        detection *dets = get_network_boxes(net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes);
        //printf("%d\n", nboxes);
        //if (nms) do_nms_obj(boxes, probs, l.w*l.h*l.n, l.classes, nms);
        if (nms) do_nms_sort(dets, nboxes, l.classes, nms);
        draw_detections(im, dets, nboxes, thresh, names, alphabet, l.classes);
        free_detections(dets, nboxes);
        if(outfile){
            save_image(im, outfile);
        }
        else{
            save_image(im, "predictions");
#ifdef OPENCV
            make_window("predictions", 512, 512, 0);
            show_image(im, "predictions", 0);
#endif
        }
        free_image(im);
        free_image(sized);
        if (filename) break;
    }
}
```

该函数显示了整个网络执行的轮廓，其中部分行代码被作者注释掉了，其实刚刚我们将代码跟到了`network_predict`函数，可以在上面这个函数里找到`network_predict`，其实现是将`network_forward`的结果直接返回，从这里我们开始，忽略掉网络初始化和后面处理的无关代码，简化后，我们只需要`test_detector`函数中以下代码的实现：

```c
        network_predict(net, X);
        printf("%s: Predicted in %f seconds.\n", input, what_time_is_it_now()-time);
        int nboxes = 0;
        detection *dets = get_network_boxes(net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes);
        if (nms) do_nms_sort(dets, nboxes, l.classes, nms); // nms:.45，nms值前面写死的0.45，非0会执行do_nms_sort
        draw_detections(im, dets, nboxes, thresh, names, alphabet, l.classes);
```

- 看了这部分代码，可以看出darknet给出的网络预测时间，实际上是不算后处理（nms，detect等）的网络执行时间；  
- 从这个轮廓来看，检测层，也就是后处理部分大致有：`get_network_boxes`、`do_nms_sort`两部分；
- 执行结束`network_predict`时，`net`这个结构体变量已经挂载了最后卷积的结果（具体说来是`network_forward`函数中有一句`net.input = l.output;`）。

到此，我们找到了检测层计算的起始位置。

## 3.检测层分析

刚已经说到检测层致有：`get_network_boxes`、`do_nms_sort`两部分。

### 3.1 检测层：获取检测框

接下来，上面代码中`net`被传给`get_network_boxes`得到`det`结构体，需要仔细看看`get_network_boxes`是怎么做的。这其中会有基于最后一层卷积结果的`region`层计算。其要点如下：

get_network_boxes流程如下：
1. `make_network_boxes`：初始化申请检测框结构体内存（检测框的定义可在`include/darknet.h`中找到`detection`与`box`结构体）；
2. `fill_network_boxes`：将检测层上一层（卷积）的结果填充到`detection`与`box`结构体变量中；
	1. `get_yolo_detections`：cfg中的yolo层，yolov2-tiny没有执行该层；
	2. `get_region_detections`：cfg中的region层，yolov2-tiny有该层；
		1. `region`层主体部分的计算；
		2. `get_region_box`；
		3. `correct_region_boxes`；
	3. `get_detection_detections`：cfg中的detection层，yolov2-tiny没有执行该层。

经过以上两个大的步骤，得到了最后检测框的结果。

#### 3.1.1 检测层：`get_network_boxes`

`get_network_boxes`位于`src/network.c:589`，函数实现如下：

```c
detection *get_network_boxes(network *net, int w, int h, float thresh, float hier, int *map, int relative, int *num)
{
    fprintf(stderr, "==== get_network_boxes ====\n");
    fprintf(stderr, "int nboxes = 0;\n");
    fprintf(stderr, "get_network_boxes(net, im.w, im.h, thresh, hier_thresh, 0, 1, &nboxes)\n");
    fprintf(stderr, "w:%d h:%d\n", w, h);        // w:768 h:576 原始图像的大小
    fprintf(stderr, "thresh:%f\n", thresh);      // 0.500000 位于example/darknet.c:432-437，if-else在detect部分定义，若用户未定义则该值默认为0.5
    fprintf(stderr, "hier:%f\n", hier);          // 0.500000 位于example/darknet.c:432-437，if-else在detect部分定义，值为0.5
    fprintf(stderr, "relative:%d\n", relative);  // 1        位于example/detector.c:600，值被固定为1
    detection *dets = make_network_boxes(net, thresh, num);
    fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);
    return dets;
}

// 又看了下include/darknet.h中关于detection与box这两个结构体的定义
typedef struct detection{
    box bbox;
    int classes;
    float *prob;
    float *mask;
    float objectness;
    int sort_class;
} detection;

typedef struct{
    float x, y, w, h;
} box;
```

该函数做了两件事：先调用`make_network_boxes`对`detection`结构体变量初始化，后调用`fill_network_boxes`填充值到其中。`make_network_boxes`先看看其初始化过程：

```c
detection *make_network_boxes(network *net, float thresh, int *num) {   
    fprintf(stderr, "==== make_network_boxes ====\n");
    fprintf(stderr, "net->n:%d\n", net->n);                  // net->n:16
    layer l = net->layers[net->n - 1];                             // 拿到最后一层
    int i;                                                                  // 初始化 i 用来后面遍历 nboxes
    int nboxes = num_detections(net, thresh);                    // 调用num_detection(net, thresh)获取到boxes的数量
    fprintf(stderr, "nboxes:%d\n", nboxes);                   // nboxes:845                                                   
    fprintf(stderr, "l.coords:%d\n", l.coords);                 // l.coords:4
    if(num) *num = nboxes;                                          // 初始时num作为指针传入，实际结果需要返回去
    detection *dets = calloc(nboxes, sizeof(detection));         // 申请空间，nboxes个detection元素的结构体数组
    for(i = 0; i < nboxes; ++i){                                        // 初始化每个detection box
        dets[i].prob = calloc(l.classes, sizeof(float));               // 每个detection box都有一个classes元素个数的float *prob数组，对应当前box每个类别的概率
        if(l.coords > 4){ // 坐标数目大于4放到mask中，coords定义在cfg文件中，即每个box的四个坐标，yolov1到v3的cfg文件中定义均为4
            dets[i].mask = calloc(l.coords-4, sizeof(float));
        }
    }
    return dets;
}
```

`num_detections`函数返回网络结果的box数量，其函数定义在`src/network.c:538`：

```c
int num_detections(network *net, float thresh)
{
    fprintf(stderr, "==== num_detections ====\n");
    int i;
    int s = 0; // 返回box的数量
    for(i = 0; i < net->n; ++i)
    { //该过程，累加YOLO、DETECTION、REGION类型的层的box
        layer l = net->layers[i];
        if(l.type == YOLO){
            fprintf(stderr, "layer idx: %d type: YOLO\n", i);
            s += yolo_num_detections(l, thresh);
        }    
        if(l.type == DETECTION || l.type == REGION){ // yolov2和yolov2-tiny的网络配置cfg文件中，检测层定义均为region
           fprintf(stderr, "layer idx: %d type: DETECTION || REGION\n", i); // layer idx: 15 type: DETECTION || REGION 
           fprintf(stderr, "l.w:%d l.h:%d l.n:%d\n", l.w, l.h, l.n); // l.w:13 l.h:13 l.n:5
           s += l.w*l.h*l.n;   // 网络输出的通道数是h*w*anchor_num*(coord_num + conf + class_num)，即13*13*5*(5+80)
        }
    }
    fprintf(stderr, "s:%d\n", s); // s:845
    return s;
}
```
- 对于输入图像尺寸为`416 x 416 x 3`，经过网络中5个`stride = 2`的max-pooling尺寸缩小32倍，得到输出的特征图的宽高为`13(=416/32)`（输出特征图的通道数下面会提到）；  
- 输入图像尺寸`Si x Si`，输出特征图宽高为`Oi = Si / 2^5`；  
- 最后卷积层输出数目定义的由来：输出图像为Oi x Oi个栅格，每个栅格预测#anchors种boxes大小，每个box包含4个坐标值（x,y,h,w，即中心点坐标+宽度和高度偏移量）,1个置信度和#classes个条件类别概率，所以输出维度是`Oi x Oi x #anchors x (5 + #classes)`；  
- tiny-yolov2中，输入大小`416 x 416 x 3`，输出大小`13 x 13 x 425`，其中的输出通道个数425，也即最后卷积层的输出通道个数的计算就是根据：`输出宽度 x 输出高度 x anchors数目 x (坐标信息数目 + 类别数目)`得到的，除去图像二维的乘积，其中要注意坐标信息数目是5（x,y,h,w,conf，conf置信度，其反映是否包含物体以及包含物体情况下位置的准确性，在YOLOv1中的定义为`Pr(Object)×IOU^truth_pred`，其中`Pr(Object)∈{0,1}`。），因而是`425 = 5 x (4+1+80)`。

`detection *dets = make_network_boxes(net, thresh, num);`对检测结果的结构体变量初始化结束后，紧接着是填充初始化的结构体，即`fill_network_boxes(net, w, h, thresh, hier, map, relative, dets);`，该函数实现位于`src/network.c:569`：

#### 3.1.2 检测层：`fill_network_boxes`

```c
void fill_network_boxes(network *net, int w, int h, float thresh, float hier, int *map, int relative, detection *dets)
{
    fprintf(stderr, "==== fill_network_boxes ====\n");
    int j;
    for(j = 0; j < net->n; ++j){
        layer l = net->layers[j];
        if(l.type == YOLO){
            fprintf(stderr, "YOLO\n");
            int count = get_yolo_detections(l, w, h, net->w, net->h, thresh, map, relative, dets);
            dets += count;
        }    
        if(l.type == REGION){
            fprintf(stderr, "REGION\n");
            get_region_detections(l, w, h, net->w, net->h, thresh, map, hier, relative, dets);
            dets += l.w*l.h*l.n;
        }    
        if(l.type == DETECTION){
            fprintf(stderr, "DETECTION\n");
            get_detection_detections(l, w, h, thresh, dets);
            dets += l.w*l.h*l.n;
        }    
    }    
}
```

上面三个if-else分支中，执行tiny-yolov2模型走的是`l.type == REGION`这个分支，那么再看看`get_region_detections`这个函数的实现，位于`src/region_layer.c:364`：

```c
void get_region_detections(layer l, int w, int h, int netw, int neth, float thresh, int *map, float tree_thresh, int relative, detection *dets)
{
    fprintf(stderr, "==== get_region_detections ====\n");
    fprintf(stderr, "l.batch:%d\n", l.batch); // l.batch:1
    fprintf(stderr, "dets[%d].mask:%p\n", 0, dets[0].mask); // dets[0].mask:(nil)
    //if(!(dets[0].mask))
    //    fprintf(stderr, "*(dets[%d].mask):%f\n", 0, *(dets[0].mask));
    fprintf(stderr, "l.softmax_tree:%p\n", l.softmax_tree); // l.softmax_tree:(nil)
    fprintf(stderr, "map:%p\n", map); // map:(nil)
    fprintf(stderr, "l.batch:%d\n", l.batch); // l.batch:1
    fprintf(stderr, "l.n:%d\n", l.n); // l.n:5
    fprintf(stderr, "l.h:%d\n", l.h); // l.h:13
    fprintf(stderr, "l.w:%d\n", l.w); // l.w:13
    fprintf(stderr, "l.classes:%d\n", l.classes); // l.classes:80
    fprintf(stderr, "l.coords:%d\n", l.coords); // l.coords:4
    fprintf(stderr, "l.background:%d\n", l.background); // l.background:0
    fprintf(stderr, "thresh:%f\n", thresh); // thresh:0.500000,  0.500000 位于example/darknet.c:432-437，if-else在detect部分定义，若用户未定义则该值默认为0.5
    fprintf(stderr, "tree_thresh:%f\n", tree_thresh); // tree_thresh:0.500000，位于example/darknet.c同上附近的位置，变量hier，if-else在detect部分定义为0.5

    int i,j,n,z;
    float *predictions = l.output;
    if (l.batch == 2) {
        float *flip = l.output + l.outputs;
        for (j = 0; j < l.h; ++j) {
            for (i = 0; i < l.w/2; ++i) {
                for (n = 0; n < l.n; ++n) {
                    for(z = 0; z < l.classes + l.coords + 1; ++z){
                        int i1 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + i;
                        int i2 = z*l.w*l.h*l.n + n*l.w*l.h + j*l.w + (l.w - i - 1); 
                        float swap = flip[i1];
                        flip[i1] = flip[i2];
                        flip[i2] = swap;
                        if(z == 0){ 
                            flip[i1] = -flip[i1];
                            flip[i2] = -flip[i2];
                        }   
                    }   
                }   
            }   
        }   
        for(i = 0; i < l.outputs; ++i){
            l.output[i] = (l.output[i] + flip[i])/2.;
        }   
    }   
    for (i = 0; i < l.w*l.h; ++i){
        int row = i / l.w;
        int col = i % l.w;
        for(n = 0; n < l.n; ++n){
            int index = n*l.w*l.h + i;
            for(j = 0; j < l.classes; ++j){
                dets[index].prob[j] = 0;
            }   
            int obj_index  = entry_index(l, 0, n*l.w*l.h + i, l.coords);
            int box_index  = entry_index(l, 0, n*l.w*l.h + i, 0); 
            int mask_index = entry_index(l, 0, n*l.w*l.h + i, 4); 
            float scale = l.background ? 1 : predictions[obj_index];
            dets[index].bbox = get_region_box(predictions, l.biases, n, box_index, col, row, l.w, l.h, l.w*l.h);
            dets[index].objectness = scale > thresh ? scale : 0;
            if(dets[index].mask){
                for(j = 0; j < l.coords - 4; ++j){
                    dets[index].mask[j] = l.output[mask_index + j*l.w*l.h];
                }
            }

            int class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + !l.background);
            if(l.softmax_tree){

                hierarchy_predictions(predictions + class_index, l.classes, l.softmax_tree, 0, l.w*l.h);
                if(map){
                    for(j = 0; j < 200; ++j){
                        int class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + map[j]);
                        float prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                } else {
                    int j =  hierarchy_top_prediction(predictions + class_index, l.softmax_tree, tree_thresh, l.w*l.h);
                    dets[index].prob[j] = (scale > thresh) ? scale : 0;
                }
            } else {
                if(dets[index].objectness){
                    for(j = 0; j < l.classes; ++j){
                        int class_index = entry_index(l, 0, n*l.w*l.h + i, l.coords + 1 + j);
                        float prob = scale*predictions[class_index];
                        dets[index].prob[j] = (prob > thresh) ? prob : 0;
                    }
                }
            }
        }
    }
    fprintf(stderr, "l.w:%d\n", l.w); // l.w:13
    fprintf(stderr, "l.h:%d\n", l.h); // l.h:13
    fprintf(stderr, "l.n:%d\n", l.n); // l.n:5
    fprintf(stderr, "w:%d\n", w); // w:768
    fprintf(stderr, "h:%d\n", h); // h:576
    fprintf(stderr, "netw:%d\n", netw); // netw:416
    fprintf(stderr, "neth:%d\n", neth); // neth:416
    fprintf(stderr, "relative:%d\n", relative); // relative:1

    correct_region_boxes(dets, l.w*l.h*l.n, w, h, netw, neth, relative);
}
```











## 参考

1. [【YOLO v3】Series: YOLO object detector in PyTorch - Hello Paperspace](https://blog.paperspace.com/tag/series-yolo/)  
2. [YOLO v3算法笔记 - AI之路 - CSDN博客](https://blog.csdn.net/u014380165/article/details/80202337)  
3. [【v1到v3详解】目标检测网络之 YOLOv3 - 康行天下 - 博客园](https://www.cnblogs.com/makefile/p/YOLOv3.html)  
4. [【v2、v3】YOLOv2与YOLOv3学习笔记基本思路模型训练YOLOv3 - 云+社区 - 腾讯云](https://cloud.tencent.com/developer/article/1156245)  
5. [darknet/region_layer.c at master · hgpvision/darknet](https://github.com/hgpvision/darknet/blob/master/src/region_layer.c)    
6. [YOLO配置文件理解 - hrsstudy的博客 - CSDN博客](https://blog.csdn.net/hrsstudy/article/details/65447947)  
