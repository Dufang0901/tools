#ifndef _YCT_H_
#define _YCT_H_

#include "OsPack.h"
#include <time.h>

#define YCT_COM_DEV             "/dev/ttyUSB3"  /*�����ͨ�ն�ͨ�ŵĴ����豸*/
#define YCT_COM_BAUD            B9600          /*�����ͨ�ն�ͨ�ŵĴ��ڲ�����*/
#define YCT_BUF_LEN_MAX         1024

#define YCT_HEAD_LEN			sizeof(S_YctMsgHead)

/*���ͨ�ն���ص���Ϣ֡����*/
/*BIC->���ͨ*/
#define YCT_TYPE_STAT_QUERY			0xC8	/*���ͨ״̬��Ϣ��ѯ*/
#define YCT_TYPE_TIME_ADJUST		0xC9	/*���ͨʱ��У׼*/
#define YCT_TYPE_COM_REPLY			0x7F	/*���ͨͨ��Ӧ��*/
/*���ͨ->BIC*/
#define YCT_TYPE_STAT_REPLY			0xC0	/*���ͨ״̬��Ϣ��ѯӦ��*/
/*���ͨ->˾������*/
#define YCT_TYPE_ROUTE_INFO			0x90	/*���ͨ��·��Ϣ�ϱ�*/
#define YCT_TYPE_CHARGE_INFO		0x91	/*���ͨ�۷�����*/
#define YCT_TYPE_INFO_REPLY			0x92	/*���ͨ��Ϣ��ѯӦ��*/
#define YCT_TYPE_SET_REPLY			0x93	/*���ͨ������ϢӦ��*/
#define YCT_TYPE_ERROR_INFO			0x94	/*���ͨ������Ϣ*/
/*˾������->���ͨ*/
#define YCT_TYPE_INFO_QUERY			0xA0	/*���ͨ��Ϣ��ѯӦ��*/
#define YCT_TYPE_SET_INFO			0xA1	/*���ͨ������Ϣ*/
#define YCT_TYPE_KEYBOARD_UP		0xA2	/*�����ϵ�֪ͨ*/

/*˾������->���ͨ  ���ͨ��Ϣ��ѯ����*/
#define YCT_INFO_QUERY_INFO			0x01	/*���ͨ�շ��ն���Ϣ*/
#define YCT_INFO_QUERY_PARAM		0x02	/*��Ӫ����*/
#define YCT_INFO_QUERY_DEAL			0x03	/*δ�ϴ��Ľ��׼�¼������*/
#define YCT_INFO_QUERY_TIME			0x04	/*���ͨ�ն�ϵͳʱ��*/

#define YCT_KEYBOARD_UP_NOTICE_INTERVAL		120		/*˾�����������ͨ�����ϵ�֪ͨ��ʱ����*/


/*���ͨ��Ϣ������֤�ķ���ֵ*/
#define YCT_OK                   1               /*����OK*/
#define YCT_ERROR	             0               /*����ʧ��*/
#define YCT_MSG_LEN_ERROR       -1              /*������Ϣ���ȴ���*/
#define YCT_CRC_ERROR           -2              /*������ϢУ�����*/
#define YCT_DEST_ID_ERROR		-3				/*������ϢdestId����*/
#define YCT_RECV_ERROR          -4              /*���մ���*/
#define YCT_SEND_ERROR          -5              /*���ʹ���*/

/*���ͨ��Ϣ����Ľ��*/
#define YCT_PRO_NO_REPLY		0				/*���ͨ��Ϣ����������ҪӦ��*/
#define YCT_PRO_REPLY			1				/*���ͨ��Ϣ������Ϊ��ҪӦ��*/

/*˾������->���ͨ ��������*/
#define YCT_SET_DIRECTION			0x01		/*������ʻ����*/
#define YCT_SET_STA_ID				0x02		/*����վ��*/
#define YCT_SET_SEGMENT_POINT		0x03		/*���÷ֶμƼ۵ķֶε�*/
#define YCT_SET_BACKLIGHT			0x04		/*����Һ�����⿪��*/
#define YCT_SET_TIME				0x05		/*�������ͨʱ��*/


