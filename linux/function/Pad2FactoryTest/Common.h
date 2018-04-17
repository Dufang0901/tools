#ifndef _COMMON_H_
#define _COMMON_H_

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

#include <math.h>

#include "MyType.h"

#define HMI_HTE_BUF_LEN_MAX         512 
#define HMI_HTE_UNIX_DIR            "/tmp/can/"
#define HMI_HTE_UNIX_DEST           "/tmp/can/can_client"       /*obu接收pad发送的hte消息时绑定的套接字*/ 
#define HMI_HTE_UNIX_SOURCE         "/tmp/can/can_server"       /*obu接收pad发送的hte消息时绑定的套接字*/ 

typedef struct
{
    Int32               hteComFd;
    
}S_HmiProcessRes;

Int32 HteMsgRecvAndPro(S_HmiProcessRes *psHmiProcessRes);
Int32 HteComOpenCfg(void);
#endif
