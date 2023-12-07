#include <stdio.h>

// ��ʾ������ͼ��
void ShowImage(unsigned char high, unsigned char low, FILE *f);

int main()
{
    char high, low;
   
    scanf(" %c%c", &high, &low);
    FILE *f = fopen("HZK16.DAT", "rb");
    if (f)
    {
        ShowImage(high, low, f);
        fclose(f);
    }
    return 0;
}

/* ���ύ�Ĵ��뽫��Ƕ������ */
void ShowImage(unsigned char high, unsigned char low, FILE *f)
{
	int i,j;
	unsigned char buffer[32];
	long int move;
	
	move = ((high - 0xa1) * 94 + low - 0xa1) * 32;
	
	fseek(f,move,0);
	
	for(i = 0;i < 32;i++) buffer[i] = fgetc(f);
	
	for(i = 0;i < 32;i++)
	{
		for(j = 0;j < 8;j++)
		{
			if(buffer[i] & 0x80)printf("��");
			else printf("��");
			
			buffer[i] <<= 1;
		}
		if(i % 2)printf("\n");
	}
}
