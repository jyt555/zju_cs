/*
 * addany.c: �����г��򣬸��ݲ���ѡ�������������֮��
       addany -d 12 23 34 45 56 ...  ѡ��-d��ʾ���������в�����Ϊ��Ҫ��͵�����
	   addany -f filename            ѡ��-f��ʾ��Ҫ��͵�����������ı��ļ�filename�� 
*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int sum = 0;
	FILE *fp; 
	int flag,i;
printf("1");	
	if (argc == 1) {
		printf("Usage: %s -[d|f] ...\n", argv[0]);
		exit(-1);
	}
	
	if (strcmp(argv[1], "-d") == 0) {
		flag = 0;
	} else if (strcmp(argv[1], "-f") == 0) {
		flag = 1;
	} else {
		printf("Bad option ''%s''\n", argv[1]);
		exit(-1);
	}

	if (flag == 0) {
		for (i = 2; i < argc; i++) sum += atoi(argv[i]);
	} else {
		fp = fopen(argv[2], "r");
		int num;
		while (fscanf(fp, "%d", &num) == 1) sum += num;
		fclose(fp);
	}
	
	printf("sum = %d\n", sum);
	
	printf("%d",argc);
	
	return 0;
}
