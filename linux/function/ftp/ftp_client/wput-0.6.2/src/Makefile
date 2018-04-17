#wput.mk
#mm make file for wput
SHELL=/bin/bash
prefix=/usr/local
bindir=${exec_prefix}/bin
localedir=$(prefix)/share/locale
CC=gcc
CFLAGS=  -Wall -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64  -g -DLOCALEDIR=\"$(localedir)\" -INONE/include
LIBS=   -lgnutls-openssl
EXE=../wput
GETOPT=
MEMDBG=
OBJ=wput.o netrc.o ftp.o ftplib.o utils.o progress.o socketlib.o queue.o ftp-ls.o $(GETOPT) $(MEMDBG)
HEAD=wput.h netrc.h ftp.h ftplib.h utils.h progress.h socketlib.h _queue.h windows.h config.h constants.h

all: wput

wput.o ftp.o queue.o: $(HEAD)
$(OBJ): utils.h windows.h constants.h config.h
socketlib.o: socketlib.h
progress.o: progress.h
ftplib.o: socketlib.h ftplib.h
ftp-ls.o: ftp.h wget.h url.h

wput:   $(OBJ)
	$(CC) -o $(EXE) $(OBJ) $(LIBS)
clean:
	rm -f *.o *~ *.bak ../wput ../wdel getopt/*.o
win-clean: clean
	rm -r msvcpp/[Dd]ebug msvcpp/[Rr]elease ../wput.exe
