#include "graph.h"
#include "Timer.h"
void Read_City_CSV(char* Filename, Cities*& mycities) {
	FILE* fp = NULL;
	fp = fopen(Filename, "r");
	if (!fp) {
		cout << "Error!" << endl;
		exit(0);
	}
	//���建����
	char tem1='\0';//�ַ�
	float tem2=0;//����
	//�����Ŀ����NUM_CITY���ռ�
	mycities = new Cities[NUM_CITY];
	int i = 0;
	for (int i = 0; i < NUM_CITY; i++) {
		tem1 = fgetc(fp);
		for (; tem1 != ','; tem1 = fgetc(fp)) {
			if (tem1 =='\'')tem1 = '\\\'';
			mycities[i]._Country += tem1;
		}
		tem1 = fgetc(fp);
		for (; tem1 != ','; tem1 = fgetc(fp)) {
			if (tem1 == '\'')tem1 = '\\\'';
			mycities[i]._City+= tem1;//������
		}
		fscanf(fp, "%f", &tem2);//��γ��
		mycities[i]._Latitude = tem2;
		tem1 = fgetc(fp);//���嶺��
		fscanf(fp, "%f\n", &tem2);//������
		mycities[i]._Longitude = tem2;
		
	}
	fclose(fp);
}

void Read_Route_CSV(char* Filename, Routes*& myroutes)
{
	FILE* fp = NULL;
	fp = fopen(Filename, "r");
	if (!fp) { cout << "Error��" << endl; exit(0); }
	//���建����
	char tem1='\0';//�ַ�
	float tem2=0;//����
	//������Ŀ���� NUM_ROUTE ���ռ�
	myroutes = new Routes[NUM_ROUTE];
	for (int i = 0; i < NUM_ROUTE;i++) {
		tem1 = fgetc(fp);
		for (; tem1 != ','; tem1 = fgetc(fp)) {
			if (tem1 == '\'')tem1 = '\\\'';
       myroutes[i]._StartCity += tem1;//�����
		}
		tem1 = fgetc(fp);
		for (; tem1 != ','; tem1 = fgetc(fp)) {
			if (tem1 == '\'')tem1 = '\\\'';
			myroutes[i]._EndCity += tem1;//���յ�
		}
		tem1 = fgetc(fp);
		for (; tem1 != ','; tem1 = fgetc(fp)) {
			myroutes[i]._GoWay+= tem1;//����ͨ��ʽ
		}
		fscanf(fp, "%f", &tem2);//��ʱ��
		myroutes[i]._Time = tem2;
		tem1 = fgetc(fp);//���嶺��

		fscanf(fp, "%f", &tem2);//������
		myroutes[i]._Cost = tem2;
		tem1 = fgetc(fp);//���嶺��
		tem1 = fgetc(fp);//��ȡ��һ��
		for (; tem1 != '\n'; tem1 = fgetc(fp)) {
			if (tem1 == '\'')tem1 = '\\\'';
			myroutes[i]._Description += tem1;//��������Ϣ
		}
	}
	fclose(fp);
}

int Locate(string city, Cities* mycities) {
	for (int i = 0; i < NUM_CITY; i++)
	{
		if (city == mycities[i]._City)
		{//һ��������

			return i;//�ҵ��˷������к�
		}
	}
	return -1;//û�ҵ�������ֵ-1
}

