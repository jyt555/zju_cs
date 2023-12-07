#include <stdio.h>

#define MAXLINE 1024

typedef struct PolynomialStruct{
    int coef; // 系数
    int expo;  // 幂次
    struct PolynomialStruct * next;
} PolynomialNode;

//判断字符串 s 的起始字符是否为：x^ 或 X^
int IsEXP(char *s)
{
    return ( (s[0]=='x' || s[0]=='X') && s[1]=='^');
}

//字符是否为\n或\0
int IsEndingChar(char ch)
{
    return (ch==0 || ch=='\n');
}

//跳过字符串起始部分的空格和制表符
//返回值：一个指针
//      指向字符串前面的第一个非空白字符
char * SkipSpaceChars(char *s)
{
    while( *s==' ' || *s=='\t' )
        s++;
    return s;
}

char* InputMonomial(char *s, PolynomialNode *p );
void OutputMonomial(int coef, int expo); 

int main()
{
    char linebuffer[MAXLINE], *p;
    PolynomialNode node;

    p = fgets(linebuffer, sizeof(linebuffer), stdin);
    while( p = InputMonomial(p, &node) ) {
        OutputMonomial(node.coef, node.expo);
        printf("\n");
    }

    return 0;
}
/* 请在这里填写答案 */
char* InputMonomial(char *s, PolynomialNode *p )
{
	s = SkipSpaceChars(s);
	int n;
	
	if(IsEndingChar(*s)) return NULL;
	p->coef = 1;p->expo = 0;
	
	if(!IsEXP(s)){
		if(*s == '-' && IsEXP(s + 1)){
			p->coef = -1;
			s++;
		}else{
			sscanf(s,"%d%n",&(p->coef),&n);
			s = s + n;
		}
	}
	
	if(*s == 'x'){
		s++;
		if(*s != '^'){
			p->expo = 1;
			s++;
		}else{
			s++;
			sscanf(s,"%d%n",&(p->expo),&n);
			s = s + n;
		}
	}
	return s;
}
void OutputMonomial(int coef, int expo)
{
	if(!expo){
		printf("%d",coef);
		return;
	}
	
	if(coef != 1 && coef != -1)
		printf("%d",coef);
	else if(coef == -1)
		printf("-");
		
	printf("x");
	
	if(expo == 1) return;
	
	printf("^%d",expo);
	return;
	
}
