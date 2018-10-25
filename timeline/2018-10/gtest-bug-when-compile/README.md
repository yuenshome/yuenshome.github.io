[![header](../../../assets/header21.jpg)](https://yuenshome.github.io)

# gtest编译错误typeinfo error: undefined reference to 'typeinfo for testing::Test'

在编译 InferXLite 过程中，CPU 的 PerfBLAS+PerfDNN 版本编译 test 正确且通过，但是 GPU 编译 test 的时候总是失败，报错：`typeinfo error: undefined reference to 'typeinfo for testing::Test'`。

- 首先，不编译 test 时，二者都可以通过，排除了编译 `inferxlite_model`, `inferxlite` 的问题；  
- 因而认为是 test 写的有问题，逐一检查 test 下的 cmake 写法，发现也没什么错误，格式上调整了下，再就是检查 test 里相关的代码，在注释掉代码的情况下，还是报错；  
- 看来可能还不是 test 的 cmake 以及 test 代码的问题，又返回头去检查根目录的 cmake ，先把所有调用库都删干净/注释掉大量`CMakeLists.txt`的内容，然后编译成功了 test ，虽然此时是没有调用任何加速库的情况下，不过看来思路是正确的；  
- 慢慢往里取消注释，发现是调用 OpenCL 假库的时候出现的，只要加上了`link_directories(${LIB_INFERX_OPENCL_DIR}/lib)`这句，就会报错；  
- 进入`${LIB_INFERX_OPENCL_DIR}/lib`该路径检查，发现引入了 gtest 的静态库，删掉，重新编译，通过！

