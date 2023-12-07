#include <stdio.h>
#include <stdlib.h>
#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

#define MAXSTUDENTS 50000
#define MAXCOURSES  2000

string courseNames[MAXCOURSES];//记录课程名字符串
int courseNumber = 0;//实际课程数

struct student {//学生记录类型
    string ID;//学号字符串
    string name;//姓名字符串
    int courseNo[MAXCOURSES+1];//所选课程序号，-1表示结束
};

struct student students[MAXSTUDENTS];//学生记录数组
int studentNumber = 0;//学生记录数

int ReadinCourseNames(FILE *fp, string courseNames[]);//从文本文件fp的第一行依次读取课程名
int ReadinStudentRecords(FILE *fp, struct student students[]);//从文本文件fp的第二行开始，依次读取每一条学生的记录信息
void PrintStudentCourses(struct student students[], int studentNumber);//打印每个学生的选课清单
void PrintCourseStudents(struct student students[], int studentNumber);//打印每门课的选课学生信息

int main(void)
{
    FILE *fp;
    fp = fopen("实验04-1测试数据.txt", "r");//以文本模式打开数据文件
    courseNumber = ReadinCourseNames(fp, courseNames);//读取第一行的课程名称，返回总课程数
    studentNumber = ReadinStudentRecords(fp, students);//读取全部学生的记录信息，返回总学生数
    PrintStudentCourses(students, studentNumber);//打印每个学生的选课清单
    PrintCourseStudents(students, studentNumber);//打印每门课的选课学生信息
    fclose(fp);

    return 0;
}

int ReadinCourseNames(FILE *fp, string courseNames[])
{
    char buf[20];
    int n, m;
    string line = ReadLine(fp);//读取第一行字符串
    /*
     *下面的sscanf表示从line指针所指向的字符串位置开始读取格式化数据，其中%n格式表示本次共成功读取了多少个字符，
     *该字符数存放在对应的整型变量中。注意：%n不实际读取字符。
     */
    sscanf(line, "%s%s%n", buf, buf, &m);//从line开始读掉Number和Name字符串，并将本次成功读取的字符个数存放在变量m中
    line += m;//调整line字符串指针到Name字段之后的位置，以便于下面开始读取表示是否选课的字符串Yes/No
    n = 0;
    //sscanf的返回值为本次实际读到的对象个数。注意：一个字符串算作1个对象
    while (sscanf(line, "%s%n", buf, &m) == 1) {//如果可以成功读取一个字符串（表示可以读到Yes或No）
        courseNames[n++] = CopyString(buf);//保存课程名，CopyString给读入到buf中的字符串做个副本（会自动申请内存空间）
        line += m;//line移动位置，以便于下一次读取Yes/No
    }
    return n;
}

int ReadinStudentRecords(FILE *fp, struct student students[])
{
    int n, m;
    char buf[20];
    string line;

    n = 0;
    //ReadLine从文件fp的当前位置开始读取一行并形成字符串（会自动按需申请内存空间）,返回该字符串指针；
    //若读到一个空行（仅有一个回车）返回空字符串；若读到文件末尾，则会返回一个空指针NULL
    while ((line = ReadLine(fp)) != NULL) {//从文件fp中读取一行字符串（学生及其选课记录），且并没有读到文件末尾
        sscanf(line, "%s%n", buf, &m);//从line中读取第一个字符串（学号），m保存实际读取的字符个数
        students[n].ID = CopyString(buf);//创建学号字符串副本并保存
        line += m;//调整line指针的位置
        sscanf(line, "%s%n", buf, &m);//从line中读取第一个字符串（姓名），m保存实际读取的字符个数
        students[n].name = CopyString(buf);//创建姓名字符串副本并保存
        line += m;//调整line指针位置
        int k = 0;
        for (int i = 0; i < courseNumber; i++) {//读取该学生每门课的选课情况Yes/No
            sscanf(line, "%s%n", buf, &m);//读取选课情况字符串Yes/No
            if (StringCompare(buf, "Yes") == 0) {//若是Yes
                students[n].courseNo[k++] = i;//则记录该课程序号
            }
            line += m;//调整line指针位置
        }
        students[n].courseNo[k] = -1;//-1表示选课序号结束
        n++;//学生数加1
    }

    return n;
}

//打印所有学生的选课清单
void PrintStudentCourses(struct student students[], int studentNumber)
{
    for (int i = 0; i < studentNumber; i++) {//对每一个学生依次循环
        printf("%s %s: ", students[i].ID, students[i].name);
        for (int k = 0; students[i].courseNo[k] != -1; k++) {//-1为选课结束标志
            printf("%s ", courseNames[students[i].courseNo[k]]);//打印所选课程名字
        }
        printf("\n");
    }
}

//打印每门课的选课学生信息
void PrintCourseStudents(struct student students[], int studentNumber)
{
    for (int i = 0; i < courseNumber; i++) {//对每门课依次循环
        printf("%s:\n", courseNames[i]);//打印课程名称
        int count = 0;//该课程选课学生计数
        for (int k = 0; k < studentNumber; k++) {//对每个学生依次循环
            for (int j = 0; students[k].courseNo[j] != -1; j++) {//对当前第k个学生的所选课程序号循环
                if (students[k].courseNo[j] == i) {//若所选课程中有第i号课程
                    printf("  %s %s", students[k].ID, students[k].name);//打印该学生的信息
                    count++;//选课学生计数加1
                    if (count % 10 == 0) printf("\n");//每打印10个学生则换一行
                    break;//终止for-j循环
                }
            }
        }
        if (count % 10 != 0) printf("\n");//若最后一行不满10个学生则打印换行
    }
}