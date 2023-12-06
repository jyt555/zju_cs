/*��֪����˵����
 *	1.cmd_scan.c���ͷŶ�̬�ڴ�һ��������ʱ�ᵼ��SIGTRAP�Ĵ���
 *	2.���ʱ���������һ���У� 
 *	3.����filename.txtʱ�����һ�е�ĩβ���뻻�У�����������ͬһ����� 
 */

#include "cmd.h"

int main(int argc,char *argv[])
{
	flag = -1;								//��flag��ʾ�����ѡ�� 

	if (argc == 1) {
		printf("Usage: %s\n", argv[0]);		//û��������ļ� 
		exit(-1);
		
	}else if (argc == 2) {
		flag = 0;
		cmd_scan(argv[1]);	
		
	}else if(argc == 3){
		if(strcmp(argv[1],"-l") == 0){
			flag = 1;
		}else if(strcmp(argv[1],"-d") == 0){
			flag = 2;
		}else if(strcmp(argv[1],"-i") == 0){
			flag = 3;
		}
		cmd_scan(argv[2]);	
	}else{
		printf("Not a specified command!");	//����ָ�������� 
		exit(-1);
	}
	
	return 0;
}

