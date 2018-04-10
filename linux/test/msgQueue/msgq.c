/******************************************************************/
/*目的：BicRun经常跑着跑着消息队列堵塞，导致center进程阻塞，想写个*/
/*小程序把堵塞的消息队列中的内容拿出来，看是哪个进程发的，以便定位*/
/*问题。                                                          */
/*结果：方法可行，可以把堵塞的消息内容获取出来分析是哪个进程发送的*/
/******************************************************************/
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdint.h>
#include <strings.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define PC_OAM_KEY     9211

struct S_Msg
{
	long mtype;
	char buf[2048];
};

typedef struct 
{
	uint32_t magicNum;                    /*指定为0xAABBCCDD*/
	uint32_t type;                        /*消息类型*/
	uint32_t cmd;                         /*消息命令*/
	uint32_t sn;                          /*消息流水号号*/
	uint32_t msgLen;                      /*消息总长度（包括消息头）*/
	uint32_t reserved;                    /*保留位*/
}S_PcMsgHead;

typedef struct
{
	char reserved[3];
	char subId;
	S_PcMsgHead sMsgHead;
}S_MsgRcv;

int main(int argc, const char *argv[])
{
	int msgid, ret, i;
	struct S_Msg msg;
	S_MsgRcv *psMsgRcv = (S_MsgRcv *)msg.buf;
	S_PcMsgHead *psMsgHead = &psMsgRcv->sMsgHead;

	if(argc < 2)
	{
		printf("please inpute the key of msgq.\n");
		printf("center:9201  location:9202\n");
		printf("server:9203  audio   :9204\n");
		printf("online:9205  peripheral:9206\n");
		printf("hmi   :9207  station :9208\n");
		printf("ble   :9209  oam     :9212\n");
		printf("lcdshow:9213 routeplay:9214\n");
		exit(-1);
	}
	else if(argc >2)
	{
		printf("too many arguments,exit.");
		exit(-1);
	}
		
	msgid = msgget((key_t)atoi(argv[1]), 0664);
	perror("msgget");
	printf("msgqid = %d\n",msgid);

	while(1)
	{
		bzero(&msg, sizeof(msg));
		ret = msgrcv(msgid, &msg, sizeof(msg.buf), 0, 0);
		perror("msgrcv");

		printf("msglen=%d  mtype=%ld cmd=%d\n",ret, msg.mtype,psMsgHead->cmd);
		for(i = 0;i < ret;i++)
			printf("%02x ",msg.buf[i]);
		printf("\n\n");
		
		sleep(1);
		
		
	}

	return 0;
}
