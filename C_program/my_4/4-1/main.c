#include <stdio.h>
#include <stdlib.h>

#define STUMAXN 50002
#define COUMAXN 2002
#define NAMELEN 12

//链表内彼此相连的结点 
typedef struct node{
	int data;
	struct node* next;
}node;

//外层链表 
typedef struct list{
	node *head, *tail;
}list;

list studentList[STUMAXN], courseList[COUMAXN];
int stuID[STUMAXN];
char *studentName[STUMAXN], *courseName[COUMAXN];

int couN = 0, stuN = 0;
char buffer[5];

//函数声明 
void add(list* L, const int c);	
char nextchar();
void Readin(FILE *fp);
void printcourse();
void printstudent();

//主函数 
int main(void)
{
	int i;
	FILE *fp;
	fp = fopen("实验04-1测试数据.txt","r");
	
	Readin(fp);
	printcourse();
	printstudent();
	
	return 0;
	
}

void add(list* L, const int c)//增加结点 
{
	node *p = (node*)malloc(sizeof(node));
	
	p->data = c;		//结点中储存常熟，在coursename和studentname里面找 
	p->next = NULL;
	
	if(L->head == NULL)L->head = L->tail = p;
	else L->tail = L->tail->next = p;
}

char nextchar(FILE* fp)	//跳过空白字符 
{
	char ch;
	while((ch=fgetc(fp)) == ' ');
	ungetc(ch,fp);
	return ch;
}

void Readin(FILE *fp)	//读入文件数据 
{
	int i;
	fscanf(fp,"%*s%*s");	//跳过课程名前面的 Number和Name 
	while(nextchar(fp) != '\n')	
	{
		courseName[couN] = (char*)malloc(sizeof(char)*NAMELEN);
		fscanf(fp,"%s",courseName[couN++]);//读入课程名称 
	}
	while(fscanf(fp,"%d",&stuID[stuN]) != EOF)//读入学生学号 
	{
		studentName[stuN] = (char*)malloc(sizeof(char)*NAMELEN);
		fscanf(fp,"%s",studentName[stuN]);//读入学生姓名 
		for(i = 0;i < couN;i++){
			fscanf(fp,"%s",buffer);
			if(buffer[0] == 'Y'){
				add(&studentList[stuN],i);//增加结点 
				add(&courseList[i],stuN);
			}
		}
		stuN++;
	}
}

void printcourse()//打印courselist 
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

void printstudent()//打印studentlist 
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
