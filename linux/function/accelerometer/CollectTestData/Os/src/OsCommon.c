/*****************************************************************************
  FileName:     OsCommon.cpp
  Author:       林锴
  Version:      1.0
  Date:         2013.03.11
  Description:  封装共用函数
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2014.03.20     V1.0      create
*****************************************************************************/

#include "OsCommon.h"

static const char *cutFile;

/*********************************************************
    Function:       customFilter
    Description:    文件滤波器
    Param:          none
    Return:         none
    Other:          none
*********************************************************/
static int customFilter(const struct dirent *pDir)
{
    if (strncmp(cutFile, pDir->d_name, strlen(cutFile)) == 0)
    {
        return 1;
    }
    return  0;
}

/*********************************************************
    Function:       FileGetFrDir()
    Description:    获取文件夹内,固定前缀文件名的文件
    Param:          nameList    存放文件名的指针
                    dir            查找目录
                    file        文件名称
    Return:         成功返回文件个数,失败返回 -1
    Other:          使用后要用free释放nameList
                    当文件名为NULL时返回所有文件
*********************************************************/
int FileGetFrDir(struct dirent ***nameList,const char *dir,const char *file)
{
    int n;
    if(file == NULL)
    {
        /*不需要滤除文件*/
        n = scandir(dir, nameList, 0, alphasort);
    }
    else
    {
        cutFile = file;
        n = scandir(dir, nameList, customFilter, alphasort);
    }
    return n;
}

/*********************************************************
    Function:       CreatSem
    Description:    创建信号量
    Param:          key 是一个用来允许不相关的进程访问相同信号量的整数值
    Return:         返回信号量的ID,失败返回-1
    Other:          
*********************************************************/
int SemCreat(key_t key)
{
   union semun arg;
   int  semId = -1;

   if((semId = semget(key,1,0666 | IPC_CREAT | IPC_EXCL)) < 0 )
   {                                                    
        if(errno == EEXIST)                                    
        {                                                    
            semId = semget(key,1,0666 | IPC_CREAT);      
            if(semId < 0)
             {                                      
                printf("Create sem failed,semget error.\n");
                return -1;
             }                     
        }
        else
        {
            printf("Create sem failed,errno is %d.\n.",errno);
            return -1;
        }                                                    
    }
    arg.val = 0;
    if (semctl(semId,0,SETVAL,arg) < 0)
    {
        printf("Create sem failed,Semctl error!\n");
        return -1;
    }
    return semId;  
}

/*********************************************************
    Function:       SemPend
    Description:    信号量PEND
    Param:          信号ID
    Return:         none
    Other:          
*********************************************************/
void SemPend(int semId)
{
    struct sembuf semPend = {0,-1,SEM_UNDO};
    semop(semId,&semPend,1);
}

/*********************************************************
    Function:       SemPose
    Description:    信号量POSE
    Param:          信号ID
    Return:         none
    Other:          
*********************************************************/
void SemPose(int semId)
{
    struct sembuf semPose = {0,1,SEM_UNDO};
    semop(semId,&semPose,1);
}

/*********************************************************
    Function:       SemMutex
    Description:    信号量上锁判断
    Param:          锁的状态
    Return:         未上锁返回1,已上锁返回0
    Other:          
*********************************************************/
int SemMutex(int semId)
{
    struct sembuf semMutex = {0,-1,IPC_NOWAIT};
    if(semop(semId,&semMutex,1) == 0)
    {
        return 1;
    }
    else
    {
        return 0; 
    }
}

/*********************************************************
    Function:       SemRemove
    Description:    删除信号量
    Param:          semId 信号量
    Return:         删除结果,成功返回1,失败返回0
    Other:          
*********************************************************/
int SemRemove(int semId)
{
    union semun ignored_argument; 
    ignored_argument.val = 0;
    if(semctl (semId, 1, IPC_RMID,ignored_argument) > 0)
    {
        return 1;
    }
    else
    {
        printf("Sem remove failed,errno is %d.\n",errno);
        return 0;
    }
}


/*********************************************************
    Function:       ShmCreat
    Description:    创建共享内存
    Param:          key 是一个用来允许不相关的进程访问相同信号量的整数值
                    len 数据长度
    Return:         返回共享内存ID的指针,失败返回-1
    Other:          
*********************************************************/
int ShmCreat(key_t key,Uint32 len)
{
    int shmId = -1;
    struct shmid_ds shmBuf; 
    if((shmId = shmget(key,len,0666 | IPC_CREAT )) < 0 )
    {                                                          
        printf("Create shm failed,shmget error.\n");   
        return -1;
    }
    else
    {
        if(shmctl(shmId,IPC_STAT,&shmBuf) == 0)
        {
            if(len > shmBuf.shm_segsz)
            {
                printf("Creat shm failed,shm len is wrong.\n");
                return -1;
            }
        }
    }
    return shmId;
}

/*********************************************************
    Function:       ShmRemove
    Description:    删除共享内存
    Param:          shmid 共享内存ID
    Return:         删除结果
    Other:          
*********************************************************/
int ShmRemove(int shmId)
{
    int ret;
    ret = shmctl( shmId,IPC_RMID,0 );
    if( ret < 0 )
    {
        printf( "Shared memory remove failed /n" );
    }
    return ret;
}

/*********************************************************
    Function:       ShmBonding
    Description:    绑定共享内存
    Param:          shmid 共享内存ID
    Return:         var 共享内存的指针
    Other:          
*********************************************************/
char *ShmBonding(int shmId)
{
    char *var;
    var = (char *)shmat(shmId,0,0);
    return var;
}


