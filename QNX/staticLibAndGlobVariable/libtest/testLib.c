#include <stdio.h>
#include "appModule.h"
extern int appVal;
void lib_print_val()
{
	int val = 0;
	printf("#### lib_print_val 1:val = %d\n",appVal);
	set_val(val + 1);
	val = get_val();
	printf("#### lib_print_val 2:val = %d\n",val);
}