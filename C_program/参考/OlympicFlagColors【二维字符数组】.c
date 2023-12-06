#include <stdio.h>
#include <string.h>

#define MAXLEN 81

void sort(char colors[][MAXLEN], int n);
int FindMin(char colors[][MAXLEN], int begin, int end);
void swap(char colors[][MAXLEN], int p, int q);

int main(void)
{
    int i;
	char colors[][MAXLEN] = {"white", "yellow", "blue", "black", "red", "green"};
    int n = sizeof colors / sizeof colors[0];

    sort(colors, n);

    for (i = 0; i < n; i++) printf("%s\n", colors[i]);

    return 0;
}

void sort(char colors[][MAXLEN], int n)
{
    int minindex,i;

    for (i = 0; i < n-1; i++) {
        minindex = FindMin(colors, i, n-1);
        if (minindex != i) swap(colors, i, minindex);
    }
}

int FindMin(char colors[][MAXLEN], int begin, int end)
{
    int minindex,i;

    minindex = begin;
    for (i = begin+1; i <= end; i++) {
        if (strcmp(colors[i], colors[minindex]) < 0) minindex = i;
    }
    return minindex;
}

void swap(char colors[][MAXLEN], int p, int q)
{
    char buf[MAXLEN];

    strcpy(buf, colors[p]);
    strcpy(colors[p], colors[q]);
    strcpy(colors[q], buf);
}
