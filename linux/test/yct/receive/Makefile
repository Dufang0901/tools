all:
	arm-linux-gcc main.c yct.c OsPack.c -o yctReceive -lm -lrt
	cp yctReceive ../
	chmod 777 ../yctReceive
clean:
	rm yctReceive ../yctReceive