#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int data;
    struct ListNode *next;
};

struct ListNode *createlist(); /*����ʵ�֣�ϸ�ڲ���*/
struct ListNode *reverse( struct ListNode *head );
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
    struct ListNode  *head;

    head = createlist();
    head = reverse(head);
    printlist(head);
    
    return 0;
}

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
/* ��Ĵ��뽫��Ƕ������ */
struct ListNode *reverse( struct ListNode *head )
{
	struct ListNode *p0, *p1, *p2;
	p0 = head;
	p2 = NULL;
	if(p0 != NULL)p1 = p0->next;
	else return NULL;
	
	while(1){
		p0->next = p2;
		if(p1 == NULL)break;
		p2 = p0;
		p0 = p1;
		p1 = p1->next;
	}
	return p0;
}
