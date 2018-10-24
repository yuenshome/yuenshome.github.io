# CMake中使用set定义变量

在写项目CMake中，想在test下的CMake中定义`target_link_library`时不再重复定义，即在根目录下的cmake就把要连接的库定义好，避免二次写这些库名的时候出错，这里就用到了set。

具体来说，test目录下的CMake原本是写成下面这样的：

```cmake
# LIBRARY CHOICE: OPEN/PERF/PERF_CL
if(BUILD_WITH_OPENBLAS)
    TARGET_LINK_LIBRARIES(${the_target} openblas)
endif()
if(BUILD_WITH_PERFBLAS_PERFDNN)
    TARGET_LINK_LIBRARIES(${the_target} perfblas perfdnn)
endif()
if(BUILD_WITH_PERFDNN_CL)
    TARGET_LINK_LIBRARIES(${the_target} PerfDNN_core PerfDNN_ocl OpenCL)
endif()
```

但是这里1.三个if-else非常冗余;2.根目录已经定义过，这里二次定义如果根目录有改动这里忘了改会出错误等。因而需要一个将要链接的库作为变量，通过在根目录下定义好，然后直接传到这里即可作为`target_link_library`的第二个参数即可。

这时便想到了set，然而set可以定义bool或者str类型，可以定义这种不带双引号的类型吗。我先试了写成下面这样，

```cmake
# LIBRARY CHOICE: OPEN/PERF/PERF_CL
if(BUILD_WITH_OPENBLAS)
    TARGET_LINK_LIBRARIES(${the_target} "openblas")
endif()
if(BUILD_WITH_PERFBLAS_PERFDNN)
    TARGET_LINK_LIBRARIES(${the_target} "perfblas perfdnn")
endif()
if(BUILD_WITH_PERFDNN_CL)
    TARGET_LINK_LIBRARIES(${the_target} "PerfDNN_core PerfDNN_ocl OpenCL")
endif()
```

将要链接的库名带上双引号，编译时候出错，看来是不行的，那么set用字符串的形式也不对：

```cmake
set(test_target_link_library "perfblas perfdnn")
TARGET_LINK_LIBRARIES(${the_target} ${test_target_link_library})
```

即写成上面这样也不对，因为当做字符串处理了，那么有没有办法可以解决这个问题。查了CMake的set使用文档，发现如下内容：

```markdown
set(<variable> <value>... CACHE <type> <docstring> [FORCE])  

Set the given cache <variable> (cache entry). Since cache entries are meant to provide user-settable values this does not overwrite existing cache entries by default. Use the FORCE option to overwrite existing entries.

The <type> must be specified as one of:

BOOL  
Boolean ON/OFF value. cmake-gui(1) offers a checkbox.
FILEPATH  
Path to a file on disk. cmake-gui(1) offers a file dialog.
PATH  
Path to a directory on disk. cmake-gui(1) offers a file dialog.
STRING  
A line of text. cmake-gui(1) offers a text field or a drop-down selection if the STRINGS cache entry property is set.
INTERNAL  
A line of text. cmake-gui(1) does not show internal entries. They may be used to store variables persistently across runs. Use of this type implies FORCE.
The <docstring> must be specified as a line of text providing a quick summary of the option for presentation to cmake-gui(1) users.

If the cache entry does not exist prior to the call or the FORCE option is given then the cache entry will be set to the given value. Furthermore, any normal variable binding in the current scope will be removed to expose the newly cached value to any immediately following evaluation.

It is possible for the cache entry to exist prior to the call but have no type set if it was created on the cmake(1) command line by a user through the -D<var>=<value> option without specifying a type. In this case the set command will add the type. Furthermore, if the <type> is PATH or FILEPATH and the <value> provided on the command line is a relative path, then the set command will treat the path as relative to the current working directory and convert it to an absolute path.
```

除了`BOOL, FILEPATH, PATH, STRING`外，还提供一种`INTERNAL`类型，介绍说该类型相当于一行文本，用来存储内部的实体，后面写的`They may be used to store variables persistently across runs. Use of this type implies FORCE.`我也看不懂，那就试试看：

```cmake
# 根目录中CMakeLists.txt中定义
set(test_target_link_library perfblas perfdnn CACHE INTERNAL "test target link library")

# test目录下的CMakeLists.txt中定义
TARGET_LINK_LIBRARIES(${the_target} ${test_target_link_library})
```

编译成功，没有出问题。我想以`INTERNAL`这种类型来定义变量就可以引用多个变量，正如其中所说：`used to store variables persistently across runs`，这个`variables`是复数，而且这样看来这些库名应该就是作为变量出现的。
