/*****************************************************************************
  FileName:     OsPack.c
  Author:       林锴
  Version:      1.0
  Date:         2013.03.11
  Description:  封装底层函数
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2013.03.04     V1.0      create
                 linca    2013.07.03     V2.0     添加各种功能
                 linca    2015.03.16     V3.0     添加线程部分功能,添加消息队列部分功能  
*****************************************************************************/

#include "OsPack.h"
/*******************************套接字包裹函数*******************************/

/*********************************************************
    Function:       Socket
    Description:    包裹socket函数,socket成功返回相应的套接
                    字描述符,失败则返回-1;包裹函数在socket
                    打开失败的时候迫使程序退出
    Param:          none
    Return:         成功打开套接字的描述符
    Other:          none
*********************************************************/
int Socket(int family, int type,int protocol)
{
    int fd;
    if( (fd = socket(family,type,protocol)) <= 0 )
    {
        printf("**********************\n");
        printf("* socket open failed *\n");
        printf("**********************\n");
        //exit(-1);
    }
    return fd;
}

/*********************************************************
    Function:       Bind
    Description:    包裹bind函数,bind成功返回0
                    返回-1且错误号为EADDRINUSE则说明指定端口
                    被其他程序占用,函数报错且迫使程序退出
    Param:          none
    Return:         成功返回0
    Other:          none
*********************************************************/
int Bind(int sockfd,struct sockaddr *myaddr, socklen_t addrlen)
{
    int ret;
    struct sockaddr_in* pAddr = (struct sockaddr_in *)myaddr;
    
    ret = bind(sockfd,myaddr,addrlen);
    if(ret == -1)
    {
        if(errno == EADDRINUSE)
        {
            printf("**********************\n");
            printf("Bind failed,the port %d has been in used.\n", ntohs(pAddr->sin_port));
            printf("**********************\n");
        }
        else
        {
            printf("**********************\n");
            printf("Bind failed,errno num is  %d.\n", errno);
            printf("**********************\n");            
        } 
    }
    return ret;
}

/*********************************************************
    Function:       Listen
    Description:    包裹listen函数,成功返回0
                    失败返回-1,并迫使程序退出
    Param:          none
    Return:         成功返回0
    Other:          none
*********************************************************/
int Listen(int sockfd, int backlog)
{
    int ret;
    ret = listen(sockfd,backlog);
    
    if( ret == -1)
    {
        printf("**********************\n");
        printf("* Listen failed *\n");
        printf("**********************\n");
        //exit(-1);
    }
    return ret;
}

/*********************************************************
    Function:       Accept
    Description:    accept
                    成功返回相应的套接字描述符
                    失败返回-1,并迫使程序退出
    Param:          none
    Return:         成功返回0
    Other:          none
*********************************************************/
int Accept(int sockfd, struct sockaddr *cliaddr, socklen_t *addrlen)
{
    int ret;
ACCEPTAGAIN:
    ret = accept(sockfd,cliaddr,addrlen);
    
    if(ret == -1)
    {
        if(errno == EINTR)
        {
            printf("**********************\n");
            printf("Accept has been interrupted.\n");
            printf("**********************\n");
            goto ACCEPTAGAIN;
        }
        else
        {
            printf("**********************\n");
            printf("Accept failed,.\n");
            printf("**********************\n");            
        }
    }
    return ret;
}

/*********************************************************
    Function:       Connect
    Description:    包裹connect函数,成功返回0
                    失败返回-1,并迫使程序退出
    Param:          none
    Return:         成功返回0
    Other:          none
*********************************************************/
int Connect(int sockfd, const struct sockaddr *servaddr,socklen_t addrlen)
{
    int ret;
    ret = connect(sockfd,servaddr,addrlen);
    
    if(ret == -1)
    {
        printf("**********************\n");
        printf("Connect failed,errno num is  %d.\n", errno);
        printf("**********************\n");
    }
    return ret;
}

