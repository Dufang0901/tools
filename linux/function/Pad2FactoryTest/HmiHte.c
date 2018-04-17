/*****************************************************************************
  FileName:     HmiHte.c
  Author:       �ź�
  Version:      1.0
  Date:         2017/01/21 11:39:30
  Description:  �˻��������̶��յ���pad��������hteЭ����Ϣ�Ĵ���
  History:  
                <author>    <time>    <version>    <desc>
                 duheng    2017.01.18    V1.0       create
*****************************************************************************/
#include "Common.h"
#include "HtePadProtocol.h"

static int HteSendMsg(int comFd,Uint8 *sendBuf,Uint16 sendLen);
static int HteRecvMsg(int comFd,Uint8 *recvBuf,Uint16 *recvLen);
static int HteRdMsgVerify(Uint8 *pMsg, Uint16 msgLen);
static void PadKeyPressPro(S_HmiProcessRes *psHmiProcessRes,Uint8 keyId);
static void PadMicStaPro(S_HmiProcessRes *psHmiProcessRes,Int32 state);
static void PackMsgHead(S_HteMsgHead *psMsgHead,Uint8 type,Uint8 cmd,Int32 sn,Int16 msgLen);
static Uint16 PadCrcCal(Uint8 *msgBuf,int msgLen);
static Int32 PackBacklightSetMsg(Uint8 *msgBuf,Uint8 value);
static Int32 PackModeSwitchMsg(Uint8 *msgBuf,Uint8 mode);
static Int32 PackBuzzCtrlMsg(Uint8 *msgBuf,Uint8 cmd);


