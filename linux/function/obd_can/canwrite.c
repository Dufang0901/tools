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

	/*1 建立套接字，设置为原始套接字，原始CAN协议 */
	sock_fd = socket(PF_CAN,SOCK_RAW,CAN_RAW);

	/*2 对CAN接口进行初始化，如设置CAN接口名，即当我们用ifconfig命令时显示的名字 */
	strcpy((char *)(ifr.ifr_name), "can0");
	ioctl(sock_fd, SIOCGIFINDEX, &ifr);
	printf("can0 can_ifindex = %x\n", ifr.ifr_ifindex);

	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	/*3 将刚生成的套接字与CAN套接字地址进行绑定*/
	bind(sock_fd, (struct sockaddr*)&addr, sizeof(addr));

	/*设置CAN帧的ID号，可区分为标准帧和扩展帧的ID号*/
	frame.can_id = 0x18412345;
	strcpy((char *)frame.data,"hello");
	frame.can_dlc = strlen(frame.data);
	printf("Send a CAN frame from interface %s\n", ifr.ifr_name);

	/*开始发送数据*/
	nbytes = sendto(sock_fd, &frame, sizeof(struct can_frame), 0, (struct sockaddr*)&addr, sizeof(addr));

	return 0;

}