#include <stdio.h>
int main()
{
	unsigned char data[4];
	FILE *fp;
	fp = fopen("zju.bmp", "rb");
	fseek(fp, 0x12, SEEK_SET);
	fread(data, 4, 1, fp);
	printf("Í¼Æ¬¿í¶ÈÎª%02x%02x%02x%02xÏñËØ\n",data[3],data[2],data[1],data[0]);
	fseek(fp, 0x16, SEEK_SET);
	fread(data, 4, 1, fp);
	printf("Í¼Æ¬¸ß¶ÈÎª%02x%02x%02x%02xÏñËØ\n",data[3],data[2],data[1],data[0]);
	return 0;
}

