#include "head.h"

void scan()			//���� 
{
	while((c=getchar())!='\n')
	{
		a[i] = c;
		i++;
	}
	a[i] = '\n';
	i = 0;
	
}

void print()		//��� 
{
	while(1)
	{
		if(i == 0)							word = 0;	//�״����� 
		else if(letter(a[m])&&!letter(a[i]))word = 1;	//��ȡһ������ 
		else 								word = 0;
		
		if(!letter(a[m])&&letter(a[i])) n = i;
		
		if(word)
		{
			judge = fjudge(n); 		//�ж������������һ�� 
			Pig_Latin(judge);		//���� 
		}
		if(!letter(a[i]))printf("%c",a[i]);	//���ԭ����� 
		m = i;
		i++;
		if(a[i - 1] == '\n')return;
	}
}
