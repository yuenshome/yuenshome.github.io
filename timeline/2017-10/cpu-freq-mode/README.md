[![header](../../../assets/header32.jpg)](https://yuenshome.github.io)

# CPU超频解读

<blockquote>转载自：超频必读CPU模式ondemand, - Galaxy Nexus ROM 相关下载 机锋论坛
http://bbs.gfan.com/android-4165836-1-1.html</blockquote>
这是在网络上看到的一篇文章，关于介绍各种CPU模式，当然GPU也可以参考基本。

linux系统中五种<span style="color: #ff0000;">主要的</span>CPU频率调节方式分别是：<strong><span style="color: #ff0000;">ondemand</span></strong>，<strong><span style="color: #ff0000;">userspace</span></strong>，<strong><span style="color: #ff0000;">performance</span></strong>。
<ol>
	<li><strong><span style="color: #ff0000;">ondemand</span></strong>指的是平时以低速方式运行，当系统负载提高时候自动提高频率。以这种模式运行不会因为降频造成性能降低，同时也能节约电能和降低温度。对于普通用户应该使用ondemand，能兼顾性能和功耗。
会根据设备运行负载迅速调整CPU频率到达特定点，然后逐步调整CPU频率回到配置范围内；當 cpu loading 達到 up threshold 的時候，setcpu 會馬上拉高速度；當 cpu 沒什麼在用的時候，則是慢慢降低。与conservative分歧的是，当需要时，cpu顿时会很给力，不需要时再渐渐泄力。如：充电：用于控制充电状态下CPU的运行频率。在此情景模式下，降低CPU最大运行频率可以加快电池充电速度。如：电池配置：用于控制当电池电量在指定范围内时CPU的运行频率。在此情景模式下，根据电池电量情况，配置不同的CPU运行频率，可有效延长电池待机时间。</li>
	<li><span style="color: #ff0000;"><strong>performance</strong></span>指满速运行，即使系统负载非常低cpu的频率也为最高。则性能很好，但是电量消耗较快，温度也高一些。
cpu 速度會保持在設定範圍內的最大值，最费电。這跟ondemand 模式中 min-max 都設最大值略為不同，在performance 模式時，setcpu 不會浪費資源去偵測 cpu loading。</li>
	<li><span style="color: #ff0000;"><strong>userspace</strong></span>是任何情况下都会控制CPU运行在配置的频率范围内，配置中的用户自己添加的省电设置。要启用配置中的省电设置就必须在主页面里选择userspace,也就是用户自定的意思。如：休眠/待机：用于控制休眠/待机状态下CPU的运行频率。在此情景模式下，降低CPU最大运行频率可以延长电池待机时间，但同时也会降低机器的唤醒速度。</li>
	<li><strong><span style="color: #ff0000;">conservative</span></strong>: 字面含义是传统的、保守的，跟 ondemand 相似，差別在於 cpu 速度調整會比較緩慢一些，但是比較省電。</li>
	<li><strong><span style="color: #ff0000;">powersave</span></strong>:与performance 模式的相反，保持在最小值。始终保持cpu利用min频率，省电。</li>
</ol>
此外，还有更多的模式，可以参考
<ul>
	<li>Linux系统CPU频率调整工具使用 - 生活费 - 博客园
https://www.cnblogs.com/276815076/p/5434295.html</li>
</ul>
&nbsp;

再说说setcpu中其它辅助及高级设置：

<!--more-->

第一个
charging就是充电中，（若是需要）勾选“启用”然掉队进点窜建设最大max和最小min cpu频率；和缩放办理的种类。之前没有选择设备型号的童鞋（而是手动建设文件），此刻应当有700M 的最高频，充电式当然要超，插电玩游戏嘛，哈哈回正本身挑吧

第二个
sleep、standby休眠和待机

同理，可以启用设置休眠和待机状况的cpu设置当然是低一些好，低到多少好呢，我没试过，有乐趣的童鞋可以本身往试。

接下来是电池百分比的建设可以分3个档次，大师可以按本身平常平凡利用的环境需要来调剂
≥90%
≥75%
≥50%或是你只需要两档，回正本身设置看看就好了
根据只有一个，就是电越少，频率应当越低，省电。

最后一个是温度建设
这项小我感觉是给超频狂人用的，一般不消启用

记得让每个你要用的建设生效，设置完成后，在主界面里勾选“启动时设置”

关于“系统信息”这里就未几说了，里面供给了系统cpu和内存等具体信息，并且可以在此进行测试。
撑持信息复制倒剪贴板。

最后说说“高级”（慎进）
高级只有在cpu办理设置为“节俭”或“按软件需求”时可用

sampling Rate
采样率就是指setcpu多久探查一次手机需求（查询的时候间距），当发现有需要增加或削减时，setcpu产生感化。设置范围是25000-25000000微秒。

Up Threshold
上阈值（只有节俭模式conservative中起感化）：设置百分比，当cpu达到这个百分比负载时，下调cpu

Ignore Nice Load
忽视：当为1时，将会忽视“nice”过程（这点我也很费解）

Powersave Bias
节电方向值（只有按需模式odemand中起感化）：当数字越大时，setcpu会把cpu设置在一个比设置的频率还低的频率（好别扭），数字1000-0，对应100.0%-0%。

Freq Step（新版）
步进值（只有节俭模式conservative中起感化）：设置每次cpu上调数字（相对最高max频率的百分比），因为conservative下cpu是慢慢上调，直到上阈值。

勾选set on boot可以在手机启动主动设置。
