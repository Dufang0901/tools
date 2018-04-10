#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/timerfd.h>
#include <sys/select.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include "Mytype.h"
#include "pad2Test.h"

static int sn;
static int blSetFlag;
static int blSetWaitCnt;
static int blsetReplyFlag;
static int modeSetFlag;
static int modeSetWaitCnt;
static int modeSetReplyFlag;
static int padInteractCnt;
static int padStaWriteCnt;
static int padStaFileUploadCnt;
static int padInteractFlag;
static int padBlValue;
static int padModeValue;
static int cmd;

#define LOG_FILE_NAME_SUFFIX "Factory"



int TimerInit(void)
{
    struct itimerspec newValue;
    struct timespec now;
    int fd;
    
    if (clock_gettime(CLOCK_MONOTONIC , &now) == -1)
    {
        perror("Server process clock_gettime");
        return -1;
    }
    
    newValue.it_value.tv_sec = now.tv_sec + 1;
    newValue.it_value.tv_nsec = now.tv_nsec;
    newValue.it_interval.tv_sec = 1;
    newValue.it_interval.tv_nsec = 0;
    
    fd = timerfd_create(CLOCK_MONOTONIC , 0);
    
    if (timerfd_settime(fd, TFD_TIMER_ABSTIME, &newValue, NULL) == -1)
    {
        perror("Server process timerfd_settime");
        return -1;
    }
    
    return fd;
}

int CanBusInit(const char *canName)
{
	int sock_fd;
	struct ifreq ifr;
	struct sockaddr_can addr;
	
	if(strcmp(canName,"can0") != 0 && strcmp(canName,"can1") != 0)
		return -1;
	
	/*1 建立套接字，设置为原始套接字，原始CAN协议 */
	sock_fd = socket(PF_CAN,SOCK_RAW,CAN_RAW);

	/*2 对CAN接口进行初始化，如设置CAN接口名，即当我们用ifconfig命令时显示的名字 */
	strcpy((char *)(ifr.ifr_name), canName);
	ioctl(sock_fd, SIOCGIFINDEX, &ifr);
	printf("can0 can_ifindex = %x\n", ifr.ifr_ifindex);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	/*3 将刚生成的套接字与CAN套接字地址进行绑定*/
	bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr));
	return sock_fd;
}

void PackMsgHead(S_MsgHead *psMsgHead,Uint8 type,Uint8 cmd,Int32 sn,Int16 msgLen)
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

Uint16 PadCrcCal(Uint8 *msgBuf,int msgLen)
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

Int32 PackBacklightSetMsg(Uint8 *msgBuf,Uint8 value)
{
	S_MsgHead *psMsgHead = (S_MsgHead *)msgBuf;
	S_BackLight *psBacklightSet = (S_BackLight *)(psMsgHead + 1);
	Int32 msgLen = PAD_MSG_HEAD_LEN + sizeof(S_BackLight);
	Uint16 crc;

	PackMsgHead(psMsgHead,PAD_MSG_REQ,PAD_CMD_BL,sn++,msgLen);
	psBacklightSet->backlight = value;

	crc = PadCrcCal(msgBuf,msgLen);
	psMsgHead->crc[0] = (crc >> 8) & 0xFF;
	psMsgHead->crc[1] = crc & 0xFF;

	return msgLen;

}

Int32 PackModeSwitchMsg(Uint8 *msgBuf,Uint8 mode)
{
	S_MsgHead *psMsgHead = (S_MsgHead *)msgBuf;
	S_SwitchMode *psModeSet = (S_SwitchMode *)(psMsgHead + 1);
	Int32 msgLen = PAD_MSG_HEAD_LEN + sizeof(S_SwitchMode);
	Uint16 crc;

	PackMsgHead(psMsgHead,PAD_MSG_REQ,PAD_CMD_MODE,sn++,msgLen);
	psModeSet->displayMode = mode;

	crc = PadCrcCal(msgBuf,msgLen);
	psMsgHead->crc[0] = (crc >> 8) & 0xFF;
	psMsgHead->crc[1] = crc & 0xFF;

	return msgLen;
}