/*�����ͨͨ�ŵ���Ϣͷ�ṹ*/
typedef struct
{
    Uint8    srcDevId[3];                           //��ʼ�豸����
    Uint8    dstDevId[3];                           //Ŀ���豸����
    Uint8    sn[4];                                 //��ˮ��
    Uint8    len[2];                                //֡����
    Uint8    crc[2];                                //CRCУ��
    Uint8    cmd;                                   //֡����
}S_YctMsgHead;

/*ͨ��Ӧ����Ϣ�ṹ*/
typedef struct
{
	Uint8 	sn[4];
}S_YctCommunicationReply;


/*���ͨ->BIC��Ϣ�ṹ*/
typedef struct
{
	Uint8   hwVersion[5];				/*Ӳ���汾,ASCII��*/
	Uint8	majorCpuSwVersion[5];		/*��CPU����汾,ASCII��*/
	Uint8	subCpuSwVersion[5];			/*��CPU����汾,ASCII��*/
	Uint8	statusInfo[2];				/*״̬��Ϣ,HEX*/
	Uint8	yctTerId[3];				/*���ͨ�ն�ID��,BCD��*/
	Uint8	psamNum[4];					/*���ͨ�ն�PSAM����,BCD��*/
	Uint8	yctRouteCode[5];			/*���ͨϵͳ����·����,ASCII��*/
	Uint8	yctBusId[8];				/*���ͨϵͳ�ĳ����Ա��,ASCII��*/
	Uint8	yctCompanyId[6];			/*���ͨϵͳ����Ӫ��˾���,BCD��*/	
	Uint8	yctStaId;					/*���ͨϵͳ��վ��,HEX*/
	Uint8	yctTime[7];					/*���ͨ�ն˵�ǰʱ��,BCD��*/
	Uint8	yctMonthTicket[4];			/*��ǰ����Ʊ���,BCD��*/
	Uint8	yctTicketPrice[2];			/*��ǰƱ��,HEX*/
}S_YctStaReply;

/*���ͨ->˾��������Ϣ�ṹ*/
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
	Uint8	cardBalance[3];	/*���ֽ���ǰ����λΪ0.01Ԫ*/
	Uint8	charge[2];		/*���ֽ���ǰ����λΪ0.01Ԫ*/
	Uint8	cardNum[6];
	Uint8	dealTime[7];	/*BCD�룬��ʽ:YYYYMMDDhhmmss*/
	Uint8	validTime[2];	/*BCD�룬��ʽ:YYMM*/
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
	Uint8	setResult;		/*0x00:���óɹ�  ����:����ʧ��*/
}S_YctSetReply;

typedef struct
{
	Uint8	errorType;
	Uint8	errorCode;
	Uint8	errorInfo;		/*�绰�����Ƭ���*/
}S_YctErrorInfo;

/*˾������->���ͨ��Ϣ�ṹ*/
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
			Uint8	staId[2];			/*վ��*/
			Uint8	segmentPoint[2];	/*�ֶμƼ۵ķֶε�*/
			Uint8	backlight;			/*Һ�����⿪��*/
			Uint8	yctTime[14];		/*���ͨʱ�����ã���ʽΪYYYYMMDDhhmmss,ASCII��*/
		}setContent;
}S_YctSetReq;

typedef struct
{
	Uint8	flag;		/*��ر�־λ��Ŀǰ�̶�Ϊ0x00*/
}S_YctKeyboardUpNotice;


/*Bic->���ͨ��Ϣ�ṹ*/
typedef struct
{
	Uint8 yctTimeAdjust[16];	/*��ʽ:YYYYMMDDHHMMSSWW*/
}S_YctTimeAdjust;


int YctComOpenCfg(const Char *comDev);
int YctRdMsgPro(Uint8 *pRdMsg, int msgRdLen,Uint8 *pSdMsg,int *msgSdLen);
void YctTimePack(Uint8 *pSdMsg, int *sdMsgLen);
void YctStaQueryPack(Uint8 *pSdMsg, int *sdMsgLen);
void YctComEmptyBuf(const int comFd);
int YctRdMsgVerify(Uint8 *pMsg,Uint16 msgLen);
void YctParamInit(void);






#endif
