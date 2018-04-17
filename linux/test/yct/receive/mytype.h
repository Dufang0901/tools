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

#define BIC_STAFF_ID_LEN				16			/*Ա��ID����*/
#define BIC_STAFF_NAME_LEN				16			/*Ա�����ֳ���*/

#define BIC_ROUTE_CODING_LEN			8			/*��·���볤��*/
#define BIC_ROUTE_NAME_LEN				16			/*��·���Ƴ���*/
#define BIC_ROUTE_FILE_LEN				16			/*��·�ļ�������*/

#define BIC_SITE_CODING_LEN	     	    16			/*վ��/��Ȥ����볤��*/
#define BIC_SITE_NAME_LEN				48			/*վ��/��Ȥ�����Ƴ���*/


#define BIC_ROOT_DIR                "/home/gzbic/"
#define BIC_ROUTE_DIR      		CONCAT(BIC_ROOT_DIR,"release/route/")        /*���ִ����·����·�ļ�*/
#define BIC_NMEA_DIR       		CONCAT(BIC_ROOT_DIR,"release/nmea/")         /*���ִ����·��nmea�ļ�*/
#define BIC_BT_DIR         		CONCAT(BIC_ROOT_DIR,"release/bt/")           /*���ִ����·��bt�ļ�*/
#define BIC_MP3_DIR				CONCAT(BIC_ROOT_DIR,"release/mp3/")			 /*���ִ����·��mp3�ļ�*/
#define BIC_ETC_DIR				CONCAT(BIC_ROOT_DIR,"release/etc/")			 /*��������ļ�*/
#define BIC_LBOX_DIR			CONCAT(BIC_ROOT_DIR,"release/lbox/")		 /*���GPS�켣�طŵ��ļ�*/
#define BIC_LOG_DIR             CONCAT(BIC_ROOT_DIR,"release/log/")			 /*���ִ��ʱ���е���־�ļ�*/
#define BIC_COLLECT_DIR			CONCAT(BIC_ROOT_DIR,"release/collect/")		 /*��Ųɼ�ʱ�ļ�*/


/*���浱ǰ���е�״̬*/
#define CENT_ROUTE_INFO_FILE    CONCAT(BIC_ETC_DIR,"route-info")


/*�����ļ�*/
#define BIC_PARAM_FILE              CONCAT(BIC_ETC_DIR,"bic.conf")
#define BIC_PARAM_FILE_BAK          CONCAT(BIC_ETC_DIR,"bic.conf.bak")
#define BIC_PARAM_FILE_BAK1         CONCAT(BIC_ETC_DIR,"bic.conf.bak1")
#define BIC_VOICE_HINT_FILE			CONCAT(BIC_ETC_DIR,"voicehint.lst")


#define UNIX_EXPAND_DIR			CONCAT(BIC_ROOT_DIR,"Expand/") 
#define UNIX_LOCAL_UART_L       CONCAT(UNIX_EXPAND_DIR,"uart_local")        //����socketͨ�ŵı��ش���sun_path
#define UNIX_EXPAND_UART_E0     CONCAT(UNIX_EXPAND_DIR,"uart_expand0")      //����socketͨ�ŵ���չ����0sun_path
#define UNIX_EXPAND_UART_E1     CONCAT(UNIX_EXPAND_DIR,"uart_expand1")      //����socketͨ�ŵ���չ����1sun_path
#define UNIX_EXPAND_UART_E2     CONCAT(UNIX_EXPAND_DIR,"uart_expand2")      //����socketͨ�ŵ���չ����2sun_path
#define UNIX_EXPAND_UART_E3     CONCAT(UNIX_EXPAND_DIR,"uart_expand3")      //����socketͨ�ŵ���չ����3sun_path


#define HTTP_SAVE_FILE_NAME      		CONCAT(BIC_ETC_DIR,"httpbusinfo")

/*������Ϣ��ŵ��ļ�·�����ļ���*/
#define CENT_DISPATCH_TABLE_DIR    		CONCAT(BIC_ETC_DIR,"dispatch_info.ini")
#define CENT_DISPATCH_TABLE_DIR_BAK     CONCAT(BIC_ETC_DIR,"dispatch_info.ini.bak")

/*Ա����Ϣ��ŵ��ļ�·�����ļ���*/
#define CENT_STAFF_TABLE_DIR    		CONCAT(BIC_ETC_DIR,"stafftable.ini")
#define CENT_STAFF_TABLE_DIR_BAK    	CONCAT(BIC_ETC_DIR,"stafftable.ini.bak")


/*�켣�ط�ʹ�õĹܵ��ļ���*/
#define BIC_GPS_FIFO		CONCAT(BIC_LBOX_DIR,"gps_fifo")

#endif
