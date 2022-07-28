#include<iostream>
#include<stdio.h>
#include<math.h>
#include<string.h>

using namespace std;

void initial();
void dijkstra(int start);

const int maxInt = 1000000;
int start;
int m,n;			//m个顶点，n条边 
int dis[100];		//起始点到点i之间的最短距离，会不断被更新 
int book[100];		//标志着i有无被访问 
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

	cout<<"请输入顶数和边数"<<endl;
	cin>>m>>n;		//m个顶点,n条边 
	cout<<"请输入起始点"<<endl;
	cin>>start;		//求start到各个点的距离 
	cout<<"请输入顶点间边的权重，如顶点1和2之间的边权为3，则输入1 2 3"<<endl;
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
		dis[i] = dis[i] < map[start][i]? dis[i]:map[start][i];	//先进行一轮初始化 
	}

	book[start] = 1;
	
	for(int i = 0; i<m; i++){
		int min = maxInt;
		int next = 0;
		for(int j = 0; j<m; j++){	//记录当前时刻距离源点最近距离，且未被探索的点，将其作为扩展点 
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
		cout<<"点"<<start<<"到第"<<i<<"个节点的最短距离为："<<dis[i]<<endl;
	}
	
	cout<<"从终点到起点（逆向）的最短路径为："; 
 	int p = 6;
	cout<<p;
	while(path[p]!=-1){
		cout<<"-->"<<path[p];
		p = path[p];
	}
	cout<<"-->"<<start<<endl;
 } 

