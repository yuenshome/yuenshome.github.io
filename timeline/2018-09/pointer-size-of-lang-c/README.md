# C语言指针字节数

## 64位机器

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

