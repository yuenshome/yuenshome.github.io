[![header](../../../assets/header21.jpg)](https://yuenshome.github.io)

# MXNet将旧版Symbol转换为新版

跑inception-v3的模型时候，mxnet 0.93打出如下log：
<pre class="lang:sh decode:true">src/nnvm/legacy_json_util.cc:175: Loading symbol saved by previous version v0.8.0. Attempting to upgrade...</pre>
看来之前的symbol模型是一个较早版MXNet的接口保存的，这样每次predict之前都有一个旧到新模型转换的过程，感觉很麻烦，感觉上应该比较耗时间吧（后面会测一下这个转换的时间，其实这个<strong><span style="color: #ff0000;">转换时间模型越大时间越长，且gpu和cpu作为context时是不同的</span></strong>，不过像vgg16这样的模型，CPU为context时，其转化时间8.2秒，保存这个模型则花了24秒，当GPU为context时，起转化时间为1.93秒，保存模型的时间为21秒）。

所以在此想对模型进行upgrade，因为加载pretrained model需要两个文件：二进制的网络权重参数文件params和网络架构的配置json文件（对于inception-v3而言，就是Inception-v3-0001.params和Inception-v3-symbol.json，0001表示基于1次epoch训练完成的参数）。[toc]
<h1>1. 转换的实现思路：</h1>
<ol>
	<li>既然每次跑模型的时候会给出upgrade的提示，那就意味着predict之前，模型已经升级了，只要在这之后另存为新模型即可。下次load模型选用这次另存为的转换过的模型，就不会出现升级的警告了。</li>
	<li>如果上面这条另存为之路走不通（没有对应的保存或者转换），那就要找源码中做这个转换过程的部分，插入一个保存新模型的过程。其实，也可以在识别出是旧模型后，直接对旧模型升级后的新模型进行另存为，但是就害怕用户不愿意（比方说空间比较小，模型文件很大等等情况）。
<!--more--></li>
</ol>
<h1>2. 定位模型转换代码所处位置</h1>
下面是inception-v3在inference的代码，删除掉不重要的行，更具体的基于pretrained model进行predict的代码可以参考MXNet的文档（Predict with pre-trained models，http://mxnet.io/tutorials/python/predict_imagenet.html）：
<pre class="lang:python decode:true"># coding: utf-8
import os, sys
import urllib
import mxnet as mx
import time
# 导入opencv库后面会用到cv2对图像进行resize
sys.path.append('~/install/opencv-3.1.0/lib/python2.7/dist-packages')

# 加载label文件
with open('synset.txt', 'r') as f:
    synsets = [l.rstrip() for l in f]

# 读取模型参数和网络配置文件
# 绑定参数分配空间
sym, arg_params, aux_params = mx.model.load_checkpoint('Inception-7-upgraded-by-save_checkpoint-fun', 1)
mod = mx.mod.Module(symbol=sym, context=mx.cpu(0))
mod.bind(for_training=False, data_shapes=[('data', (1, 3, 299, 299))])
mod.set_params(arg_params, aux_params)

import cv2
import numpy as np
from collections import namedtuple
Batch = namedtuple('Batch', ['data'])

def predict(filename, mod, synsets):
    # 加载图像并转换颜色空间和缩放
    img = cv2.cvtColor(cv2.imread(filename), cv2.COLOR_BGR2RGB)
    if img is None:
        return None
    img = cv2.resize(img, (299, 299))
    img = np.swapaxes(img, 0, 2)
    img = np.swapaxes(img, 1, 2)
    img = img[np.newaxis, :]

    # 做100次predict并打印出平均每次的时间
    start = time.time()
    for i in range(100):
         mod.forward(Batch([mx.nd.array(img)]))
         prob = mod.get_outputs()[0].asnumpy()
         prob = np.squeeze(prob)
         a = np.argsort(prob)[::-1]
    print "each image take {0}".format((time.time()-start)/100)
    #for i in a[0:5]:
    #    print('probability=%f, class=%s' % (prob[i], synsets[i]))

# 加载图片并进行预测
img_path = './Cat-hd-wallpapers.jpg'
predict(img_path, mod, synsets)</pre>
通过对源程序定位，发现进行upgrade的过程是在这执行下面这句时进行的：
<pre class="lang:python decode:true">mx.model.load_checkpoint('Inception-7', 1)</pre>
<h1>3.查找文档并查看源码</h1>
<h2>3.1 文档</h2>
那么可以在此步骤之后对模型另存为。根据这个mx.model.load_checkpoint函数（既然有load，那肯定有save，以及save_checkpoint），查了下model部分的文档，发现两个函数：
<h3><code class="descname">3.1.1 save</code><span class="sig-paren">(</span><em>prefix</em>, <em>epoch=None</em><span class="sig-paren">)</span></h3>
Checkpoint the model checkpoint into file. You can also use pickle to do the job if you only work on python. The advantage of load/save is the file is language agnostic. This means the file saved using save can be loaded by other language binding of mxnet. You also get the benefit being able to directly load/save from cloud storage(S3, HDFS)
<table class="docutils field-list" frame="void" rules="none"><colgroup> <col class="field-name" /> <col class="field-body" /></colgroup>
<tbody valign="top">
<tr class="field-odd field">
<th class="field-name">Parameters:</th>
<td class="field-body"><strong>prefix</strong> (<em>str</em>) – Prefix of model name.</td>
</tr>
</tbody>
</table>
<p class="rubric">Notes</p>

<ul class="simple">
	<li><code class="docutils literal"><span class="pre">prefix-symbol.json</span></code> will be saved for symbol.</li>
	<li><code class="docutils literal"><span class="pre">prefix-epoch.params</span></code> will be saved for parameters.</li>
</ul>
<dl class="function"><dt id="mxnet.model.save_checkpoint">
<h3><code class="descclassname">3.1.2 mxnet.model.</code><code class="descname"><span class="highlighted">save_checkpoint</span></code><span class="sig-paren">(</span><em>prefix</em>, <em>epoch</em>, <em>symbol</em>, <em>arg_params</em>, <em>aux_params</em><span class="sig-paren">)</span></h3>
</dt><dd>Checkpoint the model data into file.
<table class="docutils field-list" frame="void" rules="none"><colgroup> <col class="field-name" /> <col class="field-body" /></colgroup>
<tbody valign="top">
<tr class="field-odd field">
<th class="field-name">Parameters:</th>
<td class="field-body">
<ul class="first last simple">
	<li><strong>prefix</strong> (<em>str</em>) – Prefix of model name.</li>
	<li><strong>epoch</strong> (<em>int</em>) – The epoch number of the model.</li>
	<li><strong>symbol</strong> (<a class="reference internal" title="mxnet.symbol.Symbol" href="http://mxnet.io/api/python/symbol.html#mxnet.symbol.Symbol"><em>Symbol</em></a>) – The input symbol</li>
	<li><strong>arg_params</strong> (<em>dict of str to NDArray</em>) – Model parameter, dict of name to NDArray of net’s weights.</li>
	<li><strong>aux_params</strong> (<em>dict of str to NDArray</em>) – Model parameter, dict of name to NDArray of net’s auxiliary states.</li>
</ul>
</td>
</tr>
</tbody>
</table>
<p class="rubric">Notes</p>

<ul class="simple">
	<li><code class="docutils literal"><span class="pre">prefix-symbol.json</span></code> will be saved for symbol.</li>
	<li><code class="docutils literal"><span class="pre">prefix-epoch.params</span></code> will be saved for parameters.</li>
</ul>
</dd></dl>猜想说不定，这个save_checkpoint函数和save函数有关，可能save_checkpoint函数的内部是基于save实现的，或者反之。我们下面可以查看下源码进一步探索下。
<h2>3.2 源码</h2>
在./mxnet/python/mxnet目录下，根据在命令行输入<b>grep -rn load_checkpoint</b>（不需要前后加上*，或者找save_checkpoint或者save）来查找文件内容找到对应源码。下面这个是model.py中save，load，load_checkpoint和save_checkpoint文件：
<h3>3.2.1 mxnet.model.save（类model的函数）</h3>
<pre class="lang:python decode:true" title="Model.save">@staticmethod
def save(self, prefix, epoch=None):
"""Checkpoint the model checkpoint into file.
You can also use pickle to do the job if you only work on python.
The advantage of load/save is the file is language agnostic.
This means the file saved using save can be loaded by other language binding of mxnet.
You also get the benefit being able to directly load/save from cloud storage(S3, HDFS)

Parameters
----------
prefix : str
    Prefix of model name.

Notes
-----
- ``prefix-symbol.json`` will be saved for symbol.
- ``prefix-epoch.params`` will be saved for parameters.
"""
    if epoch is None:
        epoch = self.num_epoch
    assert epoch is not None
    save_checkpoint(prefix, epoch, self.symbol, self.arg_params, self.aux_params)
</pre>
可以看到save函数调用了save_checkpoint，save内部的实现先判断是否epoch为None，再直接调用save_checkpoint，非常简明。另外save和load模型文件（模型符号文件：prefix-symbol.json，类似caffe的prototxt；模型参数文件：prefix-epoch.params）也可以直接在S3和HDFS上进行。另外函数注释里也说明，如果是python使用者，也可以直接用python的pickle来实现模型的保存。

下面就看看同一个文件里的save_checkpoint的实现。
<h3>3.2.2 mxnet.model.save_checkpoint</h3>
<pre class="lang:python decode:true">def save_checkpoint(prefix, epoch, symbol, arg_params, aux_params):
    """Checkpoint the model data into file.

    Parameters
    ----------
    prefix : str
        Prefix of model name.
    epoch : int
        The epoch number of the model.
    symbol : Symbol
        The input symbol
    arg_params : dict of str to NDArray
        Model parameter, dict of name to NDArray of net's weights.
    aux_params : dict of str to NDArray
        Model parameter, dict of name to NDArray of net's auxiliary states.
    Notes
    -----
    - ``prefix-symbol.json`` will be saved for symbol.
    - ``prefix-epoch.params`` will be saved for parameters.
    """
    if symbol is not None:
        symbol.save('%s-symbol.json' % prefix)

    save_dict = {('arg:%s' % k) : v.as_in_context(cpu()) for k, v in arg_params.items()}
    save_dict.update({('aux:%s' % k) : v.as_in_context(cpu()) for k, v in aux_params.items()})
    param_name = '%s-%04d.params' % (prefix, epoch)
    nd.save(param_name, save_dict)
    logging.info('Saved checkpoint to \"%s\"', param_name)</pre>
里面的实现也很简单，主要是对两个参数进行保存（symbol和params）。

代码先检查symbol是否为None，若不是，<strong><span style="color: #ff6600;">首先对模型的symbol进行保存</span></strong>（<span style="color: #ff0000;">具体的实现过程还是要进一步探究mxnet.symbol.save，具体如下</span>）。
<pre class="lang:python decode:true" title="mxnet.symbol的类内函数">    def save(self, fname):
        """Save symbol into file.

        You can also use pickle to do the job if you only work on python.
        The advantage of load/save is the file is language agnostic.
        This means the file saved using save can be loaded by other language binding of mxnet.
        You also get the benefit being able to directly load/save from cloud storage(S3, HDFS)

        Parameters
        ----------
        fname : str
            The name of the file
            - s3://my-bucket/path/my-s3-symbol
            - hdfs://my-bucket/path/my-hdfs-symbol
            - /path-to/my-local-symbol

        See Also
        --------
        symbol.load : Used to load symbol from file.
        """
        if not isinstance(fname, string_types):
            raise TypeError('fname need to be string')
        check_call(_LIB.MXSymbolSaveToFile(self.handle, c_str(fname)))</pre>
可以看到这个mxnet.symbol.save方法也提到了这里的save是语言无关的，方法的实现很简单，就是先检查fname的类型是否是string_types。在该代码文件中查了下，这个string_types来自：from .base import string_types，然后这个.base是上一层目录下的base.py文件中的，有这么一句：string_types = basestring。在ipython中查到对应的basestring有如下解释：
<ul>
	<li>Docstring: Type basestring cannot be instantiated; it is the base for str and unicode.</li>
	<li>Type: type</li>
</ul>
另外，最后的<span style="color: #ff0000;">check_all方法只是检查这个调用是否成功</span>（成功调用的话里面的值为0，否则就会抛出MXNetError错误。其中的，<span style="color: #ff0000;">_LIB.MXSymbolSaveToFile(self.handle, c_str(fname))是比较关键的地方</span>，这个API是将传入的fname，即对symbol文件进行保存，<span style="color: #ff0000;">在后面我们会继续探究这个_LIB.MXSymbolSaveToFile(self.handle, c_str(fname))</span>），也在该文件中的开头找到from .base import check_call, MXNetError，上一层文件夹的base.py中有如下check_all的方法定义：
<pre class="lang:python decode:true">def check_call(ret):
    """Check the return value of C API call

    This function will raise exception when error occurs.
    Wrap every API call with this function

    Parameters
    ----------
    ret : int
        return value from API calls
    """
    if ret != 0:
        raise MXNetError(py_str(_LIB.MXGetLastError()))</pre>
该方法当遇到错误时会抛出API调用异常（<strong><span style="color: #ff0000;">调用正常ret值为0，异常为-1，在后面的分析中有写，这里不重要可以跳过</span></strong>），也就是当传入的变量ret!=0时，抛出MXNetError，其中py_str = lambda x: x，查到_LIB为_LIB = _load_lib()，改行代码的上方注释是# library instance of mxnet。在该文件找到_load_lib()方法：
<pre class="lang:python decode:true">def _load_lib():
    """Load libary by searching possible path."""
    lib_path = libinfo.find_lib_path()
    lib = ctypes.CDLL(lib_path[0], ctypes.RTLD_GLOBAL)
    # DMatrix functions
    lib.MXGetLastError.restype = ctypes.c_char_p
    return lib</pre>
（<strong><span style="color: #ff0000;">这里不重要可以跳过</span></strong>）该函数会去读取可能搜到的所有路径来加载库，不关心第一句，第二中的ctypes.CDLL中的ctype来自该文件开头的import ctypes，在自己本地开了ipython，import ctypes，?ctypes有如下信息：
<ul>
	<li>Type: module</li>
	<li>String form: &lt;module 'ctypes' from '/usr/local/lib/anaconda2/lib/python2.7/ctypes/__init__.pyc'&gt;</li>
	<li>File: /usr/local/lib/anaconda2/lib/python2.7/ctypes/__init__.py</li>
	<li>Docstring: create and manipulate C data types in Python</li>
	<li>同时，我查了下ctypes.RTLD_GLOBAL：</li>
	<li>In [11]: ?ctypes.RTLD_GLOBAL
Type: int
String form: 256
Docstring:
int(x=0) -&gt; int or long
int(x, base=10) -&gt; int or longConvert a number or string to an integer, or return 0 if no arguments
are given. If x is floating point, the conversion truncates towards zero.
If x is outside the integer range, the function returns a long instead.If x is not a number or if base is given, then x must be a string or
Unicode object representing an integer literal in the given base. The
literal can be preceded by '+' or '-' and be surrounded by whitespace.
The base defaults to 10. Valid bases are 0 and 2-36. Base 0 means to
interpret the base from the string as an integer literal.
&gt;&gt;&gt; int('0b100', base=0)
4</li>
	<li>另外，这个ctypes.c_char_p的解释如下：</li>
	<li>In [13]: ?ctypes.c_char_p
Docstring: &lt;no docstring&gt;
File: /usr/local/lib/anaconda2/lib/python2.7/ctypes/__init__.py
Type: PyCSimpleType</li>
</ul>
能看出来ctypes是用来创建和操作C语言的数据类型的python包。至此，我们差不多把MXNet在保存模型的Python部分代码看完了。更多的还需要进一步深入ctypes等Python与C相关调用的部分。

（<strong><span style="color: #ff0000;">这里开始很重要</span></strong>）下面继续说一下mxnet.symbol.save其中的check_all的输入参数，即<span style="color: #ff0000;"><strong>_LIB.MXSymbolSaveToFile(self.handle, c_str(fname))</strong>，这个比较关键的地方是将symbol进行保存，返回的值为0或-1（0表示成功调用，成功调用时check_all的输入即为0，不会抛出MXNetError的错误）。</span>在mxnet.symbol文件中开头找到from .base import _LIB, numeric_types，在同一目录下找到base.py中的_LIB = _load_lib()，再进一步定位到_load_lib()：
<pre class="lang:python decode:true ">def _load_lib():
    """Load libary by searching possible path."""
    lib_path = libinfo.find_lib_path()
    lib = ctypes.CDLL(lib_path[0], ctypes.RTLD_GLOBAL)
    # DMatrix functions
    lib.MXGetLastError.restype = ctypes.c_char_p
    return lib</pre>
这个_load_lib()没啥说的（前面也说过了），就是加载load进C语言写的一些包，供Python来调用，借助Python的ctypes包可以让Python创建和操作C语言的数据类型。再在./mxnet/python/目录下就没有其它关于“MXSymbolSaveToFile”的发现了，我回到./mxnet根目录了下再次查找grep -rn MXSymbolSaveToFile，找到除了Python目录下以外另外的几个库和C文件也包含MXSymbolSaveToFile：
<pre class="lang:sh decode:true">yuens@Spark:~/Software/mxnet$ grep -rn MXSymbolSaveToFile
python/mxnet/symbol.py:657:        check_call(_LIB.MXSymbolSaveToFile(self.handle, c_str(fname)))
python/build/lib.linux-x86_64-2.7/mxnet/symbol.py:657:        check_call(_LIB.MXSymbolSaveToFile(self.handle, c_str(fname)))
include/mxnet/c_api.h:623:MXNET_DLL int MXSymbolSaveToFile(SymbolHandle symbol, const char *fname);
R-package/src/symbol.cc:122:  MX_CALL(MXSymbolSaveToFile(handle_, fname.c_str()));
Binary file lib/libmxnet.so matches
Binary file lib/libmxnet.a matches
Binary file build/src/c_api/c_api_symbolic.o matches
scala-package/native/src/main/native/ml_dmlc_mxnet_native_c_api.cc:1365:  int ret = MXSymbolSaveToFile(reinterpret_cast&lt;SymbolHandle&gt;(symbolPtr), fname);
src/c_api/c_api_symbolic.cc:333:int MXSymbolSaveToFile(SymbolHandle symbol, const char *fname) {
</pre>
忽略掉Python、R、Scala相关的，就剩下这几条：
<ul>
	<li>Binary file lib/libmxnet.so matches：.so文件是shared object,用于动态连接的,和windows的dll差不多，使用时才载入。</li>
	<li>Binary file lib/libmxnet.a matches：.a文件是好多个.o合在一起,用于静态连接 ，即STATIC mode，a文件作为一个库为外部程序提供函数，接口。多个.a可以链接生成一个exe的可执行文件。</li>
	<li>Binary file build/src/c_api/c_api_symbolic.o matches：.o文件，即object文件，俗称目标文件，是编译产生的目标文件，有一定格式的二进制文件，除了头信息，debug信息之外，仔细观察，实际上就是平台的机器码。相当于win下的obj文件，可以理解为编译了一半的二进制数据，一个.c或.cpp文件对应一个.o文件。</li>
	<li><span style="color: #ff0000;">include/mxnet/c_api.h:623:MXNET_DLL int MXSymbolSaveToFile(SymbolHandle symbol, const char *fname);</span></li>
	<li><span style="color: #ff0000;">src/c_api/c_api_symbolic.cc:333:int MXSymbolSaveToFile(SymbolHandle symbol, const char *fname) {</span></li>
</ul>
那么重点就在于最后两个了，<strong><span style="color: #ff0000;">先去看这个include/mxnet/c_api.h文件（.h文件是头文件</span></strong>，内含函数声明、宏定义、结构体定义等内容）。定位到623行，可以看到周围也是有关Symbol操作的操作：
<pre class="lang:haskell decode:true">MXNET_DLL int MXSymbolCreateFromFile(const char *fname, SymbolHandle *out);
/*!
 * \brief Load a symbol from a json string.
 * \param json the json string.
 * \param out the output symbol.
 * \return 0 when success, -1 when failure happens
 */
MXNET_DLL int MXSymbolCreateFromJSON(const char *json, SymbolHandle *out);
/*!
 * \brief Save a symbol into a json file.
 * \param symbol the input symbol.
 * \param fname the file name.
 * \return 0 when success, -1 when failure happens
 */
MXNET_DLL int MXSymbolSaveToFile(SymbolHandle symbol, const char *fname);
/*!
 * \brief Save a symbol into a json string
 * \param symbol the input symbol.
 * \param out_json output json string.
 * \return 0 when success, -1 when failure happens
 */
MXNET_DLL int MXSymbolSaveToJSON(SymbolHandle symbol, const char **out_json);
/*!
 * \brief Free the symbol handle.
 * \param symbol the symbol
 * \return 0 when success, -1 when failure happens
 */</pre>
MXNET_DLL int MXSymbolSaveToFile(SymbolHandle symbol, const char *fname);下面的注释写的是将一个symbol保存为json字符串，输入参数是symbol和symbol输出将保存的文件名，返回0表示成功，失败则返回-1。

好了，看完了include/mxnet/c_api.h头文件，我们再去看看<strong><span style="color: #ff0000;">src/c_api/c_api_symbolic.cc文件</span></strong>（.cc文件是Linux/Unix下为C++源文件的默认扩展名，与.cpp(windows下)一个意思，用GCC/G++在 Linux/Unix下可以打开和编译，用一般的记事本就可以打开和编辑。）
<pre class="lang:c decode:true">int MXSymbolSaveToFile(SymbolHandle symbol, const char *fname) {
  nnvm::Symbol *s = static_cast&lt;nnvm::Symbol*&gt;(symbol);
  API_BEGIN();
  std::unique_ptr&lt;dmlc::Stream&gt; fo(dmlc::Stream::Create(fname, "w"));
  dmlc::ostream os(fo.get());
  os &lt;&lt; nnvm::pass::SaveJSON(Symbol2Graph(*s));
  // reset file pointer, force flush
  os.set_stream(nullptr);
  API_END(); 
  // 对应API_END()源码位置在同目录下的c_api_common.h中是一个宏定义
  //#define API_END() } catch(dmlc::Error &amp;_except_) { return MXAPIHandleException(_except_); } return 0; &amp;nbsp;// NOLINT(*)
}

// 下面这个是与MXSymbolSaveToFile有一点点关系的SaveToJSON函数
// 两个写法很相像
int MXSymbolSaveToJSON(SymbolHandle symbol, const char **out_json) {
  nnvm::Symbol *s = static_cast&lt;nnvm::Symbol*&gt;(symbol);
  MXAPIThreadLocalEntry *ret = MXAPIThreadLocalStore::Get();
  API_BEGIN();
  ret-&gt;ret_str = nnvm::pass::SaveJSON(Symbol2Graph(*s));
  *out_json = ret-&gt;ret_str.c_str();
  API_END();
}</pre>
可以在int MXSymbolSaveToFile(SymbolHandle symbol, const char *fname)中看到，<span style="color: #ff0000;">比较关键的一句是：os &lt;&lt; nnvm::pass::SaveJSON(Symbol2Graph(*s));</span>，<strong><span style="color: #ff0000;">我们进一步去查看这个nnvm::pass::SaveJSON和Symbol2Graph方法</span></strong>（MXSymbolSaveToJSON中的关键位置也有这个nnvm::pass::SaveJSON方法），<strong><span style="color: #ff0000;">通过在./mxnet根目录下检索grep -rn SaveToJSON得到两个比较值得探索的地方</span></strong>：
<ul>
	<li><strong><span style="color: #ff6600;">第一个从SaveToJSON拓展出的地方：</span></strong>nnvm/include/nnvm/pass_functions.h:39:inline std::string SaveJSON(Graph graph) {
具体如下：
<ul>
	<li>
<pre class="lang:c decode:true">/*!
 * \brief Save a graph to json, redirects to "SaveJSON" pass.
 * \param graph The graph to be saved as json format.
 * \return The json string.
 */
inline std::string SaveJSON(Graph graph) {
  Graph ret = ApplyPass(std::move(graph), "SaveJSON");
  return ret.GetAttr&lt;std::string&gt;("json");
}
</pre>
</li>
</ul>
inline内联函数，其返回结果类型为string的SaveJSON函数，它应用ApplyPass方法将Graph类型的变量，转存成数据图存储成json格式，重定向 SaveJSON 的通路。下面是./mxnet/nnvm/include/nnvm/graph.hGraph数据结构以及（后面的一个代码）ApplyPass方法的代码：
<pre class="lang:c decode:true">/*!
 * \brief Symbolic computation graph.
 *  This is the intermediate representation for optimization pass.
 */
class Graph {
 public:
  /*! \brief outputs of the computation graph. */
  std::vector&lt;NodeEntry&gt; outputs;
  /*!
   * \brief attributes of a graph
   *  Note that attribute is shared pointer and can be shared across graphs.
   *
   *  It is highly recommended to keep each attribute immutable.
   *  It is also safe to implement an copy-on-write semnatics.
   *
   *  Copy when shared_ptr.unique is not true, while reuse original space
   *  when shared_ptr.unique is true.
   */
  std::unordered_map&lt;std::string, std::shared_ptr&lt;any&gt; &gt; attrs;
  /*!
   * \brief Get the immutable attribute from attrs.
   * \param attr_name the name of the attribute
   * \return the reference to corresponding attribute
   * \tparam T the type of the attribute.
   */
  template&lt;typename T&gt;
  inline const T&amp; GetAttr(const std::string&amp; attr_name) const;
  /*!
   * \brief Get a move copy of the attribute, implement copy on write semantics.
   *  The content is moved if the reference counter of shared_ptr is 1.
   *  The attribute is erased from attrs after the call.
   *
   * \param attr_name the name of the attribute
   * \return a new copy of the corresponding attribute.
   * \tparam T the type of the attribute.
   */
  template&lt;typename T&gt;
  inline T MoveCopyAttr(const std::string&amp; attr_name);
  /*!
   * \brief get a indexed graph of current graph, if not exist, create it on demand
   * \return The indexed graph.
   * \sa IndexedGraph
   */
  const IndexedGraph&amp; indexed_graph();

 private:
  // internal structure of indexed graph
  std::shared_ptr&lt;const IndexedGraph&gt; indexed_graph_;
};</pre>
同时也找到./mxnet/nnvm/include/nnvm/pass.h中的ApplyPass方法代码：
<pre class="lang:c++ decode:true">inline Graph ApplyPass(Graph src, const std::string&amp; pass) {
  return ApplyPasses(src, {pass});
}</pre>
以及在./mxnet/nnvm/src/core/pass.cc找到ApplyPasses方法（如下），传入的参数分别为std::move(graph)和"SaveJSON"：
<pre class="lang:c++ decode:true">Graph ApplyPasses(Graph g,
                  const std::vector&lt;std::string&gt;&amp; pass) {
  std::vector&lt;const PassFunctionReg*&gt; fpass;
  for (auto&amp; name : pass) {
    auto* reg = dmlc::Registry&lt;PassFunctionReg&gt;::Find(name);
    CHECK(reg != nullptr)
        &lt;&lt; "Cannot find pass " &lt;&lt; name &lt;&lt; " in the registry";
    fpass.push_back(reg);
  }

  for (auto r : fpass) {
    for (auto&amp; dep : r-&gt;graph_attr_dependency) {
      if (g.attrs.count(dep) == 0) {
        auto* pass_dep = FindPassDep(dep);
        std::string msg;
        if (pass_dep != nullptr) {
          msg = " The attribute is provided by pass " + pass_dep-&gt;name;
        }
        LOG(FATAL) &lt;&lt; "Graph attr dependency " &lt;&lt; dep 
                   &lt;&lt; " is required by pass " &lt;&lt; r-&gt;name
                   &lt;&lt; " but is not available "
                   &lt;&lt; msg;
      }   
    }   
    g = r-&gt;body(std::move(g));
  }

  return g;
}</pre>
&nbsp;</li>
	<li><strong><span style="color: #ff6600;">第二个从SaveToJSON拓展出的地方：./mxnet/</span></strong>nnvm/src/pass/saveload_json.cc:205:Graph SaveJSON(Graph src) {
具体如下：
<pre class="lang:c decode:true">// save a graph to json
Graph SaveJSON(Graph src) {
  JSONGraph jgraph;
  jgraph.attrs = src.attrs;
  std::unordered_map&lt;Node*, uint32_t&gt; node2index;
  jgraph.node_row_ptr.push_back(0);
  DFSVisit(src.outputs, [&amp;node2index, &amp;jgraph](const NodePtr&amp; n) {
      uint32_t nid = static_cast&lt;uint32_t&gt;(jgraph.nodes.size());
      node2index[n.get()] = nid;
      if (n-&gt;is_variable()) {
        jgraph.arg_nodes.push_back(nid);
      }   
      JSONNode jnode;
      jnode.node = n;
      jnode.inputs.reserve(n-&gt;inputs.size());
      for (const NodeEntry&amp; e : n-&gt;inputs) {
        jnode.inputs.emplace_back(
            JSONNode::Entry{node2index.at(e.node.get()), e.index, e.version});
      }   
      for (const NodePtr&amp; c : n-&gt;control_deps) {
        jnode.control_deps.push_back(node2index.at(c.get()));
      }   
      jgraph.node_row_ptr.push_back(
          jgraph.node_row_ptr.back() + n-&gt;num_outputs());
      jgraph.nodes.emplace_back(std::move(jnode));
    }); 

  for (const NodeEntry&amp; e : src.outputs) {
    jgraph.heads.push_back(
        JSONNode::Entry{node2index.at(e.node.get()), e.index, e.version});
  }

  std::ostringstream os; 
  dmlc::JSONWriter writer(&amp;os);
  jgraph.Save(&amp;writer);
  Graph ret;
  ret.attrs["json"] = std::make_shared&lt;any&gt;(os.str());
  return ret;
}</pre>
&nbsp;

&nbsp;</li>
</ul>
再次在from .base import _LIB, numeric_types回到3.2.2 mxnet.model.save_checkpoint开头的save_checkpoint函数中，上文一开始说道保存模型的symbol文件，<span style="color: #ff6600;"><strong>另一个要保存的是模型的params文件（arg_params）</strong></span>，因为params是网络权重每层有对应的名字和值，所以在使用中我们是以Python的dict形式，保存的时建立字典，该过程是对<span style="color: #ff0000;">arg_params</span>遍历所有&lt;key, value&gt;，用key初始化这个字典的同时也把value保存其中。保存时会先将数据转移到CPU中（查到这个as_in_context是类mxnet.ndarray.NDArray的实例的方法，大概看下下面的文档就不深究了）。

<dl class="class"><dd><dl class="method"><dt id="mxnet.ndarray.NDArray.as_in_context"><code class="descname"><span class="highlighted">as_in_context</span></code><span class="sig-paren">(</span><em>context</em><span class="sig-paren">)</span><a class="headerlink" title="Permalink to this definition" href="http://mxnet.io/zh/api/python/ndarray.html?highlight=as_in_context#mxnet.ndarray.NDArray.as_in_context">¶</a></dt><dd>Return an <cite>NDArray</cite> that lives in the target context. If the array is already in that context, <cite>self</cite> is returned. Otherwise, a copy is made.
<table class="docutils field-list" frame="void" rules="none"><colgroup> <col class="field-name" /> <col class="field-body" /></colgroup>
<tbody valign="top">
<tr class="field-odd field">
<th class="field-name">Parameters:</th>
<td class="field-body"><strong>context</strong> (<a class="reference internal" title="mxnet.context.Context" href="http://mxnet.io/zh/api/python/ndarray.html?highlight=as_in_context#mxnet.context.Context"><em>Context</em></a>) – The target context we want the return value to live in.</td>
</tr>
<tr class="field-even field">
<th class="field-name">Returns:</th>
<td class="field-body"></td>
</tr>
<tr class="field-odd field">
<th class="field-name">Return type:</th>
<td class="field-body">A copy or <cite>self</cite> as an <cite>NDArray</cite> that lives in the target context.</td>
</tr>
</tbody>
</table>
</dd></dl></dd></dl>对于辅助参数（aux_params）的保存是和arg_params一样，在同一个字典变量save_dict中（通过dict的update方法将对应的aux_params的键和值保存其中）。将<span style="color: #ff0000;">打包后的save_dict变量通过nd.save函数进行保存（具体源码如下）</span>，完成后再打上保存的日志。
<pre class="lang:python decode:true">def save(fname, data):
    """Save list of NDArray or dict of str-&gt;NDArray to binary file.

    You can also use pickle to do the job if you only work on python.
    The advantage of load/save is the file is language agnostic.
    This means the file saved using save can be loaded by other language binding of mxnet.
    You also get the benefit being able to directly load/save from cloud storage(S3, HDFS)

    Parameters
    ----------
    fname : str
        The name of the file.Can be S3 or HDFS address (remember built with S3 support).
        Example of fname:

        - `s3://my-bucket/path/my-s3-ndarray`
        - `hdfs://my-bucket/path/my-hdfs-ndarray`
        - `/path-to/my-local-ndarray`

    data : list of NDArray or dict of str to NDArray
        The data to be saved.
    """
    handles = [] 
    if isinstance(data, dict):
        keys = [] 
        for key, val in data.items():
            if not isinstance(key, string_types):
                raise TypeError('save only accept dict str-&gt;NDArray or list of NDArray')
            if not isinstance(val, NDArray):
                raise TypeError('save only accept dict str-&gt;NDArray or list of NDArray')
            keys.append(c_str(key))
            handles.append(val.handle)
        keys = c_array(ctypes.c_char_p, keys)
    else:
        for val in data:
            if not isinstance(val, NDArray):
                raise TypeError('save only accept dict str-&gt;NDArray or list of NDArray')
            handles.append(val.handle)
        keys = None 
    check_call(_LIB.MXNDArraySave(c_str(fname),
                                  mx_uint(len(handles)),
                                  c_array(NDArrayHandle, handles),
                                  keys))</pre>
同样提到了该方法语言无关的优势。
<h3>3.2.3 mxnet.model.load</h3>
<pre class="lang:python decode:true ">@staticmethod
def load(prefix, epoch, ctx=None, **kwargs):
"""Load model checkpoint from file.

Parameters
----------
prefix : str
    Prefix of model name.
epoch : int
    epoch number of model we would like to load.
ctx : Context or list of Context, optional
    The device context of training and prediction.
kwargs : dict
    other parameters for model, including num_epoch, optimizer and numpy_batch_size

Returns
-------
model : FeedForward
    The loaded model that can be used for prediction.

Notes
-----
- ``prefix-symbol.json`` will be saved for symbol.
- ``prefix-epoch.params`` will be saved for parameters.
"""
    symbol, arg_params, aux_params = load_checkpoint(prefix, epoch)
    return FeedForward(symbol, ctx=ctx,
                       arg_params=arg_params, aux_params=aux_params,
                       begin_epoch=epoch,
                       **kwargs)</pre>
<h3>3.2.4 mxnet.model.load_checkopint</h3>
<pre class="lang:python decode:true">def load_checkpoint(prefix, epoch):
    """Load model checkpoint from file.

    Parameters
    ----------
    prefix : str
        Prefix of model name.
    epoch : int
        Epoch number of model we would like to load.

    Returns
    -------
    symbol : Symbol
        The symbol configuration of computation network.
    arg_params : dict of str to NDArray
        Model parameter, dict of name to NDArray of net's weights.
    aux_params : dict of str to NDArray
        Model parameter, dict of name to NDArray of net's auxiliary states.

    Notes
    -----
    - symbol will be loaded from ``prefix-symbol.json``.
    - parameters will be loaded from ``prefix-epoch.params``.
    """
    symbol = sym.load('%s-symbol.json' % prefix)
    save_dict = nd.load('%s-%04d.params' % (prefix, epoch))
    arg_params = {}
    aux_params = {}
    for k, v in save_dict.items():
        tp, name = k.split(':', 1)
        if tp == 'arg':
            arg_params[name] = v
        if tp == 'aux':
            aux_params[name] = v
    return (symbol, arg_params, aux_params)</pre>
<h3></h3>
<h1>4. 转换代码</h1>
<pre class="lang:python decode:true">import time
import mxnet as mx

# input params
prefix = "caffenet"
epoch = 0 
upgraded_prefix = "caffenet-upgrade"
upgraded_epoch = 1 

# upgrade previous symbol
upgrade_model_start = time.time()
sym, arg_params, aux_params = mx.model.load_checkpoint(prefix, epoch)
arg_params['prob_label'] = mx.nd.array([0])

mod = mx.mod.Module(symbol=sym, context=mx.cpu(0))
mod.bind(for_training=False, data_shapes=[('data', (1, 3, 224, 224))])
mod.set_params(arg_params, aux_params)
print "upgrade previous symbol take {0}".format((time.time()-upgrade_model_start)/100)

# save upgraded _symbol
save_model_start = time.time()
## save by save_checkpoint function
## ps: FeedForward.save function is achieved by calling mx.model.save_checkpoint 
mx.model.save_checkpoint(prefix=upgraded_prefix, epoch=upgraded_epoch, symbol=sym, arg_params=arg_params, aux_params=aux_params)
print "save upgraded symbol take {0}".format((time.time()-save_model_start)/100)</pre>
&nbsp;
