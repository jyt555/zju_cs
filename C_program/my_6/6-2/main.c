/*已知的问题：
 *中文输入时，光标仍是以一个字节为单位,此时删除会对后面文字有影响
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
void FormBackspaceBuffer();//生成进行backspace后的新串 
void FormDeleteBuffer();//生成进行delete后的新串 
void RefreshCursor();//清除光标 
void RewriteCursor();//显示光标 

static double cy;
static char buffer[BUFFERSIZE],temp[BUFFERSIZE];
static int cp,cstate;//cp表示当前位置 
static double precur,nowcur;

//main函数 
void Main()
{
	InitGraphics();
	cy = GetWindowHeight() - GetFontHeight();//当前y坐标（以文字下限为准） 

	registerKeyboardEvent(KeyProcess);
	registerCharEvent(CharProcess);
	registerTimerEvent(TimerProcess);
	startTimer(1,600);
}

// 回调函数 
void CharProcess(char ch)
{
	RefreshCursor();
	if(ch&&'\n'&&ch!='\r'&&ch!='\b'){	
		OperateString(0);//清除原先内容 
		FormNewBuffer(ch);//加入新的字符 
		OperateString(1);//输出新内容 
	}
}
void KeyProcess(int key,int event)
{
	RefreshCursor();
	if(event == KEY_DOWN) //按键时输出 
	switch(key){
		case VK_LEFT:
			if(cp) cp--;
			break;
		case VK_RIGHT:
			if(buffer[cp]) cp++;
			break;
		case VK_RETURN://按下回车时在控制台窗口显示 
			cy -= GetFontHeight();
			OpenConsole();
			puts(buffer);//控制台窗口输出字符 
			buffer[0] = 0;
			cp = 0;
			break;
		case VK_BACK://按下backspace删除左边 
			OperateString(0);
			FormBackspaceBuffer();
			OperateString(1);
			break;
		case VK_DELETE://按下delete删除右边 
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
void OperateString(int opr)//输出字符或清除字符 
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

//删除字符 
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

//光标 
void RefreshCursor()//清楚光标 
{
	MovePen(precur,cy);
	SetEraseMode(1);
	DrawLine(0,GetFontHeight());
	cstate = 0;
}
void RewriteCursor()//显示光标 
{
	precur = nowcur = getlen();
	MovePen(nowcur,cy);
	SetEraseMode(0);
	DrawLine(0,GetFontHeight());
	cstate = 1;
}
