#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNUM 10

typedef char *string;
/////////////////////////

string GetWord(void);
int ReadStrings(string pcolors[]);
void sort(string pcolors[], int n);
int FindMin(string pcolors[], int begin, int end);
void swap(string pcolors[], int p, int q);

int main(void)
{
    string pcolors[MAXNUM];
    int n;

    n = ReadStrings(pcolors);
    sort(pcolors, n);
    printf("After sort: \n");
    for (int i = 0; i < n; i++) printf("%s\n", pcolors[i]);
    for (int i = 0; i < n; i++) free(pcolors[i]);

    return 0;
}

#define MAXWORD 1024

string GetWord(void)
{
    char buf[MAXWORD];
	string word;
	
	scanf("%s", buf);
	word = (string)malloc(strlen(buf)+1);
	strcpy(word, buf);
	
    return word;
}

int ReadStrings(string pcolors[])
{
    int n;
    string ptr;

    n = 0;
    while (1) {
    	ptr = GetWord();
    	if (strcmp(ptr, "#") == 0) {
    		free(ptr);
    		break;
		}
        pcolors[n++] = ptr; 
    }

    return n;
}

void sort(string pcolors[], int n)
{
    int minindex;

    for (int i = 0; i < n-1; i++) {
        minindex = FindMin(pcolors, i, n-1);
        if (minindex != i) swap(pcolors, i, minindex);
    }
}

int FindMin(string pcolors[], int begin, int end)
{
    int minindex;

    minindex = begin;
    for (int i = begin+1; i <= end; i++) {
        if (strcmp(pcolors[i], pcolors[minindex]) < 0) minindex = i;
    }
    return minindex;
}

void swap(string pcolors[], int p, int q)
{
    string temp;

    temp = pcolors[p];
    pcolors[p] = pcolors[q];
    pcolors[q] = temp;
}
