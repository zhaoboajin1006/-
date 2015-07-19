/*************************************************************************
	> File Name: keshe.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2015年06月30日 星期二 10时31分17秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

struct bookdir
{
	char bookgroup[20];
	struct book *pdir;
	struct bookdir *next;
};

struct book
{
	char bookname[20];
	char bookwriter[20];
	char bookgroup[20];
	int  booknumber;  /*现有总数*/
	int  booksp;  /*剩余数量*/
	struct book *next;
};

struct usr
{
	char name[20];
	char password[20];
	struct usr *next;
};

struct history
{
	char flag;
	char bookname[20];
	char bookwriter[20];
	char usrname[20];
	struct history *next;
};

int adm(struct usr *pusrhead);

int user(struct usr *pusr);

int main_interface(void);  /*绘制主界面*/

int adm_interface(void);  /*绘制管理员界面*/

int user_interface(void);  /*绘制用户界面*/

struct bookdir *bookdiradd(struct bookdir *pdirhead,struct bookdir *pbookdir);  /*书籍目录信息添加*/

int bookadd(struct bookdir *pdirhead,struct book *pbook);  /*书籍信息添加*/

struct bookdir *bookload(void);  /*书库数据读取*/

int booksave(struct bookdir *pbookdirhead);  /*书库数据存储*/

struct book *bookdiscover(struct bookdir *pbookdirhead,struct book *pbook);  /*查询某个书籍的信息*/

struct book *bookdirdiscover(struct bookdir *pbookdirhead,char *group); /*按类查找书籍*/

int bookchange(struct bookdir *pdirhead);    /*书籍信息修改*/

int bookprint(struct bookdir *pbookdirhead);    /*数据统计后输出*/

struct usr *usrdenglu(struct usr *pusrhead);     /*用户登陆,传入用户信息链表的头指针，返回登陆成功时用户信息的节点指针*/

int usrhistorysave(char flag,struct book *pbook,const char *name,char *time);    /*在用户专属文件中存储借阅记录*/

struct history *usrhistoryload(const char *name);

void usrhistoryprint(const char *name);/**/

/*归还或借出书籍并记录借阅历史*/
int bookoperation(struct book *pbook,const char *name,char flag);

struct usr *usradd(struct usr *pusrhead,struct usr *pusr);  /*添加用户信息*/

struct usr *usrload(void);  /*载入用户信息*/

int usrsave(struct usr *pusrhead);   /*存储用户信息*/


int main(void)
{
	struct usr *pusrhead=NULL;
	struct usr *pusr=NULL;
	struct usr *pusrtemp=NULL;
	int i=0;
	char password[20];
	pusrhead=usrload();
	while(1){
		i=0;
		pusr=(struct usr *)malloc(sizeof(struct usr));
		pusr->next=NULL;
		switch(main_interface()){
			case 1:
				if((pusrtemp=usrdenglu(pusrhead)) != NULL){
					if(strcmp(pusrtemp->name,"0000") == 0){
						printf("管理员登陆成功.\n");
						adm(pusrhead);
					}
					else{
						printf("登陆成功.\n");
						user(pusrtemp);
					}
				}
				break;
			case 2:
				i=0;
				printf("注册用户名：\n");
				scanf("%s",pusr->name);
				printf("请输入密码：\n");
				scanf("%s",pusr->password);
				printf("请再次输入密码：\n");
				scanf("%s",password);
				while(strcmp(pusr->password,password) != 0){
					i++;
					if(i == 3){
						printf("连续三次错误，注册失败。\n");
						break;
					}
				}
				if(strcmp(pusr->password,password) == 0){
					printf("注册成功。（按任意键返回。）\n");
					pusrhead=usradd(pusrhead,pusr);
					getchar();
					getchar();
					break;
				}
				free(pusr);
				break;
			default:
				usrsave(pusrhead);
				free(pusr);
				return 0;
				break;
		}
	}
	return 0;
}

/*管理员系统*/

