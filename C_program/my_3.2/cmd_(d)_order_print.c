//cmd_(d)_order_print

#include "cmd.h"

void cmd_d_order_print(char *cp[])
{
	if(order == -1)
	for(i = 1;i < n;i++){
		for(j = 0;j < n - i;j++){
			if(strcmp(cp[j],cp[j+1]) < 0){
				swap(cp[j],cp[j+1]);
			}
		}
	}
	
	if(order == 1)
	for(i = 1;i < n;i++){
		for(j = 0;j < n - i;j++){
			if(strcmp(cp[j],cp[j+1]) > 0){
				swap(cp[j],cp[j+1]);
			}
		}
	}
	
	cmd_directly_print(cp); 
}

void swap(char *a, char *b)
{
	char item[MAXN];
	
	strcpy(item,a);
	strcpy(a,b);
	strcpy(b,item);
}