Int32 CanSendMsg(Int32 sock_fd,Uint8 *msgBuf,Int32 msgLen)
{
	/*注意:一帧can数据只能发送8个字节的用户数据*/
	ssize_t nbytes;
	struct can_frame frame;
	int sendLen,sentLen,i;
	Uint8 *head;

	frame.can_id = 0x18400001 | CAN_EFF_FLAG ;
	
	sendLen = msgLen;
	head = msgBuf;

	printf("Send %d bytes through can0:",msgLen);

	while(sendLen > 0)
	{
		/*封装can数据帧*/
		if(sendLen >= 8)
		{
			frame.can_dlc = 8;
			memcpy(frame.data,head,8);

			sendLen -= 8;
			sentLen = 8;
		}
		else
		{
			sentLen = sendLen;
			frame.can_dlc = sendLen;
			memcpy(frame.data,head,sendLen);
			sendLen = 0;
		}

		/*发送can数据帧*/
		nbytes = write(sock_fd, (void *)&frame, sizeof(frame));
		if(nbytes < 0)
		{
			printf("can write failed:%s\n",strerror(errno));;
			return nbytes;
		}
		else if(nbytes != sizeof(struct can_frame))
		{
			printf("can write failed,nbytes=%d\n",nbytes);
			return 0;
		}
		else
		{
			for(i = 0;i < sentLen;i++)
				printf("%02hhx ",head[i]);
			head += sentLen;
		}

		
	}
	printf("\n");
	return msgLen;
	

}

Int32 CanRecvMsg(Int32 sock_fd,Uint8 *msgBuf)
{
	struct can_frame frame;
	S_MsgHead   *psMsgHead = (S_MsgHead*)msgBuf;
	int readLen,msgLen;
	Uint8 *head;

	/*开始读取数据-消息头需要3帧才读完,frame.id:can的ID,frame.can_dlc:can数据长度,frame.data:can数据内容*/
	readLen = msgLen = 0;
	head = msgBuf;

	/*读取消息头的第一帧*/
	if(read(sock_fd,&frame,sizeof(frame)) < 0)
		return -1;

	/*判断是否是第一帧数据*/
	if(frame.data[0] != HTE_MSG_HEAD0 || frame.data[1] != HTE_MSG_HEAD1)
	{
		printf("############## msg magic number error,return.\n");
		return -2;
	}
	memcpy(head,frame.data,frame.can_dlc);
	readLen += frame.can_dlc;
	head += frame.can_dlc;

	/*读取消息头的第二帧*/
	if (read(sock_fd,&frame,sizeof(frame)) < 0)
	{
		printf("############## read second frame of head failed.\n");
		return -1;
	}

	memcpy(head,frame.data,frame.can_dlc);
	readLen += frame.can_dlc;
	head += frame.can_dlc;

	/*读取消息头的第三帧*/
	if (read(sock_fd,&frame,sizeof(frame)) < 0)
	{
		printf("############## read third frame of head failed.\n");
		return -1;
	}

	memcpy(head,frame.data,frame.can_dlc);
	readLen += frame.can_dlc;
	head += frame.can_dlc;

	/*判断读取三帧数据后是否读到了足够的数据*/
	if(readLen < 20)
	{
		printf("read msg head error,return.\n");
		return -1;
	}

	msgLen = (psMsgHead->msgLen[0] << 8) | psMsgHead->msgLen[1];
#if 0
	/*打印接收到的前3帧的数据*/
	int i;
	for(i = 0;i < readLen;i++)
		printf("%02hhx ",msgBuf[i]);
	printf("\n");
#endif	
	//根据消息头的里面的长度,读取剩余的帧数
	while(readLen < msgLen)
	{
		ssize_t ret;
		
		ret = read(sock_fd,&frame,sizeof(frame));
		printf("ret = %d\n",ret);
		if(ret < 0)
		{
			return -1;
		}
		memcpy(head,frame.data,frame.can_dlc);
		readLen += frame.can_dlc;
		head += frame.can_dlc;
	}

	return readLen;

}

