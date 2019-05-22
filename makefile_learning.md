# makefle编写
+ makefile命名规则
  - makefile
  - Makefile
+ makefile的三要素
  - 目标
  - 依赖
  - 规则命令

+ 写法
目标:依赖
tab键规则命令
(依赖和规则可以不写)

## 第一版makefile
```
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ cat makefile
app:main.c add.c sub.c div.c mul.c
	gcc -o app -I ./include main.c add.c sub.c div.c mul.c

xxy@xxy:~/guiji_2019/linux_learning/MAKE$ make
make: 'app' is up to date.
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ ./app
sum = 26
```
如果更改其中一个文件，所有的源码都重新编译
可以考虑编译过程分解，先生成.o文件，使用.o文件变成结果

## 第二版makefile
```
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ cat makefile
app:main.o add.o sub.o div.o mul.o
	gcc -o app -I ./include main.o add.o sub.o div.o mul.o
main.o:main.c
	gcc -c main.c -I ./include
add.o:add.c
	gcc -c add.c -I ./include
sub.o:sub.c
	gcc -c sub.c -I ./include
mul.o:mul.c
	gcc -c mul.c -I ./include
div.o:div.c
	gcc -c div.c -I ./include

xxy@xxy:~/guiji_2019/linux_learning/MAKE$ make
gcc -c main.c -I ./include
gcc -c add.c -I ./include
gcc -c sub.c -I ./include
gcc -c div.c -I ./include
gcc -c mul.c -I ./include
gcc -o app -I ./include main.o add.o sub.o div.o mul.o
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ ./app
sum = 26
```
测试一下修改某一个文件是否部分编译
```
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ make
make: 'app' is up to date.
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ touch main.c
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ make
gcc -c main.c -I ./include
gcc -o app -I ./include main.o add.o sub.o div.o mul.o
```
makefile的隐藏规则：默认处理第一个目标

函数：
wildcard 可以进行文件匹配
patsubst 内容的替换

```
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ cat makefile
#get all .c files
SrcFiles=$(wildcard *.c)

# ObjFiles 定义目标文件
ObjFiles=main.o add.o sub.o div.o mul.o
# 目标文件的用法$(Var)
app:$(ObjFiles)
	gcc -o app -I ./include main.o add.o sub.o div.o mul.o
main.o:main.c
	gcc -c main.c -I ./include
add.o:add.c
	gcc -c add.c -I ./include
sub.o:sub.c
	gcc -c sub.c -I ./include
mul.o:mul.c
	gcc -c mul.c -I ./include
div.o:div.c
	gcc -c div.c -I ./include
test:
	echo $(SrcFiles)
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ make test  # 模式是编译第一个目标，编译其他目标就指定它
echo mul.c add.c main.c div.c sub.c
mul.c add.c main.c div.c sub.c
```

makefile的变量
+ $@ 代表目标
+ $^ 代表全部依赖
+ $< 第一个依赖
+ $? 第一个变化的依赖
## 第三版makefile
```
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ cat makefile
#get all .c files
SrcFiles=$(wildcard *.c)

#all .c files -> .o files
ObjFiles=$(patsubst %.c,%.o,$(SrcFiles))

# 目标文件的用法$(Var)
app:$(ObjFiles)
	gcc -o app -I ./include $(ObjFiles)
#模式匹配规则，$@，$<这样的变量，只能在规则中出现
%.o:%.c
	gcc -c $< -I ./include -o $@
test:
	echo $(SrcFiles)
	echo $(ObjFiles1)
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ make
gcc -c mul.c -I ./include -o mul.o
gcc -c main.c -I ./include -o main.o
gcc -c add.c -I ./include -o add.o
gcc -c div.c -I ./include -o div.o
gcc -c sub.c -I ./include -o sub.o
gcc -o app -I ./include mul.o main.o add.o div.o sub.o
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ ./app
sum = 26
```