int adm(struct usr *pusrhead)
{
	char name[20];
	struct usr *pusr=NULL;
	struct bookdir *pbookdirhead=NULL;
	struct bookdir *pbookdirtemp=NULL;
	struct book *pbook=NULL;
	struct book *pbooktemp=NULL;
	char c;
	int number=0;
	if((pbookdirhead=bookload()) == NULL){
		printf("读取文件出错\n");
	}
	while(1){
		switch(adm_interface()){
			case 1:
				bookprint(pbookdirhead);
				break;
			case 2:

				/*填写新添加的图书信息*/
				printf("添加新的图书信息");
				pbook=(struct book *)malloc(sizeof(struct book));
				pbook->next=NULL;
				printf("请输入书名：");
				scanf("%s",pbook->bookname);
				printf("请输入书的作者：");
				scanf("%s",pbook->bookwriter);
				printf("请输入书所属的类型：");
				scanf("%s",pbook->bookgroup);
				printf("请输入书的数量：");
				scanf("%d",&pbook->booknumber);
				pbook->booksp=pbook->booknumber;
				if(bookadd(pbookdirhead,pbook) == -1){
					printf("添加失败");
				}
				break;
			case 3:

				/*添加新的图书目录信息*/
				printf("添加新的图书类型信息");
				pbookdirtemp=(struct bookdir *)malloc(sizeof(struct bookdir));
				pbookdirtemp->next=NULL;
				pbookdirtemp->pdir=NULL;
				printf("请输入类型：");
				scanf("%s",pbookdirtemp->bookgroup);
				if((pbookdirhead=bookdiradd(pbookdirhead,pbookdirtemp)) == NULL){
					printf("添加类型失败");
				}
				break;
			case 4:

				/*修改书籍信息*/
				bookchange(pbookdirhead);
				break;
			case 5:
				printf("输入欲借阅或归还的书籍信息\n");	
				pbook=(struct book *)malloc(sizeof(struct book));
				printf("请输入书名：");
				scanf("%s",pbook->bookname);
				printf("请输入书的作者：");
				scanf("%s",pbook->bookwriter);
				if((pbooktemp=bookdiscover(pbookdirhead,pbook)) == NULL){
					printf("无此书籍信息");
					break;
				}
				else{
					printf("查找到书籍:\n书名:%s,书籍作者:%s,书籍类型;%s,书籍总数:%d,书籍剩余数量:%d\n",pbooktemp->bookname,pbooktemp->bookwriter,pbooktemp->bookgroup,pbooktemp->booknumber,pbooktemp->booksp);
				}
				printf("借阅用户信息确认：\n");
				if((pusr=usrdenglu(pusrhead)) == NULL){
					printf("信息确认失败。");
					break;
				}
				if(strcmp(pusr->name,"0000") == 0){
					printf("借阅用户不允许为管理员用户。\n");
					break;
				}
				printf("用户信息确认成功。\n");
				printf("\n1.借阅。\n2.归还。\n0.取消。\n请选择：");
				getchar();
				scanf("%c",&c);
				switch(c){
					case '1':
						c='b';
						if(bookoperation(pbooktemp,pusr->name,c) == -1){
							printf("操作失败。\n");
						}
						break;
					case '2':
						c='r';
						if(bookoperation(pbooktemp,pusr->name,c) == -1){
							printf("操作失败。\n");
						}
						break;
					default:
						break;
				}
				break;
			case 6:
				usrhistoryprint("bookhistory");
				break;
			default:
				if(booksave(pbookdirhead) == -1){
					printf("书库数据存储失败。");
					return -1;
				}
				return 0;
				break;
		}
	}
}



/*用户系统*/

