//cmd_scan

#include "cmd.h"

void cmd_scan(char *filename)
{
	char *cp[MAXN];
	FILE *fp;
	
	fp = fopen(filename,"r");
	
	i = 0;
	cp[i] = (char *)malloc(sizeof(char*));
	while(fgets(cp[i],MAXN,fp)){
		i++;
		cp[i] = (char *)malloc(sizeof(char*));
	}

	n = i;
	
	switch(flag){
		case 0:
			printf("将文本文件filename原样输出:\n");
			cmd_directly_print(cp);
			break;
		case 1:
			printf("将文本文件filename的每一行前面加上行号，然后输出:\n");
			cmd_add_linenumber(cp);
			break;
		case 2:
			printf("将文本文件filename的每一行字符串，按照降序输出:\n");
			order = -1;
			cmd_d_order_print(cp);
			break;
		case 3:
			printf("将文本文件filename的每一行去掉空白字符（空格、制表符）后，按照升序输出:\n"); 
			order = 1; 
			cmd_del_blank_order(cp);
			break;
	}
	
	for(i = 0;i < n;i++)free(cp[i]);	//*bug,调试时会出现SIGTRAP错误 
	
	fclose(fp);	
}
