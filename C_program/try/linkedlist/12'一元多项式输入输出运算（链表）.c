#include <stdlib.h>
#include <stdio.h>

#define MAXLINE 1024

// 交换 a, b，以t为中间变量
#define SWAP(a,b,t) (t)=(a),(a)=(b),(b)=(t)

//  单项式结构 
typedef struct MonomialStruct{
    int coef; // 系数
    int expo;  // 幂次
    struct MonomialStruct * next;
} Monomial; 

//  多项式类型定义
typedef Monomial *Polynomial;

void       PrintMonomial(int coef, int expo); 
Monomial*  ParseMonomial(char **s);
Polynomial InsertAfterTail(Polynomial head, Monomial* pNewNode);
Polynomial TrimZeroTerms(Polynomial head);
Polynomial SortPolynomial(Polynomial head);
Polynomial Minus(Polynomial p1, Polynomial p2);
Polynomial Multiply(Polynomial p1, Polynomial p2);

/*******************************************************************************
 函数 CreateMonomial - 创建一个单项式项式节点 
      Monomial * CreateMonomial(int coef, int expo); 
 参数
     coef - 系数
     expo - 幂指数  
 返回值
     成功 - 所创建的节点指针 
     失败 - NULL 
*******************************************************************************/
Monomial * CreateMonomial(int coef, int expo)
{
    Monomial * p = (Monomial*)malloc(sizeof(Monomial));
    if( !p )
        return NULL;
    p->coef = coef;
    p->expo = expo;
    p->next = NULL;    
    return p;
}

/*******************************************************************************
 函数 DeletePolynomial - 删除一个多项式，并释放所有的节点内存 
      Polynomial DeletePolynomial(Polynomial p)
 参数
     head - 被删除的多项式的头节点指针 
 返回值
     空指针 NULL 
*******************************************************************************/
Polynomial DeletePolynomial(Polynomial head)
{
    while(head) {
        Monomial * d = head;
        head = head->next;
        free(d);
    }
    return NULL;
}

/*****************************************************
 函数 AddMonomial - 在多项式中添加一个单项式
      如果链表中已经存在同次项，则不会创建新节点，而是对系数进行累加。
      Polynomial AddMonomial(Polynomial head, int coef, int expo); 
 参数
      head - 多项式的头节点指针 
      coef - 单项式的系数
      expo - 单项式的幂指数 
 返回值
      新的多项式链表的表头指针
*****************************************************/
Polynomial AddMonomial(Polynomial head, int coef, int expo)
{
    Monomial * p;
    if( coef==0 )
        return head;

    for( p = head; p && p->expo!=expo; p = p->next )
        ; // 寻找同次项
    if( p ) 
        // 找到同次项 
        p->coef += coef;
    else {
        p = CreateMonomial(coef, expo);
        head = InsertAfterTail(head, p);
    }
    return head;
}

/*****************************************************
  函数 Add - 多项式求和： p1 + p2 
               链表 p1 和 p2保持不变 
  返回值：
      多项式 "和" 的表头指针 
*****************************************************/ 
Polynomial Add(Polynomial p1, Polynomial p2)
{
    Polynomial h = NULL;
    for( ; p1; p1=p1->next )
        h = AddMonomial(h, p1->coef, p1->expo);
    for( ; p2; p2=p2->next )
        h = AddMonomial(h, p2->coef, p2->expo);
    return h;
}

// 判断字符串 s 的起始字符是否为：x^ 或 X^
int IsEXPO(char *s)
{
    return ( (s[0]=='x' || s[0]=='X') && s[1]=='^');
}

// 字符是否为\n或\0
int IsEndingChar(char ch)
{
    return (ch==0 || ch=='\n');
}

// 跳过字符串起始部分的空格和制表符
// 返回值：一个指针
//        指向字符串前面的第一个非空白字符
char * SkipSpaceChars(char *s)
{
    while( *s==' ' || *s=='\t' )
        s++;
    return s;
}

/*****************************************************
  函数 GetSignChar - 字符串*s中第一个单项式的符号字符
       int GetSignChar(char **s);
  参数 
       s - *s是字符串指针 
  返回值
    成功   -返回符号字符，将*s指向符号之后的字符 
    不成功 -返回 0 
*****************************************************/
int GetSignChar(char **s)
{
    char *p = SkipSpaceChars(*s); // 忽略空白字符
    if( *p=='+' || *p=='-' ) {
        *s = p + 1;
        return (*p);
    }
    return 0;
}

/*****************************************************
从一行标准输入，读取一个一元多项式 
返回值：
    所读取的多项式链表的表头指针
*****************************************************/ 
Polynomial ParsePolynomial()
{
    char linebuffer[1024], *s = linebuffer;
    Polynomial hResult = NULL;

    if( !fgets(linebuffer, sizeof(linebuffer), stdin) )
        return NULL;
    while( 1 ) {
        Monomial *pNewNode;
        int signChar = GetSignChar(&s); 
//printf("%d\n",signChar);/////////////////////////
        pNewNode = ParseMonomial(&s);
        if( !pNewNode ) 
            break;
        if( signChar == '-' )
            pNewNode->coef =  -pNewNode->coef; 
        hResult = InsertAfterTail(hResult, pNewNode);
    }
    return hResult;
}

