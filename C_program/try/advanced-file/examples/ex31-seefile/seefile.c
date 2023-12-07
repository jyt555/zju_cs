#include <stdio.h>
#include <ctype.h>
#include <string.h>
 
#define MAXLEN 100
 
int DISPLAY = 80;
int PAGE_LENGTH = 20;
 
int main(int argc,char *argv[]){
 
	char filename[MAXLEN];

	FILE *pfile = NULL;
 
	unsigned char buffer[DISPLAY/4-1];
	int count = 0;
	int lines = 0;
 	int i,size;

	//要求输入文件名
	if(argc == 1){
		printf("请输入文件名:\n");
		scanf("%s",filename);
		//printf("%s\n",filename);
	}else{
		strcpy(filename,argv[1]);
		printf("执行程序:%s\n",argv[0]);
	}
	
 
	//打开文件
	if(!(pfile = fopen(filename,"rb"))){
		printf("未能打开该文件.\n");
		return -1;
	}
 
	//读取并显示文件内容
	while(!feof(pfile)){
		size=sizeof(buffer);
		if(count < size)
			buffer[count++] = (unsigned char)fgetc(pfile);
		else{
			for(count = 0;count < size; count++){
				printf("%02X ",buffer[count]);
			}
			printf("| ");
 
 
			for(count = 0;count < size; count++){
				printf("%c",isprint(buffer[count])?buffer[count]:'.');
			}
			printf("\n");
			count = 0;
 
			if(!(++lines%PAGE_LENGTH))
				if(getchar() == 'E')
					return 0;
		}
			
	}
 
	//显示最后一行
	for(i = 0;i < sizeof(buffer); i++){
		if(i < count)
			printf("%02X ",buffer[i]);
		else
			printf("   ");
	}
	printf("| ");
 
	for(i = 0;i < count; i++){
		printf("%c",isprint(buffer[i])?buffer[i]:'.');
	}
	printf("\n");
	
	//结束
	fclose(pfile);
	return 0;
}