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
    
    mqID = mq_open( "/DMS_MQUEUE_SEND",O_WRONLY);
	//mqID = mq_open( "/DMS_MQUEUE_SEND",O_RDWR | O_CREAT | O_NONBLOCK, S_IRWXU | S_IRWXO | S_IRWXG, &mqAttr);
	
	/* getting the attributes from the queue        --  mq_getattr() */
    if(mq_getattr(mqID, &msgq_attr) == -1)
    {
        perror("mq_getattr");
        exit(1);
    }

    printf("Queue \"%s\":\n\t- stores at most %ld messages\n\t- \
        large at most %ld bytes each\n\t- currently holds %ld messages\n", 
        "/DMS_MQUEUE_SEND", msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);
	
	
    if(-1 == mqID)
    {
        printf("mq_open() failed.\n");
        return -1;
    }
	
	

    /*send message*/
    int ret;
    int sndBuf = 100;
	int buf[3]={5,6,7};
    
    ret = mq_send(mqID,(char *)&buf[0],sizeof(buf),0);
	//ret = mq_send(mqID,&sndBuf,sizeof(sndBuf),0);
    if(-1 == ret)
		printf("######## mq_send returns %d,%s\n",ret,strerror(errno));

    return 1;

}

