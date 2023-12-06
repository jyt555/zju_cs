/*2、打印年历
 *请编写程序，输入1900年（含）以后的年号，输出该年的年历（即每个月的月历）。
 *提示：已知1900年1月1日是星期一。
 *要求：采用模块化（结构化）程序设计方法：自顶向下，逐步求精，函数实现。 
 */
 
 #include <stdio.h>
 int leap_year(int n);
 int first_day(int n);
 void print_first(int month,int n);
 void print_second();
 int print_calendar(int month,int first,int flag);
 void print_day(int first,int n);

 /* main */
 int main(void)
 {
 	int n,flag,first;
 	scanf("%d",&n);
 	
 	flag = leap_year(n);
 	first = first_day(n);
 	
 	for(int month=1;month<=12;month++)
 	{
 		print_first(month,n);
 		print_second();
 		first = print_calendar(month,first,flag);
	 }
 	
 	return 0;
 }
 
 /* 判断闰年 */
 int leap_year(int n)
 {
 	if(!(n%4)&&(n%100)||!(n%400)) return 1;
 	return 0;
 }
 
 /* 判断第一天是星期几 */
 int first_day(int n)
 {
 	int day_sum = 0;
 	
 	for(int i=1900;i<n;i++)
 	{
 		if(leap_year(i)){day_sum += 366;}
 		else{day_sum += 365;}
	}
 	day_sum %= 7;  
	day_sum++;			/*星期天用 7 表示*/
	
	return day_sum;
 }
 
 /* 输出每月第一行 */
 void print_first(int month,int n)
 {
	switch(month){
		case 1:printf("   January %d\n",n);break;
		case 2:printf("   February %d\n",n);break;
		case 3:printf("   March %d\n",n);break;
		case 4:printf("   April %d\n",n);break;
		case 5:printf("   May %d\n",n);break;
		case 6:printf("   June %d\n",n);break;
		case 7:printf("   July %d\n",n);break;
		case 8:printf("   August %d\n",n);break;
		case 9:printf("   September %d\n",n);break;
		case 10:printf("   October %d\n",n);break;
		case 11:printf("   November %d\n",n);break;
		case 12:printf("   December %d\n",n);break;
	}
 }
 
 /* 输出每月第二行 */
 void print_second()
 {
 	printf("Su Mo Tu We Th Fr Sa\n");
 }
 
 /* 输出日历部分 */
int print_calendar(int month,int first,int flag)
{
	if(first != 7)
	for(int i = 0;i < 3 * first - 1;i++)
	{printf(" ");}
	
	if(month == 2){
		if(flag) {print_day(first,29);first += 29;}
		else	 {print_day(first,28);first += 28;}
	}else{
		switch(month){
			case 1:case 3:case 5: case 7:case 8:case 10:case 12:
				print_day(first,31);first += 31;break;
			case 4:case 6:case 9:case 11:
				print_day(first,30);first += 30;break;
		}
	}
	
	first %= 7;
	return first;
	
 } 
 
 void print_day(int first,int n)
 {
 	for(int i = 1;i <= n;i++){
 		if(first == 0){
 			printf("%2d",i);
 			first = 1;
		 }else if(first == 7){
 			printf("\n%2d",i);
 			first = 1;
		 }else{
		 	printf(" %2d",i);
		 	first++;
		 }
	 }
	 
	 printf("\n\n");
 }
 
