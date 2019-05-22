# Linux动态库开及部署
## 使用GCC编译程序时，编译过程可以被细分为四个阶段
+ 预处理(Pre-Processing)
+ 编译(Compiling)
+ 汇编(Asse mbling)
+ 链接(Linking)

## 不同阶段文件后缀名列表

|后缀名|描述|
|:-:|:-:|
|.c|C原始程序|
|.i|经过预处理的C原始程序|
|.s/.S|汇编语言远程程序|
|.o|目标文件|
|.a/.so|编译后的库文件，a是静态库，so是动态库|

## GCC编译过程
1）gcc预处理阶段：主要对包含头文件(#include)和宏定义(#define, #ifdef...)进行处理。可以使用`gcc -E`让gcc在预处理之后定制编译过程，生成`*.i`文件。
```bash
gcc -E hello.c -o hello.i
```

2)gcc编译阶段：gcc首先要检查代码的规范性，是否有语法错误等，以确定代码实际要做的工作，检查无误后，gcc把代码翻译成汇编语言。可以使用`-S`选项记性查看，该选项只进行编译而不进行汇编，生成汇编代码。
```bash
gcc -S hello.i -o hello.s
```
3)gcc汇编阶段：生成目标代码`*.o`。有两种方式：使用gcc直接从源码生成目标代码`gcc -c *.s -o *.o`;使用汇编器从汇编代码生成目标代码`as *.s -o *.o`
```bash
gcc -c hello.s -o hello.o
```
```bash
as hello.s -o hello.o
```
4)gcc链接阶段：生成可执行文本。可以生成的可执行文件格式有`a.out/*`等。
```bash
gcc hello.o -o hello
```

## gcc常用编译选项
|选项|描述|
|-|-|
|-c | 只编译不链接，生成目标文件`*.o` |
|-S | 只编译不汇编，生成汇编代码`*.s` |
|-E | 只记性预编译，不做其他处理`*.i` |
|-g | 用于gdb调试，不加此选项不能gdb调试   |
|-o file | 指定file文件作为输出文件 |
|-v | 打印出编译器内部编译各个过程的命令行信息和编译器的版本  |
|-I dir | 在头文件的搜索路径列表中添加dir目录 |
|-static | 进行静态编译，即链接静态库，禁止使用动态库 |
|-shared | 1.可以生成动态库文件；2进行动态编译，尽可能地链接动态库，只有没哟动态库时才会链接同名的静态库(默认选项，即可省略) |
|-L dir | 在库文件的搜索路径列表中添加dir目录 |
|-l dir | 指定库名(通常libxxx.so或libxxx.am)，写成-lxxx  |
|-i   |包含头文件路径(可以绝对路径，可以相对路径)   |
|-lname | 链接称为libname.a(静态库)或者libname.so(动态库)的库文件。若两个库都存在，则根据编译方式(-static还是-shared)进行链接 |
| -Wall  | 显示更多的警告|
|-D|指定宏定义|
|-lstdc++ |编译c++代码 |
|-fPIC(-fpic) |生成使用相对地址的位置无关的目标代码(Position Independent Code)，通常使用gcc的-static选项从该PIC目标文件生成动态库文件  |


在Linux下开发软件时，通常需要借助一个或多个库函数的支持才能完成相应的功能。从程序员的角度看，函数库实际上就是一些头文件(.h)和库文件(.so或.a)的集合。虽然Linux下的大多数函数都默认将`头文件`放到`/usr/include/`目录下，而`库文件`放到`/usr/lib`目录下，但不是所有情况都是这样。

GCC采用搜索目录的办法来查找所需要的文件,`-I`选项可以想GCC的头文件搜索路径中添加的新的目录。例如。如果/home/xxy/include目录下有编译时所需的头文件，为了让gcc找到它们，就可以使用`-I`选项：
```bash
gcc foo.c -I /home/xxy/include -o foo
```

