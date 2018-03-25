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
    
    mqID = mq_open("/processA",O_WRONLY);
    if(-1 == mqID)
    {
        printf("mq_open() failed.\n");
        return -1;
    }

    /*send message*/
    int ret;
    int sndBuf = 100;
    
    ret = mq_send(mqID,&sndBuf,sizeof(sndBuf),0);
    printf("######## mq_send returns %d\n",ret);

    return 1;

}

