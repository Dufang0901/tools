#include "Yct.h"

static Uint8 yctId[3];
static Uint8 keyBoardId[3];
static Uint8 bicId[3];

static Uint16 yctRdSn;
static Uint16 yctSdSn = 100;
static Uint16 yctLastSn;
static Uint8  yctLastCmd;

/*���ڱ������ͨ�ն���Ϣ*/
S_YctStaReply 	sYctStaInfo;		/*���ͨ�ն�״̬��Ϣ*/
S_YctRouteInfo	sYctRouteInfo;		/*���ͨ�ն���·��Ϣ*/
S_YctChargeInfo	sYctChargeInfo;		/*���ͨ�ն���һ���ϱ��Ŀ۷�����*/

static const Uint16 crcTable[256]=
{
    0xF078,0xE1F1,0xD36A,0xC2E3,0xB65C,0xA7D5,0x954E,0x84C7,
    0x7C30,0x6DB9,0x5F22,0x4EAB,0x3A14,0x2B9D,0x1906,0x088F,
    0xE0F9,0xF170,0xC3EB,0xD262,0xA6DD,0xB754,0x85CF,0x9446,
    0x6CB1,0x7D38,0x4FA3,0x5E2A,0x2A95,0x3B1C,0x0987,0x180E,
    0xD17A,0xC0F3,0xF268,0xE3E1,0x975E,0x86D7,0xB44C,0xA5C5,
    0x5D32,0x4CBB,0x7E20,0x6FA9,0x1B16,0x0A9F,0x3804,0x298D,
    0xC1FB,0xD072,0xE2E9,0xF360,0x87DF,0x9656,0xA4CD,0xB544,
    0x4DB3,0x5C3A,0x6EA1,0x7F28,0x0B97,0x1A1E,0x2885,0x390C,
    0xB27C,0xA3F5,0x916E,0x80E7,0xF458,0xE5D1,0xD74A,0xC6C3,
    0x3E34,0x2FBD,0x1D26,0x0CAF,0x7810,0x6999,0x5B02,0x4A8B,
    0xA2FD,0xB374,0x81EF,0x9066,0xE4D9,0xF550,0xC7CB,0xD642,
    0x2EB5,0x3F3C,0x0DA7,0x1C2E,0x6891,0x7918,0x4B83,0x5A0A,
    0x937E,0x82F7,0xB06C,0xA1E5,0xD55A,0xC4D3,0xF648,0xE7C1,
    0x1F36,0x0EBF,0x3C24,0x2DAD,0x5912,0x489B,0x7A00,0x6B89,
    0x83FF,0x9276,0xA0ED,0xB164,0xC5DB,0xD452,0xE6C9,0xF740,
    0x0FB7,0x1E3E,0x2CA5,0x3D2C,0x4993,0x581A,0x6A81,0x7B08,
    0x7470,0x65F9,0x5762,0x46EB,0x3254,0x23DD,0x1146,0x00CF,
    0xF838,0xE9B1,0xDB2A,0xCAA3,0xBE1C,0xAF95,0x9D0E,0x8C87,
    0x64F1,0x7578,0x47E3,0x566A,0x22D5,0x335C,0x01C7,0x104E,
    0xE8B9,0xF930,0xCBAB,0xDA22,0xAE9D,0xBF14,0x8D8F,0x9C06,
    0x5572,0x44FB,0x7660,0x67E9,0x1356,0x02DF,0x3044,0x21CD,
    0xD93A,0xC8B3,0xFA28,0xEBA1,0x9F1E,0x8E97,0xBC0C,0xAD85,
    0x45F3,0x547A,0x66E1,0x7768,0x03D7,0x125E,0x20C5,0x314C,
    0xC9BB,0xD832,0xEAA9,0xFB20,0x8F9F,0x9E16,0xAC8D,0xBD04,
    0x3674,0x27FD,0x1566,0x04EF,0x7050,0x61D9,0x5342,0x42CB,
    0xBA3C,0xABB5,0x992E,0x88A7,0xFC18,0xED91,0xDF0A,0xCE83,
    0x26F5,0x377C,0x05E7,0x146E,0x60D1,0x7158,0x43C3,0x524A,
    0xAABD,0xBB34,0x89AF,0x9826,0xEC99,0xFD10,0xCF8B,0xDE02,
    0x1776,0x06FF,0x3464,0x25ED,0x5152,0x40DB,0x7240,0x63C9,
    0x9B3E,0x8AB7,0xB82C,0xA9A5,0xDD1A,0xCC93,0xFE08,0xEF81,
    0x07F7,0x167E,0x24E5,0x356C,0x41D3,0x505A,0x62C1,0x7348,
    0x8BBF,0x9A36,0xA8AD,0xB924,0xCD9B,0xDC12,0xEE89,0xFF00
};



