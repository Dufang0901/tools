#ifndef _MYTYPE_H_
#define _MYTYPE_H_

#include "stdint.h"

#if 0

typedef char Int8;
typedef unsigned char Uint8;
typedef short Int16;
typedef unsigned short Uint16;
typedef int Int32;
typedef unsigned int Uint32;
typedef long long Int64;
typedef unsigned long long Uint64;

#else

typedef char        Char;
typedef int8_t      Int8;
typedef uint8_t     Uint8;
typedef int16_t     Int16;
typedef uint16_t    Uint16;
typedef int32_t     Int32;
typedef uint32_t    Uint32;
typedef int64_t     Int64;
typedef uint64_t    Uint64;

#endif

#define BIC_STAFF_ID_LEN				16			/*员工ID长度*/
#define BIC_STAFF_NAME_LEN				16			/*员工名字长度*/

#define BIC_ROUTE_CODING_LEN			8			/*线路编码长度*/
#define BIC_ROUTE_NAME_LEN				16			/*线路名称长度*/
#define BIC_ROUTE_FILE_LEN				16			/*线路文件名长度*/

#define BIC_SITE_CODING_LEN	     	    16			/*站点/兴趣点编码长度*/
#define BIC_SITE_NAME_LEN				48			/*站点/兴趣点名称长度*/


#define BIC_ROOT_DIR                "/home/gzbic/"
#define BIC_ROUTE_DIR      		CONCAT(BIC_ROOT_DIR,"release/route/")        /*存放执行线路的线路文件*/
#define BIC_NMEA_DIR       		CONCAT(BIC_ROOT_DIR,"release/nmea/")         /*存放执行线路的nmea文件*/
#define BIC_BT_DIR         		CONCAT(BIC_ROOT_DIR,"release/bt/")           /*存放执行线路的bt文件*/
#define BIC_MP3_DIR				CONCAT(BIC_ROOT_DIR,"release/mp3/")			 /*存放执行线路的mp3文件*/
#define BIC_ETC_DIR				CONCAT(BIC_ROOT_DIR,"release/etc/")			 /*存放配置文件*/
#define BIC_LBOX_DIR			CONCAT(BIC_ROOT_DIR,"release/lbox/")		 /*存放GPS轨迹回放的文件*/
#define BIC_LOG_DIR             CONCAT(BIC_ROOT_DIR,"release/log/")			 /*存放执行时运行的日志文件*/
#define BIC_COLLECT_DIR			CONCAT(BIC_ROOT_DIR,"release/collect/")		 /*存放采集时文件*/


/*保存当前运行的状态*/
#define CENT_ROUTE_INFO_FILE    CONCAT(BIC_ETC_DIR,"route-info")


/*配置文件*/
#define BIC_PARAM_FILE              CONCAT(BIC_ETC_DIR,"bic.conf")
#define BIC_PARAM_FILE_BAK          CONCAT(BIC_ETC_DIR,"bic.conf.bak")
#define BIC_PARAM_FILE_BAK1         CONCAT(BIC_ETC_DIR,"bic.conf.bak1")
#define BIC_VOICE_HINT_FILE			CONCAT(BIC_ETC_DIR,"voicehint.lst")


#define UNIX_EXPAND_DIR			CONCAT(BIC_ROOT_DIR,"Expand/") 
#define UNIX_LOCAL_UART_L       CONCAT(UNIX_EXPAND_DIR,"uart_local")        //用于socket通信的本地串口sun_path
#define UNIX_EXPAND_UART_E0     CONCAT(UNIX_EXPAND_DIR,"uart_expand0")      //用于socket通信的扩展串口0sun_path
#define UNIX_EXPAND_UART_E1     CONCAT(UNIX_EXPAND_DIR,"uart_expand1")      //用于socket通信的扩展串口1sun_path
#define UNIX_EXPAND_UART_E2     CONCAT(UNIX_EXPAND_DIR,"uart_expand2")      //用于socket通信的扩展串口2sun_path
#define UNIX_EXPAND_UART_E3     CONCAT(UNIX_EXPAND_DIR,"uart_expand3")      //用于socket通信的扩展串口3sun_path


#define HTTP_SAVE_FILE_NAME      		CONCAT(BIC_ETC_DIR,"httpbusinfo")

/*调度信息存放的文件路径和文件名*/
#define CENT_DISPATCH_TABLE_DIR    		CONCAT(BIC_ETC_DIR,"dispatch_info.ini")
#define CENT_DISPATCH_TABLE_DIR_BAK     CONCAT(BIC_ETC_DIR,"dispatch_info.ini.bak")

/*员工信息存放的文件路径和文件名*/
#define CENT_STAFF_TABLE_DIR    		CONCAT(BIC_ETC_DIR,"stafftable.ini")
#define CENT_STAFF_TABLE_DIR_BAK    	CONCAT(BIC_ETC_DIR,"stafftable.ini.bak")


/*轨迹回放使用的管道文件名*/
#define BIC_GPS_FIFO		CONCAT(BIC_LBOX_DIR,"gps_fifo")

#endif