/*********************************************************
    Function:       HteComOpenCfg
    Description:    �򿪲�������keyboardͨ�ŵ�socket�ļ�������
    Param:          void
    Return:         ������
    Other:          none
*********************************************************/
Int32 HteComOpenCfg(void)
{
	int sockFd;
	struct sockaddr_un localAddr;

    /*����socket�ļ�Ŀ¼*/ 
    if(mkdir(HMI_HTE_UNIX_DIR,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0) 
    {
        if(errno != EEXIST)
        {
            /*����������ɹ�����Ŀ¼������*/
            return errno;
        }
    }

    if((sockFd = socket(PF_UNIX,SOCK_DGRAM,0)) < 0)  
    {  
        printf("Ble location blind fd socket failed.\n");  
        return -1;  
    }
    
    unlink(HMI_HTE_UNIX_DEST);
    
    bzero(&localAddr,sizeof(localAddr));  
    localAddr.sun_family = PF_UNIX;
    strcpy(localAddr.sun_path,HMI_HTE_UNIX_DEST);

    if(bind(sockFd,(struct sockaddr *)&localAddr,sizeof(localAddr)) < 0)  
    {  
        printf("PadComOpenCfg:fd bind failed.\n"); 
        close(sockFd); 
        return -1; 
    }
    
    return sockFd;	
    
}

/*********************************************************
    Function:       HteRecvMsg
    Description:    ���հ�����Ϣ
    Param:          comFd �ļ�������
                    recvBuf
                    recvLen
    Return:         ���ս��
    Other:          none
*********************************************************/
static int HteSendMsg(int comFd,Uint8 *sendBuf,Uint16 sendLen)
{
	struct  sockaddr_un destAddr;
    int     addrLen;
    ssize_t ret;

    bzero(&destAddr,sizeof(destAddr));  
    destAddr.sun_family = PF_UNIX;
    strcpy(destAddr.sun_path,HMI_HTE_UNIX_SOURCE);
    addrLen = sizeof(destAddr);

	ret = sendto(comFd,sendBuf,sendLen,0,(struct sockaddr *)&destAddr,addrLen);
    if(ret != sendLen)
	{
		printf("$$$$$$$$$$$$$$$$$$$$ HteSendMsg: error,ret is %d\n",ret);
		return -1;
	}
	else
	{
		printf("$$$$$$$$$$$$$$$$$$$$ HteSendMsg: successful,sendLen is %d\n",sendLen);
		return 1;
	}

}

/*********************************************************
    Function:       HteRecvMsg
    Description:    ���հ�����Ϣ
    Param:          comFd �ļ�������
                    recvBuf
                    recvLen
    Return:         ���ս��
    Other:          none
*********************************************************/
static int HteRecvMsg(int comFd,Uint8 *recvBuf,Uint16 *recvLen)
{
	struct  sockaddr_un srcAddr;
    int     peerAddrLen;

	*recvLen = recvfrom(comFd,recvBuf,HMI_HTE_BUF_LEN_MAX,0,(struct sockaddr *)&srcAddr,(socklen_t *) &peerAddrLen);
	if(*recvLen <= 0)
	{
		//printf("$$$$$$$$$$$$$$$$$$$$ PadRecvMsg: error,recvLen is %d\n",*recvLen);
		return -1;
	}
	else
	{
		//printf("$$$$$$$$$$$$$$$$$$$$ PadRecvMsg: successful,recvLen is %d\n",*recvLen);
		return 1;
	}

}


/***********************************************************************
    Function:       HteRdMsgVerify
    Description:    ���յ��İ�����Ϣ����У��
    Param:          pMsg          ����ָ��
                    msgLen        ���ݳ���
    Return:         У����
    Other:          У����Ϣ����,���ݳ��Ⱥ�CRC
***********************************************************************/
static int HteRdMsgVerify(Uint8 *pMsg, Uint16 msgLen)
{     
    S_HteMsgHead *psHteMsgHead = (S_HteMsgHead *)pMsg;
    
    if(msgLen != (psHteMsgHead->msgLen[0] << 8 | psHteMsgHead->msgLen[1]))
        return -1;
    return 1;
}


/*********************************************************
    Function:       HteMsgRecvAndPro
    Description:    �Խ��յ���pad��Ϣ���д���
    Param:          psHmiProcessRes	������Դ��
    Return:         ���
    Other:          none
*********************************************************/
Int32 HteMsgRecvAndPro(S_HmiProcessRes *psHmiProcessRes)
{	
    static Uint8   recvBuf[512];
    static Uint16  msgLen = 0;
    S_HteMsgHead   *pMsgHead = (S_HteMsgHead *)recvBuf;

    /*����hte��Ϣ*/
    if(HteRecvMsg(psHmiProcessRes->hteComFd,recvBuf, &msgLen) != 1)
    {
        printf("HmiKeyboardMsgPro:recv keyboard msg error.\n");
        return -1;
    }

    /*��֤hte��Ϣ*/
    if(HteRdMsgVerify(recvBuf, msgLen) != 1)
    {
        printf("HmiKeyboardMsgPro:keyboard msg verify error.\n");
        return -1;
    }

#if 0
    /*��ӡ��Ϣ����*/
    int i;
    for(i = 0;i < psHmiProcessRes->recvKeyboardMsgLen;i++)
        printf("%02hhx ",psHmiProcessRes->recvKeyboardMsgBuf[i]);
    printf("key is %02hhx\n",key);

#endif
    printf("HteMsgRecvAndPro:cmd=0x%hhx\n",pMsgHead->cmd);
    switch(pMsgHead->cmd)
    {
        case HTE_PAD_KEY_DOWN:
        {
			PadKeyPressPro(psHmiProcessRes,*(Uint8 *)(pMsgHead + 1));
        }
        break;
        case HTE_PAD_MIC_STATE:
        {
            PadMicStaPro(psHmiProcessRes,*(Int32 *)(pMsgHead + 1));
	    }
        break;
    }
	return 1;
}

/*********************************************************
    Function:       PadKeyPressPro
    Description:    �԰������µ���Ϣ������
    Param:          keyId	�ĸ�����������
    Return:         ���
    Other:          none
*********************************************************/
static void PadKeyPressPro(S_HmiProcessRes *psHmiProcessRes,Uint8 keyId)
{
    static Uint8 displayMode[4] = {0,6,7,8};
    static int mode = 0;    /*��ǰ���õ���ʾģʽ,ȡֵ0-3*/
    static Uint8 bl = 0;    /*��ǰ���õı���ֵ*/
    static Uint8 buzz = 0;        /*��ǰ�ķ�����״̬ 0-�أ�1-��*/
    static Uint8 sendBuf[512];

    int msgLen;

    memset(sendBuf,0,sizeof(sendBuf));
    
    msgLen = PackBuzzCtrlMsg(sendBuf,1);
    HteSendMsg(psHmiProcessRes->hteComFd,sendBuf,msgLen);
    memset(sendBuf,0,sizeof(sendBuf));
	if(KEY_FN == keyId)
	{
        /*FN�����л�����ģʽ*/
        if(++mode > 3)
            mode = 0;
        msgLen = PackModeSwitchMsg(sendBuf,displayMode[mode]);
        HteSendMsg(psHmiProcessRes->hteComFd,sendBuf,msgLen);
	}
    else if(KEY_BAOJING == keyId)
    {
#if 0
        /*�������������Ʒ�����*/
        if(buzz == 1)
            buzz = 0;
        else if(buzz == 0)
            buzz = 1;
        else
            buzz = 0;
        msgLen = PackBuzzCtrlMsg(sendBuf,buzz);
        HteSendMsg(psHmiProcessRes->hteComFd,sendBuf,msgLen);
#endif
    }
	else if(KEY_UP == keyId)
	{
        /*up�������󱳹�����*/
        if(++bl >= 10)
            bl = 10;
        msgLen = PackBacklightSetMsg(sendBuf,bl);
        HteSendMsg(psHmiProcessRes->hteComFd,sendBuf,msgLen);
	}
	else if(KEY_DOWN == keyId)
    {
        /*down�������ͱ�������*/
        if(bl > 0)
        {
            if(--bl == 0)
                bl = 0;
        }
        else
            bl = 0;
        
        msgLen = PackBacklightSetMsg(sendBuf,bl);
        HteSendMsg(psHmiProcessRes->hteComFd,sendBuf,msgLen);
    }   

}

static void PadMicStaPro(S_HmiProcessRes *psHmiProcessRes,Int32 state)
{
    printf("PadMicStaPro:state=%d\n",state);
   // if(state == 1)
    //{        
        //printf("PadMicStaPro:on\n");
        /*����*/
        system("ObuIoClient out 5 1");
        system("ObuIoClient out 4 0");
    //}
#if 0
    else
    {
        printf("PadMicStaPro:off\n");
        system("ObuIoClient out 5 0");
        system("ObuIoClient out 4 1");

    }
#endif
}

static void PackMsgHead(S_HteMsgHead *psMsgHead,Uint8 type,Uint8 cmd,Int32 sn,Int16 msgLen)
{
	psMsgHead->head[0] = 'H';
	psMsgHead->head[1] = 'T';
	psMsgHead->srcDevType = 0x01;
	psMsgHead->srcDevId[0] = 0x00;
	psMsgHead->srcDevId[1] = 0x00;
	psMsgHead->srcDevId[2] = 0x00;
	psMsgHead->dstDevType = 0x05;
	psMsgHead->dstDevId[0] = 0x00;
	psMsgHead->dstDevId[1] = 0x00;
	psMsgHead->dstDevId[2] = 0x00;
	psMsgHead->sn[0] = (sn >> 24) & 0xFF;
	psMsgHead->sn[1] = (sn >> 16) & 0xFF;
	psMsgHead->sn[2] = (sn >> 8) & 0xFF;
	psMsgHead->sn[3] = sn & 0xFF;
	psMsgHead->type = type;
	psMsgHead->cmd = cmd;
	psMsgHead->msgLen[0] = (msgLen >> 8) & 0xFF;
	psMsgHead->msgLen[1] = msgLen & 0xFF;
	psMsgHead->crc[0] = 0x00;
	psMsgHead->crc[1] = 0x00;
#if 0
	int i;
	Uint8 *p = (Uint8 *)psMsgHead;
	printf("PackMsgHead:");
	for(i = 0;i < sizeof(S_MsgHead);i++)
		printf("%02hhx ",p[i]);
	printf("\n");
#endif
}

static Uint16 PadCrcCal(Uint8 *msgBuf,int msgLen)
{
	Uint16 crc = 0;
	int i;
	for(i = 0;i < msgLen;i++)
	{
		crc += msgBuf[i];
	}

	crc ^= 0xA596;
	return crc;
}

static Int32 PackBacklightSetMsg(Uint8 *msgBuf,Uint8 value)
{
	S_HteMsgHead *psMsgHead = (S_HteMsgHead *)msgBuf;
	S_HtePadBlSet *psBacklightSet = (S_HtePadBlSet *)(psMsgHead + 1);
	Int32 msgLen = HTE_MSG_HEAD_LEN + sizeof(S_HtePadBlSet);
	Uint16 crc;

    printf("PackBacklightSetMsg:backlight set to %hhd\n",value);
	PackMsgHead(psMsgHead,HTE_MSG_TYPE_REQ,HTE_PAD_BL_SET,0,msgLen);
	psBacklightSet->blValue = value;

	crc = PadCrcCal(msgBuf,msgLen);
	psMsgHead->crc[0] = (crc >> 8) & 0xFF;
	psMsgHead->crc[1] = crc & 0xFF;

	return msgLen;

}

static Int32 PackModeSwitchMsg(Uint8 *msgBuf,Uint8 mode)
{
	S_HteMsgHead    *psMsgHead = (S_HteMsgHead *)msgBuf;
	S_HtePadModeSet *psModeSet = (S_HtePadModeSet *)(psMsgHead + 1);
	Int32 msgLen = HTE_MSG_HEAD_LEN + sizeof(S_HtePadModeSet);
	Uint16 crc;

    printf("PackModeSwitchMsg: mode set to %hhd\n",mode);
	PackMsgHead(psMsgHead,HTE_MSG_TYPE_REQ,HTE_PAD_MODE_SET,0,msgLen);
	psModeSet->displayMode = mode;

	crc = PadCrcCal(msgBuf,msgLen);
	psMsgHead->crc[0] = (crc >> 8) & 0xFF;
	psMsgHead->crc[1] = crc & 0xFF;

	return msgLen;
}

static Int32 PackBuzzCtrlMsg(Uint8 *msgBuf,Uint8 cmd)
{
	S_HteMsgHead     *psMsgHead = (S_HteMsgHead *)msgBuf;
	S_HtePadBuzCtrl  *psBuzzSet = (S_HtePadBuzCtrl *)(psMsgHead + 1);
	Int32 msgLen = HTE_MSG_HEAD_LEN + sizeof(S_HtePadBuzCtrl);
	Uint16 crc;

    printf("PackBuzzCtrlMsg:buzz set to %hhd\n",cmd);
	PackMsgHead(psMsgHead,HTE_MSG_TYPE_REQ,HTE_PAD_BUZZER_CONTROL,0,msgLen);
	psBuzzSet->cmd = cmd;

	crc = PadCrcCal(msgBuf,msgLen);
	psMsgHead->crc[0] = (crc >> 8) & 0xFF;
	psMsgHead->crc[1] = crc & 0xFF;

	return msgLen;
}





