/*����11-9����дһ������calc(f, a, b)�������ι�ʽ����f(x)��[a, b]��
 * ����ֵ���֡�
 * f1(x) = x * x
 * f2(x) = sin(x) / x
 * f3(x) = x * sin(x)
 */
 
/* ������ֵ���֣�����ָ����Ϊ��������ʾ���� */
#include <stdio.h>
#include <math.h>

double calc(double (*funcptr)(double x), double a, double b);  /*�ܿغ���ԭ��˵�� */
double f1(double x);
double f2(double x);
double f3(double x); 

int main(void)
{
    double result;
    double (*funcptr)(double x);

	funcptr = f1;
    result = calc(funcptr, 0.0, 1.0);        
    printf("1: resule=%.4f\n", result);
    funcptr = f2;
  	result = calc(funcptr, 1.0, 2.0);     
    printf("2: resule=%.4f\n", result);
  	result = calc(f3, 1.0, 2.0);     
    printf("3: resule=%.4f\n", result);

    return 0;
}

double calc(double funcptr(double x), double a, double b) 
{
    double z;
    
    z = (b-a)/2 * (funcptr(a) + funcptr(b));   
    
    return(z);
}

double f1(double x)
{   
    return x*x;
}

double f2(double x)
{
    return sin(x)/x;
}

double f3(double x)
{
	return x*sin(x);
}