void Init_Create_Graph(Graph& mygraph, Cities* mycities, Routes* myroutes) {
	//��ʼ��ʼ��ͼ
	mygraph.arcnum = NUM_CITY * NUM_CITY;
	mygraph.vexnum = NUM_CITY;
	mygraph.vertex = mycities;
	for (int i = 0; i < NUM_CITY; i++) {
		for (int j = 0; j < NUM_CITY; j++) {
			if (i == j) {//�ڽӾ���Խ����ϵĴ���
				mygraph.arcs[i][j]._Cost = 0;
				mygraph.arcs[i][j]._Time = 0;
			}
			else {//û�ж�������֮ǰȫ����ʼ��Ϊ�����
				mygraph.arcs[i][j]._Cost = INF;
				mygraph.arcs[i][j]._Time = INF;
			}
		}

	}
	//��ʼ¼��ߵ�����
	for (int k = 0; k < NUM_ROUTE; k++) {
		//��ȷ��·��ʼ�������յ�ض�Ӧ��������
		int _Start = -1;
		_Start = Locate(myroutes[k]._StartCity, mycities);
		int _End = -1;
		_End = Locate(myroutes[k]._EndCity, mycities);
		//�ҵ��߶�Ӧ�Ķ��������ź�
		if ((_Start>=0) && (_End>=0)) {
			/*if (mygraph.arcs[_Start][_End]._Time == NULL) {*/
				//¼��·���е�ʱ�������
				mygraph.arcs[_Start][_End]._Time = myroutes[k]._Time;
				mygraph.arcs[_Start][_End]._Cost = myroutes[k]._Cost;
				//¼��·�ߵĽ�ͨ��ʽ��������Ϣ
				mygraph.arcs[_Start][_End]._GoWay = myroutes[k]._GoWay;
				mygraph.arcs[_Start][_End]._Description = myroutes[k]._Description;
			
			//else {//���ڽ�ͨ��ʽ��ͬ������·�߻��к�ʱ��ͬ��������������ǻ����ʱ�̵�
			//	//���´���Ḳ����ʱ����·��
			//	if (myroutes[k]._Time < mygraph.arcs[_Start][_End]._Time) {
			//		//����¼��·���е�ʱ�������
			//		mygraph.arcs[_Start][_End]._Time = myroutes[k]._Time;
			//		mygraph.arcs[_Start][_End]._Cost = myroutes[k]._Cost;
			//		//����¼��·�ߵĽ�ͨ��ʽ��������Ϣ
			//		mygraph.arcs[_Start][_End]._GoWay = myroutes[k]._GoWay;
			//		mygraph.arcs[_Start][_End]._Description = myroutes[k]._Description;

			//	}
			//	//��һ��˼�����������·�ߺ�ʱһ�����Ϳ��Ƿ���
			//	//���´���Ḳ�Ƿ��øߵ���Ϣ
			//	if (myroutes[k]._Cost < mygraph.arcs[_Start][_End]._Cost) {
			//		//����¼��·���еķ���
			//		mygraph.arcs[_Start][_End]._Cost = myroutes[k]._Cost;
			//		//����¼��·�ߵĽ�ͨ��ʽ��������Ϣ
			//		mygraph.arcs[_Start][_End]._GoWay = myroutes[k]._GoWay;
			//		mygraph.arcs[_Start][_End]._Description = myroutes[k]._Description;
			//	}
			//	//�õ�����С��ʱ����С����
			//}
		}
	}
}

void DFS(Graph &mygraph, int _Begin, bool*myvisit)
{ 
	myvisit[_Begin] = true;//�ݹ�˼�룬һ�����뼴Ϊ���ʵ�
	printf("%-40s %-40s %-20f %-20f\n",//��ӡ������̨��
		mygraph.vertex[_Begin]._Country.c_str(),
		mygraph.vertex[_Begin]._City.c_str(),
		mygraph.vertex[_Begin]._Latitude, 
		mygraph.vertex[_Begin]._Longitude);
	for (int j = 0; j < mygraph.vexnum; j++)
	{//���Ĵ���
		if ((mygraph.arcs[_Begin][j]._Cost < INF)&&(mygraph.arcs[_Begin][j]._Cost >=0) && myvisit[j] == false)
		{//����������·��û������
			DFS(mygraph, j,  myvisit);
		}
	}
}