Int32 CanMsgVerify(Uint8 *msgBuf,Int32 msgLen)
{
	S_MsgHead *psMsgHead = (S_MsgHead *)msgBuf;
	Int32 len = (psMsgHead->msgLen[0] << 8) | psMsgHead->msgLen[1];
	Uint16 crc;

	if(len != msgLen)
	{
		printf("CanMsgVerify:msgLen error.\n");
		return 0;
	}

	crc = (psMsgHead->crc[0] << 8) | psMsgHead->crc[1];
	psMsgHead->crc[0] = psMsgHead->crc[1] = 0x00;

	if(crc != PadCrcCal(msgBuf,msgLen))
	{
		printf("CanMsgVerify:msg crc error.\n");
		return 0;
	}

	return 1;
	
}

void CanRecvMsgPro(Uint8 *msgBuf,Int32 msgLen,S_PadStaMonit *psPadState)
{
#if 0
	int i;
	printf("CanRecvMsgPro(%d bytes):",msgLen);
	for(i = 0;i < msgLen;i++)
		printf("%02hhx ",msgBuf[i]);
	printf("\n");
#endif
	time_t timet;
	time(&timet);

	S_MsgHead *psMsgHead = (S_MsgHead *)msgBuf;
	if(psMsgHead->cmd == PAD_CMD_BL)
	{
		printf("CanRecvMsgPro:receive pad backlight set reply.\n");
		psPadState->blSetSuccessCnt++;
		blSetFlag = 0;
		localtime_r(&timet,&psPadState->sLastConnectTime);
	}
	else if(psMsgHead->cmd == PAD_CMD_MODE)
	{
		printf("CanRecvMsgPro:receive pad mode set reply.\n");
		psPadState->modeSetSuccessCnt++;
		modeSetFlag = 0;
		localtime_r(&timet,&psPadState->sLastConnectTime);
	}
	else if(psMsgHead->cmd == PAD_CMD_TS_DOWN)
	{
		printf("CanRecvMsgPro:receive touch screen down.\n");
		psPadState->touchCnt++;
		localtime_r(&timet,&psPadState->sLastConnectTime);
	}
}
Int32 PadStaRdRromFile(S_PadStaMonit *psPadState)
{
	time_t timet;
	struct tm sTime;
	S_CommonTime sNowTime;
	Char fileName[50] = {0};
	Char lineStr[50];
	FILE *fp;
	
	/*1 获取当前时间*/
	time(&timet);
	localtime_r(&timet,&sTime);
	sNowTime.year = sTime.tm_year + 1900;
	sNowTime.month = sTime.tm_mon + 1;
	sNowTime.day = sTime.tm_mday;

	sprintf(fileName,"/home/bic/Release/data/padState%s-%04d-%02d-%02d",LOG_FILE_NAME_SUFFIX,sNowTime.year,sNowTime.month,sNowTime.day);
	fp = fopen(fileName,"r");
	if(fp == NULL)
	{
		printf("open %s failed:%s\n",fileName,strerror(errno));
		return 0;
	}
	
	/*获取时间*/
	if(fgets(lineStr,50,fp) != NULL)
	{
		if(strncmp(lineStr,"Date:",5) == 0)
			sscanf(lineStr,"Date:%04d-%02d-%02d\n",&psPadState->sTime.year,&psPadState->sTime.month,&psPadState->sTime.day);
		else
		{
			fclose(fp);
			printf("********** %s:%d\n",__func__,__LINE__);
			return 0;
		}
	}
	else
	{
		fclose(fp);
		printf("********** %s:%d\n",__func__,__LINE__);
		return 0;
	}
	
	/*获取背光设置信息*/
	if(fgets(lineStr,50,fp) != NULL)
	{
		if(strstr(lineStr,"背光设置:") != NULL)
			sscanf(lineStr,"背光设置:发送%d次,发送成功%d次,设置成功%d次\n"
			,&psPadState->blSendCnt,&psPadState->blSendSuccessCnt,&psPadState->blSetSuccessCnt);
		else
		{
			fclose(fp);
			printf("********** %s:%d\n",__func__,__LINE__);
			return 0;
		}
	}
	else
	{
		fclose(fp);
		printf("********** %s:%d\n",__func__,__LINE__);
		return 0;
	}

	/*获取模式设置信息*/
	if(fgets(lineStr,50,fp) != NULL)
	{
		if(strstr(lineStr,"模式切换:") != NULL)
			sscanf(lineStr,"模式切换:发送%d次,发送成功%d次,设置成功%d次\n"
			,&psPadState->modeSendCnt,&psPadState->modeSendSuccessCnt,&psPadState->modeSetSuccessCnt);
		else
		{
			fclose(fp);
			printf("********** %s:%d\n",__func__,__LINE__);
			return 0;
		}
	}
	else
	{
		fclose(fp);
		printf("********** %s:%d\n",__func__,__LINE__);
		return 0;
	}

	/*获取触摸屏点击信息*/
	if(fgets(lineStr,50,fp) != NULL)
	{
		if(strstr(lineStr,"触摸屏点击:") != NULL)
			sscanf(lineStr,"触摸屏点击:%d次\n",&psPadState->touchCnt);
		else
		{
			fclose(fp);
			printf("********** %s:%d\n",__func__,__LINE__);
			return 0;
		}
	}
	else
	{
		fclose(fp);
		printf("********** %s:%d\n",__func__,__LINE__);
		return 0;
	}

	fclose(fp);
	return 1;
	
}