static void YctStaReplyPro(Uint8 * pRdMsg);
static void YctRouteInfoPro(Uint8 *pRdMsg, int msgRdLen,Uint8 *pSdMsg,int *msgSdLen);
static void YctChargeInfoPro(Uint8 *pRdMsg);
static void YctInfoReplyPro(Uint8 *pRdMsg);
static void YctSetReplyPro(Uint8 *pRdMsg);
static void YctErrorInfoPro(Uint8 *pRdMsg);
static void YctCommunicateReplyPack(Uint8 *sdBuf,int *sdMsgLen);





/*********************************************************
    Function:       YctComOpenCfg
    Description:    ���ͨ�ն˴�������
    Param:          comDev �����豸
    Return:         ������
    Other:          none
*********************************************************/
int YctComOpenCfg(const Char *comDev)
{
    int comFd;
    struct termios comTty;
    
    comFd = open(comDev, O_RDWR|O_NOCTTY, 0);

    if(comFd < 0)
    {
        return comFd;
    }
    
    if (fcntl(comFd, F_SETFL, 0) < 0)
    {
        return -1;
    }
    
    bzero( &comTty, sizeof( comTty ) );
    cfsetispeed(&comTty,YCT_COM_BAUD);  /*����*/
    cfsetospeed(&comTty,YCT_COM_BAUD);  /*���*/  
    
    comTty.c_cflag  |=  CLOCAL | CREAD;
    comTty.c_cflag &= ~CSIZE;
    comTty.c_cflag |= CS8;
    
    comTty.c_cflag &= ~PARENB;
    
    
    comTty.c_cflag &=  ~CSTOPB;
    /*��ʱ�ȴ�ʱ��200����*/
    comTty.c_cc[VTIME]  = 2;
    comTty.c_cc[VMIN] = 0;
    tcflush(comFd,TCIFLUSH);
    if((tcsetattr(comFd,TCSANOW,&comTty))!=0)
    {
        return -1;
    }
    return comFd;
    
}

/***********************************************************************
    Function:       YctCrcCalc
    Description:    �������ͨЭ��CRCֵ
    Param:          pData
                    len
    Return:         none
    Other:          none
***********************************************************************/
Uint16 YctCrcCalc(Uint8 *pData, Uint16 len)
{
    Uint16 crcVal = 0;
    Uint16 i;

    for( i = 0; i < len; i++)
    {
        crcVal = crcTable[(crcVal ^= pData[i] ) & 0xFF] ^ (crcVal >> 8);
    }
    
    return crcVal;

}


