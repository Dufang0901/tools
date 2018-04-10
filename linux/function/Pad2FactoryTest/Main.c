/*****************************************************************************
  FileName:     HmiProcess.c
  Author:       ����
  Version:      1.0
  Date:         2016/7/5 10:34:49
  Description:  �˻��������
  History:
                <author>    <time>    <version>    <desc>
                 linca    2016.07.05     V1.0      create
*****************************************************************************/

#include "Common.h"

/*********************************************************
    Function:       HpTimerInit
    Description:    ��ʼ�������˻�������̵Ķ�ʱ��
    Param:          none          
    Return:         ��ʱ������
    Other:          none
*********************************************************/ 
static int HpTimerInit(void)
{
    struct itimerspec newValue;
    struct timespec now;
    int fd;
    
    if (clock_gettime(CLOCK_MONOTONIC , &now) == -1)
    {
        perror("Location process clock_gettime");
        return -1;
    }
    
    newValue.it_value.tv_sec = now.tv_sec + 1;
    newValue.it_value.tv_nsec = now.tv_nsec;
    newValue.it_interval.tv_sec = 1;
    newValue.it_interval.tv_nsec = 0;
    
    fd = timerfd_create(CLOCK_MONOTONIC , 0);
    
    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &newValue, NULL) == -1)
    {
        perror("Location process timerfd_settime");
        return -1;
    }
    
    return fd;
}

/*********************************************************
    Function:       HpTimerPro
    Description:    �˻�������̵Ķ�ʱ����
    Param:          inTimerFd
                    psInHmiProcessOam
                    psInHmiProcessRes �˻�������̵Ĺ�����Ϣ
    Return:         none
    Other:          none
*********************************************************/ 
static void HpTimerPro(int inTimerFd)
{
    Uint64  timerCnt;

    if(read(inTimerFd, &timerCnt, sizeof(Uint64)) == sizeof(Uint64))
    {
        /*��ʱ����*/
        
    }
}

/*********************************************************
    Function:       HmiProcess
    Description:    �˻��ṩ����
    Param:          psInHmiProcessPar
                    inOamShmId
    Return:         none
    Other:          none
*********************************************************/
int main()
{
    fd_set fds;
    int maxFds;
    int ret;
    
    int     hteComFd = -1;      /*���ڽ���pad���͹�����hteЭ����Ϣ���ļ�������*/
    int     timerFd = -1;
    
    S_HmiProcessRes sHmiProcessRes;
    
    memset(&sHmiProcessRes,0,sizeof(S_HmiProcessRes));
    
    /*������ʱ����FD*/
    timerFd = HpTimerInit();
    if(timerFd < 0)
    {
        printf("Create timefd failed.\n");
        exit(-1);
    }
	
    /*�����ڽ���pad��������het��Ϣ���ļ�������*/
    if((hteComFd = HteComOpenCfg()) <= 0)
    {
        printf("hte com fd open failed, exit.\n");
        exit(-1);
    }
    sHmiProcessRes.hteComFd = hteComFd;

    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(timerFd,&fds);
        FD_SET(hteComFd,&fds);
        maxFds = timerFd  > hteComFd ? timerFd : hteComFd;
        maxFds += 1;
        
Again:
        ret = select(maxFds,&fds,NULL,NULL,NULL);
        if(-1 == ret)
        {
            if(EINTR == errno)
            {
                goto  Again;
            }
        }
       
        if(0 == ret)
        {
            //��ʱ
            continue;
        }
       
        if(FD_ISSET(timerFd,&fds))
        {
            /*��ʱ1��Ĵ���*/
            HpTimerPro(timerFd);
            continue;
        }
        
        if(hteComFd > 0)
        {
            if(FD_ISSET(hteComFd,&fds))
            {
                /*�յ�hteЭ����Ϣ*/
                HteMsgRecvAndPro(&sHmiProcessRes);
                continue;
            }
        }
    }
    return 1;
}