/*********************************************************
    Function:       Close
    Description:    包裹close函数,成功返回0
                    失败返回-1
    Param:          none
    Return:         成功返回0
    Other:          none
*********************************************************/
int Close(int sockfd)
{
    int ret;
    ret = close(sockfd);
    
    if(ret == -1)
    {
        printf("**********************\n");
        printf("Closef failed,pid is  %d,errno is %d.\n", getpid(),errno);
        printf("**********************\n");
    }
    return ret;
}

/*********************************************************
    Function:       Sendto
    Description:    包裹sendto函数,成功返回发送长度
                    失败返回-1,并迫使程序退出
    Param:          none
    Return:         成功返回0
    Other:          none
*********************************************************/
ssize_t Sendto(int sockfd, const void *buf, size_t nbytes ,int flags,const struct sockaddr *to, socklen_t addrlen)
{
    ssize_t n;
SENTOAGAIN:
    n = sendto(sockfd,buf,nbytes,flags,to,addrlen);
    if(n == -1)
    {
        if(errno == EINTR)
        {

            goto SENTOAGAIN;
        }
        else if((errno == EAGAIN) || (errno == EWOULDBLOCK))
        {
            return -1;           
        }
        else
        {
            printf("**********************\n");
            printf("Sendto failed,pid is  %d,errno is %d.\n", getpid(),errno);
            printf("**********************\n");
            return -1;
            //exit(-1);
        }
    }
    else
    {
        return n;
    }
}

/*********************************************************
    Function:       Recvfrom
    Description:    包裹recvfrom函数,成功返回接收长度
                    失败返回-1,并迫使程序退出
    Param:          none
    Return:         成功返回0
    Other:          none
*********************************************************/
ssize_t Recvfrom(int sockfd, void *buf, size_t nbytes ,int flags, struct sockaddr *from, socklen_t *pAddrlen)
{
    ssize_t n;
RECVAGAIN:
    n =recvfrom(sockfd,buf,nbytes,flags,from,pAddrlen);
    if(n == -1)
    {
        if(errno == EINTR)
        {

            goto RECVAGAIN;
        }
        else if((errno == EAGAIN) || (errno == EWOULDBLOCK))
        {
            return -1;           
        }
        else
        {
            printf("**********************\n");
            printf("recvfrom failed,pid is  %d,errno is %d.\n", getpid(),errno);
            printf("**********************\n");
            return -1;
            //exit(-1);
        }
    }
    else
    {
        return n;
    }
}

/****************************IO读写操作的包裹函数*****************************/

/*********************************************************
    Function:       Read
    Description:    包裹read函数,成功返回读取长度
                    失败返回-1,并迫使程序退出
    Param:          none
    Return:         成功返回0
    Other:          包裹函数会检查错误类型,如果是EINTR,EAGAIN
                    和EWOULDBLOCK则重新读取
*********************************************************/
ssize_t Read(int fd, void *buf, size_t nbytes)
{
#if 0
    ssize_t n;
READAGAIN:
    n = read(fd,buf,nbytes);
    if(n < 0)
    {   
        if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        {
            goto READAGAIN;
        }
        else
        {
            printf("**********************\n");
            printf("Read failed,pid is  %d,errno is %d.\n", getpid(),errno);
            printf("**********************\n");
            return -1;
            //exit(-1);
        }
    }
    return n;
#else
    /*由于存在非阻塞模式下PIPE为空时读会返回-1,errno为EAGAIN,重新修改Read函数*/
    ssize_t n;
READAGAIN:
    n = read(fd,buf,nbytes);
    if(n < 0)
    {
        if(errno == EWOULDBLOCK)
        {
            return 0;
        }
        else if(errno == EINTR)
        {
            goto READAGAIN;
        }
        else
        {
            printf("**********************\n");
            printf("Read failed,pid is  %d,errno is %d.\n", getpid(),errno);
            printf("**********************\n");
            return -1;
            //exit(-1);
        }
    }
    return n;
#endif
}

