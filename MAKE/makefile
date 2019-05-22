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