/*********************************************************
    Function:       ShmDetach
    Description:    脱离共享内存
    Param:          ptr 共享内存的指针
    Return:         脱离结果
    Other:          
*********************************************************/
int ShmDetach(char *ptr)
{
    int ret;
    if((ret = shmdt(ptr)) < 0)
    {
        printf( "Shm detached failed,errno is %d\n",errno);
    }
    return ret;
}

/*********************************************************
    Function:       MsgCreat
    Description:    创建消息队列
    Param:          key 是一个用来允许不相关的进程访问消息队列的整数值
    Return:         返回消息队列ID,失败返回-1
    Other:          
*********************************************************/
int MsgCreat(key_t key)
{
    int msgId;
    
    if((msgId = MsgGet(key,0666 | IPC_CREAT )) < 0 )
    {                                                          
        printf("Create msg failed.\n");   
        return -1;
    }

    return msgId;
}

/*********************************************************
    Function:       MsgRemove
    Description:    删除消息队列
    Param:          msgid
    Return:         删除结果
    Other:          
*********************************************************/
int MsgRemove(int msgId)
{
    int ret;
    ret = msgctl( msgId,IPC_RMID,0 );
    if( ret < 0 )
    {
        printf( "Msg remove failed /n" );
    }
    return ret;
}

/*********************************************************
    Function:       FdGetBlock
    Description:    获取描述符是否为阻塞
    Param:          none
    Return:         阻塞返回1,或则范围0
    Other:          
*********************************************************/
int FdGetBlock(int fd)
{
    int val;
    val = fcntl(fd,F_GETFL,0);
    if((val & O_NONBLOCK) != O_NONBLOCK)
    {
        return 1;
    }
    return 0;
}

/*********************************************************
    Function:       FdSetNoBlock
    Description:    描述符设置非阻塞
    Param:          none
    Return:         none
    Other:          
*********************************************************/
void FdSetNoBlock(int fd)
{
    int val;
    val = fcntl(fd,F_GETFL,0);
    fcntl(fd, F_SETFL, val | O_NONBLOCK);
}

/*********************************************************
    Function:       FdSetBlock
    Description:    描述符设置阻塞
    Param:          none
    Return:         none
    Other:          
*********************************************************/
void FdSetBlock(int fd)
{
    int val;
    val = fcntl(fd,F_GETFL,0);
    fcntl(fd, F_SETFL, val & ~O_NONBLOCK);
}

/*********************************************************
    Function:       FileExistStatGet
    Description:    查看文件是否存在
    Param:          fileName          
    Return:         存在返回 1
                    不存在返回 0
    Other:          
*********************************************************/ 
int FileExistStatGet(const char *fileName)
{   
   return (access(fileName, F_OK) == 0);   
}

/*********************************************************
    Function:       DirEmpStatGet
    Description:    查看文件是否为空
    Param:          dir          
    Return:         空文件夹返回 1
                    非空 返回0
    Other:          
*********************************************************/ 
int DirEmpStatGet(const char *dir)
{
    DIR *dirp; 
    int num = 0;
    int ret = 1;

    dirp = opendir(dir); 
    while (dirp) 
    { 
        if ( readdir(dirp) != NULL) 
        {
            ++num;
            if(num > 2)
            {
                ret = 0;
                break;
            }
        }
        else
        {
            break; 
        }
    } 

    closedir(dirp); 
    return ret;
}

/*********************************************************
    Function:       Mkdir
    Description:    创建目录
    Param:          dir          
    Return:         创建成功返回 1,失败返回err
    Other:          none
*********************************************************/ 
int Mkdir(const char *dir)
{
    if(mkdir(dir,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0) 
    {
        if(errno != EEXIST)
        {
            /*如果创建不成功或则目录不存在*/
            return errno;
        }
    }
    return 1;
}

/*********************************************************
    Function:       Rmdir
    Description:    删除目录
    Param:          dir          
    Return:         none
    Other:          调用shell删除目录
*********************************************************/ 
void Rmdir(const char *dir)
{
    char cmd[256];
    
    sprintf(cmd,"rm -rf %s",dir);
    
    System(cmd);
}

/*********************************************************
    Function:       Itoa
    Description:    整形转字符串
    Param:          none
    Return:         none
    Other:          
*********************************************************/
int Itoa(int val, char* buf)
{
    const unsigned int radix = 10;

    char* p;
    unsigned int a;        //every digit
    int len;
    char* b;            //start of the digit char
    char temp;
    unsigned int u;

    p = buf;

    if (val < 0)
    {
        *p++ = '-';
        val = 0 - val;
    }
    u = (unsigned int)val;

    b = p;

    do
    {
        a = u % radix;
        u /= radix;

        *p++ = a + '0';

    } while (u > 0);

    len = (int)(p - buf);

    *p-- = 0;

    //swap
    do
    {
        temp = *p;
        *p = *b;
        *b = temp;
        --p;
        ++b;

    } while (b < p);

    return len;
}

/*********************************************************
    Function:       DigitaltConf
    Description:    确认字符串N的长度是否都为数字格式
    Param:          buf     字符串头指针
                    charLen 长度
    Return:         全为数字返回1
    Other:          
*********************************************************/
int DigitaltConf(const char *buf,int charLen)
{
    int i;
    for( i = 0; i < charLen; i++)
    {
        if(buf[i] < '0' || buf[i] > '1')
        {
            return 0;
        }
    }
    return 1;
}
