/*************************************************************************
	> File Name: fibonacci.c
	> Author: ma6174
	> Mail: ma6174@163.com 
	> Created Time: 2015年06月25日 星期四 16时05分10秒
 ************************************************************************/

/*使用动态规划算法解决斐波拉契数列问题*/

#include<stdio.h>
#include<string.h>
int dpcalculation(int num);

int main(void)
{
	int n,m;
	printf("求斐波拉契数列的第几个数:");
	scanf("%d",&n);
	printf("该数为：%d\n",dpcalculation(n));
	return 0;
}

int dpcalculation(int num)
{
	int i=0;
	int dp[num];
	memset(dp,-1,num*sizeof(int));  /*把dp数组全部置为-1*/
	for(i=0;i<num;i++){
		if((i == 0) || (i == 1)){
			dp[i]=1;
		}
		else{
			if(dp[i] == -1){
				dp[i]=dp[i-1]+dp[i-2];
			}
		}
	}
	return dp[num-1];
}

