#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include "OsPack.h"
#include "OsCommon.h"
#include "get_cpu.h"
#include "PeripheralsAcce.h"
#include <math.h>

#define OBU_TEST_SOFT_WARE  "1.1.0"



static Char g_LocalogName[128];    //本地日志命名

static void ObuTestTimerPro(Int32 inTimerFd, S_PhAcce *psPhAcce);
static int TestTimerInit(void);

static int executeCmd(const char *cmd, char *result)
{
    char buf_ps[1024];
    char ps[1024]={0};
    FILE *ptr;
    strcpy(ps, cmd);
    if((ptr=popen(ps, "r"))!=NULL)
    {
        while(fgets(buf_ps, 1024, ptr)!=NULL)
        {
           strcat(result, buf_ps);
           if(strlen(result)>1024)
               break;
        }
        pclose(ptr);
        ptr = NULL;
        return 0;
    }
    else
    {
        printf("popen %s error\n", ps);
        return -1;
    }
}


/*********************************************************
    Function:       TestTimerInit
    Description:    初始化定时器
    Param:          none          
    Return:         定时描述符
    Other:          none
*********************************************************/ 
static int TestTimerInit(void)
{
    struct itimerspec newValue;
    struct timespec now;
    int fd;
    
    if (clock_gettime(CLOCK_MONOTONIC , &now) == -1)
    {
        perror("StationProcess clock_gettime");
        exit(0);
    }
    
    newValue.it_value.tv_sec = now.tv_sec + 1;
    newValue.it_value.tv_nsec = now.tv_nsec;
    newValue.it_interval.tv_sec = 1;
    newValue.it_interval.tv_nsec = 0;
    
    fd = timerfd_create(CLOCK_MONOTONIC , 0);
    
    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &newValue, NULL) == -1)
    {
        perror("Station timerfd_settime");
        exit(-1);
    }
    
    return fd;
}

/*********************************************************
    Function:       ObuTestTimerPro
    Description:    定时器的处理
    Param:          inTimerFd          
    Return:         定时描述符
    Other:          none
*********************************************************/ 
static void ObuTestTimerPro(Int32 inTimerFd, S_PhAcce *psPhAcce)
{
    static Int32 ledBlink = 0;
    Uint64  timerCnt;
	
	char buf[1024] = {0};
        
    if(Read(inTimerFd, &timerCnt, sizeof(Uint64)) == sizeof(Uint64))
    {

        time_t timetTemp;
        struct tm  sTimeTm;        
        time(&timetTemp);
        localtime_r(&timetTemp,&sTimeTm);
        char dateStr[32];
        char timeStr[32];
        sprintf(dateStr, "%04d-%02d-%02d", sTimeTm.tm_year+1900,sTimeTm.tm_mon+1,sTimeTm.tm_mday);
        sprintf(timeStr, "%02d:%02d:%02d", sTimeTm.tm_hour, sTimeTm.tm_min, sTimeTm.tm_sec);
        
        if(ledBlink++ %10){
            float pcup = get_pcpuTotal();
   			float pmem = get_pmemTotal();

            sprintf(buf, "%s,%s,cpu,%0.3f,mem,%0.3f\n", dateStr,timeStr,pcup,pmem);
              
			FILE *fp;
			fp = fopen("/root/sysData.csv", "a+");
			fputs(buf, fp);
			fflush(fp);
			fclose(fp);
		}
 
       // if(psPhAcce->okFlag){
            float tmp = psPhAcce->acceX * psPhAcce->acceX + 
                    psPhAcce->acceY * psPhAcce->acceY + 
                    psPhAcce->acceZ * psPhAcce->acceZ -
                    9.8*9.8;
            sprintf(buf, "%s,%s,x,%3.1f,y,%3.1f,z,%3.1f,accle,%3.1f\n", 
                dateStr,timeStr,
                psPhAcce->acceX,psPhAcce->acceY,psPhAcce->acceZ,
                tmp>0?sqrt(tmp):0);
            printf("%s", buf);
            FILE *fp;
			fp = fopen("/root/accleData.csv", "a+");
			fputs(buf, fp);
			fflush(fp);
			fclose(fp);
      //  }
        
        
    }
}

    
int main(void) 
{	
    fd_set fds;
    Int32 maxFds;
    Int32 ret;
   
    /*设置定时器*/
    Int32 timerFd = TestTimerInit();   
    
    S_PhAcce sPhAcce;
    PhAccePthreadStart(&sPhAcce);
    
    printf("begin to loop obuTest program\n"); 
    
    while(1)
    {
        FD_ZERO(&fds);
        FD_SET(timerFd, &fds);
        maxFds = timerFd;
Again:
        ret = select(maxFds + 1,&fds,NULL,NULL,NULL);
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
            ObuTestTimerPro(timerFd, &sPhAcce);
        }
       
    }      
}

