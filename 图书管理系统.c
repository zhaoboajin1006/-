/*************************************************************************
	> File Name: keshe.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2015年06月30日 星期二 10时31分17秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

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

int my_interface();  /*绘制界面*/

int booksave(struct bookdir *pbookdirhead);  /*书库数据存储*/

struct bookdir *bookload(struct bookdir *pbookdirhead);  /*书库数据读取*/

struct bookdir *bookdiradd(struct bookdir *pdirhead,struct bookdir *pbookdir);  /*书籍目录信息添加*/

int bookadd(struct bookdir *pdirhead,struct book *pbook);  /*书籍信息添加*/

int bookchange(struct bookdir *pdirhead);  /*书籍信息修改*/

void booksort();  /*数据排序*/

int bookprint(struct bookdir *pbookdirhead);  /*数据统计后输出*/

void bookdiscover();  /*数据查询*/

int main(void)
{
	struct bookdir *pbookdirhead=NULL;
	struct bookdir *pbookdirtemp=NULL;
	struct book *pbook=NULL;
	if((pbookdirhead=bookload(pbookdirhead)) == NULL){
		printf("读取文件出错");
	}
	while(1){
		switch(my_interface()){
			case 1:
				pbookdirhead=bookload(pbookdirhead);
				break;
			case 2:
				/*填写新添加的图书信息*/
				pbook=(struct book *)malloc(sizeof(struct book));
				printf("请输入书名：");
				scanf("%s",pbook->bookname);
				printf("请输入书的作者：");
				scanf("%s",pbook->bookwriter);
				printf("请输入书所属的类型：");
				scanf("%s",pbook->bookgroup);
				printf("请输入书的数量：");
				scanf("%d",pbook->booknumber);
				pbook->booksp=pbook->booknumber;
				if(bookadd(pbookdirhead,pbook) == -1){
					printf("添加失败");
				}
				break;
			case 3:
				/*添加新的图书目录信息*/
				pbookdirtemp=(struct bookdir *)malloc(sizeof(struct bookdir));
				printf("请输入类型：");
				scanf("%s",pbookdirtemp->bookgroup);
				pbookdirhead=bookdiradd(pbookdirhead,pbookdirtemp);
				break;
			default:
				return 0;
				break;
		}
	}
	/*
	printf("请输入书籍名：");
	scanf("%s",pbooktemp->bookname);
	printf("请输入书籍类型：");
	scanf("%s",pbooktemp->bookgroup);
	printf("请输入书籍作者：");
	scanf("%s",pbooktemp->bookwriter);
	printf("请输入数量：");
	scanf("%d",pbooktemp->booknumber);
	pbooktemp->booksp=pbooktemp->booknumber;
	return 0;*/
}


/*绘制界面*/
int my_interface()
{
	int c=-1;
	printf("**************************************\n");
	printf("*           管理员系统               *\n");
	printf("*                                    *\n");
	printf("*  1.查询书籍信息。                  *\n");
	printf("*  2.修改书库中书籍信息。            *\n");
	printf("*  3.查看书库借阅记录。              *\n");
	printf("*  0.退出。                          *\n");
	printf("*                                    *\n");
	printf("**************************************\n");
	printf("请选择：");
	scanf("%d",&c);
	return c;
}


/*书籍信息遍历输出*/
int bookprint(struct bookdir *pbookdirhead)
{
	struct bookdir *pbookdirtemp=NULL;
	struct book *pbooktemp=NULL;
	if(pbookdirhead == NULL){
		return -1;
	}
	while(pbookdirtemp != NULL){
		printf("书的类型：%s:",pbookdirtemp->bookgroup);
		pbooktemp=pbookdirtemp->pdir;
		while(pbooktemp != NULL){
			printf("书名：%s 作者：%s 总数：%d 剩余数量：%d\n",pbooktemp->bookname,pbooktemp->bookwriter,pbooktemp->booknumber,pbooktemp->booksp);
			pbooktemp=pbooktemp->next;
		}
		pbookdirtemp=pbookdirtemp->next;
	}
	return 0;
}


