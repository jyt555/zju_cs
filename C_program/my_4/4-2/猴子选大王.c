#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int num;
    struct Node *next;
}monkey;

monkey *createLink(int n)		//创建链表 
{
    int i = 0;
    monkey *head = NULL, *cir = NULL;
    
    head = (monkey*)malloc(sizeof(monkey));
    head->num = 1;
    head->next = NULL;
    cir = head;

    for(i = 1;i < n;i++){
        monkey *p = (monkey*)malloc(sizeof(monkey));
        p->num = i + 1;
        p->next = NULL;
        cir->next = p;
        cir = cir->next;
    }
    cir->next = head;			//首尾相连
    return head;
}

void findAndDel(monkey *head)
{
    int i;
	monkey *p = head, *tail = head;

    while(tail->next != head){
        tail = tail->next;
    }

    while(p->next != p){
        for(i = 1;i < 3;i++){
            tail = p;
            p = p->next;
        }
        tail->next = p->next;
        free(p);
        p = tail->next; 
    }
   
    printf("%d ",p->num);
    free(p);
}


int main(void)
{
    int n;
    
    monkey *head = NULL;
    scanf("%d",&n);
    head = createLink(n);
    findAndDel(head);
    
    return 0;
}


