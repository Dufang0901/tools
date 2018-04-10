#ifndef _PAD_2_TEST_H_
#define _PAD_2_TEST_H_

#define BIC_DATA_DIR     "/home/bic/Release/data"
#define PAD_INTERACT_TIME     			5	//与pad交互的时间间隔5*60
#define PAD_STATE_WRITE_FILE_TIME   	60	//更新padState文件的时间间隔20*60
#define PAD_STATE_FILE_UPLOAD_TIME		70	//上传padState文件的时间间隔120 *60

#define PAD_MSG_COMMON   0x00
#define PAD_MSG_REQ      0x01
#define PAD_MSG_RLY		 0x10

#define HTE_MSG_HEAD0  'H'
#define HTE_MSG_HEAD1  'T'

/*pad->bic*/
#define PAD_CMD_HB				0x71
#define PAD_CMD_PAD_POWERON		0x01
#define PAD_CMD_SELF_CHECK  	0x02
#define PAD_CMD_KEY_DOWN		0x11
#define PAD_CMD_KEY_HOLD		0x12
#define PAD_CMD_KEY_RELEASE		0x13
#define PAD_CMD_TS_DOWN			0x21
#define PAD_CMD_TS_RELEASE		0x22
#define PAD_CMD_TS_COORD		0x23

/*bic->pad*/
#define PAD_CMD_BL				0x31
#define PAD_CMD_MODE			0x32

typedef struct
{
	Uint8 head[2];
	Uint8 srcDevType;
	Uint8 srcDevId[3];
	Uint8 dstDevType;
	Uint8 dstDevId[3];
	Uint8 sn[4];
	Uint8 type;
	Uint8 cmd;
	Uint8 msgLen[2];
	Uint8 crc[2];
}S_MsgHead; 

typedef struct{
	Uint8 backlight;	/*取值为0-10,0是关闭背光，10最亮*/
}S_BackLight;

typedef struct{
	Uint8 displayMode;	/*输入选择：0-VGA 1-AV1 2-AV2 3-AV3*/
}S_SwitchMode;

typedef struct{
	Uint8 result;		/*0x10-成功 0x01-失败*/	
}S_SwitchResult;

#define PAD_MSG_HEAD_LEN sizeof(S_MsgHead)
typedef struct{
	Int32 year;
	Int32 month;
	Int32 day;
}S_CommonTime;

typedef struct{
	S_CommonTime sTime;
	struct tm sLastConnectTime;
	Int32 blSendCnt;
	Int32 blSendSuccessCnt;
	Int32 blSetSuccessCnt;
	Int32 modeSendCnt;
	Int32 modeSendSuccessCnt;
	Int32 modeSetSuccessCnt;
	Int32 touchCnt;
}S_PadStaMonit;

#endif