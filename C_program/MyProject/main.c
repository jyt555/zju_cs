#include <stdio.h>
#include <stdlib.h>
#include "genlib.h"
#include "simpio.h"

int main() 
{
	int n;
	
	printf("Enter num: ");
	n = GetInteger();
	printf("Hello, %d!\n", n);
	
	return 0; 
}
