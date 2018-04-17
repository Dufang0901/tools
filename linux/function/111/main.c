#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/un.h>
#include <sys/timerfd.h>
#include <sys/reboot.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/ip_icmp.h>
#include <fcntl.h>
//#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <mqueue.h>
#include <netdb.h>
#include <termio.h>
#include <getopt.h>
#include <setjmp.h>
#include <dirent.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#include <linux/if.h>
#include <linux/i2c.h>  
#include <linux/i2c-dev.h> 
#include <linux/reboot.h>
#include <linux/watchdog.h>
#include <linux/types.h>


#define PING_SUCCESFUL          1
#define PING_FAIEED             0


#define ICMP_PACK_BYTES         56              /*ICMP�����ֽ���*/
#define ICMP_PACKET_SIZE        256             /*ICMP�Ľ��շ��ͻ�������С*/
#define PING_ICMP_SEND_MAX      4               /*ICMP��������ʹ���*/
#define PING_WAIT_TIME_MAX      5               /*ICMP�ȴ�Ӧ���ʱ��*/

#define ICMP_SUCCESFUL          1
#define ICMP_FAILED             0

typedef int Int32;
typedef char Char;
typedef unsigned short Uint16;
typedef unsigned char Uint8;

static Uint16 IcmpCalcChksum(Uint16 *pAddr,int len)
{
    int nLeft = len;
    int sum = 0;
    Uint16 *w = pAddr;
    Uint16 answer = 0;
            
    /*��ICMP��ͷ������������2�ֽ�Ϊ��λ�ۼ�����*/
    while(nLeft > 1)
    {
        sum += *w++;
        nLeft -= 2;
    }
    /*��ICMP��ͷΪ�������ֽڣ���ʣ�����һ�ֽڡ������һ���ֽ���Ϊһ��2�ֽ����ݵĸ��ֽڣ����2�ֽ����ݵĵ��ֽ�Ϊ0�������ۼ�*/
    if( nLeft == 1)
    {
        *(Uint8 *)(&answer) = *(Uint8 *)w;
        sum += answer;
    }
    sum = (sum>>16) + (sum & 0xffff);
    sum += (sum>>16);
    answer =~ sum;
    return answer;
}

static int IcmpSetPack(Char *pBuf,int seq)
{
    int packSize;
    struct icmp *icmp;
    struct timeval *tval;
    
    icmp = (struct icmp*)pBuf;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_cksum = 0;
    icmp->icmp_seq = seq;
    icmp->icmp_id = getpid();
    packSize = 8 + ICMP_PACK_BYTES;
    tval= (struct timeval *)icmp->icmp_data;
    gettimeofday(tval,NULL);    /*��¼����ʱ��*/
    icmp->icmp_cksum = IcmpCalcChksum((Uint16 *)icmp,packSize); /*У���㷨*/
    return packSize;
}

/*********************************************************
    Function:       IcmpUnpack
    Description:    ICMP���ݰ��������
    Param:          buf    ����ָ��
                    len    ���ݳ��� 
    Return:         ������
    Other:          none
*********************************************************/
static int IcmpUnpack(Char *pBuf,int len)
{
    int ipHeadLen;
    struct ip *ip;
    struct icmp *icmp;
    
    ip = (struct ip *)pBuf;
    ipHeadLen = ip->ip_hl << 2;    /*��ip��ͷ����,��ip��ͷ�ĳ��ȱ�־��4*/
    icmp = (struct icmp *)(pBuf + ipHeadLen); /*Խ��ip��ͷ,ָ��ICMP��ͷ*/
    len -= ipHeadLen;            /*ICMP��ͷ��ICMP���ݱ����ܳ���*/
    if( len < 8)
    {                /*С��ICMP��ͷ�����򲻺���*/
        printf("ICMP packets length is less then 8.\n");
        return ICMP_FAILED;
    }
    /*ȷ�������յ����������ĵ�ICMP�Ļ�Ӧ*/
    if ((icmp->icmp_type == ICMP_ECHOREPLY) && (icmp->icmp_id == getpid()) )
    {
        printf("Ping succesful,seq is %d.\n",icmp->icmp_seq);
    }
    else
    {
        return ICMP_FAILED;
    }
    return ICMP_SUCCESFUL;
}


