#include<stdio.h>

struct head
{
	char    srcDevId[3];                           //��ʼ�豸����
    char    dstDevId[3];                           //Ŀ���豸����
    char    sn[4];                                 //��ˮ��
    char    len[2];                                //֡����
    char    crc[2];                                //CRCУ��
    char    cmd;  
};

void main()
{
	struct head h1;
	printf("h1      =%p\n",&h1);
	printf("h1.srcId=%p\n",h1.srcDevId);
	printf("h1.dstId=%p\n",h1.dstDevId);
	printf("h1.sn   =%p\n",h1.sn);
	printf("h1.len  =%p\n",h1.len);
	printf("h1.crc  =%p\n",h1.crc);
	printf("h1.cmd  =%p\n",&h1.cmd);
}