/*********************************************************
    Function:       YctParamInit
    Description:    ��ʼ�����ͨ��ز���
    Param:          centerParam ���ؽ��̲���
    Return:         none
    Other:          none
*********************************************************/
void YctParamInit()
{

	bicId[0] = 0x10;
	bicId[1] = 0x00;
	bicId[2] = 0x00;
	
	yctId[0] = 0x03;
	yctId[1] = 0x00;
	yctId[2] = 0x00;

	keyBoardId[0] = 0x04;
	keyBoardId[1] = 0x00;
	keyBoardId[2] = 0x00;
}
/*********************************************************
    Function:       YctRecvMsg
    Description:    ���մ����ͨ�ն˷��͹�������Ϣ
    Param:          comFd ����������
                    recvBuf
                    recvLen
    Return:         ���ս��
    Other:          none
*********************************************************/
void YctPrintMsg(Uint8 *buf, int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		printf("%02x ",buf[i]);
	}
	printf("\n");
}
/*********************************************************
    Function:       YctRecvMsg
    Description:    ���մ����ͨ�ն˷��͹�������Ϣ
    Param:          comFd ����������
                    recvBuf
                    recvLen
    Return:         ���ս��
    Other:          none
*********************************************************/
int YctRecvMsg(int comFd,Uint8 *recvBuf,Uint16 *recvLen)
{
    size_t hByte;	/*��Ϣͷ�ֽ���*/
    size_t dByte;	/*���ݳ���*/
    size_t rByte;	/*�ѽ��յ����ֽ���*/
    int n;
	Uint8	flag = 0;	/*���ڽ�����ʼ�ͽ�����־*/
    S_YctMsgHead *sYctMsgHead = (S_YctMsgHead *)recvBuf;

	/*������ʼ��־*/
	do
	{
		if(Read(comFd, &flag, 1) != 1)
		{
			printf("%s:receive start flag failed.\n ",__func__);
			return YCT_RECV_ERROR;
		}
	}while( flag != 0x7E);
	
    
    hByte = YCT_HEAD_LEN;
    rByte = 0;
    while(rByte < hByte)
    {
    	/*������Ϣͷ*/
        if((n = Read(comFd,recvBuf + rByte,hByte - rByte)) > 0)
        {
            rByte += n;
        }
        else
        {   
        	return YCT_RECV_ERROR;
        }
    }
	
    dByte = (sYctMsgHead->len[0] << 8) + sYctMsgHead->len[1] - YCT_HEAD_LEN;

	/*��������Ϣͷ�������������ݲ���*/
    if(dByte > (YCT_BUF_LEN_MAX - YCT_HEAD_LEN))
    {
        return YCT_MSG_LEN_ERROR;
    }
    
    rByte = 0;
        
    while(rByte < dByte)
    {
        if((n = Read(comFd,recvBuf + hByte + rByte,dByte - rByte)) > 0)
        {
            rByte += n;
        }
        else
        {
            return YCT_RECV_ERROR;
        }
    }
    
    *recvLen = (hByte + dByte);

	/*���ս�����־*/
	flag = 0;

	if(Read(comFd, &flag, 1) != 1)
	{
		printf("%s:receive end flag failed.\n ",__func__);
		return YCT_RECV_ERROR;
	}
	else if(flag != 0x7E)
	{
		printf("%s:receive end flag failed.\n ");
		return YCT_RECV_ERROR;
	}

    return YCT_OK;
}


/***********************************************************************
    Function:       SvClientRdMsgVerify
    Description:    �Խ��յ������ݽ���У��
    Param:          pMsg          ����ָ��
                    msgLen        ���ݳ���
    Return:         У����
    Other:          У����Ϣ����,���ݳ��Ⱥ�CRC
***********************************************************************/
int YctRdMsgVerify(Uint8 *pMsg,Uint16 msgLen)
{
    /*ȷ�����ݳ��Ⱥ�У��ֵ�Ƿ�һ��*/
    Uint16 crcRd,yctCrc;
    Uint16 bsrRdDataLen;
	Uint16 dataLen;
    S_YctMsgHead *sYctMsgHead = (S_YctMsgHead *)pMsg;

    //BicLogPrintf(LOG_IN_FLAG,"Bsr SvClientRdMsgVerify pMsg is 0x%x,msgLen is %d.\n",(Uint32)pMsg,msgLen);    
    dataLen = (sYctMsgHead->len[0] << 8) + sYctMsgHead->len[1];
    
    if(dataLen != msgLen)
    {
        /*��Ϣ���Ȳ�ƥ��*/
		printf("YctRdMsgVerify:Yct recv msg length error.\n");
        return YCT_MSG_LEN_ERROR;
    }
    
    crcRd = (sYctMsgHead->crc[0] << 8) + sYctMsgHead->crc[1];
    sYctMsgHead->crc[0] = 0x00;
    sYctMsgHead->crc[1] = 0x00;
    yctCrc = YctCrcCalc(pMsg,msgLen);

    if(crcRd != yctCrc)
    {
        /*У�����*/
        printf("YctRdMsgVerify:Yct recv msg crc error,recv crc is 0x%04x,calc crc is 0x%04x.\n",crcRd,yctCrc);
        return YCT_CRC_ERROR;
    }
	sYctMsgHead->crc[0] = ((crcRd >> 8) & 0xFF);
	sYctMsgHead->crc[1] = crcRd & 0xFF;
    
    if(memcmp(sYctMsgHead->dstDevId,bicId,3) != 0 && memcmp(sYctMsgHead->dstDevId,keyBoardId,3) != 0)
   	{
   		/*�豸ID����*/
        printf("YctRdMsgVerify:Yct recv msg destID error,recv id is %02x%02x%02x.\n",sYctMsgHead->dstDevId[0],sYctMsgHead->dstDevId[1],sYctMsgHead->dstDevId[2]);
   		return YCT_DEST_ID_ERROR;
   	}
    
    yctRdSn = (sYctMsgHead->sn[0] << 24) | (sYctMsgHead->sn[1] << 16) | (sYctMsgHead->sn[2] << 8) | sYctMsgHead->sn[3];
    yctRdSn &= 0x7fffffff;

	/*��ӡ��־��Ϣ*/
	
	switch(sYctMsgHead->cmd)
	{
		case YCT_TYPE_STAT_REPLY:	/*���ͨ�ն�״̬��ѯӦ��*/
		{
			printf("Yct->Bic: YCT_TYPE_STAT_REPLY. cmd=0x%X\n",sYctMsgHead->cmd);
		}
		break;
		case YCT_TYPE_ROUTE_INFO:	/*��·��Ϣ*/
		{
			printf("Yct->Keyboard: YCT_TYPE_ROUTE_INFO. cmd=0x%X\n",sYctMsgHead->cmd);
		}
		break;
		case YCT_TYPE_CHARGE_INFO:	/*���ͨ�ն˿۷�����*/
		{
			printf("Yct->Keyboard: YCT_TYPE_CHARGE_INFO. cmd=0x%X\n",sYctMsgHead->cmd);
		}
		break;
		case YCT_TYPE_INFO_REPLY:	/*���ͨ�ն���Ϣ��ѯӦ��*/
		{
			printf("Yct->Keyboard: YCT_TYPE_ROUTE_INFO. cmd=0x%X\n",sYctMsgHead->cmd);
		}
		break;
		case YCT_TYPE_SET_REPLY:	/*������ϢӦ��*/
		{
			printf("Yct->Keyboard: YCT_TYPE_ROUTE_INFO. cmd=0x%X\n",sYctMsgHead->cmd);
		}
		break;
		case YCT_TYPE_ERROR_INFO:	/*���ͨ������Ϣ�ϱ�*/
		{
			printf("Yct->Keyboard: YCT_TYPE_ROUTE_INFO. cmd=0x%X\n",sYctMsgHead->cmd);
		}
		break;
		
		default:
		{
			printf("[DOWN] DEFAULT. cmd=0x%X\n",sYctMsgHead->cmd);
		}
		break;
	}
	
    return YCT_OK;
}


