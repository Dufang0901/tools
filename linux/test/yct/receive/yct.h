#ifndef _YCT_H_
#define _YCT_H_

#include "OsPack.h"
#include <time.h>

#define YCT_COM_DEV             "/dev/ttyUSB3"  /*与羊城通终端通信的串口设备*/
#define YCT_COM_BAUD            B9600          /*与羊城通终端通信的串口波特率*/
#define YCT_BUF_LEN_MAX         1024

#define YCT_HEAD_LEN			sizeof(S_YctMsgHead)

/*羊城通终端相关的消息帧类型*/
/*BIC->羊城通*/
#define YCT_TYPE_STAT_QUERY			0xC8	/*羊城通状态信息查询*/
#define YCT_TYPE_TIME_ADJUST		0xC9	/*羊城通时间校准*/
#define YCT_TYPE_COM_REPLY			0x7F	/*羊城通通信应答*/
/*羊城通->BIC*/
#define YCT_TYPE_STAT_REPLY			0xC0	/*羊城通状态信息查询应答*/
/*羊城通->司机键盘*/
#define YCT_TYPE_ROUTE_INFO			0x90	/*羊城通线路信息上报*/
#define YCT_TYPE_CHARGE_INFO		0x91	/*羊城通扣费内容*/
#define YCT_TYPE_INFO_REPLY			0x92	/*羊城通信息查询应答*/
#define YCT_TYPE_SET_REPLY			0x93	/*羊城通设置信息应答*/
#define YCT_TYPE_ERROR_INFO			0x94	/*羊城通错误信息*/
/*司机键盘->羊城通*/
#define YCT_TYPE_INFO_QUERY			0xA0	/*羊城通信息查询应答*/
#define YCT_TYPE_SET_INFO			0xA1	/*羊城通设置信息*/
#define YCT_TYPE_KEYBOARD_UP		0xA2	/*键盘上电通知*/

/*司机键盘->羊城通  羊城通信息查询类型*/
#define YCT_INFO_QUERY_INFO			0x01	/*羊城通收费终端信息*/
#define YCT_INFO_QUERY_PARAM		0x02	/*运营参数*/
#define YCT_INFO_QUERY_DEAL			0x03	/*未上传的交易记录总条数*/
#define YCT_INFO_QUERY_TIME			0x04	/*羊城通终端系统时间*/

#define YCT_KEYBOARD_UP_NOTICE_INTERVAL		120		/*司机键盘向羊城通发送上电通知的时间间隔*/


/*羊城通消息接收验证的返回值*/
#define YCT_OK                   1               /*处理OK*/
#define YCT_ERROR	             0               /*处理失败*/
#define YCT_MSG_LEN_ERROR       -1              /*接收消息长度错误*/
#define YCT_CRC_ERROR           -2              /*接收消息校验错误*/
#define YCT_DEST_ID_ERROR		-3				/*接收消息destId错误*/
#define YCT_RECV_ERROR          -4              /*接收错误*/
#define YCT_SEND_ERROR          -5              /*发送错误*/

/*羊城通消息处理的结果*/
#define YCT_PRO_NO_REPLY		0				/*羊城通消息处理结果不需要应答*/
#define YCT_PRO_REPLY			1				/*羊城通消息处理结果为需要应答*/

/*司机键盘->羊城通 设置类型*/
#define YCT_SET_DIRECTION			0x01		/*设置行驶方向*/
#define YCT_SET_STA_ID				0x02		/*设置站标*/
#define YCT_SET_SEGMENT_POINT		0x03		/*设置分段计价的分段点*/
#define YCT_SET_BACKLIGHT			0x04		/*设置液晶背光开关*/
#define YCT_SET_TIME				0x05		/*设置羊城通时间*/


/*与羊城通通信的消息头结构*/
typedef struct
{
    Uint8    srcDevId[3];                           //起始设备编码
    Uint8    dstDevId[3];                           //目的设备编码
    Uint8    sn[4];                                 //流水号
    Uint8    len[2];                                //帧长度
    Uint8    crc[2];                                //CRC校验
    Uint8    cmd;                                   //帧类型
}S_YctMsgHead;

/*通信应答消息结构*/
typedef struct
{
	Uint8 	sn[4];
}S_YctCommunicationReply;


