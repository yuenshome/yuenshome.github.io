[![header](../../../assets/header33.jpg)](https://yuenshome.github.io)

# git cannot pull, clone or push? failed to connect, connection refused

[toc]

git pull的时候出现这个问题，搜了下看到一个csdn引自stackoverflow上的回答，按照提示解决了这个问题。具体如下：
<h1>原问题</h1>
After <code>git push -u origin master</code> I got:
<pre class="lang:sh decode:true">fatal: unable to access 'https://github.com/xxxx/xxxx.git': Failed to connect to 127.0.0.1 port 8087: Connection refused</pre>
same error happen when I try to clone others repo.port 8087 doesn't seem to be busy. What could go wrong?

edit: when I do clone from git:// instead of https://, it works just fine.

我的问题类似，但是是git pull更新代码的时候，另外就是端口号不一样，其它相同。<!--more-->
<h1>回答</h1>
This happens because a proxy is configured in git.

Since it's https proxy (and not http) <code>git config http.proxy</code> and <code>git config --global http.proxy</code> can't help.
<h2>1 : take a look at your git configuration</h2>
<pre><code>git config --global -l
</code></pre>
If you have nothing related to https proxy like <code>https_proxy=...</code> the problem is not here.

If you have something related to https proxy then remove it from the file <code>~/.gitconfig</code> and try again
<h2>2 : if it still doesn't work, unset environment variables</h2>
Check your environment variables :
<pre><code>env|grep -i proxy  
</code></pre>
You should have one or several lines with <code>https_proxy=...</code>

Unset one by one with : <code>unset https_proxy</code> (or HTTPS_PROXY depending of the name of the variable)
<h2>3 : check environment variables again</h2>
<code>env|grep -i proxy</code>

If it shows nothing you should be good.

Note : This solution can applies to http and https proxy problems. just the variables name changes from https to http

参考：github - git cannot clone or push? failed to connect, connection refused - Stack Overflow
https://stackoverflow.com/questions/24543372/git-cannot-clone-or-push-failed-to-connect-connection-refused