Int32 PadStaWriteToFile(S_PadStaMonit *psPadState)
{
	/*文件:/home/bic/Release/data/padState-yyyy-mm-dd
	  格式:Date:yyyy-mm-dd
		   背光设置:成功xx次,失败xx次
		   模式切换:成功xx次,失败xx次
		   触摸屏点击:xx次
	*/
	
	Char fileName[50];
	FILE *fp;
	
	sprintf(fileName,"/home/bic/Release/data/padState%s-%04d-%02d-%02d",LOG_FILE_NAME_SUFFIX,psPadState->sTime.year,psPadState->sTime.month,psPadState->sTime.day);
	fp = fopen(fileName,"w");
	if(fp == NULL)
		return -1;
	fprintf(fp,"Date:%04d-%02d-%02d\n",psPadState->sTime.year,psPadState->sTime.month,psPadState->sTime.day);
	fprintf(fp,"背光设置:发送%d次,发送成功%d次,设置成功%d次\n"
		,psPadState->blSendCnt,psPadState->blSendSuccessCnt,psPadState->blSetSuccessCnt);
	fprintf(fp,"模式切换:发送%d次,发送成功%d次,设置成功%d次\n"
		,psPadState->modeSendCnt,psPadState->modeSendSuccessCnt,psPadState->modeSetSuccessCnt);
	fprintf(fp,"触摸屏点击:%d次\n",psPadState->touchCnt);
	fprintf(fp,"最后一次通信成功时间:%04d-%02d-%02d %02d:%02d:%02d\n",psPadState->sLastConnectTime.tm_year + 1900
		,psPadState->sLastConnectTime.tm_mon + 1,psPadState->sLastConnectTime.tm_mday,psPadState->sLastConnectTime.tm_hour
		,psPadState->sLastConnectTime.tm_min,psPadState->sLastConnectTime.tm_sec);
	fclose(fp);
	sync();
	return 1;
}
void PadStateFileTimerUpload()
{
	time_t timet;
	struct tm sTime;
	S_CommonTime sNowTime,sFileTime;
	struct dirent **namelist;
    int n,ret,uploadCnt;
    Char cmd[256] = {0};
	Char fileNameTemp[50];

	uploadCnt = 0;
	
	/*1 获取当前时间*/
	time(&timet);
	localtime_r(&timet,&sTime);
	sNowTime.year = sTime.tm_year + 1900;
	sNowTime.month = sTime.tm_mon + 1;
	sNowTime.day = sTime.tm_mday;
	
	/*2 获取所有padState-yyyy-mm-dd文件*/
	n = scandir(BIC_DATA_DIR,&namelist,0,alphasort);
	if(n < 0)
	{
		printf("no pad state file is found in %s.\n");
		return;
	}

	while(n--)
	{
		if(strncmp(namelist[n]->d_name,"padState",8) == 0)
		{
			sscanf(namelist[n]->d_name,"padState%[^-]-%04d-%02d-%02d",fileNameTemp,&sFileTime.year,&sFileTime.month,&sFileTime.day);

			/*比较时间*/
			if(sFileTime.year != sNowTime.year || sFileTime.month != sNowTime.month || sFileTime.day != sNowTime.day)
			{
				/*上传到服务器*/
				/*ftpput -u root -p esine 115.29.16.25 /home/bic/zjxc/bicFtp/padState-yyyy-mm-dd padState-yyyy-mm-dd */
				sprintf(cmd,"ftpput -u root -p esine 115.29.16.25 /home/bic/zjxc/bicFtp/%s /home/bic/Release/data/%s",namelist[n]->d_name,namelist[n]->d_name);
				printf("%s\n",cmd);
				ret = system(cmd);
				if(ret == 0)
				{
					/*上传成功后删除*/
					sprintf(cmd,"rm %s/%s",BIC_DATA_DIR,namelist[n]->d_name);
					uploadCnt++;
					system(cmd);
				}

			}
			
		}

	}
	
	if(uploadCnt > 0)
		printf("PadStateFileTimerUpload:successfully uploaded %d files to Ftp server.\n",uploadCnt);
	else
		printf("PadStateFileTimerUpload:no file needs to be uploaded.\n");


}