/*********************************************************
    Function:       Write
    Description:    包裹write函数,成功返回写入长度
                    失败返回-1,并迫使程序退出
    Param:          none
    Return:         成功返回0
    Other:          包裹函数会检查错误类型,如果是EINTR,EAGAIN
                    和EWOULDBLOCK则重新写入
*********************************************************/
ssize_t Write(int fd, void *buf, size_t nbytes)
{
    ssize_t n;
WRITEAGAIN:
    n = write(fd,buf,nbytes);
    if(n < 0)
    {
        if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        {
            goto WRITEAGAIN;
        }
        else
        {
            printf("**********************\n");
            printf("Write failed,pid is  %d,errno is %d.\n", getpid(),errno);
            printf("**********************\n");
            //exit(-1);
            return -1;
        }
    }
    return n;
}


/*********************************************************
    Function:       PeekData
    Description:    窥看SOCKED数据
    Param:          none          
    Return:         窥看结果
    Other:          超时返回-1
*********************************************************/ 
ssize_t PeekData(int sockfd, void *buff, size_t nbytes)
{
    ssize_t pkLen;
    fd_set fds;
    int maxFds;
    int ret;
    struct timeval selectTime;

    selectTime.tv_sec = 10;
    selectTime.tv_usec = 0;

    FD_ZERO(&fds);
    FD_SET(sockfd,&fds);

    maxFds = sockfd + 1;
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
        return -1;//超时啦
    }

    if( FD_ISSET(sockfd,&fds))
    {
        pkLen = recv(sockfd,buff,nbytes,MSG_PEEK);
        return pkLen;
    }
    return 0;
}

/************网络字节序二进制值与ASCII字符串之间的转换函数的包裹函数************/

/*********************************************************
    Function:       Inet_pton
    Description:    包裹inet_pton函数
                    适用于IPV4 和 IPV6,将字符串转换成网络字节序的二进制值
    Param:          none
    Return:         成功返回1
                    无效输入或则参数不正确返回0,并迫使程序退出
    Other:          
*********************************************************/
int Inet_pton(int family, const char *strptr, void *addrptr)
{
    int ret;
    
    ret = inet_pton(family,strptr,addrptr);
    if(ret == -1)
    {
        printf("Inet_pton failed\n");
    }
    else if(ret == 0)
    {
        printf("The arg os Inet_pton is wrong.\n");
        exit(-1);
    }
    return ret;
    
}


/*****************************进程控制的包裹函数******************************/

/*********************************************************
    Function:       Fork
    Description:    包裹fork函数
    Param:          none
    Return:         成功子进程返回0,父进程返回子进程的描述符
                    失败返回-1,并迫使程序退出
    Other:          
*********************************************************/
pid_t Fork(void)
{
    pid_t pid;
    pid = fork();
    if(pid == -1)
    {
        perror("Fork fail.\n");
        exit(-1);
    }
    return pid;
}

/*****************************处理信号的包裹函数******************************/

/*********************************************************
    Function:       Signal
    Description:    包裹sigaction函数,功能类似signal,
                    但不用重复装载,同时在处理过程中可
                    以阻塞相应的信号。
    Param:          none
    Return:         返回信号处理函数的指针
    Other:          
*********************************************************/
Sigfunc *Signal(int signo, Sigfunc *func)
{
    struct sigaction act,oact;
    
    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask,signo);
    act.sa_flags = 0;
    
    if(signo == SIGALRM)
    {
#ifdef SA_INTERRUPT
        act.sa_flags |=  SA_INTERRUPT;
#endif
    }
    else
    {
#ifdef SA_RESTART
    act.sa_flags |=  SA_RESTART;
#endif
    }
    if(sigaction(signo, &act, &oact) < 0)
    {
        return(SIG_ERR);
    }
    return(oact.sa_handler);
}

