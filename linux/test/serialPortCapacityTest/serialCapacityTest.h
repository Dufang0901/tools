#ifndef _SERIAL_CAPACITY_TEST_
#define _SERIAL_CAPACITY_TEST_
#include"OsPack.h"

typedef struct{
	pthread_rwlock_t lock;
	int				 comFd;
	struct sockaddr_un sDestAddr;
	int 			 sendCnt;
	int 			 sendSuccessCnt;
	int 			 recvCnt;
}S_SerialState;

typedef struct{
	S_SerialState state2323;
	S_SerialState state2324;
	S_SerialState state4851;
	S_SerialState state4852;
}S_ProcessRes;


static int TimerInit(void);

#endif