/***********************************************************************
    Function:       YctRdMsgPro
    Description:    �Խ��յ������ͨ�ն���Ϣ���д���
    Param:          pRdMsg          ��������ָ��
                    msgRdLen        �������ݳ���
                    pSdMsg          ��������ָ��
                    msgSdLen        �������ݳ���
    Return:         Ӧ���־         >= 1��ʾ��ҪӦ��
                                    ���ݷ���ֵ����������
    Other:          none
***********************************************************************/
int YctRdMsgPro(Uint8 *pRdMsg, int msgRdLen,Uint8 *pSdMsg,int *msgSdLen)
{
    int yctRdDataLen;
    int    ret = YCT_PRO_NO_REPLY;
    S_YctMsgHead *sYctMsgHead = (S_YctMsgHead *)pRdMsg;
    
    yctRdDataLen = (sYctMsgHead->len[0] << 8) + sYctMsgHead->len[1];

    /*�������ж���Ϣ�Ƿ����һ������Ϊͬһ��*/
    if(yctLastSn == yctRdSn && yctLastCmd == sYctMsgHead->cmd)
    {
    	printf("yctLastSn = yctRdSn && yctLastCmd = sPadMsgHead->cmd \n");
        /*ֱ�����ϴε�����Ӧ��*/
        return YCT_PRO_REPLY;
    }

    yctLastSn = yctRdSn;
    yctLastCmd = sYctMsgHead->cmd;

	printf("YctRdMsgPro cmd=0x%02x yctRdDataLen=%d \n",sYctMsgHead->cmd,yctRdDataLen);
	
    switch(sYctMsgHead->cmd)
    {
    /*���ͨ -> BIC*/
    	case YCT_TYPE_STAT_REPLY:
		{
			/*���ͨ״̬��Ϣ��ѯӦ��*/
			YctStaReplyPro((Uint8 *)(sYctMsgHead + 1));
			ret = YCT_PRO_NO_REPLY;
		}
		break;     
	/*���ͨ -> ˾������*/	
		case YCT_TYPE_ROUTE_INFO:
		{
			/*���ͨ��·��Ϣ�ϱ�*/
			YctRouteInfoPro((Uint8 *)(sYctMsgHead + 1),yctRdDataLen,pSdMsg,msgSdLen);
			ret = YCT_PRO_REPLY;
		}
		break; 
		case YCT_TYPE_CHARGE_INFO:
		{
			/*���ͨ�۷������ϱ�*/
			YctChargeInfoPro((Uint8 *)(sYctMsgHead + 1));
			ret = YCT_PRO_NO_REPLY;
		}
		break; 
		case YCT_TYPE_INFO_REPLY:
		{
			/*���ͨ��Ϣ��ѯӦ��*/
			YctInfoReplyPro((Uint8 *)(sYctMsgHead + 1));
			ret = YCT_PRO_NO_REPLY;
		}
		break; 
		case YCT_TYPE_SET_REPLY:
		{
			/*���ͨ���ý��Ӧ��*/
			YctSetReplyPro((Uint8 *)(sYctMsgHead + 1));
			ret = YCT_PRO_NO_REPLY;
		}
		break; 
		case YCT_TYPE_ERROR_INFO:
		{
			/*���ͨ������Ϣ�ϱ�*/
			YctErrorInfoPro((Uint8 *)(sYctMsgHead + 1));
			ret = YCT_PRO_NO_REPLY;
		}
		break; 
        default:
            ret = YCT_PRO_NO_REPLY;
        break;
    }

    
    return ret;
}