/*********************************************************
    Function:       ProcessTimer
    Description:    进程的定时器操作
    Param:          none              
    Return:         time    定时时间
                    func    定时器处理函数入口
    Other:          none
*********************************************************/
void ProcessTimer(struct itimerval *itimerval, Sigfunc *func)
{
    int res = -1;

    do 
    {
        res = setitimer(ITIMER_REAL,itimerval,NULL);
    } while(-1 == res);
    Signal(SIGALRM,func);
}

/*****************************线程控制的包裹函数******************************/
/*********************************************************
    Function:       PthreadCreate
    Description:    包裹pthread_create函数,创建线程
    Param:          参考pthread_create
    Return:         创建结果
    Other:          none
*********************************************************/
int PthreadCreate(pthread_t *thread,pthread_attr_t *attr,void *(*start_routine)(void *),void *arg)
{
    int ret;
    
    ret = pthread_create(thread,attr,start_routine,arg);
    if(ret != 0)
    {
        printf("**********************\n");
        printf("Pthread create failed,pid is %d,errno is %d,%s.\n", getpid(),errno,strerror(errno));
        printf("**********************\n");
    }
    
    return ret;
}

/*********************************************************
    Function:       PthreadExit
    Description:    包裹pthread_exit函数,等待线程结束
    Param:          参考pthread_exit
    Return:         none
    Other:          none
*********************************************************/
void PthreadExit(void *retval)
{
    pthread_exit(retval);
}

/*********************************************************
    Function:       PthreadJoin
    Description:    包裹pthread_join函数,等待线程结束
    Param:          参考pthread_join
    Return:         创建结果
    Other:          none
*********************************************************/
int PthreadJoin(pthread_t thread,void **thread_return)
{
    int ret;
    
    ret = pthread_join(thread,thread_return);
    if(ret != 0)
    {
        printf("**********************\n");
        printf("Pthread Join failed,pid is %d,errno is %d,%s.\n", getpid(),errno,strerror(errno));
        printf("**********************\n");
    }
    
    return ret;
}

/*********************************************************
    Function:       PthreadCancel
    Description:    包裹pthread_cancel函数,取消进程
    Param:          参考pthread_cancel
    Return:         结果
    Other:          none
*********************************************************/
int PthreadCancel(pthread_t thread)
{    
    int ret;
    
    ret = pthread_cancel(thread);
    if(ret != 0)
    {
        printf("**********************\n");
        printf("Pthread create failed,pid is %d,errno is %d,%s.\n", getpid(),errno,strerror(errno));
        printf("**********************\n");
    }
    
    return ret;
}

/*****************************信号量的包裹函数******************************/

/*********************************************************
    Function:       SemInit
    Description:    包裹sem_init函数,初始化信号量
    Param:          参考sem_init
    Return:         结果
    Other:          none
*********************************************************/
int SemInit(sem_t *sem, int pshared, unsigned int value)
{
    int ret;
    
    ret = sem_init(sem,pshared,value);
    if(ret != 0)
    {
        printf("**********************\n");
        printf("Sem init failed,pid is %d,errno is %d,%s.\n", getpid(),errno,strerror(errno));
        printf("**********************\n");
    }
    
    return ret;
}

/*********************************************************
    Function:       SemWait
    Description:    包裹sem_wait函数,信号量V操作
    Param:          参考sem_wait
    Return:         结果
    Other:          none
*********************************************************/
int SemWait(sem_t *sem)
{
    int ret;
    
    ret = sem_wait(sem);
    if(ret != 0)
    {
        printf("**********************\n");
        printf("Sem wait failed,pid is %d,errno is %d,%s.\n", getpid(),errno,strerror(errno));
        printf("**********************\n");
    }
    
    return ret;
}

/*********************************************************
    Function:       SemPost
    Description:    包裹sem_post函数,信号量P操作
    Param:          参考sem_post
    Return:         结果
    Other:          none
*********************************************************/
int SemPost(sem_t *sem)
{
    int ret;

    ret = sem_post(sem);
    if(ret != 0)
    {
        printf("**********************\n");
        printf("Sem post failed,pid is %d,errno is %d,%s.\n", getpid(),errno,strerror(errno));
        printf("**********************\n");
    }
    
    return ret;
}

