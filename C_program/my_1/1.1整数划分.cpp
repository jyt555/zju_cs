/*
 *1����������
 *һ�����������Էֽ�Ϊ���ɸ������ĺͣ���Ϊ���������һ�����֡�
 *��������ĳһ�������������л��֣�Ҫ����������ظ���Ҫ���õݹ�ʵ�֡�
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
 
 /* �ο������ϵĴ��� */
 