/***********************************************************************
    Function:       YctBdSdHead
    Description:    �������͸����ͨ����Ϣͷ
    Param:          pBuf      ����ָ��
                    needRply  Ӧ�������־λ
                    dataLen   ���ݳ���
                    cmd       ����
    Return:         none
    Other:          none
***********************************************************************/
static void YctBdSdHead(Uint8 *pBuf,Uint8 *srcId,Uint8 needRply,Uint16 dataLen,Uint8 cmd)
{
    S_YctMsgHead*sYctHead = (S_YctMsgHead *)pBuf;
    
    Uint16 frameLen = dataLen + YCT_HEAD_LEN;
    
    sYctHead->srcDevId[0] = srcId[0];
    sYctHead->srcDevId[1] = srcId[1];
    sYctHead->srcDevId[2] = srcId[2];
    
    sYctHead->dstDevId[0] = yctId[0];
    sYctHead->dstDevId[1] = yctId[1];
    sYctHead->dstDevId[2] = yctId[2];
    printf("%s:%d sn=%d\n",__func__,__LINE__,yctSdSn);
    sYctHead->sn[0] = (needRply << 7) | ((yctSdSn >> 24) & 0xff);
    sYctHead->sn[1] = (yctSdSn >> 16) & 0xff;
    sYctHead->sn[2] = (yctSdSn >> 8)  & 0xff;
    sYctHead->sn[3] = yctSdSn & 0xff;
    
    sYctHead->len[0] = (frameLen >> 8) & 0xff;
    sYctHead->len[1] = frameLen & 0xff;
    
    sYctHead->crc[0] = 0x00;
    sYctHead->crc[1] = 0x00;
    
    sYctHead->cmd = cmd;
    
   // yctSdSnTemp = yctSdSn;
    
    if(++yctSdSn >= 0x80000000)
    {
        yctSdSn = 0;
    }
}


/*********************************************************
    Function:       YctSendMsg
    Description:    �����ͨ�ն˷�����Ϣ
    Param:          comFd ����������
                    sendBuf
                    sendLen
    Return:         ���ͽ��
    Other:          none
*********************************************************/
int YctSendMsg(int comFd,Uint8 *sendBuf,Uint16 sendLen)
{
	size_t wByte = 0;
	Uint8 flag = 0x7E;
	int n;

	/*������ʼ��־*/
	if(Write(comFd, &flag, 1) != 1)
	{
		printf("%s:send start flag failed.\n",__func__);
		return YCT_SEND_ERROR;
	}
	
	while(wByte < sendLen)
	{
		if((n = Write(comFd,sendBuf + wByte,sendLen - wByte)) > 0)
		{
			wByte += n;
		}
		else
		{
			return YCT_SEND_ERROR;
		}
	}
	/*���ͽ�����־*/
	if(Write(comFd, &flag, 1) != 1)
	{
		printf("%s:send end flag failed.\n",__func__);
		return YCT_SEND_ERROR;
	}
   
	
	return YCT_OK;

}