static int IcmpSendPacket(int fd,struct sockaddr_in *sDstAddr)
{
    int packLen;
    int i;
    int failedCnt;
    char sendBuf[ICMP_PACKET_SIZE];
    
    
    failedCnt = 0;
    for( i = 0; i < PING_ICMP_SEND_MAX; i++)
    {
        packLen = IcmpSetPack(sendBuf,i); 
        if(sendto(fd,sendBuf,packLen,0,(struct sockaddr *)sDstAddr,sizeof(struct sockaddr_in)) <= 0 )
        {
            failedCnt++;
        }
    }
    
    if(failedCnt == PING_ICMP_SEND_MAX)
    {
        return ICMP_FAILED;
    }
    return ICMP_SUCCESFUL;
}

static int IcmpRecvPacket(int fd,struct sockaddr_in *sSrcAddr)
{
    fd_set fds;
    int maxFds;
    int ret;
    struct timeval selectTime;

    int n;
    int lenTemp;
    char recvBuf[ICMP_PACKET_SIZE];
    
    selectTime.tv_sec = PING_WAIT_TIME_MAX;
    selectTime.tv_usec = 0;
    
    
    FD_ZERO(&fds);
    FD_SET(fd,&fds);
    maxFds = fd + 1;
    
Again:
    ret = select(maxFds,&fds,NULL,NULL,&selectTime);
    if(-1 == ret)
    {
        if(EINTR == errno)
        {
            goto  Again;
        }
    }

    if(0 == ret)
    {
        return ICMP_FAILED;//��ʱ��
    }
    
    if( FD_ISSET(fd,&fds))
    {
        if((n = recvfrom(fd,recvBuf,sizeof(recvBuf),0,(struct sockaddr *)sSrcAddr,(socklen_t *)&lenTemp)) <= 0)
        {
            printf("Ping can not recv data,return %d.\n",n);
            return ICMP_FAILED;
        }
        return IcmpUnpack(recvBuf,n);
    }
    
    return ICMP_FAILED;

}

static int PingOpenDev(const Char *nicDev)
{
    int  pingFd;
    struct protoent *protocol;
    struct ifreq interface;
    int size = 50 * 1024;
    
    if( (protocol = getprotobyname("icmp") )==NULL)
    {
        perror("Ping:Get protoby name failed.");
        return 0;
    }
    
    if( (pingFd = socket(AF_INET,SOCK_RAW,protocol->p_proto) ) < 0)
    {
        perror("Ping: socket error");
        return pingFd;
    }
    
    if(nicDev != NULL)
    {
        strncpy(interface.ifr_ifrn.ifrn_name, nicDev, IFNAMSIZ);
        if(setsockopt(pingFd, SOL_SOCKET, SO_BINDTODEVICE, (char *)&interface, sizeof(interface)) < 0) 
        {
            printf("Ping:Fd SO_BINDTODEVICE failed");
            close(pingFd);
            return 0;
        }
    }

    setsockopt(pingFd,SOL_SOCKET,SO_RCVBUF,&size,sizeof(size) );
    return pingFd;
}

Int32 NicPing(const Char *nicDev, const Char *pingAddr)
{
    int  pingFd;
    struct sockaddr_in sDstAddr;
    struct sockaddr_in sSrcAddr;
    struct in_addr ip_addr;
    
    if((pingFd = PingOpenDev(nicDev)) <= 0)
    {
        return PING_FAIEED;
    }
    
    bzero(&sDstAddr,sizeof(sDstAddr));
    sDstAddr.sin_family = AF_INET;
    
    ip_addr.s_addr = inet_addr(pingAddr);
    sDstAddr.sin_addr  = ip_addr;

    printf("PING (%s): 56 bytes data in ICMP packets.\n",inet_ntoa(sDstAddr.sin_addr));

    if(IcmpSendPacket(pingFd,&sDstAddr) == ICMP_SUCCESFUL)
    {
        if(IcmpRecvPacket(pingFd,&sSrcAddr) == ICMP_SUCCESFUL)
        {
            printf("PING (%s) succesful.\n",pingAddr);
            close(pingFd);
            return PING_SUCCESFUL;
        }
    }

  
    printf("PING (%s) failed.\n",pingAddr);
    close(pingFd);
    return PING_FAIEED;
    
}

int main(int argc, char **argv)
{
	while(1)
	{
		char nicDev[] = "ppp0";
		char ipAddr[] = "192.168.201.223";
		if(NicPing(nicDev,ipAddr) == PING_SUCCESFUL)
		{
			printf("NicPing succesful\n");
			sleep(3);
		}
		else
		{
			printf("NicPing failed\n");
			usleep(500*1000);
		}
	}

    
    return 0;
}