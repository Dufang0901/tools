/*
*���������ڲ���BICͨ��wifi�������������ͨѶ
*/
#include "CardDev.h"
#include "Mytype.h"
int main()
{
	printf("%d\n",__LINE__);
	Char msg[128];
	int i;
	//��ʼ������������ͨ�ŵ�socket
	CardDevInit();
	printf("%d\n",__LINE__);
	for(i = 0;i < 10;i++)
		msg[i] = i + '0';
	printf("%d\n",__LINE__);
	while(1)
	{
		printf("%d\n",__LINE__);
		if(CardDevSendMsg(msg, strlen(msg)) == CARD_DEV_OK)
		{
			
			printf("msg sent successfully.\n");

		}	
		else
			printf("msg sent failed.\n");
		printf("%d\n",__LINE__);
		sleep(2);
	}
	
}
