#include<iostream>  
using namespace std;
#define MAX 1000  
int g[100][100], x[100], bestx[100];
 
int cl = 0, bestl = MAX, n;
 
//界定函数，按照屈婉玲老师的算法公开课视频讲解的给出的实现，这与网上绝大多数版本的界定函数不同，可以更精确的削减分支
double Bound(int t, int cl)
{
	double min1 = 0, min2 = 0, tempSum=0;
	for (int j = t; j <= n; j++)
	{
		if (g[x[t - 1]][x[j]] != -1 && g[x[t - 1]][x[j]] < min1)
		{
			min1 = g[x[t - 1]][x[j]];
		}
		for (int i = 1; i <= n; ++i)
		{
			if (g[x[j]][x[i]] != -1 && g[x[j]][x[i]] < min2)
			{
				min2 = g[x[j]][x[i]];
			}
		}
		tempSum += min2;
	}
 
	return cl + min1 + tempSum;
 
}
 
 
void Traveling(int t)
{
	int j;
	if (t>n) //到达叶子结点  
	{
		
		if (g[x[n]][1] != -1 && (cl + g[x[n]][1]<bestl))//推销员到的最后一个城市与出发的城市之间有路径，且当前总距离比当前最优值小  
		{
			for (j = 1; j <= n; j++)
				bestx[j] = x[j];
			bestl = cl + g[x[n]][1];
		}
	}
	else    //没有到达叶子结点  
	{
		for (j = t; j <= n; j++)//搜索扩展结点的左右分支，即所有与当前所在城市临近的城市  
		{
			if (g[x[t - 1]][x[j]] != -1 && Bound(t, cl)<bestl)
			//if (g[x[t - 1]][x[j]] != -1 && (cl + g[x[t - 1]][x[j]]<bestl))//若果第t-1个城市与第t个城市之间有路径且可以得到更短的路线  
			{
 
				swap(x[t], x[j]);     //保存要去的第t个城市到x[t]中  
				cl += g[x[t - 1]][x[t]]; //路线长度增加  
				Traveling(t + 1);      //搜索下一个城市  
				cl -= g[x[t - 1]][x[t]];
				swap(x[t], x[j]);
			}
		}
	}
}
int main()
{
	int i, j;
	cout << "请输入一共有几个城市：" << endl;
	cin >> n;
	cout << "请输入城市之间的距离" << endl;
 
	for (i = 1; i <= n; i++)
		for (j = 1; j <= n; j++)
			cin >> g[i][j];
 
	for (i = 1; i <= n; i++)
	{
		x[i] = i;
		bestx[i] = 0;
	}
 
	Traveling(2);
	cout << "城市路线：" << endl;
	for (i = 1; i <= n; i++)
		cout << bestx[i] << ' ';
	cout << bestx[1];
	cout << endl;
	cout << "最短路线长度：" << endl;
	cout << bestl << endl;
	return 0;
}
