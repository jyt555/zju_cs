#include <stdio.h>

int main(void)
{
	FILE *fptxt, *fpbin;//定义文本文件和二进制文件指针
	int ntxt, nbin;
	
	fptxt = fopen("data.txt", "rt");//以文本读模式打开已存在的data.txt文本文件
	fpbin = fopen("data.dat", "rb");//以二进制读模式打开已存在的data.dat二进制文件
	
	fscanf(fptxt, "%d", &ntxt);//从文本文件读取文本数据，保存到整型变量ntxt中
	fread(&nbin, sizeof(nbin), 1, fpbin);//从二进制文件中读取二进制数据，保存到整型变量nbin中
	
	fclose(fptxt);//关闭文本文件
	fclose(fpbin);//关闭二进制文件
	
	printf("ntxt = %d\n", ntxt);
	printf("nbin = %d\n", nbin);
	
	return 0;
} 
