#include <stdio.h>
#include <stdlib.h>

struct ListNode {
    int data;
    struct ListNode *next;
};

struct ListNode *createlist(); /*裁判实现，细节不表*/
struct ListNode *mergelists(struct ListNode *list1, struct ListNode *list2);
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
    struct ListNode  *list1, *list2;

    list1 = createlist();
    list2 = createlist();
    list1 = mergelists(list1, list2);
    printlist(list1);
    
    return 0;
}
struct ListNode *createlist()
{
	int data;
	struct ListNode *p, *head, *tail;
	
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
/* 你的代码将被嵌在这里 */
/*
struct ListNode *mergelists(struct ListNode *list1, struct ListNode *list2)
{
	struct ListNode *p, *q, *s, *m;
	p = list1;
	q = list2; 
	s = m = NULL;
	
    if(!p && !q)
		return s;
		
	if(p->data < q->data){
		s = p;
		p = p->next;
	}
	else{
		s = q;
		q = q->next;
	}
	
	m=s;
	
	for(;p||q;){
		if(p == NULL){
			m->next = q;
			break;
		}
		if(q == NULL){
			m->next = p;
			break;
		}
		if(p->data < q->data){
			m->next = p;
			p = p->next;
			m = m->next;
			m->next = NULL;
		}
		else{
			m->next = q;
			q = q->next;
			m = m->next;
			m->next = NULL;
		}
	}
	return s;
}


*/
/*
struct ListNode *mergelists(struct ListNode *list1, struct ListNode *list2)
{
	struct ListNode *p1,*p2,*p12,*p22,*head;
	
	p1 = list1;
	p2 = list2;

	if(p1 == NULL)return p2;
	else p12 = p1->next;
	
	if(p2 == NULL)return p1;
	else p22 = p2->next;
	
	if(p1->data > p2->data)head = p2;
	else head = p1;
	
	if(p1->data > p2->data)head = p2;
	else head = p1;
	
	while(1)
	{
		if(p12 != NULL)p12 = p1->next;
		if(p22 != NULL)p22 = p2->next;
		if(p12 == NULL && p22 == NULL)break;
		
		if(p1->data > p2->data){
			if(p22 != NULL)p1->next = p22;
			p2->next = p1;
		}else{
			if(p12 != NULL)p2->next = p12;
			p1->next = p2;
		}
		p1 = p12;
		p2 = p22;
	}
	
	return head;
}*/

/*
struct ListNode *mergelists(struct ListNode *list1, struct ListNode *list2)
{
	struct ListNode *p1,*p2,*p12,*p22,*head,*tail;
	
	p1 = list1;
	p2 = list2;

	if(p1 == NULL)return p2;
	else p12 = p1->next;
	
	if(p2 == NULL)return p1;
	else p22 = p2->next;
	
	if(p1->data > p2->data)head = p2;
	else head = tail = p1;
	
	while(1)
	{
		if(tail == p1)
		for( ;p2;p2 = p2->next){
			if(p1->data < p2->data){
				tail = p1->next = p2;
				p1 = p12;
				if(p12 == NULL)break;
				p12 = p12->next;
			}else{
				tail = p2 = p22;
				if(p22 == NULL)break;
				p22 = p22->next;
			}
		}
		
		if(tail == p2)
		for( ;p1;p1 = p1->next){
			if(p2->data < p1->data){
				tail = p2->next = p1;
				p2 = p22;
				if(p22 == NULL)break;
				p22 = p22->next;
			}else{
				tail = p1 = p12;
				if(p12 == NULL)break;
				p12 = p12->next;
			}
		}
		
		if(p12 == NULL){
			if(tail == p2)tail->next = p1;
			break;
		}
		if(p22 == NULL){
			if(tail == p1)tail->next = p2;
			break;
		}
		
	}
	
	return head;
}
*/

struct ListNode *mergelists(struct ListNode *list1, struct ListNode *list2)
{
    struct ListNode *head,*p,*px,*temp;
    if(list1==NULL)		return list2;
    if(list2==NULL)		return list1;

    if(list1->data >= list2->data)
    {
        p = head = list2;
        px = list1;
    }
    else
    {
        p = head = list1;
        px = list2;
    }

    while(1)
    {
        while(p -> next != NULL && p->next->data <= px->data)
        {
            p = p->next;
        }
 
        if(p -> next == NULL){
            p -> next = px;
            return head;
        }else{
            temp = p->next;
            p -> next = px;
            px = temp; 
        }
    }
}


