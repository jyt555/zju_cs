#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
// ɾ��ע��

/* ��Ȼ�����Լ���ע�Ͷ�ɾ���ɾ�������ptaȫ����...
 *�������
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
        printf("%s �޷���!\n", sfile);
    }
    dfile = fopen(dname, "wb");
    if (!dfile)
    {
        printf("%s �޷���!\n", dfile);
    }
 
    if (sfile && dfile)
    {
        printf("��������...");
        Pack(sfile, dfile);
        puts("�������!");
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
    #define IN_QUOT 3//���ַ���  
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
        }else if(state == IN_MULTI)//�ڶ���ע�� 
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
        }else if(state == IN_SINGLE)//����ע�� 
        { 
 
            if(c == '\n')//���� 
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
            if(c == '\"')//�ַ�������  
            { 
                fputc(c,dst); 
                state = 0;
            }else if(c == '\\')//ת���ַ�,�ų�\\"  
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

