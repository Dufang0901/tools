#include<stdio.h>
#include<string.h>
#include"serialOpen.h"
#include"serialCapacityTest.h"

char testBuf[]="Trainline started as a call center for booking train tickets. Fifteen years later, they’re on a mission to help their customers make smarter rail journeys. With ￡1.6 billion ticket transactions annually and 27 million visitors per month to the Trainline App and website, Trainline sets the pace in mobile rail-commerce. They have been able to evolve thanks to a robust, scalable IT platform—now supported by GitHub Enterprise.";

void RsMsgPro(S_SerialState *psRsState)
{
	char recvBuf[1024];
	bzero(recvBuf,1024);
	if(Rs485RecvMsg(psRsState->comFd,recvBuf,1023)>0)
	{
		printf("########### recv:%s.\n",recvBuf);
		if(strcmp(recvBuf,testBuf) == 0)
		{
			printf("########### recv ok.\n");
			pthread_rwlock_wrlock(&psRsState->lock);
			psRsState->recvCnt++;
			if(psRsState->recvCnt == 0x7FFFFFFF)
				psRsState->recvCnt = 0;
#if 0
			psRsState->sendCnt++;
			if(strlen(testBuf) == Rs485SendMsg(psRsState->comFd,testBuf,strlen(testBuf),&psRsState->sDestAddr))
			{
				psRsState->sendSuccessCnt++;
				//printf("@@@@@@@@@@@@@@@@ send ok.\n");
			}
			else
				//printf("@@@@@@@@@@@@@@@@ send failed.\n");
#endif
			pthread_rwlock_unlock(&psRsState->lock);
			
		}
		
		
	}
	else
		printf("#################### recv error.\n");
	
}

void *TestThread(void *arg)
{
	S_SerialState *psRsState = (S_SerialState *)arg;
	int comFd,timerFd,maxFd,ret;
	fd_set fds;
	
	timerFd = TimerInit();
	printf("in test thread, timerFd is %d\n",timerFd);
	comFd = psRsState->comFd;
	maxFd = timerFd > comFd?timerFd:comFd;
	
	while(1)
	{
        FD_ZERO(&fds);
		FD_SET(timerFd,&fds);
        FD_SET(comFd,&fds);
		
		Again:
        ret = select(maxFd + 1,&fds,NULL,NULL,NULL);
        if(-1 == ret)
        {
            if(EINTR == errno)
            {
                goto  Again;
            }
        }
       
        if(0 == ret)
        {
            //超时
            continue;
        }

        if(FD_ISSET(comFd,&fds))
        {
		printf("##########################\n");
            /*收到消息队列的消息*/
            RsMsgPro(psRsState);
            continue;
        }
		if(FD_ISSET(timerFd,&fds))
		{
			uint64_t exp;
			read(timerFd,&exp,sizeof(exp));
			pthread_rwlock_wrlock(&psRsState->lock);
			psRsState->sendCnt++;
#if 0
			if(strlen(testBuf) == Rs485SendMsg(psRsState->comFd,testBuf,strlen(testBuf),&psRsState->sDestAddr))
			{
				psRsState->sendSuccessCnt++;
				printf("@@@@@@@@@@@@@@@@ send ok.\n");
			}
			else
				printf("@@@@@@@@@@@@@@@@ send failed.\n");
#endif
			if(psRsState->sendCnt == 0x7FFFFFFF)
				psRsState->sendCnt = 0;
			if(psRsState->sendSuccessCnt == 0x7FFFFFFF)
				psRsState->sendSuccessCnt = 0;
			
			pthread_rwlock_unlock(&psRsState->lock);
		}
	}
}

