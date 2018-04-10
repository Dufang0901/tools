#ifndef _SERIAL_OPEN_H_
#define _SERIAL_OPEN_H_

#include<stdio.h>
#include"OsPack.h"

/*��չ���豸��*/
#define BIC_EXPAND_RS485_0          "ttyRs485_0"
#define BIC_EXPAND_RS485_1          "ttyRs485_1"
#define BIC_EXPAND_RS485_2          "ttyRs485_2"
#define BIC_EXPAND_RS485_3          "ttyRs485_3"

#define UNIX_LOCAL_UART_L       "/home/bic/Expand/uart_local"        //����socketͨ�ŵı��ش���sun_path
#define UNIX_EXPAND_UART_E0     "/home/bic/Expand/uart_expand0"      //����socketͨ�ŵ���չ����0sun_path
#define UNIX_EXPAND_UART_E1     "/home/bic/Expand/uart_expand1"      //����socketͨ�ŵ���չ����1sun_path
#define UNIX_EXPAND_UART_E2     "/home/bic/Expand/uart_expand2"      //����socketͨ�ŵ���չ����2sun_path
#define UNIX_EXPAND_UART_E3     "/home/bic/Expand/uart_expand3"      //����socketͨ�ŵ���չ����3sun_path
#define UNIX_EXPAND_UART_CFG    "/home/bic/Expand/uart_config"       //����socketͨ�ŵ���չ��������sun_path

int Rs485DevInit(char *pInDev);
int Rs485SendMsg(int inDevFd,char *pInMsg,int msgLen,struct sockaddr_un *psInDestAddr);
int Rs485RecvMsg(int inDevFd,char *pBuf,int bufLen);
#endif