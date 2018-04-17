/*****************************************************************************
  FileName:     OsCommon.cpp
  Author:       ����
  Version:      1.0
  Date:         2013.03.11
  Description:  ��װ���ú���
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2014.03.20     V1.0      create
*****************************************************************************/

#include "OsCommon.h"

static const char *cutFile;

/*********************************************************
    Function:       customFilter
    Description:    �ļ��˲���
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
    Description:    ��ȡ�ļ�����,�̶�ǰ׺�ļ������ļ�
    Param:          nameList    ����ļ�����ָ��
                    dir            ����Ŀ¼
                    file        �ļ�����
    Return:         �ɹ������ļ�����,ʧ�ܷ��� -1
    Other:          ʹ�ú�Ҫ��free�ͷ�nameList
                    ���ļ���ΪNULLʱ���������ļ�
*********************************************************/
int FileGetFrDir(struct dirent ***nameList,const char *dir,const char *file)
{
    int n;
    if(file == NULL)
    {
        /*����Ҫ�˳��ļ�*/
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
    Description:    �����ź���
    Param:          key ��һ������������صĽ��̷�����ͬ�ź���������ֵ
    Return:         �����ź�����ID,ʧ�ܷ���-1
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
    Description:    �ź���PEND
    Param:          �ź�ID
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
    Description:    �ź���POSE
    Param:          �ź�ID
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
    Description:    �ź��������ж�
    Param:          ����״̬
    Return:         δ��������1,����������0
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
    Description:    ɾ���ź���
    Param:          semId �ź���
    Return:         ɾ�����,�ɹ�����1,ʧ�ܷ���0
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
    Description:    ���������ڴ�
    Param:          key ��һ������������صĽ��̷�����ͬ�ź���������ֵ
                    len ���ݳ���
    Return:         ���ع����ڴ�ID��ָ��,ʧ�ܷ���-1
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
    Description:    ɾ�������ڴ�
    Param:          shmid �����ڴ�ID
    Return:         ɾ�����
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
    Description:    �󶨹����ڴ�
    Param:          shmid �����ڴ�ID
    Return:         var �����ڴ��ָ��
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
    Description:    ���빲���ڴ�
    Param:          ptr �����ڴ��ָ��
    Return:         ������
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
    Description:    ������Ϣ����
    Param:          key ��һ������������صĽ��̷�����Ϣ���е�����ֵ
    Return:         ������Ϣ����ID,ʧ�ܷ���-1
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
    Description:    ɾ����Ϣ����
    Param:          msgid
    Return:         ɾ�����
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
    Description:    ��ȡ�������Ƿ�Ϊ����
    Param:          none
    Return:         ��������1,����Χ0
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
    Description:    ���������÷�����
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
    Description:    ��������������
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
    Description:    �鿴�ļ��Ƿ����
    Param:          fileName          
    Return:         ���ڷ��� 1
                    �����ڷ��� 0
    Other:          
*********************************************************/ 
int FileExistStatGet(const char *fileName)
{   
   return (access(fileName, F_OK) == 0);   
}

/*********************************************************
    Function:       DirEmpStatGet
    Description:    �鿴�ļ��Ƿ�Ϊ��
    Param:          dir          
    Return:         ���ļ��з��� 1
                    �ǿ� ����0
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
    Description:    ����Ŀ¼
    Param:          dir          
    Return:         �����ɹ����� 1,ʧ�ܷ���err
    Other:          none
*********************************************************/ 
int Mkdir(const char *dir)
{
    if(mkdir(dir,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0) 
    {
        if(errno != EEXIST)
        {
            /*����������ɹ�����Ŀ¼������*/
            return errno;
        }
    }
    return 1;
}

/*********************************************************
    Function:       Rmdir
    Description:    ɾ��Ŀ¼
    Param:          dir          
    Return:         none
    Other:          ����shellɾ��Ŀ¼
*********************************************************/ 
void Rmdir(const char *dir)
{
    char cmd[256];
    
    sprintf(cmd,"rm -rf %s",dir);
    
    System(cmd);
}

/*********************************************************
    Function:       Itoa
    Description:    ����ת�ַ���
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
    Description:    ȷ���ַ���N�ĳ����Ƿ�Ϊ���ָ�ʽ
    Param:          buf     �ַ���ͷָ��
                    charLen ����
    Return:         ȫΪ���ַ���1
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