void PadStateTimerPro(int sock_fd,S_PadStaMonit *psPadState)
{
	time_t timet;
	struct tm sTime;
	int msgLen;
	Uint8 msg[50];
	Int32 ret;

	padInteractCnt++;
	padStaWriteCnt++;
	padStaFileUploadCnt++;

	//printf("interactCnt=%d writeCnt=%d uploadCnt=%d\n",padInteractCnt,padStaWriteCnt,padStaFileUploadCnt);

	/*更新时间*/
	time(&timet);
	localtime_r(&timet,&sTime);
	psPadState->sTime.year  = sTime.tm_year + 1900;
	psPadState->sTime.month = sTime.tm_mon + 1;
	psPadState->sTime.day   = sTime.tm_mday;

	/*判断背光设置结果*/
	if(blSetFlag >= 1)
		blSetFlag++;
	if(blSetFlag >= 6)
	{
		blSetFlag = 0;
	}
	
	/*判断模式设置结果*/
	if(modeSetFlag >= 1)
		modeSetFlag++;
	if(modeSetFlag >= 6)
	{
		modeSetFlag = 0;
	}
		
	if(padInteractCnt >= PAD_INTERACT_TIME)
	{
		padInteractCnt = 0;
		/*与pad进行一次交互*/
		if(padInteractFlag % 2 == 0)
		{
			if(cmd & 0x01)
			{
				//设置背光
				msgLen = PackBacklightSetMsg(msg,padBlValue);
				ret = CanSendMsg(sock_fd,msg,msgLen);
				psPadState->blSendCnt++;
				if(ret == msgLen)
				{
					psPadState->blSendSuccessCnt++;
					blSetFlag = 1;
				}


				if(++padBlValue > 10)
					padBlValue = 0;
			}

		}
		else
		{
			if(cmd & 0x02)
			{
				//设置模式
				msgLen = PackModeSwitchMsg(msg,padModeValue);
				ret = CanSendMsg(sock_fd,msg,msgLen);
				psPadState->modeSendCnt++;
				if(ret == msgLen)
				{
					psPadState->modeSendSuccessCnt++;
					modeSetFlag = 1;
				}

				if(++padModeValue > 3)
					padModeValue = 0;
			}
		}

		padInteractFlag++;
	}

	if(padStaWriteCnt >= PAD_STATE_WRITE_FILE_TIME)
	{
		padStaWriteCnt = 0;
		/*更新一次padState文件*/
		S_PadStaMonit sPadState;
		if(PadStaRdRromFile(&sPadState))
		{
			psPadState->blSendCnt += sPadState.blSendCnt;
			psPadState->blSendSuccessCnt   += sPadState.blSendSuccessCnt;
			psPadState->blSetSuccessCnt    += sPadState.blSetSuccessCnt;
			psPadState->modeSendCnt        += sPadState.modeSendCnt;
			psPadState->modeSendSuccessCnt += sPadState.modeSendSuccessCnt;
			psPadState->modeSetSuccessCnt  += sPadState.modeSetSuccessCnt;
			psPadState->touchCnt          += sPadState.touchCnt;
			printf("############# read pad state info from file successful:\n");
			printf("date:%04d-%02d-%02d\n",sPadState.sTime.year,sPadState.sTime.month,sPadState.sTime.day);
			printf("blSendCnt=%d blSendSuccessCnt=%d blSetSuccessCnt=%d modeSendCnt=%d modeSendSuccessCnt=%d modeSetSuccessCnt=%d touchCnt=%d\n"
				,sPadState.blSendCnt,sPadState.blSendSuccessCnt,sPadState.blSetSuccessCnt
				,sPadState.modeSendCnt,sPadState.modeSendSuccessCnt,sPadState.modeSetSuccessCnt
				,sPadState.touchCnt);
		}
		else
			printf("read padState info from file failed.\n");

		PadStaWriteToFile(psPadState);
		printf("############# write pad state info to file.\n");
		memset(psPadState,0,sizeof(S_PadStaMonit));
		printf("write padState info to file.\n");
		
	}

	if(padStaFileUploadCnt >= PAD_STATE_FILE_UPLOAD_TIME)
	{
		padStaFileUploadCnt = 0;
		if(cmd & 0x04)
		{
			/*上传一次padState文件*/
			PadStateFileTimerUpload();
		}
	}
	
	
}

