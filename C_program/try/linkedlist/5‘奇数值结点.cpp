#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int data;
    struct ListNode *next;
};

struct ListNode *readlist();
struct ListNode *getodd( struct ListNode **L );

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
    struct ListNode *L, *Odd;
    L = readlist();
    Odd = getodd(&L);
    printlist(Odd);
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

struct ListNode *getodd(struct ListNode **L)
{
	struct ListNode *p1, *p2, *odd, *oddtail;
	p2 = odd = oddtail = NULL;
	p1 = *L;
	
	while(1){
		if(p1->next == NULL){
			if(p1->data % 2){
				if(oddtail != NULL)oddtail->next = p1;
				else odd = *L;
				
				if(p2 != NULL)p2->next = NULL;
				else *L = NULL;
			}
			return odd;
		}
		if(p1 == *L){
			if(p1->data % 2){
				if(odd == NULL)odd = oddtail = p1;
				else oddtail->next = p1;
				
				oddtail = p1;
				*L = (*L)->next;
				p1->next = NULL;
				p1 = *L;
			}else{
				p2 = *L;
				p1 = p2->next;
			}
		}else{
			if(p1->data % 2){
				if(odd == NULL)odd = oddtail = p1;
				else oddtail->next = p1;
				
				oddtail = p1;
				p2->next = p1->next;
				p1->next = NULL;
				p1 = p2->next;
			}else{
				p2 = p1;
				p1 = p1->next;
			}
		}
	}
} 

/*

struct ListNode *getodd( struct ListNode **L )
{
	struct ListNode *p1, *p2, *odd, *oddtail;
	p2 = odd = oddtail = NULL;
	p1 = *L;
	
	while(1){
		if(p1->data % 2 == 1){
			if(odd == NULL){
				oddtail = odd = p2 = p1;
				*L = p1 = (*L)->next;
			}else{
				oddtail->next = p1;
				oddtail = p1;
				p2->next = p2->next->next;
				p1 = p2->next;
			}
		}else{
			p1 = p1->next;
			p2 = p2->next;
		}
		
		if(p1 == NULL){
			break;
			oddtail->next = NULL;
		}
	}
	return odd;
	
}

*/
