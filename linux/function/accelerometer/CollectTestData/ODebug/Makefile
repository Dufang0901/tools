#######################################
#    Name:Makefile
#    Author:     linca
#    Date:       2013.01.28
#   Purpose:    编译文件
#######################################

#由于jenkins编译修改
ifdef CROSS_COMPILE_BIC
    CROSS_COMPILE = $(CROSS_COMPILE_BIC)
    CROSS_TC_DIR = $(CROSS_COMPILE_BIC_TC)
else
    CROSS_COMPILE = arm-linux-gnueabihf-
    CROSS_TC_DIR = /home/linca/freescale/gcc-linaro-arm-linux-gnueabihf-4.9-2014.09_linux/
endif

ifdef CROSS_COMPILE
CC       = $(CROSS_COMPILE)gcc
CXX      = $(CROSS_COMPILE)g++
TC       = $(CROSS_TC_DIR)gcc-freescale/fsl-linaro-toolchain/
else
CC      = gcc
CXX     = g++
TC      = /home/linca/freescale/gcc-freescale/fsl-linaro-toolchain/
endif

#编译器，C文件使用GCC,cpp文件使用g++
#CC       = arm-linux-gcc
#CXX      = arm-linux-g++
#TC       = /home/linca/freescale/gcc-freescale/fsl-linaro-toolchain/
#CC      = /home/linca/freescale/gcc-freescale/fsl-linaro-toolchain/bin/arm-linux-gcc
#CXX     = /home/linca/freescale/gcc-freescale/fsl-linaro-toolchain/bin/arm-linux-g++
#CC      = gcc
#CXX     = g++

#运行平台
PLATFORM = -D PLATFORM_OBU

#编译器的配置,-Wall:输出所有的警告信息 -O2:在编译时进行优化，-g:标示编译debug的版本
CFLAGS=-Os -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wpointer-arith -Wcast-qual -Winline -I.
CFLAGS += $(PLATFORM)
#DEPFLAGS=-MMD
DEPFLAGS=
WELIB_FLAG= -DWE_NOLIBM=y
STRIPFLAGS= -Wl,-s
WEDEF_FLAG= -DWE_ESSENTIAL=y

XCFLAGS=$(CFLAGS) $(DEPFLAGS) $(WELIB_FLAG) $(WEDEF_FLAG)

#源文件目录，以Makefile所在目录为根目录
SRC =      ../Main/src                \
           ../Os/src                 \      

#头文件目录，以Makefile所在目录为根目录
INCLUDE = -I $(TC)/include \
          -I ../Lib/h                  \
          -I ../Main/h                 \
          -I ../Os/h

#库文件目录，以Makefile所在目录为根目录
#LIB = -lrt -lm -static

LIB = -L $(TC)/lib/ -L ../Lib/aObu/ -lm -lrt -lhtgps -lhtble -lhtpc -lhtroutalk -lhtrf -lhtmc -lhtlbs -lhtlog -lhte\
      -L ../../../../../Common/OpLib/libObu/  -lpthread

#链接参数
LDFLAGS = $(LIB)

#输出文件
OUTPUT = GetTestData
ifdef OUTPUT_PREFIX
    OUTPUT := $(OUTPUT_PREFIX)$(OUTPUT)
endif

####################################下面就不用该了########################################
#把所有C文件都编译成o文件
%.o : %.c
#$(CC) $(XCFLAGS) $(INCLUDE) -c $< -o $@
	@$(CC) $(XCFLAGS) $(STRIPFLAGS) $(INCLUDE) -c -g $< -o $@


#将所有目录下的C/C++文件生成一个列表，存入变量SOURCES
SOURCES := $(foreach dir,$(SRC),$(wildcard $(dir)/*.c))

OBJS = $(patsubst %.c,%.o,$(SOURCES))

$(OUTPUT) : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(XCFLAGS) $(STRIPFLAGS) -o $(OUTPUT)
	chmod a+x $(OUTPUT)

clean:
	rm -rf $(OBJS)
	rm -rf $(OUTPUT)

#########################################################################################
