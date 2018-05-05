INC_DIR = -I D:/code/myCode/tools/QNX/staticLibAndGlobVariable/libtest  \
          -I D:/code/myCode/tools/QNX/staticLibAndGlobVariable/appModule \

TARGET = test

all:
	ntoarmv7-gcc.exe  $(INC_DIR) -c main.c -o main.o
	ntoarmv7-gcc.exe  $(INC_DIR) -L./ main.o appModule.o -o $(TARGET) -ltest

clean:
	rm *.o
	rm *.a
	rm $(TARGET)
         