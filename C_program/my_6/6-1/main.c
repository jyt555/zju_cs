#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
 
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
 
int cx,cy;
 
void MouseEventProcess(int x,int y,int button,int event); 
 
void Main()
{
    InitGraphics();        	
	cx = GetWindowWidth();
    cy = GetWindowHeight();
	registerMouseEvent(MouseEventProcess);
    SetPenSize(1);
}

void MouseEventProcess(int x,int y,int button,int event)
{ 
	static double lx=0.0, ly=0.0;
    double mx, my;
    static bool isDraw = FALSE;//是否在画画进程中 
 	mx = ScaleXInches(x);//获取当前鼠标位置坐标 
 	my = ScaleYInches(y);
 
    switch (event) {
        case BUTTON_DOWN:
   		 	if (button == LEFT_BUTTON){
				  isDraw = TRUE;	
   			} 
            break;
    	case BUTTON_DOUBLECLICK:
			break;
        case BUTTON_UP:
  		 	if (button == LEFT_BUTTON) isDraw = FALSE;
            break;
        case MOUSEMOVE:
			if (isDraw) {
			  	DrawLine(mx-lx,my-ly);//画图 
			  	MovePen(mx,my);
			} 
            break;
    }	
    lx=mx;//上一时刻坐标转化成这一时刻坐标 
    ly=my;
}
 
 