void BFS(Graph& mygraph, int _Begin, bool* myvisit,SqQueue &Q) {
	for (int v = 0; v < mygraph.vexnum; v++) myvisit[v] = FALSE;
	myvisit[_Begin] = TRUE;
	printf("%-40s %-40s %-20f %-20f\n",//��ӡ������̨��
		mygraph.vertex[_Begin]._Country.c_str(),
		mygraph.vertex[_Begin]._City.c_str(),
		mygraph.vertex[_Begin]._Latitude,
		mygraph.vertex[_Begin]._Longitude);
	IniQueue(Q);
	for (int v = 0; v < mygraph.vexnum; v++) {
		if ((mygraph.arcs[_Begin][v]._Cost < INF) && myvisit[v] == false) {
		myvisit[v] = TRUE;
		printf("%-40s %-40s %-20f %-20f\n",//��ӡ������̨��
			mygraph.vertex[v]._Country.c_str(),
			mygraph.vertex[v]._City.c_str(),
			mygraph.vertex[v]._Latitude,
			mygraph.vertex[v]._Longitude);
		EnQueue(Q, v);//ͬʱ���
	}
	}

	int u = 0;
	while (Q.front != Q.rear) {
		   DeQueue(Q, u);
		   for (int k = 0; k < mygraph.vexnum; k++) {
			   if ((mygraph.arcs[u][k]._Cost < INF) && myvisit[k] == false) {
				   myvisit[k] = TRUE;
				   printf("%-40s %-40s %-20f %-20f\n",//��ӡ������̨��
					   mygraph.vertex[k]._Country.c_str(),
					   mygraph.vertex[k]._City.c_str(),
					   mygraph.vertex[k]._Latitude,
					   mygraph.vertex[k]._Longitude);
				   EnQueue(Q, k);//ͬʱ���
			   }
		   }

	}
}


void Shortest_Path_Cost(const Graph& mygraph, int _Begin, int _End, Way way[][NUM_CITY], float dist[][NUM_CITY])
{
	float mymincost = 0;//���ٻ��ѳ�ʼ��
	//��forѭ���ǳ�ʼ��
	for (int i = 0; i < mygraph.vexnum; i++)
	{
		mygraph.vertex[i]._Pass = false;//��û��ѡ��
		dist[_Begin][i] = mygraph.arcs[_Begin][i]._Cost;//��㵽���е�ķ��ø�ֵ��dist
		if (dist[_Begin][i] < INF) 
		{
			//��ֱ��·��
			way[i][_Begin]._Path = true;
			way[i][i]._Path = true;
			way[i][i]._Pre = _Begin;//ǰһ�������������
		}
		else 
		{
			//û��ֱ��·��
			way[i][i]._Pre = -1;
		}
	}
	mygraph.vertex[_Begin]._Pass = true;//��㱻ѡ��

	Timer timer1;//��ʱ��
	//�����㷨�����Ͻ�˹�����㷨
	timer1.Start();
	for (int j = 0; j < mygraph.vexnum; j++)
	{
		float mincost = INF;//��ǰֵ��ʼ��Ϊ����
		
		int  Cur_Min = 0;//��Cur_Min��ǵ�ǰ���ٻ��Ѷ�Ӧ��������
		//��forѭ������С
		for (int k = 0; k < mygraph.vexnum; k++)
		{   //��������ļ��
			if (mygraph.vertex[k]._Pass == false) 
			{   //û�б�ѡ�еĶ���
				if (dist[_Begin][k] < mincost) {
					Cur_Min = k;
					mincost = dist[_Begin][k];//��С��dist������ǰֵ
				}
			}
		}

		mygraph.vertex[Cur_Min]._Pass = true;//��һ��ѭ���ҵ������ٻ��Ѷ�Ӧ�Ķ��㱻ѡ��

		//��forѭ���ǽ����·���õ�����Cur_Min����·��
		for (int w = 0; w < mygraph.vexnum; w++) {
			
			if (mygraph.vertex[w]._Pass == false && (mincost + mygraph.arcs[Cur_Min][w]._Cost < dist[_Begin][w]))
			{//û�б�ѡ�еĶ���
			 //��_Begin��>Cur_Min->w��_CostС��_Begin->w��_Cost

				dist[_Begin][w] = mincost + mygraph.arcs[Cur_Min][w]._Cost;//������㵽����w�����ٻ���
				//����ֱ��·��
				for (int v = 0; v < mygraph.vexnum; v++) {
					way[w][v]._Path = way[Cur_Min][v]._Path;
					way[w][v]._Pre = way[Cur_Min][v]._Pre;
				}
				//���¶�ά����way�������
				way[w][w]._Path = true;
				way[w][w]._Pre = Cur_Min;//ǰһ������Ҫ����
				mymincost = dist[_Begin][_End];
			}
		}
	}
	timer1.Stop();
	printf("Elapsed time is: %4.2lf ms\n", timer1.ElapsedTime());
	if (mymincost) {
		cout << "����������յ����С���ѽ�ǮΪ��" << "$" << mymincost << endl;
	}
	else cout << "û�����·�����޷������" << endl;
	
}