int user(struct usr *pusr)
{
	struct book *pbook=NULL;
	struct bookdir *pbookdirhead=NULL;
	struct book *pbooktemp=NULL;
	struct bookdir *pbookdirtemp=NULL;
	struct usr *pusrtemp=NULL;
	char bookgroup[20];
	char password[20];
	if((pbookdirhead=bookload()) == NULL){
		printf("书库信息读取错误！\n");
		return -1;
	}
	while(1){
		switch(user_interface()){
			case 1:
				/*查找指定书籍*/
				pbook=(struct book *)malloc(sizeof(struct book));
				printf("请输入书名：");
				scanf("%s",pbook->bookname);
				printf("请输入书的作者：");
				scanf("%s",pbook->bookwriter);
				if((pbooktemp=bookdiscover(pbookdirhead,pbook)) == NULL){
					printf("无此书籍信息");
				}
				else{
					printf("查找到书籍:\n书名:%s,书籍作者:%s,书籍类型;%s,书籍总数:%d,书籍剩余数量:%d\n",pbooktemp->bookname,pbooktemp->bookwriter,pbooktemp->bookgroup,pbooktemp->booknumber,pbooktemp->booksp);
				}
				free(pbook);
				break;
			case 2:
				/*按类型查找书籍*/
				printf("请输入欲查找的类型名：");
				scanf("%s",bookgroup);
				if((pbooktemp=bookdirdiscover(pbookdirhead,bookgroup)) == NULL){
					printf("无该类型信息。");
					break;
				}
				while(pbooktemp != NULL){
					printf("书名:%s,书籍作者：%s,书籍类型：%s,书籍总数：%d,书籍剩余数量：%d\n",pbooktemp->bookname,pbooktemp->bookwriter,pbooktemp->bookgroup,pbooktemp->booknumber,pbooktemp->booksp);
					pbooktemp=pbooktemp->next;
				}
				break;
			case 3:
				printf("请输入修改后的密码：");
				scanf("%s",password);
				printf("请再次输入修改后的密码：");
				scanf("%s",bookgroup);
				if(strcmp(password,bookgroup) == 0){
					strcpy(pusr->password,password);
					printf("修改成功。");
					break;
				}
				printf("两次输入的密码不同，修改失败。");
				break;
			case 4:
				usrhistoryprint(pusr->name);
				break;
			default:
				return 0;
				break;
		}
	}
}



/*绘制主界面*/

int main_interface()
{
	int c=-1;
	printf("*************************************************\n");
	printf("*           欢迎使用图书管理系统                *\n");
	printf("*                                               *\n");
	printf("*                 1.登陆                        *\n");
	printf("*                 2.注册                        *\n");
	printf("*                 0.退出                        *\n");
	printf("*                                               *\n");
	printf("*                                               *\n");
	printf("*                                               *\n");
	printf("*************************************************\n");
	printf("请选择：");
	scanf("%d",&c);
	return c;
}



/*绘制管理员界面*/
int adm_interface()
{
	int c=-1;
	//system("clear");
	printf("**************************************\n");
	printf("*           管理员系统               *\n");
	printf("*                                    *\n");
	printf("*  1.查看全部。                      *\n");
	printf("*  2.添加书籍信息。                  *\n");
	printf("*  3.添加书籍类型信息。              *\n");
	printf("*  4.修改书籍信息。                  *\n");
	printf("*  5.书籍借阅操作。                  *\n");
	printf("*  6.查看书库历史记录。              *\n");
	printf("*  0.退出。                          *\n");
	printf("*                                    *\n");
	printf("*                                    *\n");
	printf("**************************************\n");
	printf("请选择：");
	scanf("%d",&c);
	return c;
}



/*绘制用户界面*/
int user_interface()
{
	int c=-1;
	printf("************************************\n");
	printf("*          用户系统                *\n");
	printf("*                                  *\n");
	printf("*      1.查询书籍信息              *\n");
	printf("*      2.按类查找书籍              *\n");
	printf("*      3.修改用户密码              *\n");
	printf("*      4.查看本用户借阅记录        *\n");
	printf("*      0.退出                      *\n");
	printf("*                                  *\n");
	printf("*                                  *\n");
	printf("************************************\n");
	printf("请选择：");
	scanf("%d",&c);
	return c;
}



/*在书库中添加书籍目录信息*/