void TimerPro(int timerFd,S_ProcessRes *pInRes)
{
	FILE *fp = NULL;
	int sendCnt2323,sendSuccessCnt2323,recvCnt2323;
	int sendCnt2324,sendSuccessCnt2324,recvCnt2324;
	int sendCnt4851,sendSuccessCnt4851,recvCnt4851;
	int sendCnt4852,sendSuccessCnt4852,recvCnt4852;
	uint64_t exp;

	read(timerFd,&exp,sizeof(exp));

	fp = fopen("/home/bic/Code/serialCapacity.txt","w");
	if(fp == NULL)
	{
		printf("main process timer pro: open file failed.\n");
		return;
	}
	/*获取232_3结果*/
	pthread_rwlock_rdlock(&pInRes->state2323.lock);
	sendCnt2323        = pInRes->state2323.sendCnt;
	sendSuccessCnt2323 = pInRes->state2323.sendSuccessCnt;
	recvCnt2323        = pInRes->state2323.recvCnt;
	
	pthread_rwlock_unlock(&pInRes->state2323.lock);

	/*获取232_4结果*/
	pthread_rwlock_rdlock(&pInRes->state2324.lock);
	sendCnt2324        = pInRes->state2324.sendCnt;
	sendSuccessCnt2324 = pInRes->state2324.sendSuccessCnt;
	recvCnt2324        = pInRes->state2324.recvCnt;
	pthread_rwlock_unlock(&pInRes->state2324.lock);

	/*获取485_1结果*/
	pthread_rwlock_rdlock(&pInRes->state4851.lock);
	sendCnt4851        = pInRes->state4851.sendCnt;
	sendSuccessCnt4851 = pInRes->state4851.sendSuccessCnt;
	recvCnt4851        = pInRes->state4851.recvCnt;
	pthread_rwlock_unlock(&pInRes->state4851.lock);

	/*获取485_2结果*/
	pthread_rwlock_rdlock(&pInRes->state4852.lock);
	sendCnt4852        = pInRes->state4852.sendCnt;
	sendSuccessCnt4852 = pInRes->state4852.sendSuccessCnt;
	recvCnt4852        = pInRes->state4852.recvCnt;
	pthread_rwlock_unlock(&pInRes->state4852.lock);

	fprintf(fp,"RS232_3:发送%d次 发送成功%d次 接收%d 次\n",sendCnt2323,sendSuccessCnt2323,recvCnt2323);
	fprintf(fp,"RS232_4:发送%d次 发送成功%d次 接收%d 次\n",sendCnt2324,sendSuccessCnt2324,recvCnt2324);
	fprintf(fp,"RS485_1:发送%d次 发送成功%d次 接收%d 次\n",sendCnt4851,sendSuccessCnt4851,recvCnt4851);
	fprintf(fp,"RS485_2:发送%d次 发送成功%d次 接收%d 次\n",sendCnt4852,sendSuccessCnt4852,recvCnt4852);
	fclose(fp);

	printf("RS232_3:发送%d次 发送成功%d次 接收%d 次\n",sendCnt2323,sendSuccessCnt2323,recvCnt2323);
	printf("RS232_4:发送%d次 发送成功%d次 接收%d 次\n",sendCnt2324,sendSuccessCnt2324,recvCnt2324);
	printf("RS485_1:发送%d次 发送成功%d次 接收%d 次\n",sendCnt4851,sendSuccessCnt4851,recvCnt4851);
	printf("RS485_2:发送%d次 发送成功%d次 接收%d 次\n",sendCnt4852,sendSuccessCnt4852,recvCnt4852);
}

static int TimerInit(void)
{
    struct itimerspec newValue;
    struct timespec now;
    int fd;
    
    if (clock_gettime(CLOCK_MONOTONIC , &now) == -1)
    {
        perror("Server process clock_gettime");
        return -1;
    }
    
    newValue.it_value.tv_sec = now.tv_sec + 1;
    newValue.it_value.tv_nsec = now.tv_nsec;
    newValue.it_interval.tv_sec = 1;
    newValue.it_interval.tv_nsec = 0;
    
    fd = timerfd_create(CLOCK_MONOTONIC , 0);
    
    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &newValue, NULL) == -1)
    {
        perror("Server process timerfd_settime");
        return -1;
    }
    
    return fd;
}


