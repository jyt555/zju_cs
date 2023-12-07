#include <stdio.h>
#include <stdlib.h>

struct stud_node {
     int    num;
     char   name[20];
     int    score;
     struct stud_node *next;
};

struct stud_node *createlist();
struct stud_node *deletelist( struct stud_node *head, int min_score );

int main()
{
    int min_score;
    struct stud_node *p, *head = NULL;

    head = createlist();
    scanf("%d", &min_score);
    head = deletelist(head, min_score);
    for ( p = head; p != NULL; p = p->next )
        printf("%d %s %d\n", p->num, p->name, p->score);

    return 0;
}

/* 你的代码将被嵌在这里 */

/* 你的代码将被嵌在这里 */
#include <string.h> 
struct stud_node *createlist()
{
	int 	num;
	char 	name[21];
	int 	score;
	struct stud_node *p, *head, *tail;
	
	head = tail = NULL;
	
	while(1){
		scanf("%d",&num);
		if(num ==  0)break;
		scanf("%s %d",name,&score);
		
		p = (struct stud_node*)malloc(sizeof(struct stud_node));
		p->num = num;p->score = score;
		strcpy(p->name,name);
		p->next = NULL;
		
		if(head == NULL)head = tail = p;
		else tail->next = p;
		
		tail = p;
	}
	
	return head;
}

struct stud_node *deletelist( struct stud_node *head, int min_score )
{
	struct stud_node *p1, *p2, *h;
	int i;
	p1 = h = head;
	p2 = NULL;
	
	if(p1 == NULL)return h;
	while(p1->next != NULL){
		if(p1->score < min_score){
			if(p1 == h)
				h = p2 = p1->next;
			else 
				p2->next = p1->next;
			free(p1);
			p1 = p2->next;
		}else{
			if(p1 == h)
				p2 = h;
			else
				p2 = p1;
			p1 = p2->next;
		}
	}
	if(p1->next == NULL){
		if(p1->score < min_score){
			if(p2 == NULL)return NULL;	//p2 == NULL改成 p1 == h 就过了 
			else{
				p2->next = NULL;
				free(p1);
			}
		}
	}
	return h;
}
/*
struct stud_node *deletelist( struct stud_node *head, int min_score )
{
	struct stud_node *p1, *p2;
	p1 = head;
	
	while(p1->score < min_score){
		p2 = p1->next;
		
		if(p1 == head){
			head = head->next;
			free(p1);
		}else{
			p1->next = p1->next->next;
			free(p2);
		}
		p1 = p2;
	}
	
}

/*void input()
{
	int 	num;
	char 	name[20];
	int 	score;
	struct stud_node *p, *head, *tail;
	
	head = tail = NULL;
	
	while(1){
		scanf("%d",&num);
		if(num ==  0)break;
		scanf("%s %d",name,&score);
		
		p = (struct stud_node*)malloc(sizeof(struct stud_node));
		p->num = num;p->score = score;
		strcpy(p->name,name);
		p->next = NULL;
		
		if(head == NULL)head = tail = p;
		else tail->next = p;
		
		tail = p;
	} 

}*/
