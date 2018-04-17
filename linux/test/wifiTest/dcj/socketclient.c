//c_unix.c
#include <stdio.h>
#include <sys/socket.h>  
#include <sys/types.h>
#include <sys/select.h>
#include <sys/un.h>
#include <unistd.h>
#define UNIX_DOMAIN "/tmp/wifiBus/wifibus_yct"
#define UNIX_LOCAL_WIFIBUS_L     "/tmp/wifiBus/wifibus_local"       //用于socket通信的本地串口sun_path

int SendMsg(char *pMsg,int msgLen);
int RecvMsg(char *pMsg,int *msgLen);


int connect_fd;

int main(void)
{
    
    int ret;
    char snd_buf[1024] = "1234567890abcdefg!";
	char rev_buf[1024];
	int rev_len;
    int i;
    static struct sockaddr_un srv_addr;
//creat unix socket
    connect_fd=socket(PF_UNIX,SOCK_DGRAM,0);
    if(connect_fd<0)
    {
        perror("cannot create communication socket");
        return 1;
    } 

	unlink(UNIX_DOMAIN);
  
	bzero(&srv_addr,sizeof(srv_addr));
    srv_addr.sun_family=PF_UNIX;
    strcpy(srv_addr.sun_path,UNIX_DOMAIN);

	if(bind(connect_fd,(struct sockaddr *)&srv_addr,sizeof(srv_addr)) < 0)  
    {  
        printf("Card dev bind failed.\n"); 
        close(connect_fd); 
        return -1; 
    }
   
//send info server
    while(1)
	{
		printf("pid is %d\n",getpid());
        	SendMsg(snd_buf, 15);
		//memset(rev_buf,0, sizeof(rev_buf));
		//RecvMsg(rev_buf, &rev_len);
		//printf("recive: %s\n", rev_buf);
		sleep(10);
	}
    close(connect_fd);
    return 0;
}

int SendMsg(char *pMsg,int msgLen)
{
    struct sockaddr_un serverAddr;
    int    peer_addr_len;

    peer_addr_len = sizeof(serverAddr);
    
    bzero(&serverAddr,sizeof(serverAddr));  
    serverAddr.sun_family = PF_UNIX;  
    strcpy(serverAddr.sun_path, UNIX_LOCAL_WIFIBUS_L);

    printf("CardDevSendMsg msglen is %d.\n",msgLen);

    if(sendto(connect_fd,pMsg,msgLen,0,(struct sockaddr *)&serverAddr,peer_addr_len) == msgLen)
    {
    	printf("send msg successfully.\n");
        return 1;
    }
	printf("send msg failed.\n");
    return 0;
}

int RecvMsg(char *pMsg,int *msgLen)
{
    struct  sockaddr_un serverAddr;
    int     peer_addr_len;
    ssize_t ret;
    
    peer_addr_len = sizeof(serverAddr);
    
    if((ret = recvfrom(connect_fd,pMsg,1024,0,(struct sockaddr *)&serverAddr,(socklen_t *)&peer_addr_len)) > 0)
    {
        *msgLen = ret;
        return 1;
    }
    return 0;
}