struct bookdir *bookdiradd(struct bookdir *pbookdirhead,struct bookdir *pbookdir)
{
	struct bookdir *pbookdirtemp=pbookdirhead;
	if(pbookdirhead == NULL){
		return pbookdir;
	}
	else{
		do{         /*遍历到目录链表最末尾的节点*/
			if(!strcmp(pbookdirtemp->bookgroup,pbookdir->bookgroup)){
				printf("类型已存在，添加失败。\n");
				return pbookdirhead;
			}
			if(pbookdirtemp->next == NULL){
				break;
			}
			pbookdirtemp=pbookdirtemp->next;
		}while(pbookdirtemp->next != NULL);
		pbookdirtemp->next=pbookdir;    /*将目录信息添加到目录链表末尾*/
	}
	return pbookdirhead;
}


/*将图书信息添加到书库链表中*/

int bookadd(struct bookdir *pdirhead,struct book *pbook)
{
	struct bookdir *pdirtemp=NULL;
	struct book *ptemp=NULL;
	if(pdirhead == NULL){
		return -1;
	}

	pdirtemp=pdirhead;
	while(pdirtemp != NULL){
		if(! strcmp(pbook->bookgroup,pdirtemp->bookgroup)){   /*匹配查找欲添加的图书所属组*/
			break;
		}
		pdirtemp=pdirtemp->next;
	}
	if(pdirtemp == NULL){
		printf("未匹配到该图书所属类型\n");
		return -1;
	}
	ptemp=pdirtemp->pdir;  /*进入图书某组中，将图书添加进去*/
	if(pdirtemp->pdir == NULL){  /*若书库中该类型书为空，则直接追加该书信息*/
		pdirtemp->pdir=pbook;
		pbook->next=NULL;
	}
	else{
		do{
			if(! strcmp(pbook->bookname,ptemp->bookname)){/*有重复图书信息，则只添加图书总数和剩余数量*/
				ptemp->booknumber=ptemp->booknumber+pbook->booknumber;
				ptemp->booksp=ptemp->booksp+pbook->booksp;
				free(pbook);
				return 0;
			}
			if(ptemp->next == NULL){
				break;
			}
			ptemp=ptemp->next;
		}while(ptemp->next != NULL);
		/*不是重复图书信息，则追加该图书信息*/
		ptemp->next=pbook;
		pbook->next=NULL;
	}
	return 0;
}


/*从文件中读取书籍类型信息*/

struct bookdir *bookload(void)
{
	struct bookdir *pbookdirhead=NULL;
	struct bookdir *pbookdirtemp=NULL;
	struct book *pbooktemp=NULL;
	struct bookdir *dirflag=NULL;
	struct book *flag=NULL;
	FILE *fp;
	if((fp=fopen("书库.txt","rt")) == NULL){
		return NULL;
	}
	do{
		pbookdirtemp=(struct bookdir *)malloc(sizeof(struct bookdir));
		pbookdirtemp->next=NULL;
		pbookdirtemp->pdir=NULL;
		if(fscanf(fp,"%s %p %p",pbookdirtemp->bookgroup,&dirflag,&flag) == -1){   /*读取一个新的书籍类型节点*/
			free(pbookdirtemp);
			return pbookdirhead;
		}
		pbookdirhead=bookdiradd(pbookdirhead,pbookdirtemp);  /*添加书籍类型信息*/
		while(flag != NULL){   /*该目录下有书籍信息*/
			pbooktemp=(struct book *)malloc(sizeof(struct book));
			pbooktemp->next=NULL;
			if(fscanf(fp,"%s %s %s %d %d %p",pbooktemp->bookname,pbooktemp->bookwriter,pbooktemp->bookgroup,&pbooktemp->booknumber,&pbooktemp->booksp,&flag) == -1){
				free(pbooktemp);
				break;
			}
			if(bookadd(pbookdirhead,pbooktemp) == -1){   /*添加书籍信息*/
				break;
			}
		}
	}while(dirflag != NULL);
	fclose(fp);
	return pbookdirhead;
}





