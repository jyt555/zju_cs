#include "head.h"

void scan()			//输入 
{
	while((c=getchar())!='\n')
	{
		a[i] = c;
		i++;
	}
	a[i] = '\n';
	i = 0;
	
}

void print()		//输出 
{
	while(1)
	{
		if(i == 0)							word = 0;	//首次输入 
		else if(letter(a[m])&&!letter(a[i]))word = 1;	//读取一个单词 
		else 								word = 0;
		
		if(!letter(a[m])&&letter(a[i])) n = i;
		
		if(word)
		{
			judge = fjudge(n); 		//判断三种情况中哪一种 
			Pig_Latin(judge);		//加密 
		}
		if(!letter(a[i]))printf("%c",a[i]);	//标点原样输出 
		m = i;
		i++;
		if(a[i - 1] == '\n')return;
	}
}
