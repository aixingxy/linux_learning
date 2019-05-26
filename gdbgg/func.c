#include <stdio.h>
#include "head.h"

int sum(int a, int b)
{
	printf("welcome call %s, a=%d, b=%d\n", __FUNCTION__, a, b);
	return a+b;
}

int mul(int a, int b)
{
	printf("welcome call %s, a=%d, b=%d\n", __FUNCTION__, a, b);
	return a*b;
}

