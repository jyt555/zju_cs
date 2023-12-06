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
    static bool isDraw = FALSE;//�Ƿ��ڻ��������� 
 	mx = ScaleXInches(x);//��ȡ��ǰ���λ������ 
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
			  	DrawLine(mx-lx,my-ly);//��ͼ 
			  	MovePen(mx,my);
			} 
            break;
    }	
    lx=mx;//��һʱ������ת������һʱ������ 
    ly=my;
}
 
 
