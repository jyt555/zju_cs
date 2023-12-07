#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int data;
    struct ListNode *next;
};

struct ListNode *readlist();
struct ListNode *deletem( struct ListNode *L, int m );
void printlist( struct ListNode *L )
{
     struct ListNode *p = L;
     while (p) {
           printf("%d ", p->data);
           p = p->next;
     }
     printf("\n");
}

int main()
{
    int m;
    struct ListNode *L = readlist();
    scanf("%d", &m);
    L = deletem(L, m);
    printlist(L);

    return 0;
}

/* 你的代码将被嵌在这里 */
struct ListNode *readlist()
{
	int data;
	struct ListNode *head, *tail, *p; 
	
	head = tail = NULL;
	
	while(1){
		scanf("%d",&data);
		if(data == -1)break;
		
		p = (struct ListNode* )malloc(sizeof(struct ListNode));
		p->data = data;
		p->next = NULL;
		
		if(head == NULL)head = p;
		else tail->next = p;
		tail = p;
	}
	
	return head;
}


struct ListNode *deletem( struct ListNode *L, int m )
{
	struct ListNode *head, *tail, *p; 
	head = tail = p = L;
	
	while(1){
		if(tail == NULL)return NULL;
		else if(tail->next == NULL){
			if(tail->data == m){
				if(head == tail)return NULL;
				p = tail;
				free(p);
				tail = NULL;
			}
			return head;
		}
		
		if(p->data == m){
			if(head == p){
				head = tail = head->next;
				free(p);
				p = head;
			}else{
				tail->next = p->next;
				free(p);
				p = tail->next;
			}
		}else{
			if(head == p) p = head->next;
			else{
				tail = p;
				p = p->next;
			}
		}
	}
	return head;
}
/*
struct ListNode *deletem( struct ListNode *L, int m )
{
	struct ListNode *head, *tail, *p; 
	head = tail = L;
	p = NULL;
	
	while(1){
		if(head->data == m){
			p = head;
			head = tail = head->next;
			free(p);
		}else{
			if(p == NULL) p = head->next;
			if(p->data == m){
				tail->next = p->next;
				tail = tail->next;
				p->next = NULL;
				free(p);
			}else tail = p;
		}
		if(tail == NULL || tail->next == NULL&&tail->data != m)break;
	//	else if(tail->data == m)return NULL;
		else p = tail->next;
	}
	return head;
}

/*

struct ListNode *deletem( struct ListNode *L, int m )
{
	struct ListNode *p1, *p2;
	
	if(L == NULL)return NULL;
	p1 = L;
	while(p1->data != m && p1->next != NULL){
		p2 = p1;
		p1 = p1->next;
	}
	if(p1->data == m){
		if(p1 == L)L = p1->next;
		else p2->next = p1->next;
		free(p1);
	}
	return L;
}

											ppt

*/

