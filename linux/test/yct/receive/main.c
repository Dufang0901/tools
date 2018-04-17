#include "yct.h"

/*�����ͨ�ն�ͨ�ŵĲ���*/
static int 					yctComFd;
static Uint8				yctRecvBuf[YCT_BUF_LEN_MAX];
static int					yctRecvLen;
static Uint8				yctSendBuf[YCT_BUF_LEN_MAX];
static int					yctSendLen;


/***********************************************************************
    Function:       CenterYctFdPro
    Description:    �������Ĵ�����յ������ͨ�ն���Ϣ
    Param:          none
    Return:         none
    Other:          none
***********************************************************************/
void CenterYctFdPro(void)
{
	int ret = 0;

	/*���յ���Ϣ*/
	if(YctRecvMsg(yctComFd,yctRecvBuf,&yctRecvLen) == YCT_OK)
	{	
		if(YctRdMsgVerify(yctRecvBuf,yctRecvLen) == YCT_OK)
		{
			ret = YctRdMsgPro(yctRecvBuf,yctRecvLen,yctSendBuf,&yctSendLen);
			if(YCT_PRO_REPLY == ret)
			{
				YctSendMsg(yctComFd,yctSendBuf,yctSendLen);
			}
		}
		else
		{
			printf("CenterYctFdPro: Yct message verify error.\n");
		}
		
	}
	else
	{
		printf("CenterYctFdPro: Yct receive message error.\n");
		YctComEmptyBuf(yctComFd);
	}


}


void main()
{
	int maxFd, ret;
	fd_set fds;

	yctComFd = YctComOpenCfg("/dev/ttymxc1");
	if(yctComFd <= 0)
	{
		printf("yct Open failed.\n");
		exit(-1);
	}

	printf("yct Open Successful.\n");
	YctParamInit();
	while(1)
	{
		FD_ZERO(&fds);
		FD_SET(yctComFd,&fds);
		maxFd = yctComFd + 1;

		ret = select(maxFd,&fds,NULL,NULL,NULL);
		if(0 == ret)
			continue;
		if(FD_ISSET(yctComFd,&fds))
		{
			CenterYctFdPro();
		}
	}
	
	
	
}
