/*************************************************************************
	> File Name: my_ls.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2015年06月19日 星期五 17时41分48秒
 ************************************************************************/

#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<unistd.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>

#define CMDNONE 0
#define CMD_a 1
#define CMD_l 2
#define CMD_al 3

struct cmand
{
	int flag;/*记录执行该cmd的参数*/
	char *cmd;
	struct cmand *next;
};

void my_gets(char *c);/*获取输入的命令*/

struct cmand *listadd(int flag,char *command,int size,struct cmand *phead);

struct cmand *cmdcut(char *command);/*分割命令*/

int dirprint(const char *path);/*打印path参数指定目录下的文件*/

int getfilestatus(const char *filename);/*获取文件属性*/

int dirallfile(const char *filename);/*打印filename参数指定的文件属性*/

int main(void)
{
	struct cmand *phead=NULL,*ptemp=NULL;
	char command[256];
	printf("请输入命令：");
	my_gets(command);
	phead=cmdcut(command);
	if(phead->next == NULL){
		printf("command failed");
		return -1;
	}
	ptemp=phead->next;
	do{printf("%s\n",ptemp->cmd);
		switch(ptemp->flag){
			case 0:
				if(dirprint(ptemp->cmd) == -1){
					printf("command failed");
					return -1;
				}
				break;
			case 1:
				if(getfilestatus(ptemp->cmd) == -1){
					printf("command failed");
					return -1;
				}
				break;
			case 2:
				if(dirallfile(ptemp->cmd) == -1){
					printf("command failed");
					return -1;
				}
				break;
			case 3:
				if(dirprint(ptemp->cmd) == -1){
					printf("command failed");
					return -1;
				}
				break;
			default:
				if(dirprint(ptemp->cmd) == -1){
					printf("command failed");
					return -1;
				}
		}
		ptemp=ptemp->next;
	}while(ptemp != NULL);
	return 0;
}

void my_gets(char *c)
{
	int i=0;
	do{
		c[i]=getchar();
	}
	while((c[i++] != '\n') && (i<256));
	c[i-1]='\0';
}

struct cmand *listadd(int flag,char *command,int size,struct cmand *phead)
{
	struct cmand *pnew=NULL,*ptemp=NULL;
	ptemp=phead;
	while(ptemp->next != NULL){/*遍历到链表最后一个节点*/
		ptemp=ptemp->next;
	}
	pnew=(struct cmand *)malloc(sizeof(struct cmand));
	pnew->cmd=(char *)malloc(size*sizeof(char));
	pnew->flag=flag;
	printf("%s\n",command);
	strcpy(pnew->cmd,command);/*temp中存储的单项存入链表中*/
	if(phead->next != NULL){
		pnew->next=NULL;
		ptemp->next=pnew;
		ptemp=pnew;
	}
	else{
		phead->next=pnew;
		ptemp=pnew;
	}
	return phead;
}

struct cmand *cmdcut(char *command)
{
	char temp[20];
	struct cmand *phead=NULL;
	int i=0,j=0,flag=0;
	phead=(struct cmand *)malloc(sizeof(struct cmand));
	phead->next=NULL;
	while(command[i] != '\0'){i++;}/*遍历到字符串末尾，在'\0'前加上空格*/
	command[i++]=' ';
	command[i]='\0';
	i=0;
	while(command[i] != '\0'){     /*利用单向链表存储命令*/
		if(command[i] == ' '){
			if(j == 0){    /*输入的命令一开始就有空格*/
				printf("命令格式错误。\n");
				return NULL;
			}
			if(! strcmp(temp,"ls")){    /*若命令为ls，temp归零*/
				if(j>2){     /*若ls不是第一个命令，报错*/
				printf("命令格式错误。\n");
				return NULL;
				}
				j=0;
				i++;   /*command向后移一位*/
				continue; /*结束此次循环*/
			}
			if(temp[0] == '-'){/*若分割出的命令为选项参数*/
				switch(temp[1]){
				case 'a':
					if(temp[2] == 'l'){     /*检测命令是否为-al*/
						flag=flag+CMD_al;
					}
					flag=flag+CMD_a;
					break;
				case 'l':
					if(temp[2] == 'a'){     /*检测命令是否为-la*/
						flag=flag+CMD_al;
						break;
					}
					flag=flag+CMD_l;
					break;
				}
			}
			else{     /*若分割出的是路径或文件名*/
			phead=listadd(flag,temp,strlen(temp)+1,phead);
			j=0;
			flag=0; /*flag归零*/
			}
		}
		else{
			temp[j++]=command[i];
			temp[j]='\0';
		}
		i++;
	}
	if(flag != 0){   /*若命令解析完毕后flag不等于0，则说明有选项参数但没有文件或路径*/
		phead=listadd(flag,".\0",1,phead);
	}
	return phead;
}

