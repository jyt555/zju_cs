//cmd.h
#define MAXN 10000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _CMD_H_
#define _CMD_H_

int flag,n,i,j,order;

void cmd_scan(char *filename);			//输入文件，处理命令
void cmd_directly_print(char *cp[]);	//cmd_0:直接输出
void cmd_add_linenumber(char *cp[]);	//cmd_1:每一行前加上行号，然后输出
void cmd_d_order_print(char *cp[]);		//cmd_2:降序\升序输出
void cmd_del_blank_order(char *cp[]);	//cmd_3:去掉空白，升序输出 


#endif	//_CMD_H_
