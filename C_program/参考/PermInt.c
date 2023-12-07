#include <stdio.h>

void PermInt(int a[], int n, int k);
void swap(int a[], int p, int q);

int main(void)
{
    int a[] = {1, 2, 3, 4};
    int n = sizeof a / sizeof a[0];

    PermInt(a, n, 0);

    return 0;
}

void PermInt(int a[], int n, int k)
{
    if (k == n) {
        for (int i = 0; i < n; i++) printf("%d", a[i]);
        printf("\n");
    } else {
        for (int i = k; i < n; i++) {
            swap(a, k, i);
            PermInt(a, n, k+1);
            swap(a, k, i); 
        }
    }
}

void swap(int a[], int p, int q)
{
    int t;

    t = a[p];
    a[p] = a[q];
    a[q] = t;
}