int dirprint(const char *path)
{
	DIR *dir;
	struct dirent *ptr;
	if((dir=opendir(path))==NULL){
		return -1;
	}
	while((ptr=readdir(dir))!=NULL){
		printf("%s\n",ptr->d_name);
	}
	closedir(dir);
	return 0;
}

int getfilestatus(const char *filename)
{
	char buf_time[30];/*存储文件时间*/
	struct stat buf;/*存储文件属性*/
	struct passwd *psd=NULL;/*存储用户的数据*/
	struct group *grp=NULL;/*存储用户所属组的数据*/
	if(stat(filename,&buf)==-1){
		return -1;
	}

	/*判断并打印文件类型*/
	if(S_ISLNK(buf.st_mode)){
		printf("l");
	}
	else if(S_ISREG(buf.st_mode)){
		printf("-");
	}
	else if(S_ISDIR(buf.st_mode)){
		printf("d");
	}
	else if(S_ISCHR(buf.st_mode)){
		printf("c");
	}
	else if(S_ISBLK(buf.st_mode)){
		printf("b");
	}
	else if(S_ISFIFO(buf.st_mode)){
		printf("f");
	}
	else if(S_ISSOCK(buf.st_mode)){
		printf("s");
	}

	/*打印文件所有者对该文件的操作权限*/
	if(buf.st_mode & S_IRUSR){/*判断文件所有者是否有读取权限*/
		printf("r");
	}
	else{
		printf("-");
	}
	if(buf.st_mode & S_IWUSR){/*判断文件所有者是否具有写入权限*/
		printf("w");
	}
	else{
		printf("-");
	}
	if(buf.st_mode & S_IXUSR){/*判断文件所有者是否具有运行权限*/
		printf("x");
	}
	else{
		printf("-");
	}

	/*打印文件所有者同用户组的用户对该文件的操作权限*/
	if(buf.st_mode & S_IRGRP){
		printf("r");
	}
	else{
		printf("-");
	}
	if(buf.st_mode & S_IWGRP){
		printf("w");
	}
	else{
		printf("-");
	}
	if(buf.st_mode & S_IXGRP){
		printf("x");
	}
	else{
		printf("-");
	}

	/*打印其他用户对该文件的操作权限*/
	if(buf.st_mode & S_IROTH){
		printf("r");
	}
	else{
		printf("-");
	}
	if(buf.st_mode & S_IWOTH){
		printf("w");
	}
	else{
		printf("-");
	}
	if(buf.st_mode & S_IXOTH){
		printf("x");
	}
	else{
		printf("-");
	}
	
	printf("  ");
	printf("%d  ",buf.st_nlink);/*打印文件的链接数*/

	/*打印文件所有者及文件所有者所属的组*/
	psd=getpwuid(buf.st_uid);/*getpwuid函数根据用户id获取用户信息*/
	grp=getgrgid(buf.st_gid);/*getgrgid函数根据用户组id获取用户所属用户组的信息*/
	printf("%s  ",psd->pw_name);/*打印文件所有者的名字*/
	printf("%s  ",grp->gr_name);/*打印文件所属组的名字*/

	printf("%d  ",buf.st_size);/*打印文件的大小*/
	strcpy(buf_time,ctime(&buf.st_mtime));/*ctime函数转换的字符串末尾是\n\0*/
	buf_time[strlen(buf_time)-1]='\0';/*将字符串最后的\n替换为\0*/
	printf("%s  ",buf_time);/*打印文件最近修改的时间*/
	printf("%s\n",filename);/*打印文件名*/
	return 0;
}

int dirallfile(const char *path)
{
	DIR *dir;
	struct dirent *ptr;
	if((dir=opendir(path)) == NULL){
		return 0;
	}
	while((ptr = readdir(dir)) != NULL){
		getfilestatus(ptr->d_name);
	}
	return 0;
}
