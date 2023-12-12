/* Author: Jiang Yutong; ID:3220103450; No:01 */
#include <stdio.h>
#define MAX 1000
int main(void)
{
    int n, i, num, count;
    int s1[MAX + 1], s2[MAX + 1];   // the two stacks
    int p1 = 0, p2 = 0;             // the pointers in the stacks
    char opr;

    scanf("%d\n",&n);
    for(i = 0;i < n;i++){
        scanf("%c %d\n",&opr,&num);
        count = 0;
        // printf("%d: %c %d %d %d\n",i,opr,num,p1,p2); // test

        if(opr == 'I')      // if enqueued, push it onto s1, increment the pointer P1
            s1[p1++] = num;
        else{               // if dequeued, check s2 first
            if(p2 == 0){    // if s2 is empty, transfer s1 to s2
                for(p2 = 0;p2 < p1;p2++) s2[p2] = s1[p1 - p2 - 1];
                count = 2 * p1;
                p1 = 0;     // set pointer of stack1 to zero
            }
            if(p2 == 0){     // if the queue is empty when dequeue is called, output 'ERROR'
                printf("ERROR\n");
                continue;
            }
            printf("%d %d\n",s2[--p2],++count); // dequeue the number
        }
    }
    return 0;
}