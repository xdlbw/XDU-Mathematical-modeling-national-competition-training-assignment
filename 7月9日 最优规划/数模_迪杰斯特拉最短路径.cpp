#include<iostream>
#include<stdio.h>
#include<math.h>
#include<string.h>

using namespace std;

void initial();
void dijkstra(int start);

const int maxInt = 1000000;
int start;
int m,n;			//m�����㣬n���� 
int dis[100];		//��ʼ�㵽��i֮�����̾��룬�᲻�ϱ����� 
int book[100];		//��־��i���ޱ����� 
int map[100][100];
int path[100];

int main(){
	initial();
	dijkstra(start);
	return 0;
 }
 
void initial(){
	int x,y,w;
	memset(dis, 88, sizeof dis);
	memset(map, 88, sizeof map);

	cout<<"�����붥���ͱ���"<<endl;
	cin>>m>>n;		//m������,n���� 
	cout<<"��������ʼ��"<<endl;
	cin>>start;		//��start��������ľ��� 
	cout<<"�����붥���ߵ�Ȩ�أ��綥��1��2֮��ı�ȨΪ3��������1 2 3"<<endl;
	for(int i = 0; i<n; i++){
		cin>>x>>y>>w;
		map[x][y] = w;
		map[y][x] = w;
		map[i][i] = 0;
		path[i] = -1;
	}
}
 
void dijkstra(int start){
	for(int i = 0; i<m; i++){
		dis[i] = dis[i] < map[start][i]? dis[i]:map[start][i];	//�Ƚ���һ�ֳ�ʼ�� 
	}

	book[start] = 1;
	
	for(int i = 0; i<m; i++){
		int min = maxInt;
		int next = 0;
		for(int j = 0; j<m; j++){	//��¼��ǰʱ�̾���Դ��������룬��δ��̽���ĵ㣬������Ϊ��չ�� 
			if(!book[j]&&min>dis[j]){
				min = dis[j];
				next = j;
			}
		}
		book[next] = 1;
		for(int j = 0; j<m; j++){
			if (!book[j] && dis[next] + map[next][j] < dis[j]){
                dis[j] = dis[next] + map[next][j];
                path[j] = next;
            }
		} 
	}
	
	for(int i = start; i<m; i++){
		cout<<"��"<<start<<"����"<<i<<"���ڵ����̾���Ϊ��"<<dis[i]<<endl;
	}
	
	cout<<"���յ㵽��㣨���򣩵����·��Ϊ��"; 
 	int p = 6;
	cout<<p;
	while(path[p]!=-1){
		cout<<"-->"<<path[p];
		p = path[p];
	}
	cout<<"-->"<<start<<endl;
 } 

