/*已知问题说明：
 *	1.cmd_scan.c中释放动态内存一步，调试时会导致SIGTRAP的错误；
 *	2.输出时最下面会多出一空行； 
 *	3.输入filename.txt时，最后一行的末尾必须换行，否则两行在同一行输出 
 */

#include "cmd.h"

int main(int argc,char *argv[])
{
	flag = -1;								//用flag表示命令的选择 

	if (argc == 1) {
		printf("Usage: %s\n", argv[0]);		//没有输入的文件 
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
		printf("Not a specified command!");	//不是指定的命令 
		exit(-1);
	}
	
	return 0;
}