/*********************************************************
    Function:       SemDestroy
    Description:    包裹sem_destroy函数,信号量注销
    Param:          参考sem_destroy
    Return:         结果
    Other:          none
*********************************************************/
int SemDestroy(sem_t *sem)
{
    int ret;
    
    ret = sem_destroy(sem);
    if(ret != 0)
    {
        printf("**********************\n");
        printf("Sem destroy failed,pid is %d,errno is %d,%s.\n", getpid(),errno,strerror(errno));
        printf("**********************\n");
    }
    
    return ret;
}



/*****************************消息队列的包裹函数******************************/

/*********************************************************
    Function:       MsgGet
    Description:    包裹msgget函数,创建线程
    Param:          参考msgget
    Return:         创建和访问消息队列
    Other:          none
*********************************************************/
int MsgGet(key_t key,int flag)
{
    int msgId = -1;
    msgId = msgget(key, flag);  
    if(msgId == -1)  
    {  
        printf("**********************\n");
        printf("Msg create failed,pid is %d,errno is %d,%s.\n", getpid(),errno,strerror(errno));
        printf("**********************\n");
    }
    return msgId;
}

/*********************************************************
    Function:       MsgSnd
    Description:    包裹msgsnd函数,成功返回接收长度
                    失败返回-1
    Param:          none
    Return:         长度
    Other:          none
*********************************************************/
int MsgSnd(int msgid, const void *ptr, size_t nbytes, int flag)
{
    ssize_t n;
SENDAGAIN:
    n = msgsnd(msgid, ptr, nbytes, flag);
    if(n < 0)
    {
        if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        {
            goto SENDAGAIN;
        }
        else
        {
            printf("**********************\n");
            printf("MsgSnd failed,pid is %d,errno is %d.\n", getpid(),errno);
            printf("msgid is %d len is %d flag is %d ret is %d \n",msgid,(int)nbytes,flag,n);
            printf("**********************\n");
            return -1;
        }
    }
    return n;
}

/*********************************************************
    Function:       MsgRcv
    Description:    包裹msgrcv函数,成功返回读取长度
                    失败返回-1
    Param:          参考msgrcv函数
    Return:         none
    Other:          包裹函数会检查错误类型,如果是EINTR,EAGAIN
                    和EWOULDBLOCK则重新读取
*********************************************************/
int MsgRcv(int msgid, void *ptr, size_t nbytes,long int type, int flag)
{
    int n;
RECVAGAIN:
    //printf("msgid is %d,ptr is %x,nbytes is %d,type is %d,flag is %d.\n",msgid,ptr,nbytes,type,flag);
    n = msgrcv(msgid,ptr,nbytes,type,flag);
    if(n == -1)
    {
        if(errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK)
        {
            goto RECVAGAIN;
        }
        else
        {
            printf("**********************\n");
            printf("MsgRcv failed,pid is  %d,errno is %d.\n", getpid(),errno);
            printf("**********************\n");
            return -1;
        }
    }
    return n;
}

/*********************************************************
    Function:       System
    Description:    包裹system函数
    Param:          none              
    Return:         执行结果
    Other:          none
*********************************************************/
int System(const char *cmdLine)  
{  
    int ret = 0;  
    sighandler_t oldHandler;
   
    oldHandler = signal(SIGCHLD, SIG_DFL);  
    ret = system(cmdLine);  
    signal(SIGCHLD, oldHandler);  
   
    return ret;  
} 

/*********************************************************
    Function:       Panic
    Description:    断言失败的相关执行
    Param:          file
                    line              
    Return:         none
    Other:          none
*********************************************************/
void Panic(char * file, int line)
{
    
    printf("********************************************\n");
    printf("Assertion falied! File:%s, Line: %d.\n", file, line);
    printf("********************************************\n");
    sleep(3600);
}

