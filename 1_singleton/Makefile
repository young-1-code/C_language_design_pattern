# 编译
CC   	:= gcc				#指明GCC编译器
CFLAGS  := -g				#编译参数
LDFLAGS := -pthread			#链接pthread动态库

TARGET 	:= mainApp			#生成的目标可执行文件

INC 	:= -I ./			#头文件所在目录
SRC 	:= $(wildcard *.c)	#搜索匹配当前路径的源文件
OBJS 	:= $(patsubst %.c, %.o, $(SRC)) #中间文件

CFLAGS 	+= $(INC) 

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS) 

%.o:%.c
	$(CC) -c $< -o $@ $(CFLAGS)

.PHONY:clean
clean:
	@rm $(OBJS)