#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static int cnt;
volatile char buffa[1024];
volatile char buffb[1024];
void*  function( void*  arg )
{
	int fd = -1;
	struct timespec interval;
	interval.tv_sec = 0;
	interval.tv_nsec = cnt;
   printf( "This is thread %d, sleep cnt is %d\n", pthread_self(),cnt );
   
   fd = open("/appfs/application/qnx660/qnxfs.tar.gz",O_RDONLY);
   if(fd <= 0)
   {
	   printf("open file failed.\n");
	   return 1;
   }
   while(!eof(fd))
   {
	   read(fd,buffa,1024);
	   if(cnt > 0)
	   {
		   //usleep(cnt);
		   clock_nanosleep(CLOCK_REALTIME,0,&interval,NULL);
	   }
   }
   close(fd);
   return( 0 );
}

int main(int argc,const char *argv[])
{
	if(argc != 2)
	{
		printf("usage: ./pthread_create [cnt]   where cnt is usleep time.\n");
		return 1;
	}
	
	cnt = atoi(argv[1]);
	cnt = cnt > 0 ? cnt : 0;
	
   pthread_attr_t pattr;
	struct sched_param	param;

	pthread_attr_init(&pattr);
	pthread_attr_setschedpolicy(&pattr, SCHED_RR);
	param.sched_priority = 21;
	pthread_attr_setschedparam(&pattr, &param);
	pthread_attr_setinheritsched(&pattr, PTHREAD_EXPLICIT_SCHED);

   pthread_create( NULL, &pattr, &function, NULL );

   /* Allow threads to run for 60 seconds. */
   sleep(120);
   return EXIT_SUCCESS;
}
