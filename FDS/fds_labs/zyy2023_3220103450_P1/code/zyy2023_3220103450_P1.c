#include <stdio.h>
#include <math.h>
#include <time.h>
#define K 1000        /* the program runs K times */
clock_t start, stop;
double duration, total_time;

/* function declaration: */
double func1_multiplication(double x, int n);
/* algorithm 1: use N - 1 multiplications */
double func2_recursion(double x, int n);
/* algorithm 2-1: recursive version of algorithm 2, from Figure 2.11 in textbook */
double func3_iteration(double x, int n);
/* algorithm 2-2: iterative version of algorithm 2 */
int even(int n);
/* function: judge whether n is an even, if it's return 1, else return 0 */

int main(void)
{
    int i, j;
    int n[8] = {1000,5000,10000,20000,40000,60000,80000,100000};    /* different n for test (total eight values of n)*/
    double x = 1.0001;      /* base number */
    double pow;             /* the result */

    printf("k = %d\n",K);   /* print the value of K */
    for(i = 0;i < 8;i++){    /* change the value of n */
        /* run function 1 */
        start = clock();                /* records the ticks at the beginning of the function call */
        for(j = 0;j < K;j++)            /* repeat the function calls for K times */
            pow = func1_multiplication(x, n[i]);
        stop = clock();                 /* records the ticks at the end of the function call */
        total_time = ((double)(stop - start))/CLOCKS_PER_SEC;       /* CLOCK_PER_SEC is a built-in constant = ticks per second */
        duration = total_time / K;      /* duration is the actual running time of this algorithm */
        printf("n = %6d, 1-multiplication: Ticks = %5d, Total Time = %2f seconds, Duration = %.10f seconds.\n",n[i],(int)(stop-start),total_time,duration);
        /* print the kind of algorithm, ticks, total time and duration */

        /* run function 2: recursive version */
        start = clock();
        for(j = 0;j < K;j++)
            pow = func2_recursion(x, n[i]);
        stop = clock();
        total_time = ((double)(stop - start))/CLOCKS_PER_SEC;
        duration = total_time / K;
        printf("n = %6d, 2-recursion     : Ticks = %5d, Total Time = %2f seconds, Duration = %.10f seconds.\n",n[i],(int)(stop-start),total_time,duration);

        /* run function 3: iterative version */
        start = clock();
        for(j = 0;j < K;j++)
            pow = func3_iteration(x, n[i]);
        stop = clock();
        total_time = ((double)(stop - start))/CLOCKS_PER_SEC;
        duration = total_time / K;
        printf("n = %6d, 3-iteration     : Ticks = %5d, Total Time = %2f seconds, Duration = %.10f seconds.\n",n[i],(int)(stop-start),total_time,duration);
    }
    return 0;
}

/* algorithm 1: use N - 1 multiplications */
double func1_multiplication(double x, int n)
{
    int i;
    double pow = x;
    for(i = 1;i < n;i++)    pow *= x;    /* n-1 times */
    return pow;
}

/* algorithm 2-1: recursive version of algorithm 2, from Figure 2.11 in textbook */
double func2_recursion(double x, int n)
{
    if(n == 0)  return 1.0;
    if(n == 1)  return x;
    if(even(n))
        return ( func2_recursion(x*x, n/2 ));       /* the case that n is an even */
    else
        return ( func2_recursion(x*x, n/2) * x );   /* the case that n is an odd */
}

/* algorithm 2-2: iterative version of algorithm 2 */
double func3_iteration(double x, int n)
{
    int counter = 0;        /* record how many times the program should run */
    int remainder[100];     /* record each time current n is even or odd */
    double pow = x;

    if(n == 0)  return 1.0;
    while(n != 1){
        remainder[++counter] = n % 2;   /* counter increments and record the remainder */
        n /= 2;
    }
    while(counter != 0){
        if(remainder[counter])  pow = pow * pow * x;        /* current n is an odd */
        else                    pow = pow * pow;            /* current n is an even */
        counter--;
    }
    return pow;
}

/* function: judge whether n is an even, if it's return 1, else return 0 */
int even(int n)
{
    if(n % 2 == 0) return 1;
    return 0;
}