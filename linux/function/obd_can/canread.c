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

	/*为了能够接收CAN报文，我们需要定义一个CAN数据格式的结构体变量*/
	struct can_frame frame;
	struct can_frame *ptr_frame;

	/*建立套接字，设置为原始套接字，原始CAN协议 */
	sock_fd = socket(PF_CAN,SOCK_RAW,CAN_RAW);

	/*以下是对CAN接口进行初始化，如设置CAN接口名，即当我们用ifconfig命令时显示的名字 */
	strcpy(ifr.ifr_name,"can1");
	ioctl(sock_fd, SIOCGIFINDEX, &ifr);
	printf("can1 can_ifindex = %x\n",ifr.ifr_ifindex);

	/*设置CAN协议 */
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	/*将刚生成的套接字与网络地址进行绑定*/
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
			/*开始接收数据*/
			nbytes = recvfrom(sock_fd, &frame, sizeof(struct can_frame), 0, (struct sockaddr *)&readAddr, &len);
			if (-1 == nbytes)
	        {
	            printf("received data error!\n");
	        }
			/*get interface name of the received CAN frame*/
			ifr.ifr_ifindex = readAddr.can_ifindex;
			ioctl(sock_fd, SIOCGIFNAME, &ifr);
			printf("Received a CAN frame from interface %s\n",ifr.ifr_name);

			/*将接收到的CAN数据打印出来，其中ID为标识符，DLC为CAN的字节数，DATA为1帧报文的字节数*/
			printf("CAN frame:\n ID = 0x%x, DLC = %d, DATA = %s\n",\
				frame.can_id,frame.can_dlc,frame.data);
			ptr_frame = &frame;
		}
	}

	return 0;

}