/*将书库数据存储到文件中*/
/*把链表存进文件时，链表中每个节点的指针在下次读取时只能当作是否有下一个节点的标志
 *因为这些指针早就已经失效，不能够直接读取当作指针，会引起内存读取错误*/

int booksave(struct bookdir *pbookdirhead)
{
	struct bookdir *pbookdirtemp=NULL;
	struct book *pbooktemp=NULL;
	FILE *fp;
	if(pbookdirhead == NULL){
		return -1;
	}
	pbookdirtemp=pbookdirhead;
	if((fp=fopen("书库.txt","wt")) == NULL){
		return -1;
	}

	/*将目录信息及书籍信息存入文件*/
	do{
		fprintf(fp,"%s %p %p",pbookdirtemp->bookgroup,pbookdirtemp->next,pbookdirtemp->pdir); /*存储目录信息*/
		if(pbookdirtemp->pdir == NULL){ /*某分组中没有书籍信息*/
			pbookdirtemp=pbookdirtemp->next;
			continue;
		}
		pbooktemp=pbookdirtemp->pdir;
		while(pbooktemp != NULL){
			fprintf(fp,"%s %s %s %d %d %p",pbooktemp->bookname,pbooktemp->bookwriter,pbooktemp->bookgroup,pbooktemp->booknumber,pbooktemp->booksp,pbooktemp->next);
			pbooktemp=pbooktemp->next;
		}/*将每个分组下的书籍信息全部存入文件*/
		pbookdirtemp=pbookdirtemp->next;
	}while(pbookdirtemp != NULL);
	fclose(fp);
	return 0;
}


/*从书库链表中查找某个书籍的信息*/

struct book *bookdiscover(struct bookdir *pbookdirhead,struct book *pbook)
{
	struct book *pbooktemp=NULL;
	struct bookdir *pbookdirtemp=NULL;
	if(pbookdirhead == NULL){
		return NULL;
	}
	pbookdirtemp=pbookdirhead;

	while(pbookdirtemp != NULL){
		if(pbookdirtemp->pdir == NULL){
			pbookdirtemp=pbookdirtemp->next;
			continue;
		}
		pbooktemp=pbookdirtemp->pdir;
		while(pbooktemp != NULL){
			if((!strcmp(pbooktemp->bookname,pbook->bookname)) && (!strcmp(pbooktemp->bookwriter,pbook->bookwriter))){
				return pbooktemp;
			}
			pbooktemp=pbooktemp->next;
		}
		pbookdirtemp=pbookdirtemp->next;
	}

	return NULL;
}



/*按类查找书籍*/
struct book *bookdirdiscover(struct bookdir *pbookdirhead,char *group)
{
	struct bookdir *pbookdirtemp=NULL;
	pbookdirtemp=pbookdirhead;
	while((strcmp(pbookdirtemp->bookgroup,group) != 0) && (pbookdirtemp != NULL)){
		pbookdirtemp=pbookdirtemp->next;
	}
	if(pbookdirtemp == NULL){
		return NULL;
	}
	else{
		return pbookdirtemp->pdir;
	}
}




/*修改指定的书籍信息*/

