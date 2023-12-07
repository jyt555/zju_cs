#include <stdio.h>

#define MAXLINE 1024

// ��Ԫ����ʽ
typedef struct {
    int k;    // ϵ��
    int nx;   // x ��ָ��
    int ny;   // y ��ָ��
} Term;

int ParseTerm( char *s, Term *t ); 

char * parse( char *s, Term *t )
{
    int n = ParseTerm(s,t);
    return ( n > 0 ? s + n : NULL );
}

int main()
{
    char linebuffer[MAXLINE];
    char *p = linebuffer;
    Term data;

   p = fgets(linebuffer, sizeof(linebuffer), stdin);
    
    /*int m;
	scanf("%d",&data.k);
	printf("%d",data.k);*/
 
  	while( p && (p = parse(p, &data)) ) 
    {
		printf("%dx^%dy^%d\n", data.k, data.nx, data.ny);
    }
    
    return 0;
}

/* ��Ĵ��뽫��Ƕ������ */


int ParseTerm( char *s, Term *t )
{
	int m;
	while(sscanf(s,"%dx^%dy^%d%n",&t->k,&t->nx,&t->ny,&m) == 3)
	return m;
	return 0;
}

/* ��������  2x^3y^4     5x^6y^7   77x^88y^99
	���Ǻ�����Բ���*/ 
