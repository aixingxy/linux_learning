#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "head.h"

typedef struct TTfunInfo
{
	int fun_type;
	int a;
	int b;
	char funname[10];
}TfunInfo;

int main(int argc, char *argv[])
{
	int a = 2;
	int i = 0;
	int a1 = 10;
	int b1 = 5;
	TfunInfo funinfo[2];
	char *MSg = "I will del!";
	if(argc == 3)
	{
		a1 = atoi(argv[1]);
		b1 = atoi(argv[2]);
		funinfo[0].a = a1;
		funinfo[0].b = b1;
		funinfo[1].a = a1;
		funinfo[1].b = b1;
	}
	for(i = 0; i < 2; i++)
	{
		printf("i===%d, LINE=%d\n", i, __LINE__);
		if(i == 0)
		{
			funinfo[i].fun_type = 1;
			printf("begin call sum\n");
			strcpy(funinfo[i].funname, "sum");
			sum(funinfo[i].a, funinfo[i].b);
		}	
		if(i == 1 )
		{
			funinfo[i].fun_type = 2;
			printf("begin call mul\n");
			strcpy(funinfo[i].funname, "mul");
			mul(funinfo[i].a, funinfo[i].b);
		}
	}
	printf("byebye\n");
	return 0;
}

