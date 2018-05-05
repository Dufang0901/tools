/******************************************************************************/
/* @F_NAME:           main.c                                             */
/* @F_PURPOSE:                                                                */
/* @F_CREATED_BY:       jun.ma                                                  */
/* @F_CREATION_DATE: May 10, 2017                                               */
/* @F_LANGUAGE:      C                                                        */
/* @F_MPROC_TYPE:    processor independent                                    */
/************************************** (C) Copyright 2016 Magneti Marelli ****/

#ifdef __USAGE


#endif

/*______ I N C L U D E - F I L E S ___________________________________________*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

void print_time(const char *info)
{
#if 0
  time_t t;
  
  time(&t);
  printf("**************%s: %s\n",info,ctime(&t));
  
#endif

  struct timespec tm;

  clock_gettime(CLOCK_REALTIME, &tm);
  //printf("**************%s: sec:%lld  nsec:%ld\n",info,tm.tv_sec,tm.tv_nsec);
  printf("**************%s: sec: %ld nsec:%ld\n",info,tm.tv_sec,tm.tv_nsec);

}
void main(int argc,char *argv[])  
{ 
  int rc = 0;
  if(argc != 2)
  {
	printf("invalid input.\n");
	return;
  }
  print_time("before:");
  rc = system(argv[1]);
  if( rc == -1 ) 
  {
    printf( "shell could not be run\n" );
  } 
  else 
  {
    printf( "result of running command is %d\n", WEXITSTATUS( rc ) );
  }
  print_time("after :");
}  






