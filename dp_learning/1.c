/*************************************************************************
	> File Name: 1.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2015年06月28日 星期日 20时33分57秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
int main(void)
{
	int i=0;
	int a[3];
	void *p=memset(a,49,3*sizeof(int));
	for(i=0;i<3;i++)
	{
		printf("%c\n",a[i]);
	}
	printf("%p\n",*(int *)p);
	return 0;
}
