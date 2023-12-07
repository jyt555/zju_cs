#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ListNode {
    char code[8];
    struct ListNode *next;
};

struct ListNode *createlist(); /*����ʵ�֣�ϸ�ڲ���*/
int countcs( struct ListNode *head );

int main()
{
    struct ListNode  *head;

    head = createlist();
    printf("%d\n", countcs(head));
    
    return 0;
}


struct ListNode *createlist()
{
	char code[8]; 
	struct ListNode *head, *tail, *p; 
	
	head = tail = NULL;
	
	while(1){
		scanf("%s",code);
		if(strcmp(code,"#") == 0)break;
		
		p = (struct ListNode* )malloc(sizeof(struct ListNode));
		strcpy(p->code,code);
		p->next = NULL;
		
		if(head == NULL)head = p;
		else tail->next = p;
		tail = p;
	}
	
	return head;
}
/* ��Ĵ��뽫��Ƕ������ */
int countcs( struct ListNode *head )
{
	int count = 0;
	struct ListNode  *p;
	p = head;
	
	while(1){
		if(p != NULL){
			if(p->code[1] == '0'&&p->code[2] == '2')count++;
		}else break;
		p = p->next;
	}
	
	return count;
}
