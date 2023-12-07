#include <stdio.h>
#include <stdlib.h>
#define w 200
#define h 200
void WriteBMP(char*img,const char* filename);
int main()
{
	char img[w*h*3];
	char *filename="test.bmp";
	int i;
	for(i=0; i<w*h*3; i++) img[i]=rand()%256;
	WriteBMP(img,filename);
	system("test.bmp");
	return 0;
}
void WriteBMP(char*img,const char* filename)
{
	int i=(w*3+3)/4*4;
	int bmi[]= {i*h+54,0,54,40,w,h,1|3*8<<16,0,i*h,0,0,100,0};
	FILE *fp = fopen(filename,"wb");
	fprintf(fp,"BM");
	fwrite(&bmi,52,1,fp);
	fwrite(img,1,i*h,fp);
	fclose(fp);
 }

