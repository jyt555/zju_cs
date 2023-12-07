#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
// 删除注释

/* 虽然它连自己的注释都删不干净，但是pta全过了...
 *这很难评
 */ 
void Pack(FILE *src, FILE *dst);
 
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
        Pack(sfile, dfile);
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
 
void Pack(FILE *src, FILE *dst) 
{ 
    #define IN_MULTI 1 
    #define IN_SINGLE 2 
    #define IN_SINGLE_QUOT 4 
    #define IN_QUOT 3//在字符串  
    int i = 0,j,k,state = 0; 
    char *str = (char *)malloc(sizeof(char)*10000); 
    char c,next; 
    while(!feof(src)) 
    { 
        c = fgetc(src); 
        if(c == EOF) 
            break; 
 
        if(state == 0) 
        { 
            switch(c){ 
                case '/': 
                    next = fgetc(src); 
                    if(next == '*')/**/ 
                    { 
                        state = IN_MULTI; 
                        break; 
                    } 
                    else if(next == '/')// 
                    { 
                        state = IN_SINGLE; 
                        break; 
                    }else 
                    { 
                        fputc(c,dst); 
                        fputc(next,dst); 
                        break; 
                    } 
                    break; 
                case '\'': 
                    fputc(c,dst); 
                    state = IN_SINGLE_QUOT;     
                    break; 
                case '\"': 
                    state = IN_QUOT;//" 
                    fputc(c,dst); 
                    break; 
                default: 
                    fputc(c,dst); 
                    break; 
 
            } 
        }else if(state == IN_MULTI)//在多行注释 
        { 
            if(c == '*') 
            { 
                next = fgetc(src); 
                if(next == '/') 
                { 
                    state = 0; 
                    fputc(' ',dst);     
                } 
            } 
        }else if(state == IN_SINGLE)//单行注释 
        { 
 
            if(c == '\n')//换行 
            { 
 
                    fputc(' ',dst); 
                    fputc(c,dst); 
                    state = 0; 
            } 
 
        }else if(state == IN_SINGLE_QUOT) 
        { 
            fputc(c,dst); 
            if(c == '\'') 
            { 
                state = 0; 
            } 
        }else if(state == IN_QUOT) 
        { 
            if(c == '\"')//字符串结束  
            { 
                fputc(c,dst); 
                state = 0;
            }else if(c == '\\')//转义字符,排除\\"  
            { 
                next = fgetc(src); 
                fputc(c,dst); 
                fputc(next,dst); 
            }else 
            { 
                fputc(c,dst); 
            } 
        } 
 
    } 
}

