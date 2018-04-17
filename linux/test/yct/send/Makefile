all:
	arm-linux-gcc main.c yct.c OsPack.c -o yctSend -lm -lrt
	cp yctSend ../
	chmod 777 ../yctSend
clean:
	rm yctSend
	rm ../yctSend