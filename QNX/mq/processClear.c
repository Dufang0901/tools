#include <stdlib.h>
#include <mqueue.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
int main()
{
	mq_unlink("/DMS_MQUEUE_SEND");
    return 1;

}

