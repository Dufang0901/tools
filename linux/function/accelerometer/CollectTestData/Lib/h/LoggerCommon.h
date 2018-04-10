/*****************************************************************************
  FileName：    LoggerCommon.h
  Author:       林锴
  Version:      1.0
  Date:         2016/6/22 10:33:21
  Description:  BLE应用的共用接口
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


/*日志的优先级*/
typedef enum
{
    E_LOGGER_PRIORITY_FATAL         = 10,       /*致命*/
    E_LOGGER_PRIORITY_ERROR         = 20,       /*错误*/
    E_LOGGER_PRIORITY_WARN          = 30,       /*警告*/
    E_LOGGER_PRIORITY_INFO          = 40,       /*信息*/
    E_LOGGER_PRIORITY_DEBUG         = 50        /*调试*/
}E_LOGGER_LEVEL;

/*LOGGER的使用类型*/
typedef enum
{
    E_LOGGER_USE_ONLY_THREAD        = 1,        /*单线程环境使用*/
    E_LOGGER_USE_MULTI_THREAD       = 2,        /*多线程环境使用*/
    E_LOGGER_USE_MULTI_PROCESS      = 3         /*多进程环境使用*/
}E_LOGGER_USE_ENVIRONMENT;

/*LOGGER的打印类型*/
typedef enum
{
    E_LOGGER_PRINT_NONE             = 0,        /*不打印*/
    E_LOGGER_PRINT_ONLY_STDOUT      = 1,        /*标准输出*/
    E_LOGGER_PRINT_ONLY_STREAM      = 2,        /*数据流*/
    E_LOGGER_PRINT_STDOUT_STREAM    = 3         /*标准输出与数据流*/
}E_LOGGER_PRINT_TYPE;

/*LOGGER的打印格式*/
typedef enum
{
    E_LOGGER_LAYOUT_NONE            = 0,        /*只输出打印信息*/
    E_LOGGER_LAYOUT_DATE            = 1,        /*输出时间与打印信息*/
    E_LOGGER_LAYOUT_LEVEL           = 2,        /*输出等级与打印信息*/
    E_LOGGER_LAYOUT_ALL             = 3         /*输出时间，等级与打印信息*/
}E_LOGGER_PRINT_LAYOUT;

/*LOGGER的数据流类型*/
typedef enum
{
    E_LOGGER_STREAM_FILE            = 1         /*文件流*/
    /*以下暂不开放*/
    //E_LOGGER_STREAM_TCP_SOCKET      = 2,        /*基于TCP的SOCKET数据流*/
    //E_LOGGER_STREAM_UDP_SOCKET      = 3,        /*基于UDP的SOCKET数据流*/
    //E_LOGGER_STREAM_UART_SOCKET     = 4         /*基于串口SOCKET数据流*/
}E_LOGGER_STREAM_TYPE;

/*数据流的类型*/
typedef struct
{
    Char fileName[128];
}S_LoggerStreamFile;

typedef struct
{
    /*补上网络的相关信息*/
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
        /*以下暂不开放*/
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
    
    /*多线程的互斥锁*/
    pthread_rwlock_t            threadLock;          /*互斥锁,用于避免在不同线程中打印时文件流异常*/
    /*多进程的互斥锁*/
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
