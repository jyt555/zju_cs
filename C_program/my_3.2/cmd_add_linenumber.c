//cmd_add_linenumber

#include "cmd.h"

void cmd_add_linenumber(char *cp[])
{
	for(i = 0;i < n;i++){
		printf("(line)%d. ",i + 1);
		printf("%s",cp[i]);
	}
} 
