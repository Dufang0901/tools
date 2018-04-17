#######################################
#    Name:Makefile
#    Author:     linca
#    Date:       2013.01.28
#   Purpose:    �����ļ�
#######################################

#����jenkins�����޸�
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

#��������C�ļ�ʹ��GCC,cpp�ļ�ʹ��g++
#CC       = arm-linux-gcc
#CXX      = arm-linux-g++
#TC       = /home/linca/freescale/gcc-freescale/fsl-linaro-toolchain/
#CC      = /home/linca/freescale/gcc-freescale/fsl-linaro-toolchain/bin/arm-linux-gcc
#CXX     = /home/linca/freescale/gcc-freescale/fsl-linaro-toolchain/bin/arm-linux-g++
#CC      = gcc
#CXX     = g++

#����ƽ̨
PLATFORM = -D PLATFORM_OBU

#������������,-Wall:������еľ�����Ϣ -O2:�ڱ���ʱ�����Ż���-g:��ʾ����debug�İ汾
CFLAGS=-Os -W -Wall -Wstrict-prototypes -Wmissing-prototypes -Wshadow -Wpointer-arith -Wcast-qual -Winline -I.
CFLAGS += $(PLATFORM)
#DEPFLAGS=-MMD
DEPFLAGS=
WELIB_FLAG= -DWE_NOLIBM=y
STRIPFLAGS= -Wl,-s
WEDEF_FLAG= -DWE_ESSENTIAL=y

XCFLAGS=$(CFLAGS) $(DEPFLAGS) $(WELIB_FLAG) $(WEDEF_FLAG)

#Դ�ļ�Ŀ¼����Makefile����Ŀ¼Ϊ��Ŀ¼
SRC =      ../Main/src                \
           ../Os/src                 \      

#ͷ�ļ�Ŀ¼����Makefile����Ŀ¼Ϊ��Ŀ¼
INCLUDE = -I $(TC)/include \
          -I ../Lib/h                  \
          -I ../Main/h                 \
          -I ../Os/h

#���ļ�Ŀ¼����Makefile����Ŀ¼Ϊ��Ŀ¼
#LIB = -lrt -lm -static

LIB = -L $(TC)/lib/ -L ../Lib/aObu/ -lm -lrt -lhtgps -lhtble -lhtpc -lhtroutalk -lhtrf -lhtmc -lhtlbs -lhtlog -lhte\
      -L ../../../../../Common/OpLib/libObu/  -lpthread

#���Ӳ���
LDFLAGS = $(LIB)

#����ļ�
OUTPUT = GetTestData
ifdef OUTPUT_PREFIX
    OUTPUT := $(OUTPUT_PREFIX)$(OUTPUT)
endif

####################################����Ͳ��ø���########################################
#������C�ļ��������o�ļ�
%.o : %.c
#$(CC) $(XCFLAGS) $(INCLUDE) -c $< -o $@
	@$(CC) $(XCFLAGS) $(STRIPFLAGS) $(INCLUDE) -c -g $< -o $@


#������Ŀ¼�µ�C/C++�ļ�����һ���б����������SOURCES
SOURCES := $(foreach dir,$(SRC),$(wildcard $(dir)/*.c))

OBJS = $(patsubst %.c,%.o,$(SOURCES))

$(OUTPUT) : $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(XCFLAGS) $(STRIPFLAGS) -o $(OUTPUT)
	chmod a+x $(OUTPUT)

clean:
	rm -rf $(OBJS)
	rm -rf $(OUTPUT)

#########################################################################################