void main()
{
	int timerFd,ret;
	pthread_t thread232_3,thread232_4,thread485_1,thread485_2;
	S_ProcessRes sProcessRes;
	fd_set fds;

	memset(&sProcessRes,0,sizeof(sProcessRes));
	
	/*1 初始化定时器*/
	timerFd = TimerInit();
	
	/*2 打开串口*/
	sProcessRes.state2323.comFd = Rs485DevInit(BIC_EXPAND_RS485_2);
	sProcessRes.state2324.comFd = Rs485DevInit(BIC_EXPAND_RS485_3);
	sProcessRes.state4851.comFd = Rs485DevInit(BIC_EXPAND_RS485_0);
	sProcessRes.state4852.comFd = Rs485DevInit(BIC_EXPAND_RS485_1);
	
	/*3 初始化读写锁*/
	pthread_rwlock_init(&sProcessRes.state2323.lock,NULL);
	pthread_rwlock_init(&sProcessRes.state2324.lock,NULL);
	pthread_rwlock_init(&sProcessRes.state4851.lock,NULL);
	pthread_rwlock_init(&sProcessRes.state4852.lock,NULL);
	
	/*4 初始化目的地地址*/
	bzero(&sProcessRes.state2323.sDestAddr,sizeof(struct sockaddr_un)); 
	bzero(&sProcessRes.state2324.sDestAddr,sizeof(struct sockaddr_un)); 
	bzero(&sProcessRes.state4851.sDestAddr,sizeof(struct sockaddr_un)); 
	bzero(&sProcessRes.state4852.sDestAddr,sizeof(struct sockaddr_un)); 
    sProcessRes.state2323.sDestAddr.sun_family = PF_UNIX; 
	sProcessRes.state2324.sDestAddr.sun_family = PF_UNIX; 
	sProcessRes.state4851.sDestAddr.sun_family = PF_UNIX; 
	sProcessRes.state4852.sDestAddr.sun_family = PF_UNIX; 	
    strcpy(sProcessRes.state2323.sDestAddr.sun_path,UNIX_LOCAL_UART_L);
	strcpy(sProcessRes.state2324.sDestAddr.sun_path,UNIX_LOCAL_UART_L);
	strcpy(sProcessRes.state4851.sDestAddr.sun_path,UNIX_LOCAL_UART_L);
	strcpy(sProcessRes.state4852.sDestAddr.sun_path,UNIX_LOCAL_UART_L);
	
	/*5 启动测试线程*/
	pthread_create(&thread232_3,NULL,TestThread,(void *)&sProcessRes.state2323);
	printf("RS232_3 test:comFd is %d thread id is %d\n",sProcessRes.state2323.comFd,thread232_3);
#if 0
	pthread_create(&thread232_4,NULL,TestThread,(void *)&sProcessRes.state2324);
	printf("RS232_4 test thread id is %d\n",thread232_4);
	pthread_create(&thread485_1,NULL,TestThread,(void *)&sProcessRes.state4851);
	printf("RS485_1 test thread id is %d\n",thread485_1);
	pthread_create(&thread485_2,NULL,TestThread,(void *)&sProcessRes.state4852);
	printf("RS485_2 test thread id is %d\n",thread485_2);
#endif	
	while(1)
    {
        FD_ZERO(&fds);
        FD_SET(timerFd,&fds);
		
		Again:
        ret = select(timerFd + 1,&fds,NULL,NULL,NULL);
        if(-1 == ret)
        {
            if(EINTR == errno)
            {
                goto  Again;
            }
        }
       
        if(0 == ret)
        {
            //超时
            continue;
        }

        if(FD_ISSET(timerFd,&fds))
        {
			printf("@@@@@@@@@@@@@@ timer Pro.\n");
            /*收到消息队列的消息*/
            TimerPro(timerFd,&sProcessRes);
            continue;
        }
	}
	
}