int bookchange(struct bookdir *pbookdirhead)
{
	struct bookdir *pbookdirtemp=NULL;
	struct book *pbooktemp=NULL;
	struct book *pbooklast=NULL;
	struct book *pbookch=NULL;
	char name[20];
	char t;
	char writer[20];
	int c=-1;
	pbookch=(struct book *)malloc(sizeof(struct book));
	memset(pbookch,0,sizeof(struct book));
	if(pbookdirhead == NULL){
		return -1;
	}
	/*输入要修改的书的信息*/
	printf("请输入欲修改的书名：");
	scanf("%s",name);
	printf("请输入欲修改的书的作者：");
	scanf("%s",writer);

	/*遍历书库，查找需要的书的信息*/
	pbookdirtemp=pbookdirhead;
	do{
		pbooktemp=pbookdirtemp->pdir;
		do{
			pbooklast=pbooktemp;  /*记录上一个节点的指针*/
			if((!strcmp(pbooktemp->bookname,name)) && (!strcmp(pbooktemp->bookwriter,writer))){
				break;
			}
			pbooktemp=pbooktemp->next;  /*更新节点*/
		}while(pbooktemp != NULL);
		if((!strcmp(pbooklast->bookname,name)) && (!strcmp(pbooklast->bookwriter,writer))){
			break;
		}
		pbookdirtemp=pbookdirtemp->next;
	}while(pbookdirtemp != NULL);
	if(pbookdirtemp == NULL){
		printf("未查找到需要修改的书籍信息\n");
		return -1;
	}

	/*选择需要修改的方式*/
	printf("请选择修改的方式：\n1.修改书籍信息。\n2.删除书籍的信息。\n3.取消操作。\n请选择：");
	scanf("%d",&c);
	getchar();
	switch(c){
		case 1:
			printf("请输入修改后的书名：");
			scanf("%s",&pbookch->bookname);
			printf("请输入修改后的书籍作者：");
			scanf("%s",&pbookch->bookwriter);
			printf("请输入修改后的书籍总数");
			scanf("%d",&pbookch->booknumber);
			strcpy(pbookch->bookgroup,pbooktemp->bookgroup);
			if(pbooktemp->booknumber > pbookch->booknumber){    /*若修改后书的总数小于修改前的总数，则说明总数变小*/
				if(pbooktemp->booksp <= pbookch->booknumber){   /*修改后的总数少于修改前书的剩余量,则剩余量等于总数*/
					pbookch->booksp=pbookch->booknumber;
				}
				else{    /*修改后总数大于修改前书的剩余量，则修改后剩余量的等于以前的剩余量*/
					pbookch->booksp=pbooktemp->booksp;
				}
			}
			printf("确认修改？Y or N:");
			getchar();
			scanf("%c",&t);
			if((t == 'Y') || (t == 'y')){
				strcpy(pbooktemp->bookname,pbookch->bookname);
				strcpy(pbooktemp->bookwriter,pbookch->bookwriter);
				strcpy(pbooktemp->bookgroup,pbookch->bookgroup);
				pbooktemp->booknumber=pbookch->booknumber;
				pbooktemp->booksp=pbookch->booksp;
				free(pbookch);
			}
			else{
				printf("已取消。");
				free(pbookch);
			}
			break;
		case 2:
			printf("确认删除？Y or N:");
			scanf("%c",&t);
			if(t == 'Y' || t == 'y'){
				if(pbookdirtemp->pdir == pbooktemp){
					pbookdirtemp->next=NULL;
					free(pbooktemp);
					break;
				}
				pbooklast->next=pbooklast->next->next;
				free(pbooktemp);
			}
			else{
				printf("已取消。");
			}
			break;
		default:
			printf("已取消操作。");
			break;
	}
	return 0;
}




/*书籍信息遍历输出*/
int bookprint(struct bookdir *pbookdirhead)
{
	struct bookdir *pbookdirtemp=NULL;
	struct book *pbooktemp=NULL;
	if(pbookdirhead == NULL){
		return -1;
	}
	pbookdirtemp=pbookdirhead;
	while(pbookdirtemp != NULL){
		printf("书的类型：%s\n",pbookdirtemp->bookgroup);
		pbooktemp=pbookdirtemp->pdir;
		while(pbooktemp != NULL){
			printf("书名：%s 作者：%s 总数：%d 剩余数量：%d\n",pbooktemp->bookname,pbooktemp->bookwriter,pbooktemp->booknumber,pbooktemp->booksp);
			pbooktemp=pbooktemp->next;
		}
		pbookdirtemp=pbookdirtemp->next;
	}
	return 0;
}


