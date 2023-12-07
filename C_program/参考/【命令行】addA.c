#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i,n,a,sum;
	
	sum = 0;
	n = argc;
	for ( i = 0; i < n; i++) {
		a = atoi(argv[i]);
		sum += a;
	}
	printf("sum = %d\n", sum);
	
	return 0;
}

//命令行程序，计算以命令行参数形式输入的任意多个整数的和 
