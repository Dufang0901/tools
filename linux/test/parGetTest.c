#include<stdio.h>
void main()
{
	char a[10];
	char b[10];
	char *start,*end;
	memset(a,0,10);
	memset(a,'1',9);
	memset(b,0,10);
	printf("after init: a=%s\n",a);
	start = end = &a[0];
	strncpy(a,start,end - start);
	a[end-start]='\0';
	printf("after copy:a=%s\n",a);
	
	
}