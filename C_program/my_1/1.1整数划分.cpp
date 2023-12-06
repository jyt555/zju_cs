/*
 *1、整数划分
 *一个正整数可以分解为若干个整数的和，称为这个整数的一个划分。
 *请编程序求某一个正整数的所有划分，要求输出不能重复。要求用递归实现。
 */
 
 #include <stdio.h>
 int c[200],d;
 void f(int x,int m,int l);
 int main(void)
 {
 	int n;
 	
 	scanf("%d",&n);
 	d = n;
 	f(n,n,0);
 	return 0;
 }
 void f(int x,int m,int l)
 {
 	if(x){
 		for(int i = x;i > 0;i--){
 			if(i <= m){
 				c[l] = i;
 				f(x-i,i,l+1);
			 }
		 }
	 }else{
	 	printf("%d=",d);
	 	for(int i = 0;i < l - 1;i++){
	 		printf("%d",c[i]);
	 		printf("+");
		 }
		 printf("%d\n",c[l-1]);
	 }
 }
 
 /* 参考了网上的代码 */
 
