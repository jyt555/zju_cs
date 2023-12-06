//cmd_del_blank_order

#include "cmd.h"

void cmd_del_blank_order(char *cp[])
{
	del_blank(cp);
	cmd_d_order_print(cp);
}

void del_blank(char *cp[])
{
	int i,j,k;
	char ch[MAXN][80];
	
	for(k = 0;k < n;k++){
		
		i = j = 0;
		
		while(cp[k][i] != '\0'){
			if(cp[k][i] != ' '&&cp[k][i] != '\t'){
				ch[k][j] = cp[k][i];
				j++;
			}
			i++;
		}
		
		cp[k] = ch[k];
	}
}