/*****************************************************
  函数 PrintPolynomial - 输出多项式
       Polynomial PrintPolynomial(Polynomial pHead);
       两个单项式之间的+/-左右各留出一个空格 
  返回值
       被输出的多项式 
*****************************************************/ 
Polynomial PrintPolynomial(Polynomial pHead)
{
    Polynomial p = pHead;
    int firstTerm = 1, nothingOutputYet = 1; 
    for( ; p; p = p->next )
    {
        int c = p->coef;
        int k = p->expo;
        if( c==0 ) // 忽略系数为0的项
            continue;
        if( firstTerm ) {
            PrintMonomial(c,k);
            firstTerm = 0;
        } else {
            printf(" %c ", c>0 ? '+' : '-');
            PrintMonomial(c>0?c:-c, k);
        }
        nothingOutputYet = 0; 
    }
    if( nothingOutputYet ) 
        putchar('0');
    putchar('\n');
    return pHead;
}


/*------------------------------------------------------------------

 1. 如果测试多项式输入输出，那么： 
 
   输入多项式1，回车
   回车
 
 2. 如果测试两个多项式的 "加"、"减"、"乘"运算，那么： 
 
   输入第一个多项式，回车
   输入运算符（+、-、*)，回车
   输入第二个多项式，回车

------------------------------------------------------------------*/ 
int main()
{
    Polynomial p1,  p2,  pResult;
    char cmdString[MAXLINE], cmd;
     
    p1 = ParsePolynomial(); //读入多项式 1 

    if( !fgets(cmdString, sizeof(cmdString), stdin) )
        return 0;

    cmd = cmdString[0];
    
    if( cmd!='+' && cmd!='-' && cmd!='*' ) {
        //测试多项式的输入和输出 
        printf("input=");
        PrintPolynomial( p1 );
        return 0;
    }
    
    p2 = ParsePolynomial(); //读入多项式 1 
    
    // printf("\n运算结果是:\n\n");
    PrintPolynomial( p1 );
    printf("%c\n", cmd);
    PrintPolynomial( p2 );
    printf("=\n");
        
    if( cmd=='+' )
        pResult = PrintPolynomial( SortPolynomial( TrimZeroTerms( Add(p1,p2) ) ) );
    else if( cmd=='-' )
        pResult = PrintPolynomial( SortPolynomial( TrimZeroTerms( Minus(p1, p2) ) ) );
    else //if( cmd=='*' )
        pResult = PrintPolynomial( SortPolynomial( TrimZeroTerms( Multiply(p1, p2) ) ) );

    // printf("\n");

    DeletePolynomial(p1);
    DeletePolynomial(p2);
    DeletePolynomial(pResult);
    
    return 0; 
}

/////////////////////////////////////////////////////////////////
void PrintMonomial(int coef, int expo)
{
	if(!expo){
		printf("%d",coef);
		return;
	}
	
	if(coef == -1)
		printf("-");
	else if(coef != 1)
		printf("%d",coef);
		
	printf("x");
	
	if(expo == 1) return;  // !!expo不行 
/*

x+5x^2
*
-x+5x^2

*/
	
	printf("^%d",expo);
	return;
	
}

Monomial*  ParseMonomial(char **s)
{

	*s = SkipSpaceChars(*s);
	int n;
	int coef = 1, expo = 0;
	
	if(IsEndingChar(**s)) return NULL;
	
	if(!IsEXPO(*s)){
		if(**s == '-' && IsEXPO(*s + 1)){
			coef = -1;
			(*s)++;							//  (*s)++;
		}else{								//sscanf不能输入负数 
			sscanf(*s,"%d%n",&coef,&n);
			*s = *s + n;
		}
	}
	
	if(**s == 'x'){
		(*s)++;
		if(**s != '^'){
			expo = 1;
		//	(*s)++;							//这个导致了x-4变成x+4 
		}else{
			(*s)++;
			sscanf(*s,"%d%n",&expo,&n);
			*s = *s + n;
		}
	}
//printf("%d %d\n",coef,expo);//////////////
	return CreateMonomial(coef,expo);
}

Polynomial InsertAfterTail(Polynomial head, Monomial* pNewNode)
{
	Monomial *tail;
	tail = head;
	
	if(head == NULL){
		head = pNewNode;
		head->next = NULL;
		return head;
	}
	
//	for( ;tail;tail->next != NULL) tail = tail->next;  //这条不行 
	while(tail->next !=NULL){
		tail=tail->next ;
	}

	
	tail->next = pNewNode;
	pNewNode->next = NULL;
	
	return head;
}

Polynomial TrimZeroTerms(Polynomial head)
{
	Monomial *p1, *p2;
	p1 = head;
	
	if(head == NULL)return NULL;
	
	while(p1->next != NULL){
		p2 = p1->next;
		if(!p2->coef){
			p1->next = p2->next;
			free(p2);
		}else p1 = p2;
	}
	
	if(!head->coef){
		p2 = head;
		head = head->next;
		free(p2);
	}
	
	return head;
}

Polynomial SortPolynomial(Polynomial head)
{
	int n;
	Monomial *a, *tag;
	tag = NULL;
	
	while(head != tag){
		a = head;
		while(a != tag && a->next != tag)
		{
			if((a->expo) > (a->next->expo)){
				SWAP(a->coef, a->next->coef, n);
				SWAP(a->expo, a->next->expo, n);
			}
			a = a->next;
		}
		tag = a;
	}
	return head;
}

Polynomial Minus(Polynomial p1, Polynomial p2)
{
	Polynomial h = NULL;
    for( ; p1; p1=p1->next )
        h = AddMonomial(h, p1->coef, p1->expo);
    for( ; p2; p2=p2->next )
        h = AddMonomial(h, -p2->coef, p2->expo);
    return h;
}

Polynomial Multiply(Polynomial p1, Polynomial p2)
{
	Polynomial h, p0;
	h = NULL;
	for( ;p1;p1 = p1->next){
		for(p0 = p2;p0;p0 = p0->next){
			h = AddMonomial(h, p1->coef * p0->coef, p1->expo + p0->expo);
		}
	}
	return h;
}
//////////测试点add和 input有没过的 
