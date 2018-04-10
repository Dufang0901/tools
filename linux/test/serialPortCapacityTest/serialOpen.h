#ifndef _SERIAL_OPEN_H_
#define _SERIAL_OPEN_H_

#include<stdio.h>
#include"OsPack.h"

/*扩展的设备名*/
#define BIC_EXPAND_RS485_0          "ttyRs485_0"
#define BIC_EXPAND_RS485_1          "ttyRs485_1"
#define BIC_EXPAND_RS485_2          "ttyRs485_2"
#define BIC_EXPAND_RS485_3          "ttyRs485_3"

#define UNIX_LOCAL_UART_L       "/home/bic/Expand/uart_local"        //用于socket通信的本地串口sun_path
#define UNIX_EXPAND_UART_E0     "/home/bic/Expand/uart_expand0"      //用于socket通信的扩展串口0sun_path
#define UNIX_EXPAND_UART_E1     "/home/bic/Expand/uart_expand1"      //用于socket通信的扩展串口1sun_path
#define UNIX_EXPAND_UART_E2     "/home/bic/Expand/uart_expand2"      //用于socket通信的扩展串口2sun_path
#define UNIX_EXPAND_UART_E3     "/home/bic/Expand/uart_expand3"      //用于socket通信的扩展串口3sun_path
#define UNIX_EXPAND_UART_CFG    "/home/bic/Expand/uart_config"       //用于socket通信的扩展串口配置sun_path

int Rs485DevInit(char *pInDev);
int Rs485SendMsg(int inDevFd,char *pInMsg,int msgLen,struct sockaddr_un *psInDestAddr);
int Rs485RecvMsg(int inDevFd,char *pBuf,int bufLen);
#endif