void Shortest_Path_Time(const Graph& mygraph, int _Begin, int _End, Way way[][NUM_CITY], float dist[][NUM_CITY])
{//��Ѱ�����ٻ������ƣ���û��дע��
	float mymintime = 0;
	for (int i = 0; i < mygraph.vexnum; i++) {
		mygraph.vertex[i]._Pass = false;//
		dist[_Begin][i] = mygraph.arcs[_Begin][i]._Time;
		if (dist[_Begin][i] < INF) {
			way[i][_Begin]._Path = true;
			way[i][i]._Path = true;
			way[i][i]._Pre = _Begin;
		}
		else {
			way[i][i]._Pre = -1;
		}
	}
	mygraph.vertex[_Begin]._Pass = true;
	Timer timer2;
	timer2.Start();
	for (int j = 0; j < mygraph.vexnum; j++) {
		int  Cur_Min = 0;
		float mintime = INF;
		for (int k = 0; k < mygraph.vexnum; k++) {
			if (mygraph.vertex[k]._Pass == false) {
				if (dist[_Begin][k] < mintime) {
					Cur_Min = k;
					mintime = dist[_Begin][k];
				}
			}
		}
		mygraph.vertex[Cur_Min]._Pass = true;
		for (int w = 0; w < mygraph.vexnum; ++w) {
			if (mygraph.vertex[w]._Pass == false && (mintime + mygraph.arcs[Cur_Min][w]._Time < dist[_Begin][w]))
			{
				dist[_Begin][w] = mintime + mygraph.arcs[Cur_Min][w]._Time;
				for (int v = 0; v < mygraph.vexnum; v++) {
					way[w][v]._Path = way[Cur_Min][v]._Path;
					way[w][v]._Pre = way[Cur_Min][v]._Pre;
				}
				way[w][w]._Path = true;
				way[w][w]._Pre = Cur_Min;
				mymintime = dist[_Begin][_End];
			}
		}
	}
	timer2.Stop();
	printf("Elapsed time is: %4.2lf ms\n", timer2.ElapsedTime());
	if (mymintime) {
		cout << "����������յ����̻���ʱ��Ϊ�� " << mymintime << "Сʱ" << endl;
	}
	else cout << "û�����·�����޷������" << endl;
}