void TimerPro(int timerFd,int sock_fd,S_PadStaMonit *psPadState)
{
	Uint64  timerCnt;
    if(read(timerFd, &timerCnt, sizeof(Uint64)) == sizeof(Uint64))
    {
#if 0
		static int sendFlag = 0;
		Uint8 msg[24];
		Int32 msgLen;

    	if(!sendFlag)
    	{
    		
    		printf("change backlight to 0\n");
    		sendFlag = 1;
	    	msgLen = PackBacklightSetMsg(msg,0);
			CanSendMsg(sock_fd,msg,msgLen);
			printf("\n");

			printf("switch mode to AV1\n");
			msgLen = PackModeSwitchMsg(msg,1);
			CanSendMsg(sock_fd,msg,msgLen);
			printf("\n");
    	}
#else
	PadStateTimerPro(sock_fd,psPadState);
#endif
    }
}

void * padTestVideoPlayThread(void * arg)
{
	char cmd[128];
	
	memset(cmd,0,sizeof(cmd));
	
	sprintf(cmd,"gplay /home/bic/Release/video/padTestVideo.avi");
	
	while(1)
		system(cmd);
}

void PrintHelpInfo()
{
	printf("--set pad backlight:\n");
	printf("      pad2Test bl=n         n=0-10\n");
	printf("--set pad input mode:\n");
	printf("      pad2Test mode=n       n=0-3; 0-VGA 1-AV1 2-AV2 3-AV3\n");
	printf("--auto test:\n");
	printf("      pad2Test cmd=n\n");
	printf("             n= a | (b<<1) | (c<<2);a-set backlight? b-set mode? c-upload result?\n");
}

