/*****************************************************************************
  FileName:     OsCommon.h
  Author:       ����
  Version:      1.0
  Date:         2013.03.11
  Description:  ��װ���ú���
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2014.03.20     V1.0      create
*****************************************************************************/

#ifndef _OS_COMMON_H_
#define _OS_COMMON_H_

#include "OsPack.h"

#define CONCAT(str1,str2)       str1 str2

int FileGetFrDir(struct dirent ***nameList,const char *dir,const char *file);

/*****************************�ź����İ�������******************************/
union semun{
    int              val;
    struct semid_ds *buf;  
    unsigned short  *array;
};  

int SemCreat(key_t key);
void SemPend(int semId);
void SemPose(int semId);
int SemMutex(int semId);
int SemRemove(int semId);

/*****************************�����ڴ�İ�������******************************/
int ShmCreat(key_t key,Uint32 len);
int ShmRemove(int shmId);
char *ShmBonding(int shmId);
int ShmDetach(char *ptr);

/*��Ϣ����*/
int MsgCreat(key_t key);
int MsgRemove(int msgId);


/*****************************����������״̬����******************************/
int FdGetBlock(int fd);
void FdSetNoBlock(int fd);
void FdSetBlock(int fd);

/*****************************��������******************************/
int FileExistStatGet(const char *fileName);
int DirEmpStatGet(const char *dir);
int Mkdir(const char *dir);
void Rmdir(const char *dir);

int Itoa(int val, char* buf);
int DigitaltConf(const char *buf,int charLen);

#endif