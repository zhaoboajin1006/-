/*************************************************************************
	> File Name: lanjiedaodan.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2015年06月25日 星期四 21时29分22秒
 ************************************************************************/

/*某国为了防御敌国的导弹袭击，发展出一种导弹拦截系统。但是这种导弹拦截系统有一个缺陷：虽然
 * 它的第一发炮弹能够到达任意的高度，但是以后每一发炮弹都不能高于前一发的高度。某天，雷达捕捉到
 * 敌国的导弹来袭。由于该系统还在试用阶段，所以只有一套系统，因此有可能不能拦截所有的导弹。
 * 输入导弹依次飞来的高度（雷达给出的高度数据是不大于30000 的正整数），计算
 * (1)这套系统最多能拦截多少导弹
 * (2)如果要拦截所有导弹最少要配备多少套这种导弹拦截系统。*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int max(int num1,int num2);

int dpfunc1(int num,int *high);

int dpfunc2(int num,int *high);

int main(void)
{
	int i=0,num=0,*high=NULL;
	printf("请输入导弹数量：");
	scanf("%d",&num);
	high=(int *)malloc(num*sizeof(int));
	while(i < num){
		printf("请输入第%d个导弹的高度：",i+1);
		scanf("%d",&high[i++]);
	}
	i=0;
	while(i < num){printf("%d ",high[i++]);}
	printf("这套系统最多能拦截%d个导弹\n",dpfunc1(num,high));
	printf("如果要拦截所有导弹，最少需要配备%d套这种导弹拦截系统\n",dpfunc2(num,high));
	return 0;
}

int max(int num1,int num2)
{
	if(num1 >= num2){
		return num1;
	}
	else{
		return num2;
	}
}

int dpfunc1(int num,int *high)
{
	int dp[num];
	int i=0,j=0,MAX=0;
	for(i=0;i<num;i++){
		dp[i]=1;
	}
	i=0;
	for(i=0;i<num;i++){   /*递推求解*/
		for(j=0;j<i;j++){
			if(high[i] <= high[j]){
				dp[i]=max(dp[i],dp[j]+1);   /*将dp[i],dp[j]+1中的较大者赋给dp[i]*/
				if(MAX <= dp[i]){
					MAX=dp[i];
				}
			}
		}
	}
	return MAX;
}

int dpfunc2(int num,int *high)
{
	int dp[num];
	int i=0,j=0,MAX=0,n=0,count=0;;
	for(i=0;i<num;i++){
		dp[i]=1;
	}
	i=0;
	for(i=0;i<num;i++){   /*递推求解*/
		for(j=0;j<i;j++){
			if(high[i] <= high[j]){
				dp[i]=max(dp[i],dp[j]+1);   /*将dp[i],dp[j]+1中的较大者赋给dp[i]*/
				if(MAX <= dp[i]){
					MAX=dp[i];
				}
			}
		}
	}
	i=0;
	j=1;

	/*最后一次遍历整个dp组，检查出某个子序列后j=1，故多执行了一次count++，所以答案为count-1*/
	while(j == 1){
		count++;  /*记录子序列个数*/
		j=0;   /*j作为哨兵监视是否进行归零处理，即是否有子序列*/
		for(i=1;i <= MAX;i++){
			for(n=0;n<num;n++){
				if(dp[n] == i){
					dp[n]=0;
					j=1;
					break;
				}
			}
		}
	}
	return count-1;
}

