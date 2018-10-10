[![header](../../../assets/header10.jpg)](https://yuenshome.github.io)

# 交叉编译gtest

再交叉编译机器上编译Android版本的gtest。

```shell
rm -rf build
mkdir build                                                                                                       
cd build
 
TOOLCHAIN_PATH="/home/yuanshuai/code/InferXLite_new/android-cmake/android.toolchain.cmake" 
ANDROID_ABI="armeabi-v7a"
ANDROID_NATIVE_API_LEVEL=21
 
cmake -DCMAKE_TOOLCHAIN_FILE="${TOOLCHAIN_PATH}" \
      -DCMAKE_BUILD_TYPE=RelWithDebInfo \
      -DANDROID_ABI="${ANDROID_ABI}" \
      -DANDROID_NATIVE_API_LEVEL="${ANDROID_NATIVE_API_LEVEL}" \
      ..  
 
make -j${N_JOBS}
```
