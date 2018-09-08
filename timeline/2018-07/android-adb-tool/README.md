[![header](../../../assets/header05.jpg)](https://yuenshome.github.io)

# 通过ADB工具让电脑连接手机

最近尝试了一下，用电脑去连接手机，控制手机上的一些操作。更准确来说，是想把一些程序在手机上去运行，其中用到的就是adb工具。但是这个过程有些艰难，我拿我的旧手机，魅族metal，去实现这一过程。<!--more-->
<ul>
	<li>首先我进入设置将手机获取最高权限，也因此手机无法自动联网，更新是以后需要手动进行更新。</li>
	<li>然后我在谷歌的安卓相关网站下载我自己也就是linux平台上面的adb。</li>
	<li>我使用USB线让魅族手机连接我的电脑，同时在手机上设置就USB调试模式，而且需要在关于手机页面，点击版本号七次进入开发者模式。</li>
	<li>我在电脑的命令行上输入adb devices -l命令之后就会显示出当前连接电脑的手机终端信息。</li>
	<li>我尝试将我本地的文件abc上传到手机上根目录位置下的/sdcard/tmp路径，使用命令adb push abc /sdcard/tmp，但是总是提示权限问题。</li>
	<li>另外我又查询到github上adb awesome仓库，关于相关报错了一个解决提示，说是需要使用命令adb root获取权限然后再上传，没想到我在我电脑上的命令行里执行，这个命令竟然失败了，其实我没有最高权限。</li>
</ul>
我很纳闷，根据这个仓库的提示我安装了adbInsecure，但是，这个app打开之后会退出，提示我并没有获取到手机的root权限。

之后我想可能是否是手机，并没有真正给我root权限，那我还是要进行破解，那么我下载了一个kingroot，经过两次尝试，终于成功再次执行adbInSecure这个软件可以执行进去了，而且可以成功执行adb root命令，但是即使如此在我电脑本地上传文件给手机的时候，依旧出现了权限问题。

今天我再次来到公司，在小米5s上面上传我们交叉编译机上面的编译出来的静态可执行文件上传到手机上来执行上传过程中没有问题，上传到手机的/data/local/tmp目录，然后再次执行就可以成功上传，没有问题，执行也没有问题。

其实在这之前有一个小问题，就是说程序运行没有给出任何消息提示，按理说应该会有打印信息的，但是后来发现是因为我们编译的这个可试行文件在编译过程中链接了动态库，没有链接静态库，即使他执行了，但实际上是有问题的。

大概就是这样，另外我又在我的魅族手机上的adb shell里尝试对文件使chmod 777或者666或者+x，然后再使用adb pull，也同样出现了全线的问题，我有些纳闷，莫非真的是因为路径的问题，或许我应该也和小米手机一样试一下在这个/data/local/tmp路径。当然是成功了，如下（在我的魅族S6手机上，未root，未获取root权限）：
<pre class="lang:sh decode:true ">yuens@Spark:~$ adb devices -l
List of devices attached
712QSDSEMMS7C          device usb:3-4 product:MeizuS6_CN model:Meizu_S6 device:MeizuS6

yuens@Spark:~$ ls
Desktop    Downloads         git-openssl  Pictures  setCPUMaxPerf  Snapshots  Templates  tmp     VirtualBox VMs
Documents  examples.desktop  Music        sc.png    shadowsocks    Software   test       Videos
yuens@Spark:~$ adb push sc.png /data/local/tmp
4795 KB/s (106868 bytes in 0.021s)
yuens@Spark:~$ adb shell
MeizuS6:/ $ cd /data/local/tmp                                                 
MeizuS6:/data/local/tmp $ ls
sc.png
MeizuS6:/data/local/tmp $ ls -l
total 216
-rw-rw-rw- 1 shell shell 106868 2018-07-03 12:59 sc.png
MeizuS6:/data/local/tmp $ cd ..
MeizuS6:/data/local $ ls -l
ls: .: Permission denied
1|MeizuS6:/data/local $ cd tm
/system/bin/sh: cd: /data/local/tm: No such file or directory
2|MeizuS6:/data/local $ cd -                                                   
/data/local/tmp
MeizuS6:/data/local/tmp $ ls
sc.png
MeizuS6:/data/local/tmp $ cp sc.png sc2.png                                    
MeizuS6:/data/local/tmp $ exit
yuens@Spark:~$ adb pull /data/local/tmp/sc2.png .
1206 KB/s (106868 bytes in 0.086s)</pre>

<hr />

列出一些有用的链接：
<ul>
	<li>mzlogin/awesome-adb: ADB Usage Complete / ADB 用法大全
https://github.com/mzlogin/awesome-adb</li>
</ul>
下面是魅族手机的补充内容，备份记录：

一、Ubuntu环境
1. 建立或修改文件 ~/.android/adb_usb.ini，在文件开头或末尾添加一行，内容是0x2a45。

2. 建立或修改文件 /etc/udev/rules.d/51-android.rules
2.1 修改文件权限：
sudo chmod a+rx /etc/udev/rules.d/51-android.rules

2.2 在文件中添加一行内容：
SUBSYSTEM=="usb", ATTR{idVendor}=="2a45", MODE="0666", GROUP="plugdev"

4. 确保你在用户组"plugdev"
sudo adduser plugdev

5. 按下面流程重启udev服务或直接重启PC
sudo service udev restart

6. 检查ADB设备是否存在
adb kill-server
adb devices

二、Windows XP中文环境
1. 建立或修改C:\Documents and Settings\&lt;你的用户名&gt;\.android\adb_usb.ini文件，在该文件中添加一行文本，内容是0x2a45。

2. 检查ADB设备是否找到（命令行程序中）
adb kill-server
adb devices

三、Windows 7中文环境
1. 建立或修改C:\用户\&lt;你的用户名&gt;\.android\adb_usb.ini文件，在该文件中添加一行文本，内容是0x2a45。

2. 检查ADB设备是否找到（命令行程序中）
adb kill-server
adb devices

参考文档： http://developer.android.com/tools/device.html

<audio style="display: none;" controls="controls"></audio>
