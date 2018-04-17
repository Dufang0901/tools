#include "yct.h"

/*与羊城通终端通信的参数*/
static int 					yctComFd;
static Uint8				yctRecvBuf[YCT_BUF_LEN_MAX];
static int					yctRecvLen;
static Uint8				yctSendBuf[YCT_BUF_LEN_MAX];
static int					yctSendLen;


/***********************************************************************
    Function:       CenterYctFdPro
    Description:    控制中心处理接收到的羊城通终端信息
    Param:          none
    Return:         none
    Other:          none
***********************************************************************/
void CenterYctFdPro(void)
{
	int ret = 0;
	/*接收到消息*/
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
	int maxFd, cmd;

	yctComFd = YctComOpenCfg("/dev/ttymxc1");
	if(yctComFd <= 0)
	{
		printf("yct Open failed.\n");
		exit(-1);
	}

	printf("yct Open Successful.\n");
	YctParamInit();
	
	/*发送键盘上电提醒*/
	YctKeyboardUpNoticePack(yctSendBuf,&yctSendLen);
	if(YctSendMsg(yctComFd,yctSendBuf,yctSendLen) != YCT_OK)
	{
		printf("YCT send keyboard up notice failed.\n");
	}
	else
		printf("YCT send keyboard up notice successful.\n");
	while(1)
	{
		printf("input a command:");
		scanf("%x",&cmd);
		getchar();
		printf("command is: %x\n",cmd);

		switch(cmd)
		{
			case YCT_TYPE_STAT_QUERY:
			{
				YctStaQueryPack(yctSendBuf,&yctSendLen);
				if(YctSendMsg(yctComFd,yctSendBuf,yctSendLen) != YCT_OK)
				{
					printf("YCT send status query failed.\n");
				}
				else
					printf("YCT send status query successful.\n");
			}
			break;
			case YCT_TYPE_TIME_ADJUST:
			{
				YctTimePack(yctSendBuf,&yctSendLen);
				if(YctSendMsg(yctComFd,yctSendBuf,yctSendLen) != YCT_OK)
				{
					printf("YCT send time adjust failed.\n");
				}
				else
					printf("YCT send time adjust successful.\n");
			}
			break;
			case YCT_TYPE_INFO_QUERY:
			{
				Uint8 type;
				
				printf("input query type:");
				scanf("%x",&type);
				getchar();
				if(type != 0x01 && type != 0x02 && type != 0x03 && type != 0x04)
				{
					printf("query type wrong, start over!\n");
					continue;
				}
				YctInfoQueryPack(yctSendBuf,&yctSendLen,type);
				if(YctSendMsg(yctComFd,yctSendBuf,yctSendLen) != YCT_OK)
				{
					printf("YCT send info query failed.\n");
				}
				else
					printf("YCT send info query successful.\n");
			}
			break;
			case YCT_TYPE_SET_INFO:
			{
				Uint8 type;
				printf("input set type:");
				scanf("%x",&type);
				getchar();
				switch(type)
				{
					case YCT_SET_DIRECTION:
					{
						Uint8 direction;
						printf("input set value:");
						Uint8 backlight;
						scanf("%x",&direction);
						getchar();
						YctSetMsgPack(yctSendBuf,&yctSendLen,YCT_SET_DIRECTION,&direction);
					}
					break;
					case YCT_SET_STA_ID:
					{
						Char staId[2];
						printf("input set value:");
						Uint8 backlight;
						scanf("%c%c",&staId[0],&staId[1]);
						getchar();
						YctSetMsgPack(yctSendBuf,&yctSendLen,YCT_SET_STA_ID,staId);
					}
					break;
					case YCT_SET_SEGMENT_POINT:
					{
					}
					break;
					case YCT_SET_BACKLIGHT:
					{
						printf("input set value:");
						Uint8 backlight;
						scanf("%x",&backlight);
						getchar();
						YctSetMsgPack(yctSendBuf,&yctSendLen,YCT_SET_BACKLIGHT,&backlight);
					}
					break;
					case YCT_SET_TIME:
					{
					}
					break;
					default:
					break;
					
				}

				if(YctSendMsg(yctComFd,yctSendBuf,yctSendLen) != YCT_OK)
				{
					printf("YCT send set request failed.\n");
				}
				else
					printf("YCT send set request successful.\n");
			}
			break;
			case YCT_TYPE_KEYBOARD_UP:
			{
				YctKeyboardUpNoticePack(yctSendBuf,&yctSendLen);
				if(YctSendMsg(yctComFd,yctSendBuf,yctSendLen) != YCT_OK)
				{
					printf("YCT send keyboard up notice failed.\n");
				}
				else
					printf("YCT send keyboard up notice successful.\n");
			}
			break;
			default:
				printf("unknown command.\n");
			break;
			

		}
	}
	
	
	
}
