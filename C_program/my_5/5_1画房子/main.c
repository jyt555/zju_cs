#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "graphics.h"
#include "genlib.h"
#include "conio.h"
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#define house_1_base 2
#define house_1_door_len 0.5
#define house_1_door_wid 1.6
#define house_1_win 0.4
#define house_1_chim 0.7
#define house_2_base 1.2
#define house_2_chim 0.2

/*
#define house_1_base 2
#define house_1_door_len (house_1_base / 4)
#define house_1_door_wid (house_1_base / 5 * 4)
#define house_1_win (house_1_base / 5)
#define house_1_chim (house_1_base / 3)
#define house_2_base 1.2
#define house_2_chim (house_2_base / 6)
*这里这样处理有问题 
*/

void DrawHouse1(double x, double y);
void DrawHouse2(double x, double y);
void DrawRectangle(double len, double wid);
void DrawRegularTriangle(double x, double y, double len);

/* Main */
void Main()
{
	double cx,cy;
	
	InitGraphics();
	cx = GetWindowWidth() / 2;
    cy = GetWindowHeight() / 2;
    
    DrawHouse1(cx-2*house_1_base, cy-2*house_2_chim);
    DrawHouse2(cx-house_2_base, cy);
} 

/* Draw the first house */
void DrawHouse1(double x, double y)
{
	//draw the basic part
	MovePen(x, y);
	DrawRectangle(house_1_base, house_1_base);
	
	//draw the roof
	DrawRegularTriangle(x, y, house_1_base);
	
	//draw the door
	MovePen(x+house_1_base/2, y-2*house_2_chim);
	DrawRectangle(house_1_door_len, house_1_door_wid);
	
	//draw the window
	MovePen(x+house_1_win, y-2*house_2_chim);
	DrawRectangle(house_1_win, house_1_win);
	
	//draw the chimney
	MovePen(x+house_2_chim/2, y+1.2*house_2_base);
	DrawRectangle(2.5*house_2_chim, house_2_chim);
	MovePen(x+house_2_chim, y+1.2*house_2_base-house_2_chim);
	DrawLine(0, -0.75*house_2_base);
	MovePen(x+2.5*house_2_chim, y+1.2*house_2_base-house_2_chim);
	DrawLine(0, -2*house_2_chim);
	
}

/* Draw the second house */
void DrawHouse2(double x, double y)
{
	int i,j;
	
	//draw the basic part
	MovePen(x, y);
	for(j = 0;j < 2;j++){
		for(i = 0;i < 4;i++){
		DrawRectangle(house_2_base,house_2_base);
		MovePen(x+(i+1)*house_2_base, y-j*house_2_base);
		}
		MovePen(x, y-house_2_base);
	}
	
	//draw the roof
	DrawRegularTriangle(x, y, house_2_base);
	DrawRegularTriangle(x+3*house_2_base, y, house_2_base);
	
	//draw the chimney
	MovePen(x+house_2_base, y+house_2_chim);
	for(i = 0;i < 6;i++){
		DrawRectangle(house_2_chim, house_2_chim);
		MovePen(x+house_2_base+2*(i+1)*house_2_chim, y+house_2_chim);
	}
}

//draw a rectangle
void DrawRectangle(double len, double wid)
{
	DrawLine(len, 0);
	DrawLine(0, -wid);
	DrawLine(-len, 0);
	DrawLine(0, wid);
}
//draw a regular triangle 
void DrawRegularTriangle(double x, double y, double len)
{
	MovePen(x, y);
	DrawLine(len, 0);
	DrawLine(-len/2, 0.866*len);
	DrawLine(-len/2, -0.866*len);
}