int main(int argc, char *argv[])
{
	int sock_fd;
	
	/*初始化can接口*/
	sock_fd = CanBusInit("can0");
	if(sock_fd <= 0)
	{
		printf("initializing can interface failed,return.\n");
		return -1;
	}
	
	/*增加对命令的解析和处理*/
	char *head;
	char canMsg[50];
	int  value,ret,msgLen;
	
	if(1 == argc || (argv[1] != NULL && strncmp(argv[1],"help",4) == 0))
	{
		PrintHelpInfo();
		return 1;
	}
	
	if(2 == argc && argv[1] != NULL)
	{
		if(strncmp(argv[1],"bl",2) == 0)
		{
			if((head = strchr(argv[1],'=')) != NULL)
			{
				value = atoi(head + 1);
				if(value >= 0 && value <= 10)
				{
					/*设置背光*/
					msgLen = PackBacklightSetMsg(canMsg,value);
					ret = CanSendMsg(sock_fd,canMsg,msgLen);
					if(ret == msgLen)
						printf("send backlight set command successful.\n");
					else
						printf("send backlight set command error.\n");
				}
				else
				{
					printf("backlight value error.\n");
				}
			}
			else
			{
				printf("input command error.\n\n");
				PrintHelpInfo();
			}
			return 1;
		}
		else if(strncmp(argv[1],"mode",4) == 0)
		{
			if((head = strchr(argv[1],'=')) != NULL)
			{
				value = atoi(head + 1);
				if(value >= 0 && value <= 3)
				{
					/*设置背光*/
					msgLen = PackModeSwitchMsg(canMsg,value);
					ret = CanSendMsg(sock_fd,canMsg,msgLen);
					if(ret == msgLen)
						printf("send mode set command successful.\n");
					else
						printf("send mode set command error.\n");
				}
				else
				{
					printf("mode value error.\n");
				}
			}
			else
			{
				printf("input command error.\n\n");
				PrintHelpInfo();
			}
			return 1;
		}
		else if(strncmp(argv[1],"cmd",3) == 0)
		{
			if((head = strchr(argv[1],'=')) != NULL)
			{
				cmd = atoi(head + 1);
				printf("cmd = %d\n",cmd);
			}
			else
			{
				printf("input command error.\n\n");
				PrintHelpInfo();
			}
		}
	}
	/************************/

	int timerFd,maxFd;
	fd_set fds;
	Uint8 recvBuf[128];
	Int32 recvLen;
	S_PadStaMonit sPadState;
	memset(&sPadState,0,sizeof(sPadState));

	/*初始化定时器*/
	timerFd = TimerInit();

	printf("init finished, sock_fd=%d timerFd = %d\n",sock_fd,timerFd);

	//PadStateFileTimerUpload();
	
	/*启动视频循环播放线程*/
	pthread_t videoPlayThreadId;
	pthread_create(&videoPlayThreadId,NULL,padTestVideoPlayThread,NULL);

	while(1)
	{
		/*初始化select*/
		FD_ZERO(&fds);
        FD_SET(timerFd,&fds);
        FD_SET(sock_fd,&fds);
        
        maxFd = timerFd > sock_fd ? timerFd : sock_fd;
        maxFd += 1;
        
        Again:
        ret = select(maxFd,&fds,NULL,NULL,NULL);
		if(-1 == ret)
        {
            if(EINTR == errno)
            {
                goto  Again;
            }
        }
        
        if(0 == ret)
        {
            //超时
            continue;
        }
        
        if(FD_ISSET(timerFd,&fds))
        {
            /*定时1秒的处理*/
            TimerPro(timerFd,sock_fd,&sPadState);
            continue;
        }
		if(FD_ISSET(sock_fd,&fds))
		{
			printf("########## receive msg from can\n");
			
			if((recvLen = CanRecvMsg(sock_fd,recvBuf)) < 0)
			{
				printf("can msg receive failed.\n");
				continue;
			}
			
			if(!CanMsgVerify(recvBuf,recvLen))
			{
				printf("can msg verify failed.\n");
				continue;
			}

			CanRecvMsgPro(recvBuf,recvLen,&sPadState);
		}
	}

	


	return;

}
