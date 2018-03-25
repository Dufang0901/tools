#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>

int main()
{
    /*init message queue*/
    struct mq_attr mqAttr;
    mqd_t mqID;
    
    /*open msag queue */
    memset(&mqAttr,0x00,sizeof(mqAttr));
    mqAttr.mq_msgsize = 4;
    mqAttr.mq_maxmsg = 50;
    
    mqID = mq_open("/processA",O_RDWR | O_CREAT | O_NONBLOCK, S_IRWXU | S_IRWXO | S_IRWXG, &mqAttr);
    if(-1 == mqID)
    {
        printf("mq_open() failed.\n");
        return -1;
    }

    /*receive message*/
    ssize_t recvLen;
    int recvBuf = -1;
    unsigned int prio;
    
    printf("processA begin to receive message...\n");
    recvLen = mq_receive(mqID,(char *)&recvBuf,sizeof(recvBuf),&prio);
    printf("######## recvLen is %d recvBuf is %d\n",recvLen,recvBuf);

    return 1;

}

