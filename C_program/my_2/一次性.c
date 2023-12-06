#include <stdio.h>

void scan();
void print();
int fjudge(int x);
void Pig_Latin(int judge);
int vowel(char ch);
int letter(char ch);

char	a[200],c;
int		i = 0,word = 0,n = -1,m,judge,pause,p;

int main(void) 
{
	scan();
	print();
	return 0;
}

void scan()
{
	while((c=getchar())!='\n')
	{
		a[i] = c;
		i++;
	}
	a[i] = '\n';
	i = 0;
	
}

void print()
{
	while(1)
	{
		if(i == 0)							word = 0;
		else if(letter(a[m])&&!letter(a[i]))word = 1;
		else 								word = 0;
		
		if(!letter(a[m])&&letter(a[i])) n = i;
		
		if(word)
		{
			judge = fjudge(n); 
			Pig_Latin(judge);
		}
		if(!letter(a[i]))printf("%c",a[i]);
		m = i;
		i++;
		if(a[i - 1] == '\n')return;
	}
}

int fjudge(int x)
{
	if(vowel(a[x]))return 1;
	
	while(x <= i)
	{
		if(vowel(a[x]))
		{	
			pause = x;
			return 2;
		}
		x++;
	}
	
	return 3;
}

void Pig_Latin(int judge)
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

int vowel(char ch)
{
	if(ch == 'a'||ch == 'e'||ch == 'i'||ch == 'o'
	||ch == 'u'||ch == 'A'||ch == 'E'||ch == 'I'||
	ch == 'O'||ch == 'U')
	return 1;
	
	return 0;
}

int letter(char ch)
{
	if(ch >= 'a'&&ch <= 'z'||ch >= 'A'&&ch <= 'Z')return 1;
	return 0;
}
