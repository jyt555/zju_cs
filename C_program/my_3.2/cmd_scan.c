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
			printf("���ı��ļ�filenameԭ�����:\n");
			cmd_directly_print(cp);
			break;
		case 1:
			printf("���ı��ļ�filename��ÿһ��ǰ������кţ�Ȼ�����:\n");
			cmd_add_linenumber(cp);
			break;
		case 2:
			printf("���ı��ļ�filename��ÿһ���ַ��������ս������:\n");
			order = -1;
			cmd_d_order_print(cp);
			break;
		case 3:
			printf("���ı��ļ�filename��ÿһ��ȥ���հ��ַ����ո��Ʊ�����󣬰����������:\n"); 
			order = 1; 
			cmd_del_blank_order(cp);
			break;
	}
	
	for(i = 0;i < n;i++)free(cp[i]);	//*bug,����ʱ�����SIGTRAP���� 
	
	fclose(fp);	
}
