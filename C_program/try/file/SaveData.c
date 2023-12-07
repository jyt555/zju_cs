#include <stdio.h>

int main(void)
{
	FILE *fptxt, *fpbin;//定义文件指针
	int n = 123456;//要保存的整数
	
	fptxt = fopen("data.txt", "wt");//以文本写模式，创建data.txt文件
	fpbin = fopen("data.dat", "wb");//以二进制写模式，创建data.dat文件
	
	fprintf(fptxt, "%d", n);//将n变量的值，以文本模式输出到fptxt文件指针所指向的文件data.txt中
	fwrite(&n, sizeof(n), 1, fpbin);//将n变量的值，以二进制模式（即在内存中存储的形式）保存到fptxt文件指针所指向的文件data.txt中
	
	fclose(fptxt);//关闭文本文件
	fclose(fpbin);//关闭二进制文件
	
	return 0;
} 
