[![header](../../../assets/header10.jpg)](https://yuenshome.github.io)

# 交叉编译gtest

再交叉编译机器上编译Android版本的gtest。

```shell
# init
rm -rf build
mkdir build                                                                                                       
cd build
 
# config
N_JOBS=${N_JOBS:-4}
TOOLCHAIN_PATH="/home/yuanshuai/code/InferXLite_new/android-cmake/android.toolchain.cmake" 
ANDROID_ABI="armeabi-v7a"
#ANDROID_ABI="arm64-v8a"
ANDROID_NATIVE_API_LEVEL=21

# cmake && make && install
cmake .. \
      -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_PATH}" \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DANDROID_ABI="${ANDROID_ABI}" \
      -DANDROID_NATIVE_API_LEVEL="${ANDROID_NATIVE_API_LEVEL}" \
      -DCMAKE_INSTALL_PREFIX="/home/yuanshuai/software/gtest-android/" # must be absolute path

make -j${N_JOBS}
make install/strip
```

## apt安装gtest

先前安装是执行apt相关命令：

```shell
$ sudo apt-get install libgtest-dev
$ cd /usr/src/gtest
$ sudo cmake .
$ sudo make
$ sudo rm -rf CMakeCache.txt Makefile CMakeFiles/ cmake_install.cmake
$ sudo cp libgtest*.a /usr/local/lib
```

但最近发现编译的时候，报关于pthread的错，怀疑是不支持多线程，因而从githhub下载googletest的release版本，通过源码编译安装：

```shell
$ # download source code
$ cd code/tmp
$ wget -c https://github.com/google/googletest/archive/release-1.8.1.zip
$ unzip release-1.8.1.zip
$ mv googletest-release-1.8.1 ~/code/

# compile
$ cd ~/code/googletest-release-1.8.1
$ mkdir build
$ cd build
$ cmake .. \
$   -DCMAKE_INSTALL_PREFIX="/home/yuanshuai/software/gtest/" # must be absolute path
$ make -j
$ make install
```
