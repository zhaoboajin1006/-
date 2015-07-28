/*************************************************************************
	> File Name: 1.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2015年07月20日 星期一 15时43分48秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

int a=4;	/*全局变量和静态变量属于线程共享的数据*/

pthread_once_t once = PTHREAD_ONCE_INIT;		/*将pthread_once函数中的标志参数初始化*/

void run(void);		/*用于验证pthread_once函数*/

int *thread1(void *arg);	/*线程函数1*/

int *thread2(char *arg);	/*线程函数2*/


int main(int argc,char *argv[])
{
	char *arg = NULL;
	void *status;
	pthread_t thid1,thid2;

	printf("i am main thread,my id:%u\n",pthread_self());		/*打印主线程ID*/
	if(pthread_create(&thid1,NULL,(void *)thread1,arg) != 0){		/*创建副线程1*/
		printf("thread1 create fail\n");
	}
	if(pthread_create(&thid2,NULL,(void *)thread2,arg) != 0){
		printf("thread2 create fail\n");
	}
	pthread_join(thid1,status);		/*等待副线程1结束*/
	pthread_join(thid2,status);		/*等待副线程2结束*/
	printf("a=%d\n",a);
	return 0;
}


void run(void)
{
	printf("i am run\n");
}


int *thread1(void *arg)
{
	sleep(3);
	printf("i am thread1,my id：%u\n",pthread_self());	/*打印副线程ID*/
	printf("a=%d\n",++a);
	pthread_once(&once,run);		/*验证pthread_once函数*/
	pthread_exit(0);		/*使副线程退出*/
}


int *thread2(char *arg)
{
	printf("i am thread2,my id:%u\n",pthread_self());
	pthread_once(&once,run);		/*验证pthread_once函数*/
	pthread_exit(0);	
}
