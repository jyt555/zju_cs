#include <stdio.h>
#define MAXNUM 1000

static int N;

void partition(int n, int a[], int count);
void printa(int a[], int count); 

int main(void)
{
	int a[MAXNUM];
    
	printf("Enter N: ");
    scanf("%d", &N);
    partition(N, a, 0);
    return 0;
}

void partition(int n, int a[], int count)
{
    int i;
	
	if (n == 0) {
    	printa(a, count);
    } else {
        for (i = n; i > 0; i--) {
            if (count == 0 || i <= a[count-1]) {
                a[count] = i;
                partition(n-i, a, count+1);
            }
        }
    }
}

void printa(int a[], int count)
{
	int i;
	
	if (count <= 1) return;//do not print out the case: N = N
	printf("%d = ", N); 
	for (i = 0; i < count; i++) {
		printf("%d%s", a[i], i == count-1 ? "\n" : "+");
	}
}