/*用户登陆函数，返回登陆成功的用户的用户信息的节点指针*/
struct usr *usrdenglu(struct usr *pusrhead)
{
	int i=1;
	char name[20];
	char password[20];
	struct usr *pusrtemp=NULL;
	if(pusrhead == NULL){
		return NULL;
	}
	pusrtemp=pusrhead;
	printf("请输入用户名：");
	scanf("%s",name);
	while(pusrtemp != NULL){
		if(strcmp(pusrtemp->name,name) == 0){
			break;
		}
		pusrtemp=pusrtemp->next;
	}
	if(pusrtemp == NULL){
		printf("无此用户。\n");
		return NULL;
	}
	else{
		printf("请输入密码：");
		scanf("%s",password);
		while(i <= 2){
			if(strcmp(pusrtemp->password,password) == 0){
				return pusrtemp;
			}
			printf("密码错误。请重新输入：");
			scanf("%s",password);
			i++;
		}
		if(i == 3){
			printf("错误三次，取消登陆。");
			return NULL;
		}
	}
}


struct history *usrhistoryadd(struct history *phead,struct history *pnew)
{
	struct history *ptemp=NULL;
	pnew->next=NULL;
	ptemp=phead;
	if(phead == NULL){
		return pnew;
	}
	else{
		while(ptemp->next != NULL){
			ptemp=ptemp->next;
		}
		ptemp->next=pnew;
		return phead;
	}
}


/*载入用书专属文件信息*/

struct history *usrhistoryload(const char *name)
{
	struct history *pnew=NULL;
	struct history *phead=NULL;
	FILE *fp=NULL;
	char s[5]=".txt";
	char s2[20];
	int m=0,n=0;
	strcpy(s2,name);
	do{
		n++;
	}while(s2[n] != '\0');
	do{
		s2[n++]=s[m];
	}while(s[m++] != '\0');
	if((fp=fopen(s2,"rt")) == NULL){
		return NULL;
	}
	pnew=(struct history *)malloc(sizeof(struct history));
	while(fscanf(fp,"%c %s %s %s",&pnew->flag,pnew->bookname,pnew->bookwriter,pnew->usrname) != -1){
		phead=usrhistoryadd(phead,pnew);
		pnew=(struct history *)malloc(sizeof(struct history));
		pnew->next=NULL;
	}
	free(pnew);
	return phead;
}


/*根据指定的用户名输出用户的借阅历史信息*/
void usrhistoryprint(const char *name)
{
	FILE *fp=NULL;
	char s1[5]=".txt";
	char s2[20];
	char s[50];
	int m=0,n=0;
	strcpy(s2,name);
	do{
		n++;
	}while(s2[n] != '\0');
	do{
		s2[n++]=s1[m];
	}while(s1[m++] != '\0');
	if((fp=fopen(s2,"rt")) == NULL){
		exit(0);
	}
	while(fgets(s,50,fp) != NULL){
		printf("%s",s);
	}
	fclose(fp);
}


/*管理员进行书籍借还操作时，将借还历史记录进用户专属文件*/

int usrhistorysave(char flag,struct book *pbook,const char *name,char *time)
{
	FILE *fp=NULL;
	struct history *phead=NULL,*ptemp=NULL;
	char f='0';
	char s[5]=".txt";
	char s2[20];
	int m=0,n=0;
	phead=usrhistoryload(name);
	ptemp=phead;
	strcpy(s2,name);
	do{
		n++;
	}while(s2[n] != '\0');
	do{
		s2[n++]=s[m];
	}while(s[m++] != '\0');
	
	if((fp=fopen(s2,"at")) == NULL){
		return -1;
	}
	while(ptemp != NULL){
		if((strcmp(ptemp->bookname,pbook->bookname) == 0) && (strcmp(ptemp->bookwriter,pbook->bookwriter) == 0)){
			if(f == '0'){
				if(ptemp->flag == 'r'){  /*若纪录中出现 未借便还 的情况，则报错*/
					fclose(fp);
					return 1;
				}
			}
			else{
				if(f != ptemp->flag){  /*若记录中出现连续两次 借或还 同一本书的信息，则报错*/
					fclose(fp);
					return -1;
				}
			}
			switch(ptemp->flag){
				case 'b':
					f='r';
					break;
				case 'r':
					f='b';
					break;
			}
		}
		ptemp=ptemp->next;
	}
	if(flag == f || ((f == '0') && (flag == 'b'))){/*若欲进行操作与应进行操作相符合，将该操作记录存进用户所属文件，返回1*/
		fprintf(fp,"%c %s %s %s %s",flag,pbook->bookname,pbook->bookwriter,name,time);
		fclose(fp);
		return 0;
	}
	else{
		if(f == 'r'){
			fclose(fp);
			return 1;
		}
		else if(f == 'b'){
			fclose(fp);
			return 2;
		}
		fclose(fp);
	}
}




