#include <stdio.h>
int appVal = 10;
int get_val()
{
	printf("#### get_val: appVal = %d\n",appVal);
	return appVal;
}
int set_val(int val)
{
	printf("#### set_val: before set: appVal = %d\n",appVal);
	appVal = val;
	printf("#### set_val: after set: appVal = %d\n",appVal);
	return 1;
}