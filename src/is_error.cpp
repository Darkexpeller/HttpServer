#include"is_error.h"
#include<stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
void is_error(bool expression, const char* error_mes)
{
	if (expression) 
	{
		if(error_mes)
			perror(error_mes);
		else{
			printf("%s\n", strerror(errno));
		}
		//exit(EXIT_FAILURE);
	}
	return;
}