同样的，使用不在标准位置的库文件，可以通过`-L`选项向gcc的库文件搜索路径中添加新的目录。例如，如果在/home/xxy/lib目录下有链接时所需要的库文件libfoo.so，为了让gcc找到它们，可以使用以下命令：
```bash
gcc foo.c -L /home/xxy/lib -lfoo -o foo
```

`-l`选项，它指示GCC去链接库文件libfoo.so。Linux 下的库文件在命名时有一个约定，那就是应该以lib 三个字母开头，由于所有的库文件都遵循了同样的规范，因此在用`-l`选项指定链接的库文件名时可以省去lib三个字母，也就是说GCC在对`-lfoo` 进行处理时，会自动去链接名为libfoo.so。


Linux下的库文件分为两大类分别是动态链接库（通常以.so结尾）和静态链接库（通常以.a结尾），两者的差别仅在程序执行时所需的代码是在运行时动态加载的，还是在编译时静态加载的。默认情况下，GCC在链接时优先使用动态链接库，只有当动态链接库不存在时才考虑使用静态链接库，如果需要的话可以在编译时加上-static选项，强制使用静态链接库。例如，如果在home/justin/lib/目录下有链接时所需要的库文件libfoo.so和libfoo.a，为了让GCC在链接时只用到静态链接库，可以使用下面的命令：
```bash
gcc foo.c -L /home/justin/lib -static -lfoo -o foo
```

## 样例
新建sample.c和sample.h
```c
/*sample.c*/
#include <stdio.h>
#include <sample.h>

void sayhello(void)
{
printf("say hello\n");
}
```

```c
/*sample.h*/
void sayhello(void);
```

生成动态链接库libsample.so
```
gcc -fpic -shared -o libsample.so sample.c -I .
```

新建testsample.

```c
#include <stdio.h>
#include <sample.h>
int main(void)
{
sayhello();
return 0;
}
```

生成可执行文件testsample
```bash
gcc -o testsample testsample.c -L /home/xxy/python_learning -lsample -I /home/xxy/python_learning
```

执行
```bash
./testsample
# say hello
```

# 静态库
静态库文件的命名libxxx.a
+ 制作的步骤：
  1. 编译成.o文件
  2. 将.o文件打包：ar rcs libxxx.a fil1.o file2.o file3.o ...
  3. 将头文件和库一起发布

+ 使用：
  编译时需要加静态库名(记得路径)，-I包含头文件

### 文件组织
```shell
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ tree
.
├── app
├── include
│   └── head.h
├── lib
│   └── libCalc.a
├── main.c
└── src
    ├── add.c
    ├── add.o
    ├── div.c
    ├── div.o
    ├── mul.c
    ├── mul.o
    ├── sub.c
    └── sub.o
```
###　文件内容
```bash
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ cat *.c
#include "head.h"
int add(int a, int b)
{
    int result = a + b;
    return result;
}
#include"head.h"

int div(int a, int b)
{
    int result = a / b;
    return result;
}
#include "head.h"
int mul(int a, int b)
{
    int result = a * b;
    return result;
}
#include "head.h"
int sub(int a, int b)
{
    int result = a - b;
    return result;
}

xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/include$ cat head.h
#include <stdio.h>
int add(int a, int b);
int div(int a, int b);
int mul(int a, int b);
int sub(int a, int b);

xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ cat main.c
#include <stdio.h>
#include "head.h"

int main(void)
{
    int sum = add(2, 24);
    printf("sum = %d\n", sum);
    return 0;
}
```

