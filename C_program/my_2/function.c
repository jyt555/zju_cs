#include "head.h"

int fjudge(int x)	//�ж��������������һ�� 
{
	if(vowel(a[x]))return 1;	//����ĸԪ�� 
	
	while(x <= i)
	{
		if(vowel(a[x]))
		{	
			pause = x;
			return 2;		//�м京Ԫ�� 
		}
		x++;
	}
	
	return 3;	//����Ԫ�� 
}

void Pig_Latin(int judge)	//���� 
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

int vowel(char ch)	//�ж��Ƿ���Ԫ����ĸ 
{
	if(ch == 'a'||ch == 'e'||ch == 'i'||ch == 'o'
	||ch == 'u'||ch == 'A'||ch == 'E'||ch == 'I'||
	ch == 'O'||ch == 'U')
	return 1;
	
	return 0;
}

int letter(char ch)	//�ж��Ƿ�Ϊ��ĸ 
{
	if(ch >= 'a'&&ch <= 'z'||ch >= 'A'&&ch <= 'Z')return 1;
	return 0;
}
