/*****************************************************************************
  FileName:     OsPack.h
  Author:       ����
  Version:      1.0
  Date:         2013.03.11
  Description:  ��װ�ײ㺯��
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2013.03.04     V1.0      create
                 linca    2013.07.03     V2.0     ��Ӹ��ֹ���
                 linca    2015.03.16     V3.0     ����̲߳��ֹ���  
*****************************************************************************/


#ifndef _OS_PACK_H_
#define _OS_PACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/un.h>
#include <sys/timerfd.h>
#include <sys/reboot.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>
#include <fcntl.h>
//#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <mqueue.h>
#include <netdb.h>
#include <termio.h>
#include <getopt.h>
#include <setjmp.h>
#include <dirent.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#include <linux/if.h>
#include <linux/i2c.h>  
#include <linux/i2c-dev.h> 
#include <linux/reboot.h>
#include <linux/watchdog.h>
#include <linux/types.h>

#include "mytype.h"

typedef void Sigfunc(int);
typedef void (*sighandler_t)(int); 

/*******************************�׽��ְ�������*******************************/
int Socket(int family, int type, int protocol);
int Bind(int sockfd,struct sockaddr *myaddr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen);
int Connect(int sockfd, const struct sockaddr *servaddr,socklen_t addrlen);
int Close(int sockfd);

ssize_t Sendto(int sockfd, const void *buf, size_t nbytes ,int flags,const struct sockaddr *to, socklen_t addrlen);
ssize_t Recvfrom(int sockfd, void *buf, size_t nbytes ,int flags, struct sockaddr *from, socklen_t *pAddrlen);

/****************************IO��д�����İ�������*****************************/
ssize_t Read(int fd, void *buf, size_t nbytes);
ssize_t Write(int fd, void *buf, size_t nbytes);
ssize_t PeekData(int sockfd, void *buff, size_t nbytes);

/************�����ֽ��������ֵ��ASCII�ַ���֮���ת�������İ�������************/
int Inet_pton(int family, const char *strptr, void *addrptr);

/*****************************���̿��Ƶİ�������******************************/
pid_t Fork(void);

/*****************************�����źŵİ�������******************************/
Sigfunc *Signal(int signo, Sigfunc *func);
void ProcessTimer(struct itimerval *itimerval, Sigfunc *func);

/*****************************�߳̿��Ƶİ�������******************************/
int PthreadCreate(pthread_t *thread,pthread_attr_t *attr,void *(*start_routine)(void *),void *arg);
void PthreadExit(void *retval);
int PthreadJoin(pthread_t thread,void **thread_return);
int PthreadCancel(pthread_t thread);

/*****************************�ź����İ�������******************************/
int SemInit(sem_t *sem,int pshared,unsigned int value);
int SemWait(sem_t *sem);
int SemPost(sem_t *sem);
int SemDestroy(sem_t *sem);



/*****************************��Ϣ���еİ�������******************************/
int MsgGet(key_t key,int flag);
int MsgSnd(int msgid, const void *ptr, size_t nbytes, int flag);
int MsgRcv(int msgid, void *ptr, size_t nbytes,long int type, int flag);


/*****************************ϵͳ����******************************/
int System(const char *cmdLine);
void Panic(char * file, int line);
#define Assert(cond) do {                                \
        if (!(cond))                                     \
            Panic(__FILE__, __LINE__);                   \
    } while (0) 

#endif