/***********************************************************************
    Function:       YctSendCommunicateReply
    Description:    �����ͨ�ն˷���ͨ��Ӧ��֡
    Param:          sdBuf		�洢ͨ��Ӧ��֡��ָ��    
    Return:         none
    Other:          none
***********************************************************************/
static void YctCommunicateReplyPack(Uint8 *sdBuf,int *sdMsgLen)
{
    Uint16    yctCrc; 
	int		  yctSdMsgLen;
	S_YctMsgHead*sYctHead = (S_YctMsgHead *)sdBuf;
	S_YctCommunicationReply *pYctCommRply = (S_YctCommunicationReply *)(sYctHead + 1);

	YctBdSdHead(sdBuf,keyBoardId,0,sizeof(S_YctCommunicationReply),YCT_TYPE_COM_REPLY);

    pYctCommRply->sn[0] = (yctRdSn >> 24) & 0xff;
    pYctCommRply->sn[1] = (yctRdSn >> 16) & 0xff;
    pYctCommRply->sn[2] = (yctRdSn >> 8)  & 0xff;
    pYctCommRply->sn[3] = yctRdSn & 0xff;
	
	
	yctSdMsgLen = YCT_HEAD_LEN + sizeof(S_YctCommunicationReply);

	yctCrc = YctCrcCalc(sdBuf,yctSdMsgLen);
    
    sYctHead->crc[0] = (yctCrc >> 8) & 0xff;
    sYctHead->crc[1] = yctCrc & 0xff;
	*sdMsgLen = yctSdMsgLen;

}


/*********************************************************
    Function:       YctComEmptyBuf
    Description:    ���Pad���ڻ�����
    Param:          comFd ����������
    Return:         none
    Other:          none
*********************************************************/
void YctComEmptyBuf(const int comFd)
{
    tcflush(comFd, TCIOFLUSH);
}

