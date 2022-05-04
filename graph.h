#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
using namespace std;
#include <stack>
#include<Windows.h>
constexpr auto INF = 99999; //�����;
constexpr auto NUM_CITY = 199;//�����������
constexpr auto NUM_ROUTE =1975;//�������


//�洢������Ϣ
typedef struct Cities {
	string _Country;//������
	string _City;//�׶���
	float _Longitude=0;//����
	float _Latitude=0;//γ��
	bool _Pass;//����Ƿ�ѡ�У��������·�����ã�
}Cities;


//�洢·����Ϣ
typedef struct Routes {
	string _StartCity;//�����
	string _EndCity;//���յ�
	string _GoWay;//·���Ľ�ͨ��ʽ
	float _Time=0;//����ʱ��
	float _Cost=0;//���ѷ���
	string _Description;//������Ϣ

}Routes;


//ͼ�Ĵ洢�����ڽӾ���
typedef struct Graph {

	Cities* vertex;
	Routes arcs[NUM_CITY][NUM_CITY];//�ö�ά����洢·�ߣ�������ʼ���ض�Ӧ�����ţ��������յ�ض�Ӧ������
	int vexnum;//����ĸ���
	int	arcnum;//·���ĸ���

}Graph;

//�洢·��
typedef struct {
	bool _Path;//ѡ���Ƿ�
	int _Pre;//���ǰ��һ�����е�������
}Way;

//�������ݽṹ��������
typedef struct {
	int* base;
	int front;
	int rear;

}SqQueue;

//��ȡ����CSV�ļ�
void Read_City_CSV(char* Filename, Cities*& mycities);

//��ȡ·��CSV�ļ�
void Read_Route_CSV(char* Filename, Routes*& myroutes);

//����ָ���������Ķ�Ӧ������
int Locate(string city, Cities* mycities);

//��ʼ�����Ҵ���ͼ
void Init_Create_Graph(Graph& mygraph, Cities* mycities, Routes* myroutes);

//������ȱ���:�ӵ�_Begin����������ݹ����ͼ
void DFS(Graph &mygraph, int _Begin,bool *myvisit);

//������ȱ������ӵ�_Begin�������������ͼ
void BFS(Graph& mygraph, int _Begin, bool* myvisit, SqQueue& Q);



//��С�����㷨���Ͻ�˹�����㷨��
//_Begin�����������ţ�_End�յ����������
void Shortest_Path_Cost(const Graph& mygraph, int _Begin, int _End, Way way[][NUM_CITY], float dist[][NUM_CITY]);

//���ʱ���㷨���Ͻ�˹�����㷨��
////_Begin������������,_End�յ����������
void Shortest_Path_Time(const Graph& mygraph, int _Begin, int _End, Way way[][NUM_CITY], float dist[][NUM_CITY]);


//���ӻ�����
void Output_Into_Htm(char* Filename, Graph mygraph, int _Begin, int _End, Cities* mycities, Way way[][NUM_CITY]);


//��ʼ��һ���ն���
int IniQueue(SqQueue& Q);

//�������
void EnQueue(SqQueue& Q, int e);

//�����з��ظ�e
void  DeQueue(SqQueue& Q, int& e);