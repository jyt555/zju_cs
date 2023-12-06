/*��֪�����⣺
 *��������ʱ�����������һ���ֽ�Ϊ��λ,��ʱɾ����Ժ���������Ӱ��
 */ 

#include<ctype.h>
#include<windows.h>
#include<winuser.h>
#include"graphics.h"
#include"extgraph.h"

#define BUFFERSIZE 200000

void CharProcess(char);
void KeyProcess(int,int);
void TimerProcess(int);

void OperateString(int opr);
void FormNewBuffer(char ch);
double getlen();
void FormBackspaceBuffer();//���ɽ���backspace����´� 
void FormDeleteBuffer();//���ɽ���delete����´� 
void RefreshCursor();//������ 
void RewriteCursor();//��ʾ��� 

static double cy;
static char buffer[BUFFERSIZE],temp[BUFFERSIZE];
static int cp,cstate;//cp��ʾ��ǰλ�� 
static double precur,nowcur;

//main���� 
void Main()
{
	InitGraphics();
	cy = GetWindowHeight() - GetFontHeight();//��ǰy���꣨����������Ϊ׼�� 

	registerKeyboardEvent(KeyProcess);
	registerCharEvent(CharProcess);
	registerTimerEvent(TimerProcess);
	startTimer(1,600);
}

// �ص����� 
void CharProcess(char ch)
{
	RefreshCursor();
	if(ch&&'\n'&&ch!='\r'&&ch!='\b'){	
		OperateString(0);//���ԭ������ 
		FormNewBuffer(ch);//�����µ��ַ� 
		OperateString(1);//��������� 
	}
}
void KeyProcess(int key,int event)
{
	RefreshCursor();
	if(event == KEY_DOWN) //����ʱ��� 
	switch(key){
		case VK_LEFT:
			if(cp) cp--;
			break;
		case VK_RIGHT:
			if(buffer[cp]) cp++;
			break;
		case VK_RETURN://���»س�ʱ�ڿ���̨������ʾ 
			cy -= GetFontHeight();
			OpenConsole();
			puts(buffer);//����̨��������ַ� 
			buffer[0] = 0;
			cp = 0;
			break;
		case VK_BACK://����backspaceɾ����� 
			OperateString(0);
			FormBackspaceBuffer();
			OperateString(1);
			break;
		case VK_DELETE://����deleteɾ���ұ� 
			OperateString(0);
			FormDeleteBuffer();
			OperateString(1);
			break;
	}
	RewriteCursor();
}
void TimerProcess(int timer)
{
	if(cstate) RefreshCursor();
	else RewriteCursor();
}

//
void OperateString(int opr)//����ַ�������ַ� 
{
	MovePen(0,cy);
	SetEraseMode(!opr);
	DrawTextString(buffer);
}
void FormNewBuffer(char ch)
{
	sprintf(temp,buffer+cp);
	buffer[cp++] = ch;
	sprintf(buffer+cp,temp);
}
double getlen()
{
	int i;
	for(i = 0;i < cp;++i) temp[i] = buffer[i];
	temp[cp] = 0;
	return TextStringWidth(temp);
}

//ɾ���ַ� 
void FormBackspaceBuffer()
{
	sprintf(temp,buffer+cp);
	if(cp) cp--;
	sprintf(buffer+cp,temp);
}
void FormDeleteBuffer()
{
	if(buffer[cp]){
		sprintf(temp,buffer+cp+1);
		sprintf(buffer+cp,temp);
	}
}

//��� 
void RefreshCursor()//������ 
{
	MovePen(precur,cy);
	SetEraseMode(1);
	DrawLine(0,GetFontHeight());
	cstate = 0;
}
void RewriteCursor()//��ʾ��� 
{
	precur = nowcur = getlen();
	MovePen(nowcur,cy);
	SetEraseMode(0);
	DrawLine(0,GetFontHeight());
	cstate = 1;
}
