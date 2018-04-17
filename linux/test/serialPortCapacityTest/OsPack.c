/*****************************************************************************
  FileName:     OsPack.c
  Author:       ����
  Version:      1.0
  Date:         2013.03.11
  Description:  ��װ�ײ㺯��
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2013.03.04     V1.0      create
                 linca    2013.07.03     V2.0     ��Ӹ��ֹ���
                 linca    2015.03.16     V3.0     ����̲߳��ֹ���,�����Ϣ���в��ֹ���  
*****************************************************************************/

#include "OsPack.h"
/*******************************�׽��ְ�������*******************************/

/*********************************************************
    Function:       Socket
    Description:    ����socket����,socket�ɹ�������Ӧ���׽�
                    ��������,ʧ���򷵻�-1;����������socket
                    ��ʧ�ܵ�ʱ����ʹ�����˳�
    Param:          none
    Return:         �ɹ����׽��ֵ�������
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
    Description:    ����bind����,bind�ɹ�����0
                    ����-1�Ҵ����ΪEADDRINUSE��˵��ָ���˿�
                    ����������ռ��,������������ʹ�����˳�
    Param:          none
    Return:         �ɹ�����0
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
    Description:    ����listen����,�ɹ�����0
                    ʧ�ܷ���-1,����ʹ�����˳�
    Param:          none
    Return:         �ɹ�����0
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
                    �ɹ�������Ӧ���׽���������
                    ʧ�ܷ���-1,����ʹ�����˳�
    Param:          none
    Return:         �ɹ�����0
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
    Description:    ����connect����,�ɹ�����0
                    ʧ�ܷ���-1,����ʹ�����˳�
    Param:          none
    Return:         �ɹ�����0
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
    Description:    ����close����,�ɹ�����0
                    ʧ�ܷ���-1
    Param:          none
    Return:         �ɹ�����0
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
    Description:    ����sendto����,�ɹ����ط��ͳ���
                    ʧ�ܷ���-1,����ʹ�����˳�
    Param:          none
    Return:         �ɹ�����0
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
    Description:    ����recvfrom����,�ɹ����ؽ��ճ���
                    ʧ�ܷ���-1,����ʹ�����˳�
    Param:          none
    Return:         �ɹ�����0
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

/****************************IO��д�����İ�������*****************************/

/*********************************************************
    Function:       Read
    Description:    ����read����,�ɹ����ض�ȡ����
                    ʧ�ܷ���-1,����ʹ�����˳�
    Param:          none
    Return:         �ɹ�����0
    Other:          �������������������,�����EINTR,EAGAIN
                    ��EWOULDBLOCK�����¶�ȡ
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
    /*���ڴ��ڷ�����ģʽ��PIPEΪ��ʱ���᷵��-1,errnoΪEAGAIN,�����޸�Read����*/
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
    Description:    ����write����,�ɹ�����д�볤��
                    ʧ�ܷ���-1,����ʹ�����˳�
    Param:          none
    Return:         �ɹ�����0
    Other:          �������������������,�����EINTR,EAGAIN
                    ��EWOULDBLOCK������д��
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
    Description:    ����SOCKED����
    Param:          none          
    Return:         �������
    Other:          ��ʱ����-1
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
        return -1;//��ʱ��
    }

    if( FD_ISSET(sockfd,&fds))
    {
        pkLen = recv(sockfd,buff,nbytes,MSG_PEEK);
        return pkLen;
    }
    return 0;
}

/************�����ֽ��������ֵ��ASCII�ַ���֮���ת�������İ�������************/

/*********************************************************
    Function:       Inet_pton
    Description:    ����inet_pton����
                    ������IPV4 �� IPV6,���ַ���ת���������ֽ���Ķ�����ֵ
    Param:          none
    Return:         �ɹ�����1
                    ��Ч��������������ȷ����0,����ʹ�����˳�
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


/*****************************���̿��Ƶİ�������******************************/

/*********************************************************
    Function:       Fork
    Description:    ����fork����
    Param:          none
    Return:         �ɹ��ӽ��̷���0,�����̷����ӽ��̵�������
                    ʧ�ܷ���-1,����ʹ�����˳�
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

/*****************************�����źŵİ�������******************************/

/*********************************************************
    Function:       Signal
    Description:    ����sigaction����,��������signal,
                    �������ظ�װ��,ͬʱ�ڴ�������п�
                    ��������Ӧ���źš�
    Param:          none
    Return:         �����źŴ�������ָ��
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
    Description:    ���̵Ķ�ʱ������
    Param:          none              
    Return:         time    ��ʱʱ��
                    func    ��ʱ�����������
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

/*****************************�߳̿��Ƶİ�������******************************/
/*********************************************************
    Function:       PthreadCreate
    Description:    ����pthread_create����,�����߳�
    Param:          �ο�pthread_create
    Return:         �������
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
    Description:    ����pthread_exit����,�ȴ��߳̽���
    Param:          �ο�pthread_exit
    Return:         none
    Other:          none
*********************************************************/
void PthreadExit(void *retval)
{
    pthread_exit(retval);
}

/*********************************************************
    Function:       PthreadJoin
    Description:    ����pthread_join����,�ȴ��߳̽���
    Param:          �ο�pthread_join
    Return:         �������
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
    Description:    ����pthread_cancel����,ȡ������
    Param:          �ο�pthread_cancel
    Return:         ���
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

/*****************************�ź����İ�������******************************/

/*********************************************************
    Function:       SemInit
    Description:    ����sem_init����,��ʼ���ź���
    Param:          �ο�sem_init
    Return:         ���
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
    Description:    ����sem_wait����,�ź���V����
    Param:          �ο�sem_wait
    Return:         ���
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
    Description:    ����sem_post����,�ź���P����
    Param:          �ο�sem_post
    Return:         ���
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
    Description:    ����sem_destroy����,�ź���ע��
    Param:          �ο�sem_destroy
    Return:         ���
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



/*****************************��Ϣ���еİ�������******************************/

/*********************************************************
    Function:       MsgGet
    Description:    ����msgget����,�����߳�
    Param:          �ο�msgget
    Return:         �����ͷ�����Ϣ����
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
    Description:    ����msgsnd����,�ɹ����ؽ��ճ���
                    ʧ�ܷ���-1
    Param:          none
    Return:         ����
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
    Description:    ����msgrcv����,�ɹ����ض�ȡ����
                    ʧ�ܷ���-1
    Param:          �ο�msgrcv����
    Return:         none
    Other:          �������������������,�����EINTR,EAGAIN
                    ��EWOULDBLOCK�����¶�ȡ
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
    Description:    ����system����
    Param:          none              
    Return:         ִ�н��
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
    Description:    ����ʧ�ܵ����ִ��
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

