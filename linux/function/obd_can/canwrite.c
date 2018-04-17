#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
int main()
{
	int sock_fd;
	unsigned long nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame frame;

	/*1 �����׽��֣�����Ϊԭʼ�׽��֣�ԭʼCANЭ�� */
	sock_fd = socket(PF_CAN,SOCK_RAW,CAN_RAW);

	/*2 ��CAN�ӿڽ��г�ʼ����������CAN�ӿ���������������ifconfig����ʱ��ʾ������ */
	strcpy((char *)(ifr.ifr_name), "can0");
	ioctl(sock_fd, SIOCGIFINDEX, &ifr);
	printf("can0 can_ifindex = %x\n", ifr.ifr_ifindex);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	/*3 �������ɵ��׽�����CAN�׽��ֵ�ַ���а�*/
	bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr));

	/*����CAN֡��ID�ţ�������Ϊ��׼֡����չ֡��ID��*/
	frame.can_id = 0x18412345;
	strcpy((char *)frame.data,"hello");
	frame.can_dlc = strlen(frame.data);
	printf("Send a CAN frame from interface %s\n", ifr.ifr_name);

	/*��ʼ��������*/
	nbytes = sendto(sock_fd, &frame, sizeof(struct can_frame), 0, (struct sockaddr*)&addr, sizeof(addr));

	return 0;

}