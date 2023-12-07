
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>
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

#define PI 3.1415926535
#define len 1.5

double deg = 0;
double rad(double degree);
void forward(double distance);	//
void turn(double angle);		//
void move(double distance);		//
void DrawGraphics();
void DrawRegularHexagon();

// Main
void Main()
{
	double cx, cy;
	double l = 1.0;
	
	InitGraphics();
	cx = GetWindowWidth() / 2;
    cy = GetWindowHeight() / 2;
    
    MovePen(cx, cy);
    turn(30);
    DrawGraphics();
}

void forward(double distance)
{
	DrawLine(distance*cos(rad(deg)), distance*sin(rad(deg)));
}
void turn(double angle)
{
	deg += angle;
}
void move(double distance)
{
	double x, y;
	x = GetCurrentX();
	y = GetCurrentY();
	
	MovePen(x+distance*cos(rad(deg)), y+distance*sin(rad(deg)));
}
// turn rad to degree
double rad(double degree)
{
	return (degree*2*PI)/360;
}
//draw the patten
void DrawGraphics()
{
	int i;

	for(i = 0;i < 18;i++){
		DrawRegularHexagon();
		turn(20);
	}
}
// draw a regular hexagon
void DrawRegularHexagon()
{
	int i;
	for(i = 0;i < 6;i++){
		turn(-60);
		forward(len);
	}
}
