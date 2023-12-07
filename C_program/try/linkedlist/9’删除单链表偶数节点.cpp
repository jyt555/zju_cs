#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int data;
    struct ListNode *next;
};

struct ListNode *createlist();
struct ListNode *deleteeven( struct ListNode *head );
void printlist( struct ListNode *head )
{
     struct ListNode *p = head;
     while (p) {
           printf("%d ", p->data);
           p = p->next;
     }
     printf("\n");
}

int main()
{
    struct ListNode *head;

    head = createlist();
    head = deleteeven(head);
    printlist(head);

    return 0;
}

/* 你的代码将被嵌在这里 */
struct ListNode *createlist()
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
struct ListNode *deleteeven( struct ListNode *head )
{
	struct ListNode *p1, *p2;
	
	if(head == NULL)return NULL;
	p1 = p2 = head;
	
	while(1){
		if(p1->next == NULL){
			if(p1->data % 2)return head;
			else if(p1 != p2){
				p2->next = NULL;
				free(p1);
				return head;
			}else return NULL;
		}
		if(p1->data % 2){
			if(p1 == head){
				p1 = p1->next;
			}else{
				p2 = p1;
				p1 = p1->next;
			}
		}else{
			if(p1 == head){
				p1 = p2 = head = p1->next;
			}else{
				p2->next = p1->next;
				free(p1);
				p1 = p2->next;
			}
		}
	}
}
