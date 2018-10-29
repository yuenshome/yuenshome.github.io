[![header](../../../assets/header25.jpg)](https://yuenshome.github.io)

# 性能测试常用命令及脚本

本文有点类似工作日志，我也是对Linux Shell一点都不懂，这里只是一点点积累自己写的算法测试脚本。每次做一点更新和改动，学一点新东西，还在不断完善补充中。[toc]<!--more-->
<h1>-1. 安装OpenBLAS</h1>
如果是X86平台，可以直接去下载pre-build版本通过apt的方式安装，参考：
<ul>
	<li>Precompiled installation packages · xianyi/OpenBLAS Wiki
https://github.com/xianyi/OpenBLAS/wiki/Precompiled-installation-packages</li>
</ul>
若是在arm等平台（gcc -v命令看target的值），可以下载源码包自行编译，区别主要是在于编译多少个线程的版本。比放clone下来代码或者下载好源码包后。
<ul>
	<li>若编译单线程版本，则默认缺省即可，直接在openblas源码根目录执行make命令。关于具体的target也可以不指定，openblas会根据你的平台自动查找；</li>
	<li>若编译多线程版本，则需要在make前指定编译的线程数，比方编译双线程，需要在执行make前执行：
<pre class="lang:sh decode:true ">export OMP_NUM_THREADS=2</pre>
之后，再编译执行make。</li>
</ul>
编译完成后即可安装，make成功会有提示，比方安装的命令可以是：make install（不带prefix似乎会默认安装到系统路径，这里我记不清了）。但通常自己会安装到自己的目录下，执行：
<pre class="lang:sh decode:true">make PREFIX=~/software/OpenBLAS-0.20.2 install</pre>
如果用的caffe测试，因为执行caffe根目录下的./build/tools/caffe进行分类，这个caffe的可执行文件会动态地将blas连接上。可以再在自己的环境变量`~/.bashrc`里添加：
<pre class="lang:sh decode:true">export OPENBLAS_HOME=/home/YOURNAME/software/OpenBLAS-0.2.20
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$OPENBLAS_HOME/lib</pre>
再对./build/tools/caffe执行ldd命令，看是否链接上这个blas库：
<pre class="lang:sh decode:true ">ldd ~/code/caffe/build/tools/caffe | grep blas</pre>
如果显示如下，就说明链接成功：
<pre class="lang:sh decode:true">$ ldd ./build/tools/caffe | grep blas
	libopenblas.so.0 =&gt; /home/YOURNAME/software/OpenBLAS-0.2.20/lib/libopenblas.so.0 (0xb5a98000)</pre>
此时若需要测试双核性能，举个例子需要先制定线程数，然后再制定跑caffe所用到的哪两个核心（当然前提是已经设置需要测试的cpu的主频最大）：
<pre class="lang:sh decode:true">sudo cpufreq-set -c 0,1 -g performance
export OMP_NUM_THREADS=2
nohup taskset -c 0,1 ./build/tools/caffe time --model=models/det_task/BITInfraredDetection.deploy.prototxt &gt; det_openblas_omp2_benchmark.log &amp;</pre>
之后，使用openblas的双核的caffe性能测试结果将会写到这个det_openblas_omp2_benchmark.log文件里。需要注意我这里给的脚本的路径是在caffe下时候的命令。
<h1>0. MXNet和相关环境变量设置</h1>
我是使用MXNet来做测试，需要注意的是MXNet有三种不同的Engine，在选择inference时候，NaiveEngine速度是最快，也就是说inferece前，需要：
<pre class="lang:sh decode:true">export OMP_NUM_THREADS=1
export MXNET_ENGINE_TYPE=NaiveEngine</pre>
<ul>
	<li>第一行是指定OpenBLAS使用单线程；</li>
	<li>第二行是说使用MXNET的NaiveEngine，这个引擎一般用在debug的时候（可以traceback详细的回调信息等等，使用这个引擎比默认的ThreadedEnginePerDevice引擎快30%~50%）。</li>
	<li>参考：</li>
	<li>Environment Variables — mxnet documentation
http://mxnet.io/how_to/env_var.html</li>
	<li>Where to find the benchmark of mxnet on mobile devices? · Issue #2074 · dmlc/mxnet
https://github.com/dmlc/mxnet/issues/2074</li>
</ul>
在tegraX1上测试某算法在不同线程下的性能。具体如下
<h1>1. for循环</h1>
第一次写的很简单，都是for循环。测试多线程取值在1..4上不同的forward时间，以下是程序。
<pre class="lang:sh decode:true">#!/bin/bash

echo "OMP_NUM_THREADS=4"
export OMP_NUM_THREADS=4
for i in {1..10}
do
    echo "welcome $i times"
    python inception-v3_inference.py
done
echo "======================="

echo "OMP_NUM_THREADS=3"
export OMP_NUM_THREADS=3
for i in {1..10}
do
    echo "welcome $i times"
    python inception-v3_inference.py
done
echo "======================="

echo "OMP_NUM_THREADS=2"
export OMP_NUM_THREADS=2
for i in {1..10}
do
    echo "welcome $i times"
    python inception-v3_inference.py
done
echo "======================="

echo "OMP_NUM_THREADS=1"
export OMP_NUM_THREADS=1
for i in {1..10}
do
    echo "welcome $i times"
    python inception-v3_inference.py
done</pre>
参考：Linux —— Shell编程之变量赋值和引用 - boshuzhang的专栏 - 博客频道 - CSDN.NET
http://blog.csdn.net/boshuzhang/article/details/52208998
<h1>2. 双层for循环</h1>
下面这种写法是将上面的四次重复的for改成了两层for，这样看起来就简洁多了。另外需要注意的是：在<strong><span style="color: #ff0000;">等号的左右不能有空格</span></strong>，否则就会报错，比方下面在export那句话的等号左右为了美观加上空格，报错如下：<span style="color: #ff0000;">export: `=': not a valid identifier</span>
<pre class="lang:sh decode:true ">#!/bin/bash

for thread_num in {1..4}
do
    echo "====== set OMP_NUM_THREADS = $thread_num ======"
    # no space near the equal symbol
    export OMP_NUM_THREADS=$thread_num
    
    for idx in {1..10}
    do  
        echo "[$idx]"
        python run_inference.py
    done
done</pre>
<h1>3. 后台执行与日志写入本地</h1>
如果脚本在服务器上运行时间很长，可以放在后台执行，这样ssh断开也不会影响，同时将打印的日志结果写入到文件中。

此外这个脚本还有一点<strong><span style="color: #ff0000;">正则匹配包含指定内容的行</span></strong>，<strong><span style="color: #ff0000;">if语句</span></strong>的简单用法。另外，<strong><span style="color: #ff0000;">echo的参数中， -e表示开启转义，\c表示不换行</span></strong>。

如下面这个testPerf.sh脚本，在本地创建testPerf.log脚本，使用命令：(./testPerf.sh &gt; ./testPerf.log &amp;)，如果是追加方式，则将&gt;改为&gt;&gt;，即：(./testPerf.sh &gt;&gt; ./testPerf.log &amp;)。&amp;这个字符是通过sub-shell让进程在后台运行HUP（hangup），参考：让进程在后台可靠运行的几种方法：http://www.ibm.com/developerworks/cn/linux/l-cn-nohup/
<pre class="lang:sh decode:true">#!/bin/bash

for mode in {1..2}
do
    if [ $mode = 1 ]; then
        echo "====== GPU MODE ======"
        max_idx=100
    else
        echo "====== CPU MODE ======"
        max_idx=10
    fi  
    # no space near the equal symbol
    # there're two big braces around variable 
    for ((idx = 1; idx &lt;= $max_idx; idx++))
    do
        # '-e' is parameter of echo, support escape characters in command
        # '\c' is an escape character, means cancel newline in this sentence
        echo -e "[$idx]\c"
        if [ $mode = 1 ]; then
            # GPU mode
            ./darknet detect cfg/yolo.cfg yolo.weights data/dog.jpg | grep "Predicted"
        else
            # CPU mode
            ./darknet -nogpu detect cfg/yolo.cfg yolo.weights data/dog.jpg | grep "Predicted"
        fi
    done
done</pre>
<h1>4. 加入打印时间</h1>
加入<span style="color: #ff0000;"><strong>日志时间打印</strong></span>，并将echo统一换成<strong><span style="color: #ff0000;">printf</span></strong>。使用printf替换echo的主要原因如下（摘抄自Shell printf 命令 | 菜鸟教程）：
<ul>
	<li>使用printf的脚本比使用echo移植性好。</li>
	<li>printf 使用引用文本或空格分隔的参数，外面可以在printf中使用格式化字符串，还可以制定字符串的宽度、左右对齐方式等。默认printf不会像 echo 自动添加换行符，我们可以手动添加 \n。</li>
</ul>
<pre class="lang:sh decode:true">#!/bin/bash

for mode in {1..2}
do
    if [ $mode = 1 ]; then
        printf "====== GPU MODE ======\n"
        max_idx=100
    else
        printf "====== CPU MODE ======\n"
        max_idx=10
    fi  
    # no space near the equal symbol
    # there're two spaces around variable 
    for ((idx = 1; idx &lt;= $max_idx; idx++))
    do  
        current_time=$(date +%Y-%m-%d\ %H:%M:%S)
        printf "$current_time [%3d]" ${idx}
        if [ $mode = 1 ]; then
            # GPU mode
            #./darknet detect cfg/yolo.cfg yolo.weights data/dog.jpg | grep "Predicted"
            #./darknet detector test cfg/voc.data cfg/tiny-yolo-voc.cfg tiny-yolo-voc.weights data/dog.jpg | grep "Predicted"
            #./darknet yolo test cfg/yolov1/yolo.cfg yolov1.weights data/dog.jpg | grep "Predicted"
            ./darknet yolo test cfg/yolo9000.cfg yolo9000.weights data/dog.jpg | grep "Predicted"
        else
            # CPU mode
            #./darknet -nogpu detect cfg/yolo.cfg yolo.weights data/dog.jpg | grep "Predicted"
            #./darknet -nogpu detector test cfg/voc.data cfg/tiny-yolo-voc.cfg tiny-yolo-voc.weights data/dog.jpg | grep "Predicted"
            #./darknet -nogpu yolo test cfg/yolov1/yolo.cfg yolov1.weights data/dog.jpg | grep "Predicted"
            ./darknet -nogpu yolo test cfg/yolo9000.cfg yolo9000.weights data/dog.jpg | grep "Predicted"
        fi
    done
done</pre>
<ul>
	<li>Shell printf 命令 | 菜鸟教程（另外还有<span style="color: #ff0000;">格式化字符的方法</span>）
http://www.runoob.com/linux/linux-shell-printf.html</li>
</ul>
<h1>5. 设定和查看CPU频率</h1>
<h2>5.1 tegra</h2>
查看CPU频率：
<pre class="lang:sh decode:true">#!/bin/bash

echo "Running CPU index:"
cat /sys/devices/system/cpu/online

echo "Current CPU clock frequencies:"
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq
cat /sys/devices/system/cpu/cpu1/cpufreq/scaling_cur_freq
cat /sys/devices/system/cpu/cpu2/cpufreq/scaling_cur_freq
cat /sys/devices/system/cpu/cpu3/cpufreq/scaling_cur_freq</pre>
设定tegra X1 CPU频率：
<pre class="lang:sh decode:true">#!/bin/bash
echo 0 &gt; /sys/devices/system/cpu/cpuquiet/tegra_cpuquiet/enable

echo 1 &gt; /sys/devices/system/cpu/cpu0/online
echo 1 &gt; /sys/devices/system/cpu/cpu1/online
echo 1 &gt; /sys/devices/system/cpu/cpu2/online
echo 1 &gt; /sys/devices/system/cpu/cpu3/online

echo performance &gt; /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor</pre>
<h2>5.2 firefly</h2>
<h3>5.2.1 cpufreq-info设置最大频率</h3>
设置前，可以先查看当前频率和可用频率，分别是如下命令（如若发现cpufreq-info命令没有安装，则执行：sudo apt-get install cpufrequtils）：
<pre class="lang:sh decode:true"># 查看当前频率
cpufreq-info | grep 'current CPU'
# 查看可用频率
cpufreq-info | grep 'available'
</pre>
设定A53×4（这四个小核）最大频率（也可参考下面这行改成设定大核最大频率，我没试）：
<pre class="lang:sh decode:true">sudo cpufreq-set -c 0 -g performance</pre>
设置完成后，可以再次通过如下cpufre-info的命令查看当前频率，以检查是否已经达到最大的可用频率。
<h3>5.2.2 自己写脚本设置最大频率</h3>
设定Firefly RK3399（A72×2，A53×4）最大频率（基于tegra设定最大频率修改的）：
<pre class="lang:sh decode:true">#!/bin/bash
#echo 0 &gt; /sys/devices/system/cpu/cpuquiet/tegra_cpuquiet/enable

echo 1 &gt; /sys/devices/system/cpu/cpu0/online
echo 1 &gt; /sys/devices/system/cpu/cpu1/online
echo 1 &gt; /sys/devices/system/cpu/cpu2/online
echo 1 &gt; /sys/devices/system/cpu/cpu3/online
echo 1 &gt; /sys/devices/system/cpu/cpu4/online
echo 1 &gt; /sys/devices/system/cpu/cpu5/online

echo performance &gt; /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
echo performance &gt; /sys/devices/system/cpu/cpu4/cpufreq/scaling_governor</pre>
查看cortex-A72频率：
<pre class="lang:sh decode:true">#!/bin/bash

printf "Running CPU index:"
cat /sys/devices/system/cpu/online
printf "A53 cpu-index:"
cat /sys/devices/system/cpu/cpu0/cpufreq/related_cpus
printf "A72 cpu-index:"
cat /sys/devices/system/cpu/cpu4/cpufreq/related_cpus
printf "\n"


max_cpu_idx=5
printf "Current CPU clock frequencies:\n"
for ((cpu_idx = 0; cpu_idx &lt;= $max_cpu_idx; cpu_idx++))
do
    cur_freq_path=$(printf '/sys/devices/system/cpu/cpu%s/cpufreq/scaling_cur_freq' $cpu_idx)
    cur_freq=$(cat $cur_freq_path)
    printf "cpu[%d]" $cpu_idx
    printf $cur_freq
    printf '\n'
done</pre>
参考：Shell printf 命令 | 菜鸟教程
http://www.runoob.com/linux/linux-shell-printf.html

shell读取文件中的内容，并将其存入到变量中 - sidely的专栏 - 博客频道 - CSDN.NET
http://blog.csdn.net/sidely/article/details/40426999
<h1>6. 指定CPU来执行</h1>
本来是测Cortex-A72（单核和双核）在inception-v3模型上的性能，但是firefly-RK3399上的这两个A72太不稳定，也不清楚是板子自身的问题还是本身A72就是不稳定，反正是一跑脚本，机器就卡死挂掉了。一摸CPU，超级热。

无奈脚本跑不了，下面是脚本testPerf.sh的内容（指定A72的单核和双核，分别测10次），运行的时候就在shell里写(./testPerf.sh &gt;&gt; testPerf.log &amp;)：
<pre class="lang:sh decode:true">#!/bin/bash

# set task on cpu cortex-A72
pid_num=$$
printf $pid_num
taskset -pc 4,5 $pid_num


for cpu_num in {1..2}
do
    printf "using cpu num:%s\n" $cpu_num
    printf "OMP_NUM_THREADS=%s\n" $cpu_num
    export OMP_NUM_THREADS=$cpu_num
    for exec_idx in {1..10}
    do
        if [ $cpu_num = 1 ]; then
            taskset -c 4 python inception-v3_inference-once.py | grep 'take'
        else
            taskset -c 4,5 python inception-v3_inference-once.py | grep 'take'
        fi
    done
done</pre>
测试RK3399上的Cortex-A53（对应CPU索引为0-3，这4个核）多线程的infer性能（每个跑10次）：
<pre class="lang:sh decode:true ">#!/bin/bash

# set task on cortex-A53
for cpu_num in {1..4}
do
    printf "set OMP_NUM_THREADS=%s\n" $cpu_num
    export OMP_NUM_THREADS=$cpu_num
    for exec_idx in {1..10}
    do
        if [ $cpu_num = 1 ]
        then
            taskset -c 0 python run_inference.py
        elif [ $cpu_num = 2 ]
        then
            taskset -c 0,1 python run_inference.py
        elif [ $cpu_num = 3 ]
        then
            taskset -c 0,1,2 python run_inference.py
        elif [ $cpu_num = 4 ]
        then
            taskset -c 0,1,2,3 python run_inference.py
        else
            printf "abnormal\n"
        fi
    done
done
</pre>
测试RK3399上的Cortex-A53（对应CPU索引为4和5，这2个核）多线程的infer性能：
<pre class="lang:sh decode:true ">#!/bin/bash

# set task on cortex-a72
printf "set task on cortex-a72\n"

for cpu_num in {1..2}
do
    printf "set OMP_NUM_THREADS=%s\n" $cpu_num
    export OMP_NUM_THREADS=$cpu_num
    for exec_idx in {1..10}
    do
        if [ $cpu_num = 1 ]
        then
            taskset -c 4 python run_inference.py
        elif [ $cpu_num = 2 ]
        then
            taskset -c 4,5 python run_inference.py
        else
            printf "abnormal\n"
        fi
    done
done</pre>
<h1>7. 调节和监控GPU主频</h1>
锁频模式（<span style="color: #ff0000;">但是被吐槽这个不是锁频，有待商议</span>），可以指定GPU的时钟速度，使用如下命令开启：
<pre class="lang:sh decode:true">ysh329@ubuntu:~/code$ sudo nvidia-smi -pm 1
[sudo] password for yuanshuai: 
Persistence mode is already Enabled for GPU 00000002:01:00.0.
Persistence mode is already Enabled for GPU 00000003:01:00.0.
Persistence mode is already Enabled for GPU 0000000A:01:00.0.
Persistence mode is already Enabled for GPU 0000000B:01:00.0.
All done.</pre>
高性能模式（这个频率不确定，有的chip频率就会变）：
<pre class="lang:sh decode:true">nvidia-smi -q -d PERFORMANCE</pre>
设定memory和graphics的主频（应该就是锁死了，我测了矩阵乘法的同时，实时监控Clocks发现跑程序时候的主频就是设定的固定不变的），设定如下：
<pre class="lang:sh decode:true">$ #nvidia-smi  --applications-clocks=&lt;memory,graphics&gt; 
$ sudo nvidia-smi  --applications-clocks=715,1480
Applications clocks set to "(MEM 715, SM 1480)" for GPU 00000002:01:00.0
Applications clocks set to "(MEM 715, SM 1480)" for GPU 00000003:01:00.0
Applications clocks set to "(MEM 715, SM 1480)" for GPU 0000000A:01:00.0
Applications clocks set to "(MEM 715, SM 1480)" for GPU 0000000B:01:00.0
All done.</pre>
如果你设置主频的值不在支持范围之内．会提示你用命令nvidia-smi -q -d SUPPORTED_CLOCKS来查看支持的频率值。
<pre class="lang:sh decode:true">yuanshuai@ubuntu:~$ sudo nvidia-smi  --applications-clocks=715,111
Specified clock combination "(MEM 715, SM 111)" is not supported for GPU 00000002:01:00.0. Run 'nvidia-smi -q -d SUPPORTED_CLOCKS' to see list of supported clock combinations
Treating as warning and moving on.
Specified clock combination "(MEM 715, SM 111)" is not supported for GPU 00000003:01:00.0. Run 'nvidia-smi -q -d SUPPORTED_CLOCKS' to see list of supported clock combinations
Treating as warning and moving on.
Specified clock combination "(MEM 715, SM 111)" is not supported for GPU 0000000A:01:00.0. Run 'nvidia-smi -q -d SUPPORTED_CLOCKS' to see list of supported clock combinations
Treating as warning and moving on.
Specified clock combination "(MEM 715, SM 111)" is not supported for GPU 0000000B:01:00.0. Run 'nvidia-smi -q -d SUPPORTED_CLOCKS' to see list of supported clock combinations
Treating as warning and moving on.
All done.</pre>
监控GPU实时频率（下面是4块GPU中的第一块因为下标-i参数是0，-q代表query查询，-i代表index第几块gpu默认不带该参数列出所有gpu，实时主频是看Clocks这个section）：
<pre class="lang:sh decode:true">watch -n .1 nvidia-smi -q -i 0 --display=CLOCK

==============NVSMI LOG==============

Timestamp                           : Thu Aug 31 20:25:27 2017
Driver Version                      : 384.59
Attached GPUs                       : 4
GPU 00000002:01:00.0
    Clocks
        Graphics                    : 405 MHz
        SM                          : 405 MHz
        Memory                      : 715 MHz
        Video                       : 835 MHz
    Applications Clocks
        Graphics                    : 1480 MHz
        Memory                      : 715 MHz
    Default Applications Clocks
        Graphics                    : 1328 MHz
        Memory                      : 715 MHz
    Max Clocks
        Graphics                    : 1480 MHz
        SM                          : 1480 MHz                                                                                                                                                                                                                                                                          [0/455]
        Memory                      : 715 MHz
        Video                       : 1480 MHz
    Max Customer Boost Clocks
        Graphics                    : N/A
    SM Clock Samples
        Duration                    : 947568.53 sec
        Number of Samples           : 100
        Max                         : 1480 MHz
        Min                         : 405 MHz
        Avg                         : 410 MHz
    Memory Clock Samples
        Duration                    : 947572.97 sec
        Number of Samples           : 100
        Max                         : 715 MHz
        Min                         : 715 MHz
        Avg                         : 715 MHz
    Clock Policy
        Auto Boost                  : N/A
        Auto Boost Default          : N/A
</pre>
观察是否是最大主频可以把/usr/local/cuda/sample目录整个拷贝出到自己目录（否则make时候会有helper_functions.h：No such file的错误，因这个h文件在该sample/common下），然后进入里面的矩阵运算samples/0_Simple/matrixMul的例子，make一下，然后跑matrixMul，用带watch的命令实时看看主频是否达到设定的主频。

参考（这个写的很全面）：nvidia-smi: Control Your GPUs | Microway
https://www.microway.com/hpc-tech-tips/nvidia-smi_control-your-gpus/

<hr />

&nbsp;
<ul>
	<li>参考</li>
	<li>Shell printf 命令 | 菜鸟教程
http://www.runoob.com/linux/linux-shell-printf.html</li>
	<li>利用taskset命令，将程序绑定在某个CPU上运行_CAECFD创新工场_新浪博客
http://blog.sina.com.cn/s/blog_6817db3a0100wijj.html</li>
	<li>使用taskset命令来限制进程的CPU - killkill - 博客园
http://www.cnblogs.com/killkill/archive/2012/04/08/2437960.html</li>
	<li>如何指定进程运行的CPU(命令行 taskset) - 写代码的小孩 - 博客频道 - CSDN.NET
http://blog.csdn.net/xluren/article/details/43202201</li>
	<li>Shell脚本中获取进程ID的方法_linux shell_脚本之家
http://www.jb51.net/article/62370.htm</li>
</ul>
