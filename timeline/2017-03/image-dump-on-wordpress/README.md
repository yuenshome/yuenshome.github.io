[![header](../../../assets/header36.jpg)](https://yuenshome.github.io)

# 由固定链接造成的WordPress博客外链图片挂掉解决方案

图床一直在SAE上，后来迁移博客到了阿里云，前几天不小心点了“设置----&gt;固定链接”，一些（似乎是）静态页面的图像链接的前缀被修改了，修改成了当前博客地址的域名，这下不好了，首页的几个header_image挂了，很难看。<span style="color: #ff0000;">打算分析下源码，修改加载时候的图片链接地址，让图片重新正常显示</span>。[toc]<!--more-->
<h1>1. 确认问题及代码特征</h1>
通过检查元素（inspect），先在网站首页查看元素确认挂掉图片的一些特征信息，比方如下：
<pre class="lang:xhtml decode:true">&lt;img src="http://121.42.47.99/yuenshome/wordpress/wp-content/uploads/2014/06/cropped-149b64edc406fc68cdea65cc9ca413b1.jpg" class="header-image" width="1920" height="500" alt=""&gt;</pre>
其中class="header-image"就是关键信息。
<h1>2. 根据特征查找对应源码</h1>
在wp-content路径下grep -rn 这个关键特征信息，然后得到几个包含该片段代码的文件，如下：
<pre class="lang:sh decode:true ">style.css:330:img.header-image,
style.css:362:img.header-image,
style.css:573:.header-image {
style.css:1723: img.header-image,
inc/custom-header.php:162: &lt;img src="&lt;?php echo esc_url( $header_image ); ?&gt;" class="header-image" width="&lt;?php echo get_custom_header()-&gt;width; ?&gt;" height="&lt;?php echo get_custom_header()-&gt;height; ?&gt;" alt="" /&gt;
header.php:82: &lt;a href="&lt;?php echo esc_url( home_url( '/' ) ); ?&gt;"&gt;&lt;img src="&lt;?php header_image(); ?&gt;" class="header-image" width="&lt;?php echo get_custom_header()-&gt;width; ?&gt;" height="&lt;?php echo get_custom_header()-&gt;height; ?&gt;" alt="" /&gt;&lt;/a&gt;
css/ie.css:28:img.header-image,</pre>
以上都包含header-image，但是根据文件名。最可疑的是inc/custom-header.php和header.php，在这两个文件中定位，得到是通过一个叫做get_header_image的函数得到这个值的。

然后再次过滤这个函数名，没有什么发现，在网上搜WordPress+函数名，得到一些关于该函数的信息，如下：
<div><em>用法：HEADER_IMAGE</em></div>
<div><em>修改记录：自2.1.0版本起</em></div>
<div><em>源文件：get_header_image()位于 wp-includes/theme.php中。</em></div>
<div>

参考：WordPress常用函数get header image - 站长百科http://www.zzbaike.com/wiki/WordPress%E5%B8%B8%E7%94%A8%E5%87%BD%E6%95%B0get_header_image

</div>
<div>好了，去这个wp-includes/theme.php文件中查找，发现有调用这个函数的其他函数，如has_header_image，get_header_image，header_image。</div>
<div></div>
<div>功夫不负有心人，在这个文件中找到这个函数的源码：</div>
<div></div>
<div>
<pre class="lang:php decode:true ">function get_header_image() {
    $url = get_theme_mod( 'header_image', get_theme_support( 'custom-header', 'default-image' ) ); 

    if ( 'remove-header' == $url )
        return false;

    if ( is_random_header_image() )
        $url = get_random_header_image();

    return esc_url_raw( set_url_scheme( $url ) ); 
}</pre>
<h1>3. 修改源码</h1>
自己查了下字符串替换的函数，即str_replace，自己写了一句php：
<pre class="lang:php decode:true ">$url = str_replace('http://121.42.47.99/yuenshome/wordpress/', 'http://yuenshome-wordpress.stor.sinaapp.com/', $url);</pre>
这个str_replace(A, B, C)，就是对C串，将A替换为B。

</div>
<div>参考：PHP str_replace() 函数</div>
<div>http://www.w3school.com.cn/php/func_string_str_replace.asp</div>
<div></div>
<div>修改好了保存退出，刷新首页，图片没出现，又检查（inspect）了下图片元素路径，发现的确改了，但是有点小问题，多加了'wp-content'，重新修改如下：</div>
<div>
<pre class="lang:php decode:true ">$url = str_replace('http://121.42.47.99/yuenshome/wordpress/wp-content/', 'http://yuenshome-wordpress.stor.sinaapp.com/', $url);</pre>
</div>
<div>刷新，图片出现了，激动！</div>
<h1>4.多媒体管理界面的图片挂掉（待续）</h1>
<div>同样的，在多媒体管理界面，
通过has-media-icon这一特征片段，定位grep -rn has-media-icon出wp-admin/includes/class-wp-media-list-table.php文件，然后在该文件中通过inspect元素时候的图片元素（attachment-60x60 size-60x60）找到wp_get_attachment_image函数，再使用grep -rn 'function wp_get_attachment_image'，定位出具有该函数的文件wp-includes/media.php。</div>
<div>找到几个函数function wp_get_attachment_image*等等。未完待续……</div>
