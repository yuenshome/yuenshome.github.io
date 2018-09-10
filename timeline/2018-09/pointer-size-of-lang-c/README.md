# C语言指针字节数

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

实验代码

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

结果

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

