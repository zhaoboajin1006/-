/*************************************************************************
	> File Name: shiyan.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2015年07月18日 星期六 11时21分31秒
 ************************************************************************/

#include<stdio.h>
#include<termio.h>

char getch(void)
{
	struct termios tmtemp,tm;
	char c;
	int fd=0;
	if(tcgetattr(fd,&tm) != 0){    /*获取当前的终端属性设置，并保存到tm结构体中*/
		return -1;
	}
	tmtemp=tm;
	cfmakeraw(&tmtemp);    /*将tetemp初始化为终端原始模式的属性设置*/
	if(tcsetattr(fd,TCSANOW,&tmtemp) != 0){    /*将终端设置为原始模式的设置*/
		return -1;
	}
	c=getchar();
	if(tcsetattr(fd,TCSANOW,&tm) != 0){    /*接收字符完毕后将终端设置回原来的属性*/
		return 0;
	}
	return c;
}

int main(void)
{
	char c;
	c=getch();
	printf("%c\n",c);
	c=getchar();
	printf("%c\n",c);
	return 0;
}

