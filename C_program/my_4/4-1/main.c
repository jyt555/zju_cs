#include <stdio.h>
#include <stdlib.h>

#define STUMAXN 50002
#define COUMAXN 2002
#define NAMELEN 12

//�����ڱ˴������Ľ�� 
typedef struct node{
	int data;
	struct node* next;
}node;

//������� 
typedef struct list{
	node *head, *tail;
}list;

list studentList[STUMAXN], courseList[COUMAXN];
int stuID[STUMAXN];
char *studentName[STUMAXN], *courseName[COUMAXN];

int couN = 0, stuN = 0;
char buffer[5];

//�������� 
void add(list* L, const int c);	
char nextchar();
void Readin(FILE *fp);
void printcourse();
void printstudent();

//������ 
int main(void)
{
	int i;
	FILE *fp;
	fp = fopen("ʵ��04-1��������.txt","r");
	
	Readin(fp);
	printcourse();
	printstudent();
	
	return 0;
	
}

void add(list* L, const int c)//���ӽ�� 
{
	node *p = (node*)malloc(sizeof(node));
	
	p->data = c;		//����д��泣�죬��coursename��studentname������ 
	p->next = NULL;
	
	if(L->head == NULL)L->head = L->tail = p;
	else L->tail = L->tail->next = p;
}

char nextchar(FILE* fp)	//�����հ��ַ� 
{
	char ch;
	while((ch=fgetc(fp)) == ' ');
	ungetc(ch,fp);
	return ch;
}

void Readin(FILE *fp)	//�����ļ����� 
{
	int i;
	fscanf(fp,"%*s%*s");	//�����γ���ǰ��� Number��Name 
	while(nextchar(fp) != '\n')	
	{
		courseName[couN] = (char*)malloc(sizeof(char)*NAMELEN);
		fscanf(fp,"%s",courseName[couN++]);//����γ����� 
	}
	while(fscanf(fp,"%d",&stuID[stuN]) != EOF)//����ѧ��ѧ�� 
	{
		studentName[stuN] = (char*)malloc(sizeof(char)*NAMELEN);
		fscanf(fp,"%s",studentName[stuN]);//����ѧ������ 
		for(i = 0;i < couN;i++){
			fscanf(fp,"%s",buffer);
			if(buffer[0] == 'Y'){
				add(&studentList[stuN],i);//���ӽ�� 
				add(&courseList[i],stuN);
			}
		}
		stuN++;
	}
}

void printcourse()//��ӡcourselist 
{
	int i;
	node *p;
	FILE *putcourse = fopen("courselist.txt","w");
	fprintf(putcourse,"The courselist of each student.\n\n");
	for(i = 0;i < stuN;i++)
	{
		fprintf(putcourse,"`ID %d, STUDENT %s: ",stuID[i],studentName[i]);
		for(p = studentList[i].head;p;p = p->next)
		fprintf(putcourse,"%s ",courseName[p->data]);
		
		fputc('\n',putcourse);
	}
}

void printstudent()//��ӡstudentlist 
{
	int i;
	node *p;
	FILE *putstudent = fopen("studentlist.txt","w");
	fprintf(putstudent,"The studentlist of each course.\n\n");
	for(i = 0;i < couN;i++)
	{
		fprintf(putstudent,"`COURSE %s: ",courseName[i]);
		for(p = courseList[i].head;p;p = p->next)
		fprintf(putstudent,"%s ",studentName[p->data]);
		
		fputc('\n',putstudent);
		fputc('\n',putstudent);
	}
}
