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
