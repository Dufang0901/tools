INC_DIR = -I D:/code/myCode/tools/QNX/staticLibAndGlobVariable/libtest  \
          -I D:/code/myCode/tools/QNX/staticLibAndGlobVariable/appModule \

all:
	ntoarmv7-gcc.exe $(INC_DIR) -c appModule.c -o appModule.o
	cp appModule.o ../
clean:
	rm *.o
         