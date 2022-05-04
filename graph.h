#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;
#include <stack>
#include<Windows.h>
constexpr auto INF = 99999; //无穷大;
constexpr auto NUM_CITY = 199;//读入城市数量
constexpr auto NUM_ROUTE =1975;//读入边数


//存储城市信息
typedef struct Cities {
	string _Country;//国家名
	string _City;//首都名
	float _Longitude=0;//经度
	float _Latitude=0;//纬度
	bool _Pass;//标记是否选中（后面最短路径有用）
}Cities;


//存储路径信息
typedef struct Routes {
	string _StartCity;//边起点
	string _EndCity;//边终点
	string _GoWay;//路径的交通方式
	float _Time=0;//花费时间
	float _Cost=0;//花费费用
	string _Description;//描述信息

}Routes;


//图的存储——邻接矩阵
typedef struct Graph {

	Cities* vertex;
	Routes arcs[NUM_CITY][NUM_CITY];//用二维数组存储路线，横向是始发地对应索引号，纵向是终点地对应索引号
	int vexnum;//顶点的个数
	int	arcnum;//路径的个数

}Graph;

//存储路线
typedef struct {
	bool _Path;//选中是否
	int _Pre;//标记前面一个城市的索引号
}Way;

//辅助数据结构——队列
typedef struct {
	int* base;
	int front;
	int rear;

}SqQueue;

//读取城市CSV文件
void Read_City_CSV(char* Filename, Cities*& mycities);

//读取路径CSV文件
void Read_Route_CSV(char* Filename, Routes*& myroutes);

//查找指定城市名的对应索引号
int Locate(string city, Cities* mycities);

//初始化并且创建图
void Init_Create_Graph(Graph& mygraph, Cities* mycities, Routes* myroutes);

//深度优先遍历:从第_Begin个顶点出发递归遍历图
void DFS(Graph &mygraph, int _Begin,bool *myvisit);

//广度优先遍历：从第_Begin个顶点出发遍历图
void BFS(Graph& mygraph, int _Begin, bool* myvisit, SqQueue& Q);



//最小花费算法（迪杰斯特拉算法）
//_Begin起点城市索引号，_End终点城市索引号
void Shortest_Path_Cost(const Graph& mygraph, int _Begin, int _End, Way way[][NUM_CITY], float dist[][NUM_CITY]);

//最短时间算法（迪杰斯特拉算法）
////_Begin起点城市索引号,_End终点城市索引号
void Shortest_Path_Time(const Graph& mygraph, int _Begin, int _End, Way way[][NUM_CITY], float dist[][NUM_CITY]);


//可视化操作
void Output_Into_Htm(char* Filename, Graph mygraph, int _Begin, int _End, Cities* mycities, Way way[][NUM_CITY]);


//初始化一个空队列
int IniQueue(SqQueue& Q);

//进入队列
void EnQueue(SqQueue& Q, int e);

//出队列返回给e
void  DeQueue(SqQueue& Q, int& e);