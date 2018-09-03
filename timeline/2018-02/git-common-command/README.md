# 常用git命令总结（不断补充中）

常用git命令总结，不断补充，随用随查[toc]<!--more-->
## 0. 显示提交历史记录

### 1 git log --pretty=oneline

<pre class="lang:sh decode:true">$ git log --pretty=oneline
ca82a6dff817ec66f44342007202690a93763949 changed the version number
085bb3bcb608e1e8451d4b2432f8ecbe6306e7e7 removed unnecessary test code
a11bef06a3f659402fe7563abf99ad00de2209e6 first commit</pre>
### 2 git log --pretty=format:"%h - %an, %ar : %s"
<pre class="lang:sh decode:true">$ git log --pretty=format:"%h - %an, %ar : %s"
ca82a6d - Scott Chacon, 11 months ago : changed the version number
085bb3b - Scott Chacon, 11 months ago : removed unnecessary test code
a11bef0 - Scott Chacon, 11 months ago : first commit</pre>
参考：Git - 查看提交历史
https://git-scm.com/book/zh/v1/Git-%E5%9F%BA%E7%A1%80-%E6%9F%A5%E7%9C%8B%E6%8F%90%E4%BA%A4%E5%8E%86%E5%8F%B2
<h2>3 git log -p FILENAME</h2>
查看某个文件的修改历史

查看a.c文件的提交历史：git log ./a.c
查看a.c文件的提交历史改动：git log -p ./a.c
## 1. 删除远端分支
<pre class="lang:sh decode:true"># 先查看远端分支
git branch -r
# 查看当前所有分支（红色为远端分支）
git branch -a

# 确认要删除的分支名，比方要删除的远端分支名为test_feature_gpu_conv1x1
# 先删除本地的test_feature_gpu_conv1x1分支
git branch -r -d origin/test_feature_gpu_conv1x1
# 再将修改提交到远端
# 注意origin后，分号前有空格
git push origin :test_feature_gpu_conv1x1</pre>
## 2. 撤销操作  
### 2.1 撤销未提交的修改  
使用该命令前，需要【仔细】查看有哪些文件做了改动，有几次因为没仔细看，代码重新。
<pre class="lang:sh decode:true ">git reset --hard HEAD</pre>
Git Book 中文版 - Git的撤消操作 - 重置, 签出 和 撤消
http://gitbook.liuhui998.com/4_9.html

## 3. Tag操作  
### 3.1 重命名tag  

Here is how I rename a tag <code>old</code> to <code>new</code>:
<pre class="lang:sh decode:true">git tag new old
git tag -d old
git push origin :refs/tags/old
git push --tags</pre>
The colon in the push command removes the tag from the remote repository. If you don't do this, git will create the old tag on your machine when you pull.

Finally, make sure that the other users remove the deleted tag. Please tell them(co-workers) to run the following command:
<pre class="lang:sh decode:true ">git pull --prune --tags</pre>
参考：git tag - How do you rename a Git tag? - Stack Overflow
https://stackoverflow.com/questions/1028649/how-do-you-rename-a-git-tag

## 4. git pull

### 4.1 使用git pull文件时和本地文件冲突怎么办？

使用git pull代码经常会碰到有冲突的情况，提示如下信息：
<pre class="lang:sh decode:true">error: Your local changes to 'c/environ.c' would be overwritten by merge.  Aborting.
Please, commit your changes or stash them before you can merge.</pre>
这个意思是说更新下来的内容和本地修改的内容有冲突，先提交你的改变或者先将本地修改暂时存储起来。

处理的方式非常简单，主要使用git stash命令进行处理，分成以下几个步骤进行处理：

<strong>1、先将本地修改存储起来</strong>
<pre class="lang:python decode:true">$ git stash
Saved working directory and index state WIP on master: 943294d delete report file
HEAD is now at 943294d delete report file</pre>
这样本地的所有修改就都被暂时存储起来 。是用git stash list可以看到保存的信息：
<pre class="lang:sh decode:true">$ git stash list
stash@{0}: WIP on master: 943294d delete report file</pre>
其中stash@{0}就是刚才保存的标记。

<strong>2、pull内容</strong>

暂存了本地修改之后，就可以pull了。
<pre class="lang:sh decode:true">$ git pull</pre>
<strong>3、还原暂存的内容</strong>
<pre class="lang:sh decode:true">$ git stash pop stash@{0}</pre>
系统提示如下类似的信息：
<pre class="lang:sh decode:true">Auto-merging c/environ.c
CONFLICT (content): Merge conflict in c/environ.c</pre>
意思就是系统自动合并修改的内容，但是其中有冲突，需要解决其中的冲突。

<strong>4、解决文件中冲突的的部分</strong>

打开冲突的文件，会看到类似如下的内容：

```shell
<<<<<<< Updated upstream
    int i, j;
=======
    int i, z;
>>>>>>> Stashed changes	
```

其中Updated upstream 和=====之间的内容就是pull下来的内容，====和stashed changes之间的内容就是本地修改的内容。碰到这种情况，git也不知道哪行内容是需要的，所以要自行确定需要的内容。

解决完成之后，就可以正常的提交了。

参考：使用git pull文件时和本地文件冲突怎么办？ - 快乐编程
http://www.01happy.com/git-resolve-conflicts/

## 5. 检索

### 5.1 检索提交关键词

<pre class="lang:sh decode:true ">git log --grep=word</pre>
How to grep Git commit diffs or contents for a certain word? - Stack Overflow
https://stackoverflow.com/questions/1337320/how-to-grep-git-commit-diffs-or-contents-for-a-certain-word

## 6. 撤销
### 6.1 撤销未push的commit注释：git reset id

完成Commit命令的撤销，但是不对代码修改进行撤销，可以直接通过git commit 重新提交对本地代码的修改。

参考：git撤销commit，但未git push的命令 - CSDN博客
https://blog.csdn.net/dong19900415/article/details/70495716/

<audio style="display: none;" controls="controls"></audio>
