//cmd.h
#define MAXN 10000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _CMD_H_
#define _CMD_H_

int flag,n,i,j,order;

void cmd_scan(char *filename);			//�����ļ�����������
void cmd_directly_print(char *cp[]);	//cmd_0:ֱ�����
void cmd_add_linenumber(char *cp[]);	//cmd_1:ÿһ��ǰ�����кţ�Ȼ�����
void cmd_d_order_print(char *cp[]);		//cmd_2:����\�������
void cmd_del_blank_order(char *cp[]);	//cmd_3:ȥ���հף�������� 


#endif	//_CMD_H_
