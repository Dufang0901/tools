/*****************************************************************************
  FileName��    LoggerCommon.h
  Author:       ����
  Version:      1.0
  Date:         2016/6/22 10:33:21
  Description:  BLEӦ�õĹ��ýӿ�
  History:  
                <author>    <time>    <version>    <desc>
                 linca    2016.06.22    V1.0      create
*****************************************************************************/

#ifndef _LOGGER_COMMON_H_
#define _LOGGER_COMMON_H_

#include "OsPack.h"
#include "OsCommon.h"

#define LOG_CM_OK               1
#define LOG_CM_ERROR_UNKNOW     0
#define LOG_CM_ERROR_TLOCK     -1
#define LOG_CM_ERROR_PLOCK     -2
#define LOG_CM_ERROR_NULL      -3
#define LOG_CM_ERROR_MEM       -4


/*��־�����ȼ�*/
typedef enum
{
    E_LOGGER_PRIORITY_FATAL         = 10,       /*����*/
    E_LOGGER_PRIORITY_ERROR         = 20,       /*����*/
    E_LOGGER_PRIORITY_WARN          = 30,       /*����*/
    E_LOGGER_PRIORITY_INFO          = 40,       /*��Ϣ*/
    E_LOGGER_PRIORITY_DEBUG         = 50        /*����*/
}E_LOGGER_LEVEL;

/*LOGGER��ʹ������*/
typedef enum
{
    E_LOGGER_USE_ONLY_THREAD        = 1,        /*���̻߳���ʹ��*/
    E_LOGGER_USE_MULTI_THREAD       = 2,        /*���̻߳���ʹ��*/
    E_LOGGER_USE_MULTI_PROCESS      = 3         /*����̻���ʹ��*/
}E_LOGGER_USE_ENVIRONMENT;

/*LOGGER�Ĵ�ӡ����*/
typedef enum
{
    E_LOGGER_PRINT_NONE             = 0,        /*����ӡ*/
    E_LOGGER_PRINT_ONLY_STDOUT      = 1,        /*��׼���*/
    E_LOGGER_PRINT_ONLY_STREAM      = 2,        /*������*/
    E_LOGGER_PRINT_STDOUT_STREAM    = 3         /*��׼�����������*/
}E_LOGGER_PRINT_TYPE;

/*LOGGER�Ĵ�ӡ��ʽ*/
typedef enum
{
    E_LOGGER_LAYOUT_NONE            = 0,        /*ֻ�����ӡ��Ϣ*/
    E_LOGGER_LAYOUT_DATE            = 1,        /*���ʱ�����ӡ��Ϣ*/
    E_LOGGER_LAYOUT_LEVEL           = 2,        /*����ȼ����ӡ��Ϣ*/
    E_LOGGER_LAYOUT_ALL             = 3         /*���ʱ�䣬�ȼ����ӡ��Ϣ*/
}E_LOGGER_PRINT_LAYOUT;

/*LOGGER������������*/
typedef enum
{
    E_LOGGER_STREAM_FILE            = 1         /*�ļ���*/
    /*�����ݲ�����*/
    //E_LOGGER_STREAM_TCP_SOCKET      = 2,        /*����TCP��SOCKET������*/
    //E_LOGGER_STREAM_UDP_SOCKET      = 3,        /*����UDP��SOCKET������*/
    //E_LOGGER_STREAM_UART_SOCKET     = 4         /*���ڴ���SOCKET������*/
}E_LOGGER_STREAM_TYPE;

/*������������*/
typedef struct
{
    Char fileName[128];
}S_LoggerStreamFile;

typedef struct
{
    /*��������������Ϣ*/
    int netFd;
}S_LoggerStreamNet;

typedef struct
{
    Char uartDev[64];
    int  uartFd;
}S_LoggerStreamUart;

typedef struct
{
    union
    {
        S_LoggerStreamFile sLoggerStramFile;
        /*�����ݲ�����*/
        //S_LoggerStreamNet  sLoggerStramNet;
        //S_LoggerStreamUart sLoggerStramUart;
    }params;
}S_LoggerStream;


typedef struct
{
    E_LOGGER_STREAM_TYPE    eStreamType;
    S_LoggerStream          sLoggerStram;
}S_LoggerAppender;

typedef struct
{
    E_LOGGER_USE_ENVIRONMENT    eUseEnvironment;
    E_LOGGER_PRINT_TYPE         ePrintType;
    E_LOGGER_PRINT_LAYOUT       ePrintLayout;
    Char                        prefix[64];
    
    /*���̵߳Ļ�����*/
    pthread_rwlock_t            threadLock;          /*������,���ڱ����ڲ�ͬ�߳��д�ӡʱ�ļ����쳣*/
    /*����̵Ļ�����*/
    int                         processLock;
    
    S_LoggerAppender            sLoggerAppender;
}S_Logger;


void  LoggerPrintVersion(void);
Int32 LoggerCreate(S_Logger *psInLogger[],E_LOGGER_USE_ENVIRONMENT eInUseEnvironment,key_t inProcessLockKey);
Int32 LoggerRelease(S_Logger* psInLogger);
Int32 LoggerSetAppenderType(S_Logger *psInLogger,E_LOGGER_PRINT_TYPE eInPrintType);
Int32 LoggerSetAppenderStream(S_Logger *psInLogger,E_LOGGER_STREAM_TYPE eInStreamType,const Char *pInStreamTemp);
Int32 LoggerSetLayout(S_Logger *psInLogger,E_LOGGER_PRINT_LAYOUT eInPrintLayout);
Int32 LoggerSetPrefix(S_Logger *psInLogger,const Char *pInPrefix);
void  LoggerPrint(S_Logger *psInLogger,E_LOGGER_LEVEL eInLevel,const char *format, ...);

#endif