/*羊城通->BIC消息结构*/
typedef struct
{
	Uint8   hwVersion[5];				/*硬件版本,ASCII码*/
	Uint8	majorCpuSwVersion[5];		/*主CPU软件版本,ASCII码*/
	Uint8	subCpuSwVersion[5];			/*子CPU软件版本,ASCII码*/
	Uint8	statusInfo[2];				/*状态信息,HEX*/
	Uint8	yctTerId[3];				/*羊城通终端ID号,BCD码*/
	Uint8	psamNum[4];					/*羊城通终端PSAM卡号,BCD码*/
	Uint8	yctRouteCode[5];			/*羊城通系统的线路编码,ASCII码*/
	Uint8	yctBusId[8];				/*羊城通系统的车辆自编号,ASCII码*/
	Uint8	yctCompanyId[6];			/*羊城通系统的运营公司编号,BCD码*/	
	Uint8	yctStaId;					/*羊城通系统的站标,HEX*/
	Uint8	yctTime[7];					/*羊城通终端当前时间,BCD码*/
	Uint8	yctMonthTicket[4];			/*当前的月票清次,BCD码*/
	Uint8	yctTicketPrice[2];			/*当前票价,HEX*/
}S_YctStaReply;

/*羊城通->司机键盘消息结构*/
typedef struct
{
	Uint8	yctRouteCode[5];
	Uint8	ChargeStatus;
	Uint8	direction;
	Uint8	ticketPrice[2];
	Uint8	staId;
	Uint8	inOutStaFlag;
	Uint8	upDirecStaNum;
	Uint8	dnDirecStaNum;
	Uint8	upDirecStaTab[16];
	Uint8	dnDirecStaTab[16];
}S_YctRouteInfo;

typedef struct
{
	Uint8	dealType;
	Uint8	cardType;
	Uint8	cardBalance[3];	/*高字节在前，金额单位为0.01元*/
	Uint8	charge[2];		/*高字节在前，金额单位为0.01元*/
	Uint8	cardNum[6];
	Uint8	dealTime[7];	/*BCD码，格式:YYYYMMDDhhmmss*/
	Uint8	validTime[2];	/*BCD码，格式:YYMM*/
	Uint8	routeCode[9];
}S_YctChargeInfo;

typedef struct
{
	Uint8	queryType;
	Uint8	replyContent[100];
}S_YctInfoReply;

typedef struct
{
	Uint8	setType;
	Uint8	setResult;		/*0x00:设置成功  其他:设置失败*/
}S_YctSetReply;

typedef struct
{
	Uint8	errorType;
	Uint8	errorCode;
	Uint8	errorInfo;		/*电话号码或卡片余额*/
}S_YctErrorInfo;

/*司机键盘->羊城通消息结构*/
typedef struct
{
	Uint8	queryType;
}S_YctInfoQuery;

typedef struct
{
	Uint8	setType;
	Uint8	setDataLen;
	union
		{
			Uint8	direction;
			Uint8	staId[2];			/*站标*/
			Uint8	segmentPoint[2];	/*分段计价的分段点*/
			Uint8	backlight;			/*液晶背光开关*/
			Uint8	yctTime[14];		/*羊城通时间设置，格式为YYYYMMDDhhmmss,ASCII码*/
		}setContent;
}S_YctSetReq;

typedef struct
{
	Uint8	flag;		/*相关标志位，目前固定为0x00*/
}S_YctKeyboardUpNotice;


/*Bic->羊城通消息结构*/
typedef struct
{
	Uint8 yctTimeAdjust[16];	/*格式:YYYYMMDDHHMMSSWW*/
}S_YctTimeAdjust;


int YctComOpenCfg(const Char *comDev);
int YctRdMsgPro(Uint8 *pRdMsg, int msgRdLen,Uint8 *pSdMsg,int *msgSdLen);
void YctTimePack(Uint8 *pSdMsg, int *sdMsgLen);
void YctStaQueryPack(Uint8 *pSdMsg, int *sdMsgLen);
void YctComEmptyBuf(const int comFd);
int YctRdMsgVerify(Uint8 *pMsg,Uint16 msgLen);
void YctParamInit(void);






#endif
