/*****************************************************************************
  FileName:     CardDev.c
  Author:       林锴
  Version:      1.0
  Date:         2015/11/9 17:41:57
  Description:  刷卡设备的操作
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2015.11.09    V1.0       create
*****************************************************************************/

#include "CardDev.h"



static int  cardDevSockFd;

/*********************************************************
    Function:       CardDevInit
    Description:    刷卡设备初始化
    Param:          dev
    Return:         设备描述符
    Other:          none
*********************************************************/
int CardDevInit()
{
    Char cardDev[128];
    struct sockaddr_un serverAddr;
    
    strcpy(cardDev,"/tmp/wifiBus/wifibus_yct");
    
    if((cardDevSockFd = Socket(PF_UNIX,SOCK_DGRAM,0)) < 0)  
    {  
        printf("Card dev socket failed.\n");  
        return -1;  
    }
    
    unlink(cardDev);
    
    bzero(&serverAddr,sizeof(serverAddr));  
    serverAddr.sun_family = PF_UNIX;
    strcpy(serverAddr.sun_path,cardDev);  

    if(Bind(cardDevSockFd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) < 0)  
    {  
        printf("Card dev bind failed.\n"); 
        close(cardDevSockFd); 
        return -1; 
    }
    
    return cardDevSockFd;
}

/*********************************************************
    Function:       CardDevRecvMsg
    Description:    刷卡设备接收消息
    Param:          pMsg
                    msgLen
    Return:         结果
    Other:          none
*********************************************************/
int CardDevRecvMsg(Uint8 *pMsg,Uint16 *msgLen)
{
    struct  sockaddr_un serverAddr;
    int     peer_addr_len;
    ssize_t ret;
    
    peer_addr_len = sizeof(serverAddr);
    
    if((ret = Recvfrom(cardDevSockFd,pMsg,CARD_DEV_MSG_BUF_MAX,0,(struct sockaddr *)&serverAddr,(socklen_t *)&peer_addr_len)) > 0)
    {
        *msgLen = (Uint16)ret;
        return CARD_DEV_OK;
    }
    return CARD_DEV_NONE_OK;
}

/*********************************************************
    Function:       CardDevSendMsg
    Description:    刷卡设备发送消息
    Param:          pMsg
                    msgLen
    Return:         发送返回值
    Other:          none
*********************************************************/
int CardDevSendMsg(Uint8 *pMsg,Uint16 msgLen)
{
    struct sockaddr_un serverAddr;
    int    peer_addr_len;

    peer_addr_len = sizeof(serverAddr);
    
    bzero(&serverAddr,sizeof(serverAddr));  
    serverAddr.sun_family = PF_UNIX;  
    strcpy(serverAddr.sun_path,"/tmp/wifiBus/wifibus_local");

    printf("CardDevSendMsg msglen is %d.\n",msgLen);

    if(Sendto(cardDevSockFd,pMsg,msgLen,MSG_DONTWAIT,(struct sockaddr *)&serverAddr,peer_addr_len) == msgLen)
	//if(Sendto(cardDevSockFd,pMsg,msgLen,0,(struct sockaddr *)&serverAddr,peer_addr_len) == msgLen)
    {
    	printf("%s:%d\n",__func__,__LINE__);
        return CARD_DEV_OK;
    }
	printf("%s:%d:errno=%d:%s\n",__func__,__LINE__,errno,strerror(errno));
    return CARD_DEV_NONE_OK;
}

