INC_DIR = -I D:/code/myCode/tools/QNX/staticLibAndGlobVariable/libtest  \
          -I D:/code/myCode/tools/QNX/staticLibAndGlobVariable/appModule \

LIB = libtest.a

all:testLib.o
	ntoarmv7-ar.exe  crs $(LIB) testLib.o
	cp $(LIB) ../
testLib.o:testLib.c
	ntoarmv7-gcc.exe $(INC_DIR) -c testLib.c -o testLib.o
clean:
	rm *.o
	rm $(LIB)
         