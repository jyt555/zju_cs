/*
 * File: calendar.c
 * ----------------
 * This program is used to generate a calendar for a year
 * entered by the user.
 */

#include <stdio.h>

typedef char *string;
typedef int bool;

/*
 * Constants:
 * ----------
 * Days of the week are represented by the integers 0-6.
 * Months of the year are identified by the integers 1-12;
 * because this numeric representation for months is in
 * common use, no special constants are defined.
 */

#define Sunday     0
#define Monday     1
#define Tuesday    2
#define Wednesday  3
#define Thursday   4
#define Friday     5
#define Saturday   6

/* Function prototypes */

void GiveInstructions(void);//提示信息
int GetYearFromUser(void);//获取一个合法的年份
void PrintCalendar(int year);//打印年历
void PrintCalendarMonth(int month, int year);//打印某一个月的月历
void IndentFirstLine(int weekday);//打印某个月第一天的缩进
int MonthDays(int month, int year);//计算某年某月的天数
int FirstDayOfMonth(int month, int year);//计算某年某月的第一天是周几？
string MonthName(int month);//转换某个以数字表示的月份的英文名称
bool IsLeapYear(int year);//判断是否闰年

/* Main program */

int main()
{
    int year;

    GiveInstructions();
    year = GetYearFromUser();
    PrintCalendar(year);
    return 0; 
}

/*
 * Function: GiveInstructions
 * Usage: GiveInstructions();
 * --------------------------
 * This procedure prints out instructions to the user.
 */

void GiveInstructions(void)
{
    printf("This program displays a calendar for a full year.\n");
    printf("The year must not be before 1900.\n");
}

/*
 * Function: GetYearFromUser
 * Usage: year = GetYearFromUser();
 * --------------------------------
 * This function reads in a year from the user and returns
 * that value.  If the user enters a year before 1900, the
 * function gives the user another chance.
 */

int GetYearFromUser(void)
{
    ......
}

/*
 * Function: PrintCalendar
 * Usage: PrintCalendar(year);
 * ---------------------------
 * This procedure prints a calendar for an entire year.
 */

void PrintCalendar(int year)
{
    int month;

    for (month = 1; month <= 12; month++) {
        PrintCalendarMonth(month, year);
        printf("\n");
    }
}

/*
 * Function: PrintCalendarMonth
 * Usage: PrintCalendarMonth(month, year);
 * ---------------------------------------
 * This procedure prints a calendar for the given month
 * and year.
 */

void PrintCalendarMonth(int month, int year)
{
    int weekday, nDays, day;

    printf("    %s %d\n", MonthName(month), year);
    printf(" Su Mo Tu We Th Fr Sa\n");
    weekday = FirstDayOfMonth(month, year);
    IndentFirstLine(weekday);
    nDays = MonthDays(month, year);
	for (day = 1; day <= nDays; day++) {
        printf(" %2d", day);
        if (weekday == Saturday) printf("\n");
        weekday = (weekday + 1) % 7;
    }
    if (weekday != Sunday) printf("\n");
}

/*
 * Function: IndentFirstLine
 * Usage: IndentFirstLine(weekday);
 * --------------------------------
 * This procedure indents the first line of the calendar
 * by printing enough blank spaces to get to the position
 * on the line corresponding to weekday.
 */

void IndentFirstLine(int weekday)
{
    ......
}

/*
 * Function: MonthDays
 * Usage: ndays = MonthDays(month, year);
 * --------------------------------------
 * MonthDays returns the number of days in the indicated
 * month and year.  The year is required to handle leap years.
 */

int MonthDays(int month, int year)
{
    ......
}

/*
 * Function: FirstDayOfMonth
 * Usage: weekday = FirstDayOfMonth(month, year);
 * ----------------------------------------------
 * This function returns the day of the week on which the
 * indicated month begins.  This program simply counts
 * forward from January 1, 1900, which was a Monday.
 */

int FirstDayOfMonth(int month, int year)
{
    ......
}

/*
 * Function: MonthName
 * Usage: name = MonthName(month);
 * -------------------------------
 * MonthName converts a numeric month in the range 1-12
 * into the string name for that month.
 */

string MonthName(int month)
{
    ......
}

/*
 * Function: IsLeapYear
 * Usage: if (IsLeapYear(year)) . . .
 * ----------------------------------
 * This function returns TRUE if year is a leap year.
 */

bool IsLeapYear(int year)
{
    ......
}