void Output_Into_Htm(char* Filename, Graph mygraph  ,int _Begin, int _End, Cities* mycities, Way way[][NUM_CITY]) {

	FILE* fp;
	fp = fopen(Filename, "w");
	if (fp == NULL)
	{
		cout << "error3!";
		exit(0);
	}
	stack<int>mypath;
	mypath.push(_End);
	int temp = _End;
	while (temp != _Begin) {
		temp = way[_End][temp]._Pre;
		mypath.push(temp);
	}


	//��б�� ��\������ʾ���У�ָ���ַ���д�ɶ���ʱ�Ļ���
	fprintf(fp, "<!DOCTYPE html><html><head>\
<style type='text/css'>body, html{width: 100%%;height: 100%%;margin:0;font-family:'΢���ź�';}#allmap{height:100%%;width:100%%;}#r-result{width:100%%;}</style>\
<script type='text/javascript' src='http://api.map.baidu.com/api?v=2.0&ak=nSxiPohfziUaCuONe4ViUP2N'></script><title>Shortest path from %s to %s</title></head><body>\
<div id='allmap'></div></div></body></html><script type='text/javascript'>\
var map = new BMap.Map('allmap');\
var point = new BMap.Point(0, 0);\
map.centerAndZoom(point, 2);\
map.enableScrollWheelZoom(true);\n", mycities[_Begin]._City.c_str(), mycities[_End]._City.c_str());


	int j = 0;
	int bycity = _Begin, bycity2;

	while (1)
	{ //��һ����
		fprintf(fp, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\n\
map.addOverlay(marker%d);\n\
var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\n\
marker%d.addEventListener(\"click\", function(){\
this.openInfoWindow(infoWindow%d);});\n", j, mycities[bycity]._Longitude, mycities[bycity]._Latitude, j, j, mycities[bycity]._Country.c_str(), mycities[bycity]._City.c_str(), j, j);
		j++;
		cout << mycities[mypath.top()]._City << "->->";
		bycity2 = bycity;
		mypath.pop();
		if (!mypath.empty()) 
		{
			bycity = mypath.top();
		}
		
		else break;
		//�ڶ�����
		fprintf(fp, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\n\
map.addOverlay(marker%d);\n\
var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\n\
marker%d.addEventListener(\"click\", function(){\
this.openInfoWindow(infoWindow%d);}); ", j, mycities[bycity]._Longitude, mycities[bycity]._Latitude, j, j, mycities[bycity]._Country.c_str(), mycities[bycity]._City.c_str(), j, j);
		//����
		fprintf(fp, "var contentString0%d = '%s, %s --> %s, %s (%s - %.2f hours - $%.2f - %s)';\n\
var path%d = new BMap.Polyline([new BMap.Point(%.4f, %.4f),new BMap.Point(%.4f, %.4f)], {strokeColor:'#18a45b', strokeWeight:8, strokeOpacity:0.8});\
map.addOverlay(path%d);\
path%d.addEventListener(\"click\", function(){\n\
alert(contentString0%d);});\n", j, mycities[bycity2]._City.c_str(), mycities[bycity2]._Country.c_str(),
mycities[bycity]._City.c_str(), mycities[bycity]._Country.c_str(),
mygraph.arcs[bycity2][bycity]._GoWay.c_str(), mygraph.arcs[bycity2][bycity]._Time, mygraph.arcs[bycity2][bycity]._Cost, mygraph.arcs[bycity2][bycity]._Description.c_str(),
j, mycities[bycity2]._Longitude, mycities[bycity2]._Latitude, mycities[bycity]._Longitude, mycities[bycity]._Latitude, j, j, j);
	}
	fprintf(fp, "</script>\n");
	fclose(fp);
}

int IniQueue(SqQueue&Q) {
	Q.base = (int*)malloc(NUM_CITY * sizeof(int));
	if (!Q.base)exit(0);
	Q.front = Q.rear = 0;
	return 1;
}

void EnQueue(SqQueue& Q, int e) {
	if ((Q.rear + 1) % NUM_CITY == Q.front)exit(0);//������
	Q.base[Q.rear] = e;
	Q.rear = (Q.rear + 1) % NUM_CITY;
	return;
}

void  DeQueue(SqQueue& Q, int& e) {
	//�����в��գ���ɾ��Q�Ķ�ͷԪ�أ���e������ֵ��
	//���򷵻�ERROR
	if (Q.front == Q.rear)exit(0);
	e = Q.base[Q.front];
	Q.front = (Q.front + 1) % NUM_CITY;
	return;
}