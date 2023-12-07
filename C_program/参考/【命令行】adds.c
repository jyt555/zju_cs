#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	double sum;
	
	sum = 0;
	for (int i = 1; i < argc; i++) {
		sum += atof(argv[i]);
	}
	printf("sum = %f\n", sum);
	
	return 0;
}

//命令行程序，计算以命令行参数形式输入的任意多个浮点数的和
