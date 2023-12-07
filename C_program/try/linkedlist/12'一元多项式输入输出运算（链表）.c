#include <stdlib.h>
#include <stdio.h>

#define MAXLINE 1024

// ���� a, b����tΪ�м����
#define SWAP(a,b,t) (t)=(a),(a)=(b),(b)=(t)

//  ����ʽ�ṹ 
typedef struct MonomialStruct{
    int coef; // ϵ��
    int expo;  // �ݴ�
    struct MonomialStruct * next;
} Monomial; 

//  ����ʽ���Ͷ���
typedef Monomial *Polynomial;

void       PrintMonomial(int coef, int expo); 
Monomial*  ParseMonomial(char **s);
Polynomial InsertAfterTail(Polynomial head, Monomial* pNewNode);
Polynomial TrimZeroTerms(Polynomial head);
Polynomial SortPolynomial(Polynomial head);
Polynomial Minus(Polynomial p1, Polynomial p2);
Polynomial Multiply(Polynomial p1, Polynomial p2);

/*******************************************************************************
 ���� CreateMonomial - ����һ������ʽ��ʽ�ڵ� 
      Monomial * CreateMonomial(int coef, int expo); 
 ����
     coef - ϵ��
     expo - ��ָ��  
 ����ֵ
     �ɹ� - �������Ľڵ�ָ�� 
     ʧ�� - NULL 
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
 ���� DeletePolynomial - ɾ��һ������ʽ�����ͷ����еĽڵ��ڴ� 
      Polynomial DeletePolynomial(Polynomial p)
 ����
     head - ��ɾ���Ķ���ʽ��ͷ�ڵ�ָ�� 
 ����ֵ
     ��ָ�� NULL 
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
 ���� AddMonomial - �ڶ���ʽ�����һ������ʽ
      ����������Ѿ�����ͬ����򲻻ᴴ���½ڵ㣬���Ƕ�ϵ�������ۼӡ�
      Polynomial AddMonomial(Polynomial head, int coef, int expo); 
 ����
      head - ����ʽ��ͷ�ڵ�ָ�� 
      coef - ����ʽ��ϵ��
      expo - ����ʽ����ָ�� 
 ����ֵ
      �µĶ���ʽ����ı�ͷָ��
*****************************************************/
Polynomial AddMonomial(Polynomial head, int coef, int expo)
{
    Monomial * p;
    if( coef==0 )
        return head;

    for( p = head; p && p->expo!=expo; p = p->next )
        ; // Ѱ��ͬ����
    if( p ) 
        // �ҵ�ͬ���� 
        p->coef += coef;
    else {
        p = CreateMonomial(coef, expo);
        head = InsertAfterTail(head, p);
    }
    return head;
}

/*****************************************************
  ���� Add - ����ʽ��ͣ� p1 + p2 
               ���� p1 �� p2���ֲ��� 
  ����ֵ��
      ����ʽ "��" �ı�ͷָ�� 
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

// �ж��ַ��� s ����ʼ�ַ��Ƿ�Ϊ��x^ �� X^
int IsEXPO(char *s)
{
    return ( (s[0]=='x' || s[0]=='X') && s[1]=='^');
}

// �ַ��Ƿ�Ϊ\n��\0
int IsEndingChar(char ch)
{
    return (ch==0 || ch=='\n');
}

// �����ַ�����ʼ���ֵĿո���Ʊ��
// ����ֵ��һ��ָ��
//        ָ���ַ���ǰ��ĵ�һ���ǿհ��ַ�
char * SkipSpaceChars(char *s)
{
    while( *s==' ' || *s=='\t' )
        s++;
    return s;
}

/*****************************************************
  ���� GetSignChar - �ַ���*s�е�һ������ʽ�ķ����ַ�
       int GetSignChar(char **s);
  ���� 
       s - *s���ַ���ָ�� 
  ����ֵ
    �ɹ�   -���ط����ַ�����*sָ�����֮����ַ� 
    ���ɹ� -���� 0 
*****************************************************/
int GetSignChar(char **s)
{
    char *p = SkipSpaceChars(*s); // ���Կհ��ַ�
    if( *p=='+' || *p=='-' ) {
        *s = p + 1;
        return (*p);
    }
    return 0;
}

/*****************************************************
��һ�б�׼���룬��ȡһ��һԪ����ʽ 
����ֵ��
    ����ȡ�Ķ���ʽ����ı�ͷָ��
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
  ���� PrintPolynomial - �������ʽ
       Polynomial PrintPolynomial(Polynomial pHead);
       ��������ʽ֮���+/-���Ҹ�����һ���ո� 
  ����ֵ
       ������Ķ���ʽ 
*****************************************************/ 
Polynomial PrintPolynomial(Polynomial pHead)
{
    Polynomial p = pHead;
    int firstTerm = 1, nothingOutputYet = 1; 
    for( ; p; p = p->next )
    {
        int c = p->coef;
        int k = p->expo;
        if( c==0 ) // ����ϵ��Ϊ0����
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

 1. ������Զ���ʽ�����������ô�� 
 
   �������ʽ1���س�
   �س�
 
 2. ���������������ʽ�� "��"��"��"��"��"���㣬��ô�� 
 
   �����һ������ʽ���س�
   �����������+��-��*)���س�
   ����ڶ�������ʽ���س�

------------------------------------------------------------------*/ 
int main()
{
    Polynomial p1,  p2,  pResult;
    char cmdString[MAXLINE], cmd;
     
    p1 = ParsePolynomial(); //�������ʽ 1 

    if( !fgets(cmdString, sizeof(cmdString), stdin) )
        return 0;

    cmd = cmdString[0];
    
    if( cmd!='+' && cmd!='-' && cmd!='*' ) {
        //���Զ���ʽ���������� 
        printf("input=");
        PrintPolynomial( p1 );
        return 0;
    }
    
    p2 = ParsePolynomial(); //�������ʽ 1 
    
    // printf("\n��������:\n\n");
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
	
	if(expo == 1) return;  // !!expo���� 
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
		}else{								//sscanf�������븺�� 
			sscanf(*s,"%d%n",&coef,&n);
			*s = *s + n;
		}
	}
	
	if(**s == 'x'){
		(*s)++;
		if(**s != '^'){
			expo = 1;
		//	(*s)++;							//���������x-4���x+4 
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
	
//	for( ;tail;tail->next != NULL) tail = tail->next;  //�������� 
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
//////////���Ե�add�� input��û���� 