/*书籍管理员进行书籍的借出归还操作，并记录进历史档案*/

int bookoperation(struct book *pbook,const char *name,char flag)
{
	time_t now;
	FILE *fp=NULL;
	if((fp=fopen("bookhistory.txt","at")) == NULL){
		return -1;;
	}
	time(&now);
	switch(flag){
		case 'b':
			if(pbook->booksp >= 1){
				switch(usrhistorysave(flag,pbook,name,ctime(&now))){
					case -1:
						printf("操作失败。");
						return 0;
						break;
					case 0:
						break;
					case 1:
						printf("该用户未借过这本书.");
						return 0;
					case 2:
						printf("该用户借过此本书未还.");
						break;
				}
				pbook->booksp--;
				fprintf(fp,"%s %s %s %s %s\n","b",pbook->bookname,pbook->bookwriter,name,ctime(&now));
			}
			else{
				return -1;
			}
			break;
		case 'r':
			if(pbook->booknumber >= pbook->booksp+1){
				switch(usrhistorysave(flag,pbook,name,ctime(&now))){
					case -1:
						printf("操作失败。");
						return 0;
						break;
					case 0:
						break;
					case 1:
						printf("该用户未借过这本书.");
						return 0;
					case 2:
						printf("该用户借过此本书未还.");
						break;
				}
				pbook->booksp++;
				fprintf(fp,"%s %s %s %s %s","r",pbook->bookname,pbook->bookwriter,name,ctime(&now));
			}
			else{
				return -1;
			}
			break;
		default:
			return -1;
			break;
	}
	fclose(fp);
	return 0;
}



/*用户信息添加*/
struct usr *usradd(struct usr *pusrhead,struct usr *pusr)
{
	struct usr *pusrtemp=NULL;
	pusrtemp=pusrhead;
	if(pusrhead == NULL){
		return pusr;
	}
	else{
		while(pusrtemp->next != NULL){
			pusrtemp=pusrtemp->next;;
		}
		pusrtemp->next=pusr;
		return pusrhead;
	}
}



/*用户信息载入*/
struct usr *usrload(void)
{
	struct usr *pusrhead=NULL;
	struct usr *pusrtemp=NULL;
	struct usr *flag=NULL;
	FILE *fp=NULL;
	if((fp=fopen("user.txt","rt")) == NULL){
		return NULL;;
	}
	do{
		pusrtemp=(struct usr *)malloc(sizeof(struct usr));
		pusrtemp->next=NULL;
		if(fscanf(fp,"%s %s %p",pusrtemp->name,pusrtemp->password,&flag) == -1){
			return NULL;
		}
		pusrhead=usradd(pusrhead,pusrtemp);
	}while(flag != NULL);
	fclose(fp);
	return pusrhead;
}



/*用户信息存档*/
int usrsave(struct usr *pusrhead)
{
	struct usr *pusrtemp=NULL;
	FILE *fp=NULL;
	pusrtemp=pusrhead;
	if((fp=fopen("user.txt","wt")) == NULL){
		return -1;;
	}
	while(pusrtemp != NULL){
		if(fprintf(fp,"%s %s %p",pusrtemp->name,pusrtemp->password,pusrtemp->next) == -1){
			return -1;
		}
		pusrtemp=pusrtemp->next;;
	}
	fclose(fp);
}