/***********************************************************************
    Function:       YctStaReplyPro
    Description:    �Խ��յ������ͨ�ն���Ϣ���д���
    Param:          pRdMsg          �յ�����Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
static void YctStaReplyPro(Uint8 * pRdMsg)
{
	S_YctStaReply *pYctStaInfo = (S_YctStaReply *)pRdMsg;
	memcpy(&sYctStaInfo, pYctStaInfo, sizeof(S_YctStaReply));

	printf("************************* Yct State Info ********************************\n");
	printf("hwVersion=%c%c%c%c%c\n",sYctStaInfo.hwVersion[0],sYctStaInfo.hwVersion[1],sYctStaInfo.hwVersion[2],sYctStaInfo.hwVersion[3],sYctStaInfo.hwVersion[4]);
	printf("majorCpuSwVersion=%c%c%c%c%c\n",sYctStaInfo.majorCpuSwVersion[0],sYctStaInfo.majorCpuSwVersion[1],sYctStaInfo.majorCpuSwVersion[2],sYctStaInfo.majorCpuSwVersion[3],sYctStaInfo.majorCpuSwVersion[4]);
	printf("subCpuSwVersion=%C%C%C%C%C\n",sYctStaInfo.subCpuSwVersion[0],sYctStaInfo.subCpuSwVersion[1],sYctStaInfo.subCpuSwVersion[2],sYctStaInfo.subCpuSwVersion[3],sYctStaInfo.subCpuSwVersion[4]);
}

/***********************************************************************
    Function:       YctRouteInfoPro
    Description:    �Խ��յ������ͨ�ն���·��Ϣ���д���,��Ҫͨ��Ӧ��
    Param:          pRdMsg          �յ�����Ϣ����ָ��
    				msgRdLen		�յ�����Ϣ����
    				pSdMsg			�洢������Ϣ��ָ��
    				msgSdLen		������Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
static void YctRouteInfoPro(Uint8 *pRdMsg, int msgRdLen,Uint8 *pSdMsg,int *msgSdLen)
{
	printf("*************************** %s:%d ******************************************\n",__func__,__LINE__);
	YctCommunicateReplyPack(pSdMsg,msgSdLen);
	S_YctRouteInfo *sYctData = (S_YctRouteInfo *)pRdMsg;
	
	
}

/***********************************************************************
    Function:       YctChargeInfoPro
    Description:    �Խ��յ������ͨ�ն˿۷������ϱ���Ϣ���д���
    Param:          pRdMsg          �յ�����Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
static void YctChargeInfoPro(Uint8 *pRdMsg)
{
	printf("*************************** %s:%d ******************************************\n",__func__,__LINE__);
	S_YctChargeInfo *sYctCharge = (S_YctChargeInfo *)pRdMsg;
	printf("dealType=%x cardType=%x charge=%02x %02x\n",sYctCharge->dealType,sYctCharge->cardType,sYctCharge->charge[0],sYctCharge->charge[1]);
	
}

/***********************************************************************
    Function:       YctInfoReplyPro
    Description:    �Խ��յ������ͨ�ն���Ϣ��ѯӦ����д���
    Param:          pRdMsg          �յ�����Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
static void YctInfoReplyPro(Uint8 *pRdMsg)
{
	printf("*************************** %s:%d ******************************************\n",__func__,__LINE__);
	S_YctInfoReply *sYctInfoRply = (S_YctInfoReply *)pRdMsg;
	printf("infoQueryType=%x info:%s\n",sYctInfoRply->queryType,sYctInfoRply->replyContent);
}

/***********************************************************************
    Function:       YctSetReplyPro
    Description:    �Խ��յ������ͨ�ն�����Ӧ����Ϣ���д���
    Param:          pRdMsg          �յ�����Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
static void YctSetReplyPro(Uint8 *pRdMsg)
{
	printf("*************************** %s:%d ******************************************\n",__func__,__LINE__);

}

/***********************************************************************
    Function:       YctRouteInfoPro
    Description:    �Խ��յ������ͨ�ն˴����ϱ���Ϣ���д���
    Param:          pRdMsg          �յ�����Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
static void YctErrorInfoPro(Uint8 *pRdMsg)
{
	printf("*************************** %s:%d ******************************************\n",__func__,__LINE__);

}

/***********************************************************************
    Function:       YctStaQueryPack
    Description:    ��װ���ͨ״̬��Ϣ��ѯ��Ϣ
    Param:          pSdMsg          ��ŷ�����Ϣ��ָ��
    				sdMsgLen		��Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
void YctStaQueryPack(Uint8 *pSdMsg, int *sdMsgLen)
{
	Uint16 yctCrc;
	S_YctMsgHead *sYctHead = (S_YctMsgHead *)pSdMsg;
	
	YctBdSdHead(pSdMsg,bicId,0,0,YCT_TYPE_STAT_QUERY);
	*sdMsgLen = YCT_HEAD_LEN;
	
	yctCrc = YctCrcCalc(pSdMsg,*sdMsgLen);
    sYctHead->crc[0] = (yctCrc >> 8) & 0xff;
    sYctHead->crc[1] = yctCrc & 0xff;
}

/***********************************************************************
    Function:       YctTimePack
    Description:    ��װ���ͨʱ��У׼��Ϣ
    Param:          pSdMsg          ��ŷ�����Ϣ��ָ��
    				sdMsgLen		��Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
void YctTimePack(Uint8 *pSdMsg, int *sdMsgLen)
{
	S_YctMsgHead *sYctHead = (S_YctMsgHead *)pSdMsg;
	S_YctTimeAdjust *sYctData = (S_YctTimeAdjust *)(sYctHead + 1);
	time_t timeSec = time(NULL);
	struct tm *sTime = localtime(&timeSec);
	Uint8 *ptr;
	Uint16 yctCrc;

	*sdMsgLen = YCT_HEAD_LEN + sizeof(S_YctTimeAdjust);
	YctBdSdHead(pSdMsg,bicId,0,sizeof(S_YctTimeAdjust),YCT_TYPE_TIME_ADJUST);
	ptr = sYctData->yctTimeAdjust;
	sprintf(ptr,"%04d",sTime->tm_year+1900);
	ptr += 4;
	sprintf(ptr,"%02d",sTime->tm_mon+1);
	ptr +=2;
	sprintf(ptr,"%02d",sTime->tm_mday);
	ptr +=2;
	sprintf(ptr,"%02d",sTime->tm_hour);
	ptr +=2;
	sprintf(ptr,"%02d",sTime->tm_min);
	ptr +=2;
	sprintf(ptr,"%02d",sTime->tm_sec);
	ptr +=2;
	sprintf(ptr,"%02d",sTime->tm_wday);
	ptr +=2;

	yctCrc = YctCrcCalc(pSdMsg,*sdMsgLen);
	sYctHead->crc[0] = (yctCrc >> 8) & 0xff;
    sYctHead->crc[1] = yctCrc & 0xff;
}

/***********************************************************************
    Function:       YctInfoQueryPack
    Description:    ��װ���ͨ��Ϣ��ѯ��Ϣ
    Param:          pSdMsg          ��ŷ�����Ϣ��ָ��
    				sdMsgLen		��Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
void YctInfoQueryPack(Uint8 *pSdMsg, int *sdMsgLen, Uint8 queryType)
{
	Uint16 yctCrc;
	S_YctMsgHead *sYctHead = (S_YctMsgHead *)pSdMsg;
	S_YctInfoQuery *sYctData = (S_YctInfoQuery *)(sYctHead + 1);
	
	YctBdSdHead(pSdMsg,keyBoardId,0,sizeof(S_YctInfoQuery),YCT_TYPE_INFO_QUERY);
	*sdMsgLen = YCT_HEAD_LEN + sizeof(S_YctInfoQuery);

	sYctData->queryType = queryType;
	
	yctCrc = YctCrcCalc(pSdMsg,*sdMsgLen);
	sYctHead->crc[0] = (yctCrc >> 8) & 0xff;
	sYctHead->crc[1] = yctCrc & 0xff;
}

/***********************************************************************
    Function:       YctKeyboardUpNoticePack
    Description:    ��װ�����ϵ�֪ͨ��Ϣ
    Param:          pSdMsg          ��ŷ�����Ϣ��ָ��
    				sdMsgLen		��Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
void YctKeyboardUpNoticePack(Uint8 *pSdMsg, int *sdMsgLen)
{
	Uint16 yctCrc;
	S_YctMsgHead *sYctHead = (S_YctMsgHead *)pSdMsg;
	S_YctKeyboardUpNotice *sYctData = (S_YctKeyboardUpNotice *)(sYctHead + 1);
	YctBdSdHead(pSdMsg,keyBoardId,0,sizeof(S_YctKeyboardUpNotice),YCT_TYPE_KEYBOARD_UP);
	*sdMsgLen = YCT_HEAD_LEN + sizeof(S_YctKeyboardUpNotice);

	sYctData->flag = 0x00;
	
	yctCrc = YctCrcCalc(pSdMsg,*sdMsgLen);
	sYctHead->crc[0] = (yctCrc >> 8) & 0xff;
	sYctHead->crc[1] = yctCrc & 0xff;
}

/***********************************************************************
    Function:       YctSetMsgPack
    Description:    ��װ���ͨ������Ϣ
    Param:          pSdMsg          ��ŷ�����Ϣ��ָ��
    				sdMsgLen		��Ϣ����ָ��
    Return:         none
    Other:          none
***********************************************************************/
void YctSetMsgPack(Uint8 *pSdMsg, int *sdMsgLen, Uint8 type, void *content)
{
	Uint16 yctCrc;
	S_YctMsgHead *sYctHead = (S_YctMsgHead *)pSdMsg;
	S_YctSetReq *sYctData = (S_YctSetReq *)(sYctHead + 1);

	sYctData->setType = type;

	switch(type)
	{
		case YCT_SET_DIRECTION:
		{
			sYctData->setDataLen = 1;
			
			sYctData->setContent.direction = *(Uint8 *)content;
		}
		break;
		case YCT_SET_STA_ID:
		{
			sYctData->setDataLen = 2;
			memcpy(sYctData->setContent.staId, (Uint8 *)content, 2);
		}
		break;
		case YCT_SET_SEGMENT_POINT:
		{
			sYctData->setDataLen = 2;
			memcpy(sYctData->setContent.segmentPoint, (Uint8 *)content, 2);
		}
		break;
		case YCT_SET_BACKLIGHT:
		{
			sYctData->setDataLen = 1;
			sYctData->setContent.backlight = *(Uint8 *)content;
		}
		break;
		case YCT_SET_TIME:
		{
			sYctData->setDataLen = 14;
			memcpy(sYctData->setContent.yctTime, (Uint8 *)content, 14);
		}
		break;
		default:
		break;
	}

	YctBdSdHead(pSdMsg,keyBoardId,0,sYctData->setDataLen,YCT_TYPE_SET_INFO);

	yctCrc = YctCrcCalc(pSdMsg,*sdMsgLen);
	sYctHead->crc[0] = (yctCrc >> 8) & 0xff;
	sYctHead->crc[1] = yctCrc & 0xff;
}


