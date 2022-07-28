#include<iostream>  
using namespace std;
#define MAX 1000  
int g[100][100], x[100], bestx[100];
 
int cl = 0, bestl = MAX, n;
 
//�綨������������������ʦ���㷨��������Ƶ����ĸ�����ʵ�֣��������Ͼ�������汾�Ľ綨������ͬ�����Ը���ȷ��������֧
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
	if (t>n) //����Ҷ�ӽ��  
	{
		
		if (g[x[n]][1] != -1 && (cl + g[x[n]][1]<bestl))//����Ա�������һ������������ĳ���֮����·�����ҵ�ǰ�ܾ���ȵ�ǰ����ֵС  
		{
			for (j = 1; j <= n; j++)
				bestx[j] = x[j];
			bestl = cl + g[x[n]][1];
		}
	}
	else    //û�е���Ҷ�ӽ��  
	{
		for (j = t; j <= n; j++)//������չ�������ҷ�֧���������뵱ǰ���ڳ����ٽ��ĳ���  
		{
			if (g[x[t - 1]][x[j]] != -1 && Bound(t, cl)<bestl)
			//if (g[x[t - 1]][x[j]] != -1 && (cl + g[x[t - 1]][x[j]]<bestl))//������t-1���������t������֮����·���ҿ��Եõ����̵�·��  
			{
 
				swap(x[t], x[j]);     //����Ҫȥ�ĵ�t�����е�x[t]��  
				cl += g[x[t - 1]][x[t]]; //·�߳�������  
				Traveling(t + 1);      //������һ������  
				cl -= g[x[t - 1]][x[t]];
				swap(x[t], x[j]);
			}
		}
	}
}
int main()
{
	int i, j;
	cout << "������һ���м������У�" << endl;
	cin >> n;
	cout << "���������֮��ľ���" << endl;
 
	for (i = 1; i <= n; i++)
		for (j = 1; j <= n; j++)
			cin >> g[i][j];
 
	for (i = 1; i <= n; i++)
	{
		x[i] = i;
		bestx[i] = 0;
	}
 
	Traveling(2);
	cout << "����·�ߣ�" << endl;
	for (i = 1; i <= n; i++)
		cout << bestx[i] << ' ';
	cout << bestx[1];
	cout << endl;
	cout << "���·�߳��ȣ�" << endl;
	cout << bestl << endl;
	return 0;
}
