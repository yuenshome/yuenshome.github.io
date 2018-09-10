# C语言指针字节数

结论：32位机器上，指针为4字节，64位机器上，指针为8字节

## 64位机器

armv8机器

```shell
$ gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/aarch64-linux-gnu/5/lto-wrapper
Target: aarch64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.10' --with-bugurl=file:///usr/share/doc/gcc-5/README.Bugs --enable-languages=c,ada,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-5 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-libquadmath --enable-plugin --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-5-arm64/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-5-arm64 --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-5-arm64 --with-arch-directory=aarch64 --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --enable-multiarch --enable-fix-cortex-a53-843419 --disable-werror --enable-checking=release --build=aarch64-linux-gnu --host=aarch64-linux-gnu --target=aarch64-linux-gnu
Thread model: posix
gcc version 5.4.0 20160609 (Ubuntu/Linaro 5.4.0-6ubuntu1~16.04.10)
```

### 实验代码

```c
#include <stdio.h>
int main() {
  char         *p_char;
  int          *p_int;
  unsigned int *p_uint; 
  short        *p_short;
  long         *p_long;
  long long    *p_long_long;
  float        *p_float;
  double       *p_double;
  void         *p_void;

  printf("sizeof(p_char):%lu\n", sizeof(p_char));
  printf("sizeof(p_int):%lu\n", sizeof(p_int));
  printf("sizeof(p_uint):%lu\n", sizeof(p_uint));
  printf("sizeof(p_long):%lu\n", sizeof(p_long));
  printf("sizeof(p_long_long):%lu\n", sizeof(p_long_long));
  printf("sizeof(p_float):%lu\n", sizeof(p_float));
  printf("sizeof(p_double):%lu\n", sizeof(p_double));
  printf("sizeof(p_void):%lu\n", sizeof(p_void));

  return 0;
}
```

sizeof的结果类型为`long unsigned int`

### 结果

```shell
$ gcc pointer_size.c
$ ./a.out 
sizeof(p_char):8
sizeof(p_int):8
sizeof(p_uint):8
sizeof(p_long):8
sizeof(p_long_long):8
sizeof(p_float):8
sizeof(p_double):8
sizeof(p_void):
```

## 32位机器

armv7 

```shell
$ gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/arm-linux-gnueabihf/4.8/lto-wrapper
Target: arm-linux-gnueabihf
Configured with: ../src/configure -v --with-pkgversion='Ubuntu/Linaro 4.8.4-2ubuntu1~14.04.3' --with-bugurl=file:///usr/share/doc/gcc-4.8/README.Bugs --enable-languages=c,c++,java,go,d,fortran,objc,obj-c++ --prefix=/usr --program-suffix=-4.8 --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --with-gxx-include-dir=/usr/include/c++/4.8 --libdir=/usr/lib --enable-nls --with-sysroot=/ --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --enable-gnu-unique-object --disable-libmudflap --disable-libitm --disable-libquadmath --enable-plugin --with-system-zlib --disable-browser-plugin --enable-java-awt=gtk --enable-gtk-cairo --with-java-home=/usr/lib/jvm/java-1.5.0-gcj-4.8-armhf/jre --enable-java-home --with-jvm-root-dir=/usr/lib/jvm/java-1.5.0-gcj-4.8-armhf --with-jvm-jar-dir=/usr/lib/jvm-exports/java-1.5.0-gcj-4.8-armhf --with-arch-directory=arm --with-ecj-jar=/usr/share/java/eclipse-ecj.jar --enable-objc-gc --enable-multiarch --enable-multilib --disable-sjlj-exceptions --with-arch=armv7-a --with-fpu=vfpv3-d16 --with-float=hard --with-mode=thumb --disable-werror --enable-checking=release --build=arm-linux-gnueabihf --host=arm-linux-gnueabihf --target=arm-linux-gnueabihf
Thread model: posix
gcc version 4.8.4 (Ubuntu/Linaro 4.8.4-2ubuntu1~14.04.3)
```

### 实验代码

```c
#include <stdio.h>
int main() {
  char         *p_char;
  int          *p_int;
  unsigned int *p_uint; 
  short        *p_short;
  long         *p_long;
  long long    *p_long_long;
  float        *p_float;
  double       *p_double;
  void         *p_void;

  printf("sizeof(p_char):%u\n", sizeof(p_char));
  printf("sizeof(p_int):%u\n", sizeof(p_int));
  printf("sizeof(p_uint):%u\n", sizeof(p_uint));
  printf("sizeof(p_long):%u\n", sizeof(p_long));
  printf("sizeof(p_long_long):%u\n", sizeof(p_long_long));
  printf("sizeof(p_float):%u\n", sizeof(p_float));
  printf("sizeof(p_double):%u\n", sizeof(p_double));
  printf("sizeof(p_void):%u\n", sizeof(p_void));

  return 0;
}
```

sizeof的结果类型为`unsigned int`

### 结果

```shell
$ gcc pointer_size.c 
$ ./a.out 
sizeof(p_char):4
sizeof(p_int):4
sizeof(p_uint):4
sizeof(p_long):4
sizeof(p_long_long):4
sizeof(p_float):4
sizeof(p_double):4
sizeof(p_void):4
```

