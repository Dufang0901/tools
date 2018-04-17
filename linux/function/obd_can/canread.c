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
	unsigned long nbytes, len;
	struct sockaddr_can addr,readAddr;
	struct ifreq ifr;

	/*Ϊ���ܹ�����CAN���ģ�������Ҫ����һ��CAN���ݸ�ʽ�Ľṹ�����*/
	struct can_frame frame;
	struct can_frame *ptr_frame;

	/*�����׽��֣�����Ϊԭʼ�׽��֣�ԭʼCANЭ�� */
	sock_fd = socket(PF_CAN,SOCK_RAW,CAN_RAW);

	/*�����Ƕ�CAN�ӿڽ��г�ʼ����������CAN�ӿ���������������ifconfig����ʱ��ʾ������ */
	strcpy(ifr.ifr_name,"can1");
	ioctl(sock_fd, SIOCGIFINDEX, &ifr);
	printf("can1 can_ifindex = %x\n",ifr.ifr_ifindex);

	/*����CANЭ�� */
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	/*�������ɵ��׽����������ַ���а�*/
	bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr));

	struct timeval tv;
    fd_set readfds;
    tv.tv_sec = 10;
    tv.tv_usec = 0;
    FD_ZERO(&readfds);
    FD_SET(sock_fd, &readfds);
    int res = select(sock_fd+1,&readfds,NULL, NULL, &tv);
    if(res < 0) {
		perror("select");
	} else if (res == 0) {
		printf("timeout!there is no data arrived!\n");
	} else {
		if(FD_ISSET(sock_fd,&readfds)) {
			/*��ʼ��������*/
			nbytes = recvfrom(sock_fd, &frame, sizeof(struct can_frame), 0, (struct sockaddr *)&readAddr, &len);
			if (-1 == nbytes)
	        {
	            printf("received data error!\n");
	        }
			/*get interface name of the received CAN frame*/
			ifr.ifr_ifindex = readAddr.can_ifindex;
			ioctl(sock_fd, SIOCGIFNAME, &ifr);
			printf("Received a CAN frame from interface %s\n",ifr.ifr_name);

			/*�����յ���CAN���ݴ�ӡ����������IDΪ��ʶ����DLCΪCAN���ֽ�����DATAΪ1֡���ĵ��ֽ���*/
			printf("CAN frame:\n ID = 0x%x, DLC = %d, DATA = %s\n",\
				frame.can_id,frame.can_dlc,frame.data);
			ptr_frame = &frame;
		}
	}

	return 0;

}