1. 编译成.o文件
```
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ gcc -c *.c -I ../include/
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ ls
add.c  add.o  div.c  div.o  mul.c  mul.o  sub.c  sub.o
```
2. 将.o文件打包：ar rcs libxxx.a fil1.o file2.o file3.o ...
```
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ gcc main.c -o app -I include/ -L lib/ -lCalc
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ ls
app  include  lib  main.c  src
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ rm app
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ rm lib/libCalc.a
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ cd src/
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ ar rcs libCalc.a *.o
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ ls
add.c  add.o  div.c  div.o  libCalc.a  mul.c  mul.o  sub.c  sub.o
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ mv libCalc.a ../lib/
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ cd ..
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ tree
.
├── include
│   └── head.h
├── lib
│   └── libCalc.a
├── main.c
└── src
    ├── add.c
    ├── add.o
    ├── div.c
    ├── div.o
    ├── mul.c
    ├── mul.o
    ├── sub.c
    └── sub.o
```
3. 将头文件和库一起发布
```
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ gcc main.c -o app -I include/ -L lib/ -lCalc
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ ls
app  include  lib  main.c  src
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ ./app
sum = 26
```

# 动态库

动态库文件的命名libxxx.so
+ 制作的步骤：
  1. 编译与位置无关的代码，生成.o文件，关键参数-fPIC
  2. 将.o文件打包：关键参数-shared
  3. 将头文件和库一起发布

+ 使用：
  编译时需要加静态库名(记得路径)，-I包含头文件

+ 解决不能加载动态库的问题
  1. 拷贝到/lib下(不允许)
  2. 将库路径增加到环境变量LD_LIBRARY_PATH，不是特别推荐
  3. 配置/etc/ld.so.conf文件，增加/home/xxy/guiji_2019/linux_learning/GCC/Calc/lib路径，执行sudo ldconfig -v

## 文件组织结构
```
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ tree
.
├── include
│   └── head.h
├── lib
├── main.c
└── src
    ├── add.c
    ├── div.c
    ├── mul.c
    └── sub.c
```


1. 编译与位置无关的代码，生成.o文件，关键参数-fPIC
```
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ gcc -fPIC -c *.c -I ../include/
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ ls
add.c  add.o  div.c  div.o  mul.c  mul.o  sub.c  sub.o

```

2. 将.o文件打包：关键参数-shared
```
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ gcc -shared -o libCalc.so *.o
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ ls
add.c  add.o  div.c  div.o  libCalc.so  mul.c  mul.o  sub.c  sub.o

```
+ 也可以直接从.c文件生成.so
```
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ gcc -fPIC -shared -o libCalc.so *.c -I ../include/
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ ls
add.c  add.o  div.c  div.o  libCalc.so  mul.c  mul.o  sub.c  sub.o
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ rm ../lib/libCalc.so
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ mv libCalc.so  ../lib/
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ cd ..
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ ./newapp   # 设置了/etc/ld.so.conf文件，见下面的描述
sum = 26

```


3. 将头文件和库一起发布
```
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ mv libCalc.so ../lib/  # 移动文件
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc/src$ cd ..
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ ls
include  lib  main.c  src
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ gcc main.c -o newapp -I include/ -L lib/ -lCalc
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ ./newapp
./newapp: error while loading shared libraries: libCalc.so: cannot open shared object file: No such file or directory
```
## 解决不能加载动态库的问题

这里只实验方法2,3
+ 将库路径增加到环境变量LD_LIBRARY_PATH
```
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ export LD_LIBRARY_PATH=/home/xxy/guiji_2019/linux_learning/GCC/Calc/lib:$LD_LIBRARY_PATH
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ ./newapp
sum = 26
# 退出终端，环境变量就消失，写入~/.bashrc可以永久写入
```

+ 配置/etc/ld.so.conf文件，增加/home/xxy/guiji_2019/linux_learning/GCC/Calc/lib路径，执行sudo ldconfig -v
  - -v 显示过程
```
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ ./newapp
./newapp: error while loading shared libraries: libCalc.so: cannot open shared object file: No such file or directory
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ ./newapp
./newapp: error while loading shared libraries: libCalc.so: cannot open shared object file: No such file or directory
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ sudo vim /etc/ld.so.conf
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ sudo ldconfig
xxy@xxy:~/guiji_2019/linux_learning/GCC/Calc$ ./newapp
sum = 26
```
