# gdb调试

示例代码
```bash
xxy@xxy:~/xxy_github/linux_learning/gdbgg$ ls
func.c  head.h  main.c
xxy@xxy:~/xxy_github/linux_learning/gdbgg$ cat main.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "head.h"

typedef struct TTfunInfo
{
	int fun_type;
	int a;
	int b;
	char funname[10];
}TfunInfo;

int main(int argc, char *argv[])
{
	int a = 2;
	int i = 0;
	int a1 = 10;
	int b1 = 5;
	TfunInfo funinfo[2];
	char *MSg = "I will del!";
	if(argc == 3)
	{
		a1 = atoi(argv[1]);
		b1 = atoi(argv[2]);
		funinfo[0].a = a1;
		funinfo[0].b = b1;
		funinfo[1].a = a1;
		funinfo[1].b = b1;
	}
	for(i = 0; i < 2; i++)
	{
		printf("i===%d, LINE=%d\n", i, __LINE__);
		if(i == 0)
		{
			funinfo[i].fun_type = 1;
			printf("begin call sum\n");
			strcpy(funinfo[i].funname, "sum");
			sum(funinfo[i].a, funinfo[i].b);
		}
		if(i == 1 )
		{
			funinfo[i].fun_type = 2;
			printf("begin call mul\n");
			strcpy(funinfo[i].funname, "mul");
			mul(funinfo[i].a, funinfo[i].b);
		}
	}
	printf("byebye\n");
	return 0;
}

xxy@xxy:~/xxy_github/linux_learning/gdbgg$ cat func.c
#include <stdio.h>
#include "head.h"

int sum(int a, int b)
{
	printf("welcome call %s, a=%d, b=%d\n", __FUNCTION__, a, b);
	return a+b;
}

int mul(int a, int b)
{
	printf("welcome call %s, a=%d, b=%d\n", __FUNCTION__, a, b);
	return a*b;
}

xxy@xxy:~/xxy_github/linux_learning/gdbgg$ cat head.h
#include <stdio.h>
int sum(int a, int b);
int mul(int a, int b);

```

## 使用gdb编译的时候加上-g参数
```bash
xxy@xxy:~/xxy_github/linux_learning/gdbgg$ gcc main.c func.c -o app -I . -g
xxy@xxy:~/xxy_github/linux_learning/gdbgg$ ./app 11 5
i===0, LINE=33
begin call sum
welcome call sum, a=11, b=5
i===1, LINE=33
begin call mul
welcome call mul, a=11, b=5
byebye

```

## 启动gdb `gdb app(可执行程序名)`
```bash
xxy@xxy:~/xxy_github/linux_learning/gdbgg$ ls
app  func.c  head.h  main.c
xxy@xxy:~/xxy_github/linux_learning/gdbgg$ gdb app
GNU gdb (Ubuntu 7.7.1-0ubuntu5~14.04.3) 7.7.1
Copyright (C) 2014 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from app...done.
(gdb)
```
+ 在gdb启动程序
  - r(un) 启动
  - sart 启动-停留在main函数，分步调试
  - n(ext) 下一条指令
  - enter 执行上一条指令
  - s(tep) 下一条指令，可以进入函数内部，库函数不能进
  - q(uit) 退出gdb
  - 设置启动参数set agrs 10 6

## 设置断点

```bash
xxy@xxy:~/xxy_github/linux_learning/gdbgg$ gdb app
GNU gdb (Ubuntu 7.7.1-0ubuntu5~14.04.3) 7.7.1
Copyright (C) 2014 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from app...done.
(gdb) set args 10 6
(gdb) run
Starting program: /home/xxy/xxy_github/linux_learning/gdbgg/app 10 6
i===0, LINE=33
begin call sum
welcome call sum, a=10, b=6
i===1, LINE=33
begin call mul
welcome call mul, a=10, b=6
byebye
[Inferior 1 (process 9961) exited normally]
(gdb) list
7	{
8		int fun_type;
9		int a;
10		int b;
11		char funname[10];
12	}TfunInfo;
13
14	int main(int argc, char *argv[])
15	{
16		int a = 2;
(gdb)
17		int i = 0;
18		int a1 = 10;
19		int b1 = 5;
20		TfunInfo funinfo[2];
21		char *MSg = "I will del!";
22		if(argc == 3)
23		{
24			a1 = atoi(argv[1]);
25			b1 = atoi(argv[2]);
26			funinfo[0].a = a1;
(gdb) b 20
Breakpoint 1 at 0x4005e8: file main.c, line 20.
```
+ b 行号-主函数所在文件的行
+ b 函数名
+ b 文件名:行号
+ l(ist)查看代码，默认显示10行
  - l 显示主函数对应的文件
  - l 文件名:行号
+ 查看断点 i(nfo) b 得到编号
+ 删除断点 d(el) 编号
+ c(ontinue) 跳到下一个断点
+ p(rint) 打印变量的值
+ ptype 打印变量类型
+ set 设置变量的值
  - set argc=4
  - set argv="12"
  - set argv="7"
+ display 显示变量的值，用于追踪，查看变量具体什么时候变化
+ undispaly 删除显示变量，查看变量，info display
+ 设置条件断点b line if i == 1
