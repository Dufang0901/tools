#include"OsPack.h"
#include"serialOpen.h"


/*********************************************************
    Function:       PhLedInit
    Description:    外设RS485初始化
    Param:          pInDev
    Return:         设备描述符
    Other:          none
*********************************************************/
int Rs485DevInit(char *pInDev)
{
    int Rs485DevFd;
    char Rs485Dev[128];
    struct sockaddr_un serverAddr;
    
    strcpy(Rs485Dev,UNIX_EXPAND_UART_E0);
    
    if(strncmp(pInDev,BIC_EXPAND_RS485_0,strlen(BIC_EXPAND_RS485_0)) == 0)
    {
        strcpy(Rs485Dev,UNIX_EXPAND_UART_E0);
    }
    else if(strncmp(pInDev,BIC_EXPAND_RS485_1,strlen(BIC_EXPAND_RS485_1)) == 0)
    {
        strcpy(Rs485Dev,UNIX_EXPAND_UART_E1);
    }
    else if(strncmp(pInDev,BIC_EXPAND_RS485_2,strlen(BIC_EXPAND_RS485_2)) == 0)
    {
        strcpy(Rs485Dev,UNIX_EXPAND_UART_E2);
    }
    else if(strncmp(pInDev,BIC_EXPAND_RS485_3,strlen(BIC_EXPAND_RS485_3)) == 0)
    {
        strcpy(Rs485Dev,UNIX_EXPAND_UART_E3);
    }
    
    if((Rs485DevFd = Socket(PF_UNIX,SOCK_DGRAM,0)) < 0)  
    {  
        printf("RS485 socket failed.\n");  
        return -1;  
    }
    
    unlink(Rs485Dev);
    bzero(&serverAddr,sizeof(serverAddr));  
    serverAddr.sun_family = PF_UNIX;
    strcpy(serverAddr.sun_path,Rs485Dev);  

    if(Bind(Rs485DevFd,(struct sockaddr *)&serverAddr,sizeof(serverAddr)) < 0)  
    {  
        printf("Led bind failed.\n"); 
        close(Rs485DevFd); 
        return -1; 
    }
    
    return Rs485DevFd;
}



/*********************************************************
    Function:       PhRs485SendMsg
    Description:    485发送消息
    Param:          inPassFlowFd
                    pInMsg
                    msgLen
                    psInDestAddr
    Return:         发送返回值
    Other:          none
*********************************************************/
int Rs485SendMsg(int inDevFd,char *pInMsg,int msgLen,struct sockaddr_un *psInDestAddr)
{
    int    peer_addr_len;
   
    int i;
#if 0
    for(i = 0;i < msgLen;i++)
        printf("%02x ",pInMsg[i]);
    printf("\n");
#endif
    peer_addr_len = sizeof(struct sockaddr_un);
    if(Sendto(inDevFd,pInMsg,msgLen,0,(struct sockaddr *)psInDestAddr,peer_addr_len) == msgLen)
    {
        return msgLen;
    }
    return 0;
}



/*********************************************************
    Function:       PhRs485RecvMsg
    Description:    Rs485接收
    Param:          inDevFd
                    pInMsg
                    pInMsgLen
    Return:         结果
    Other:          none
*********************************************************/
int Rs485RecvMsg(int inDevFd,char *pBuf,int bufLen)
{
    struct  sockaddr_un serverAddr;
    int     peer_addr_len;
    ssize_t ret;
    
    peer_addr_len = sizeof(serverAddr);
    
    if((ret = Recvfrom(inDevFd,pBuf,bufLen,0,(struct sockaddr *)&serverAddr,(socklen_t *)&peer_addr_len)) > 0)
    {
        return ret;
    }
    return -1;
}