### 添加清理的目标
+ @在规则前表示不输出该条规则的命令
+ -代表该条规则报错，仍然继续执行
```
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ cat makefile
#get all .c files
SrcFiles=$(wildcard *.c)

#all .c files -> .o files
ObjFiles=$(patsubst %.c,%.o,$(SrcFiles))

# 目标文件的用法$(Var)
app:$(ObjFiles)
	gcc -o app -I ./include $(ObjFiles)
#模式匹配规则，$@，$<这样的变量，只能在规则中出现
%.o:%.c
	gcc -c $< -I ./include -o $@
test:
	@echo $(SrcFiles)
	@echo $(ObjFiles)
clean:
	-@rm -f *.o  # @符号不显示信息 -符号表示这行报错不终止继续执行下面的代码
	@rm -f app
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ make
gcc -c mul.c -I ./include -o mul.o
gcc -c main.c -I ./include -o main.o
gcc -c add.c -I ./include -o add.o
gcc -c div.c -I ./include -o div.o
gcc -c sub.c -I ./include -o sub.o
gcc -o app -I ./include mul.o main.o add.o div.o sub.o
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ ./app
sum = 26
```
## 第四版makefile
```
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ cat makefile
#get all .c files
SrcFiles=$(wildcard *.c)

#all .c files -> .o files
ObjFiles=$(patsubst %.c,%.o,$(SrcFiles))

# 目标文件的用法$(Var)
app:$(ObjFiles)
	gcc -o app -I ./include $(ObjFiles)
#模式匹配规则，$@，$<这样的变量，只能在规则中出现
%.o:%.c
	gcc -c $< -I ./include -o $@
test:
	@echo $(SrcFiles)
	@echo $(ObjFiles)
clean:
	-@rm -f *.o # 不显示信息
	-@rm -f app
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ vim makefile
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ cat makefile
#get all .c files
SrcFiles=$(wildcard *.c)

#all .c files -> .o files
ObjFiles=$(patsubst %.c,%.o,$(SrcFiles))

# 目标文件的用法$(Var)
app:$(ObjFiles)
	gcc -o app -I ./include $(ObjFiles)
#模式匹配规则，$@，$<这样的变量，只能在规则中出现
%.o:%.c
	gcc -c $< -I ./include -o $@
test:
	@echo $(SrcFiles)
	@echo $(ObjFiles)
clean:
	-@rm -f *.o # 不显示信息
	-@rm -f app
```

### clean有歧义
问题描述
```
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ touch clean
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ make clean
make: 'clean' is up to date.
```

```
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ cat makefile
#get all .c files
SrcFiles=$(wildcard *.c)

#all .c files -> .o files
ObjFiles=$(patsubst %.c,%.o,$(SrcFiles))

# 目标文件的用法$(Var)
app:$(ObjFiles)
	gcc -o app -I ./include $(ObjFiles)
#模式匹配规则，$@，$<这样的变量，只能在规则中出现
%.o:%.c
	gcc -c $< -I ./include -o $@
test:
	@echo $(SrcFiles)
	@echo $(ObjFiles)
.PHONY:clean
clean:
	-@rm -f *.o # 不显示信息
	-@rm -f app
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ make
gcc -c mul.c -I ./include -o mul.o
gcc -c main.c -I ./include -o main.o
gcc -c add.c -I ./include -o add.o
gcc -c div.c -I ./include -o div.o
gcc -c sub.c -I ./include -o sub.o
gcc -o app -I ./include mul.o main.o add.o div.o sub.o
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ ./app
sum = 26

```
不会删除clean文件

## 第五版makefile
```
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ cat makefile
#get all .c files
SrcFiles=$(wildcard *.c)

#all .c files -> .o files
ObjFiles=$(patsubst %.c,%.o,$(SrcFiles))
all:app app1
# 目标文件的用法$(Var)
app:$(ObjFiles)
	gcc -o $@ -I ./include $(ObjFiles)
#模式匹配规则，$@，$<这样的变量，只能在规则中出现
app1:$(ObjFiles)
	gcc -o $@ -I ./include $(ObjFiles)

%.o:%.c
	gcc -c $< -I ./include -o $@
test:
	@echo $(SrcFiles)
	@echo $(ObjFiles)
.PHONY:clean all
clean:
	-@rm -f *.o # 不显示信息
	-@rm -f app app1
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ make
gcc -c mul.c -I ./include -o mul.o
gcc -c main.c -I ./include -o main.o
gcc -c add.c -I ./include -o add.o
gcc -c div.c -I ./include -o div.o
gcc -c sub.c -I ./include -o sub.o
gcc -o app -I ./include mul.o main.o add.o div.o sub.o
gcc -o app1 -I ./include mul.o main.o add.o div.o sub.o
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ ./app
sum = 26
xxy@xxy:~/guiji_2019/linux_learning/MAKE$ ./app1
sum = 26
```
make -f makefile1 指定具体的makefile
