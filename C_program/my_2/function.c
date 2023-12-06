#include "head.h"

int fjudge(int x)	//判断是三种情况中哪一种 
{
	if(vowel(a[x]))return 1;	//首字母元音 
	
	while(x <= i)
	{
		if(vowel(a[x]))
		{	
			pause = x;
			return 2;		//中间含元音 
		}
		x++;
	}
	
	return 3;	//不含元音 
}

void Pig_Latin(int judge)	//加密 
{
	switch(judge)
	{
		case 1:
			for(p = n;p < i;p++)printf("%c",a[p]);
			printf("-way");
			return;
		case 2:
			for(p = pause;p < i;p++)printf("%c",a[p]);
			printf("-");
			for(p = n;p < pause;p++)printf("%c",a[p]);
			printf("ay");
			return;
		case 3:
			for(p = n;p < i;p++)printf("%c",a[p]);
			return;
	}
} 

int vowel(char ch)	//判断是否是元音字母 
{
	if(ch == 'a'||ch == 'e'||ch == 'i'||ch == 'o'
	||ch == 'u'||ch == 'A'||ch == 'E'||ch == 'I'||
	ch == 'O'||ch == 'U')
	return 1;
	
	return 0;
}

int letter(char ch)	//判断是否为字母 
{
	if(ch >= 'a'&&ch <= 'z'||ch >= 'A'&&ch <= 'Z')return 1;
	return 0;
}
