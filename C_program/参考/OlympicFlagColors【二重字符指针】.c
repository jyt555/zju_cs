#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNUM 10
#define MAXLEN 100
 
typedef char *string;

string GetWord(void);
int ReadStrings(string pcolors[]);
void sort(string pcolors[], int n);
int FindMin(string pcolors[], int begin, int end);
void swap(string pcolors[], int p, int q);

int main(void)
{
    string pcolors[MAXNUM];
	string *ppcolors; //char **ppcolors;
    int n;

	ppcolors = pcolors;
    n = ReadStrings(ppcolors);
    sort(ppcolors, n);
    printf("After sort:\n");
    for (int i = 0; i < n; i++) printf("%s\n", *(ppcolors+i));
    for (int i = 0; i < n; i++) free(*(ppcolors+i));

    return 0;
}

string GetWord(void)
{
    char buf[MAXLEN];
    string word;

	scanf("%s", buf);
	word = (string)malloc(strlen(buf));
    strcpy(word, buf);
    
	return word;
}

int ReadStrings(string pcolors[])
{
    int n;

    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        pcolors[i] = GetWord(); 
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
