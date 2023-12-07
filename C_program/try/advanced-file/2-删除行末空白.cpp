#include <stdio.h>
#include <ctype.h>
#include <string.h>

// 删除行末空白字符
void Cut(FILE *src, FILE *dst);

int main()
{
    char sname[1024], dname[1024];
    FILE *sfile, *dfile;

    gets(sname);
    gets(dname);

    sfile = fopen(sname, "rb");
    if (!sfile)
    {
        printf("%s 无法打开!\n", sfile);
    }
    dfile = fopen(dname, "wb");
    if (!dfile)
    {
        printf("%s 无法打开!\n", dfile);
    }

    if (sfile && dfile)
    {
        printf("正在整理...");
        Cut(sfile, dfile);
        puts("整理完成!");
    }

    if (sfile)
    {
        fclose(sfile);
    }
    if (dfile)
    {
        fclose(dfile);
    }
    return 0;
}

/* 你提交的代码将被嵌在这里 */
/* 
void Cut(FILE *src, FILE *dst) 
{
    int c;
    int previous_char = '\n';
    int n = 0;

    while ((c = fgetc(src)) != EOF) {
        if (previous_char == '\n') 
            n = 0;
        
        if (!isspace(c)) 
            n = 0;

        if (n && c == '\n') {
            n = 0;
            continue;
        }

        fputc(c, dst);

        if (isspace(c)) 
            n = 1;

        previous_char = c;
    }
}
*/
void Cut(FILE *src, FILE *dst) 
{
    char c;
    int previous_char = '\n';
    int flag = 1,n = 1;
    int head = 0,tail = 0;

    while ((c = fgetc(src)) != EOF) {
        if (previous_char == '\n'||previous_char == '\r')
        {
        	flag = 1;
        	head = 0;
		}     
        if (!head&&isspace(c))
        {
        	flag = 0;
        	head = n;
		}
		if(head&&!isspace(c))
		{
			tail = n;
			fseek(src,head-1,SEEK_SET);
			for(flag = head;flag < tail;flag++)
			{
				c = fgetc(src);
				fputc(c,dst);
			}
			n = n - 1; 
			head = tail = 0;
			flag = 1;
		}else if(flag||c == '\n')fputc(c, dst);

        previous_char = c;
        n++;
    }
}