/*在书库中添加书籍目录信息*/
struct bookdir *bookdiradd(struct bookdir *pbookdirhead,struct bookdir *pbookdir)
{
	struct bookdir *pbookdirtemp=pbookdirhead;
	pbookdir->next=NULL;
	if(pbookdirhead == NULL){
		pbookdirhead=pbookdir;
	}
	else{
		while(pbookdirtemp->next != NULL){         /*遍历到目录链表最末尾的节点*/
			if(! strcmp(pbookdirtemp->bookgroup,pbookdir->bookgroup)){
				printf("类型已存在，添加失败。");
				return pbookdirhead;
			}
			pbookdirtemp=pbookdirtemp->next;
		}
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
		printf("未匹配到该图书所属类型");
		return -1;
	}
	ptemp=pdirtemp->pdir;  /*进入图书某组中，将图书添加进去*/
	if(ptemp == NULL){  /*若书库中该类型书为空，则直接追加该书信息*/
		ptemp=pbook;
		pbook->next=NULL;
	}
	else{
		while(ptemp != NULL){
			if(! strcmp(pbook->bookname,ptemp->bookname)){/*有重复图书信息，则只添加图书总数和剩余数量*/
				break;
			}
			ptemp=ptemp->next;
		}
		if(ptemp != NULL){  /*只添加图书总数和剩余数量，并销毁新添加图书的结构体*/
			ptemp->booknumber=ptemp->booknumber+pbook->booknumber;
			ptemp->booksp=ptemp->booksp+pbook->booksp;
			free(pbook);
		}
		else{  /*不是重复图书信息，则追加该图书信息*/
			ptemp->next=pbook;
			pbook->next=NULL;
		}
	}
	return 0;
}





/*将书库数据存储到文件中*/
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
			continue;
		}
		pbooktemp=pbookdirtemp->pdir;
		do{
			fprintf(fp,"%s %s %s %d %d %p",pbooktemp->bookname,pbooktemp->bookwriter,pbooktemp->bookgroup,pbooktemp->booknumber,pbooktemp->booksp,pbooktemp->next);
			pbooktemp=pbooktemp->next;
		}while(pbooktemp != NULL);/*将每个分组下的书籍信息全部存入文件*/
		pbookdirtemp=pbookdirtemp->next;
	}while(pbookdirtemp != NULL);
	fclose(fp);
	return 0;
}




/*从文件中读取书库数据*/
struct bookdir *bookload(struct bookdir *pbookdirhead)
{
	struct bookdir *pbookdirtemp=NULL;
	struct book *pbooktemp=NULL;
	FILE *fp;
	pbookdirtemp=(struct bookdir *)malloc(sizeof(struct bookdir));
	pbooktemp=(struct book *)malloc(sizeof(struct book));
	if((fp=fopen("书库.txt","r")) == NULL){
		return NULL;
	}

	/*读取目录信息及书籍信息并存入链表*/
	do{
		if(fscanf(fp,"%s %p %p",pbookdirtemp->bookgroup,pbookdirtemp->next,pbookdirtemp->pdir) == -1){
			return NULL;
		}
		pbookdirhead=bookdiradd(pbookdirhead,pbookdirtemp);    /*根据目录头指针添加目录信息*/
		if(pbookdirtemp->pdir == NULL){ /*某分组中没有书籍信息*/
			continue;
		}
		pbooktemp=pbookdirtemp->pdir;
		do{
			if(fscanf(fp,"%s %s %s %d %d %p",pbooktemp->bookname,pbooktemp->bookwriter,pbooktemp->bookgroup,pbooktemp->booknumber,pbooktemp->booksp,pbooktemp->next) == -1){
				return NULL;
			}
			bookadd(pbookdirhead,pbooktemp);   /*根据目录头指针添加图书信息*/
			pbooktemp=pbooktemp->next;
		}while(pbooktemp != NULL);
		pbookdirtemp=pbookdirtemp->next;
	}while(pbookdirtemp != NULL);
	fclose(fp);
	return pbookdirhead;
}




/*修改指定的书籍信息*/
int bookchange(struct bookdir *pbookdirhead)
{
	struct bookdir *pbookdirtemp=NULL;
	struct book *pbooktemp=NULL;
	struct book *pbooklast=NULL;
	struct book *pbookch=NULL;
	char name[20];
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
			if((!strcmp(pbooktemp->bookname,name)) && (!strcmp(pbooktemp->bookwriter,writer))){
				break;
			}
			pbooklast=pbooktemp;  /*记录上一个节点的指针*/
			pbooktemp=pbooktemp->next;  /*更新节点*/
		}
		while(pbooktemp != NULL);
		if((!strcmp(pbooktemp->bookname,name)) && (!strcmp(pbooktemp->bookwriter,writer))){
			break;
		}
		pbookdirtemp=pbookdirtemp->next;
	}
	while(pbookdirtemp != NULL);
	if(pbookdirtemp == NULL){
		printf("未查找到需要修改的书籍信息");
		return -1;
	}

	/*选择需要修改的方式*/
	printf("请选择修改的方式：\n1.修改书籍信息。\n2.删除该书籍的信息。\n3.取消操作。");
	scanf("%d",&c);
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
			if(getchar() == 'Y'){
				pbooklast=pbookch;
				pbookch->next=pbooktemp->next;
				free(pbooktemp);
			}
			else{
				printf("已取消。");
				free(pbookch);
			}
			break;
		case 2:
			printf("确认删除？Y or N:");
			if(getchar() == 'Y'){
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
