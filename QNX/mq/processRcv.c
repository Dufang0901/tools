#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
int main()
{
    /*init message queue*/
    struct mq_attr mqAttr;
	struct mq_attr msgq_attr;
    mqd_t mqID;
	

    /*open msag queue */
    memset(&mqAttr,0x00,sizeof(mqAttr));
    mqAttr.mq_msgsize = 12;
    mqAttr.mq_maxmsg = 50;
    
    mqID = mq_open( "/DMS_MQUEUE_SEND",O_RDWR | O_CREAT | O_NONBLOCK, S_IRWXU | S_IRWXO | S_IRWXG, &mqAttr);
	//mqID = mq_open( "/DMS_MQUEUE_SEND",O_WRONLY);
    if(-1 == mqID)
    {
        printf("mq_open() failed.\n");
        return -1;
    }
	
	
	/* getting the attributes from the queue        --  mq_getattr() */
    if(mq_getattr(mqID, &msgq_attr) == -1)
    {
        perror("mq_getattr");
        exit(1);
    }

    printf("Queue \"%s\":\n\t- stores at most %ld messages\n\t- \
        large at most %ld bytes each\n\t- currently holds %ld messages\n", 
        "/DMS_MQUEUE_SEND", msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);
	

    /*receive message*/
    ssize_t recvLen;
    int recvBuf = -1;
	int buf[3]={1,2,3};
    unsigned int prio;
	printf("processA begin to receive message...\n");
   // while(1){
 #if 1   
    recvLen = mq_receive(mqID,(char *)buf,sizeof(buf),&prio);
    printf("######## recvLen is %d recvBuf is %d %d %d,%s\n",recvLen,buf[0],buf[1],buf[2],strerror(errno));
#else
	   
    recvLen = mq_receive(mqID,(char *)&recvBuf,sizeof(recvBuf),&prio);
    printf("######## recvLen is %d recvBuf is %d,%s\n",recvLen,recvBuf,strerror(errno));
#endif
	//sleep(2);
	//}

    return 1;

}

