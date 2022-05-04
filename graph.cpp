#include "graph.h"
#include "Timer.h"
void Read_City_CSV(char* Filename, Cities*& mycities) {
	FILE* fp = NULL;
	fp = fopen(Filename, "r");
	if (!fp) {
		cout << "Error!" << endl;
		exit(0);
	}
	//定义缓存区
	char tem1='\0';//字符
	float tem2=0;//坐标
	//结合题目分配NUM_CITY个空间
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
			mycities[i]._City+= tem1;//读城市
		}
		fscanf(fp, "%f", &tem2);//读纬度
		mycities[i]._Latitude = tem2;
		tem1 = fgetc(fp);//缓冲逗号
		fscanf(fp, "%f\n", &tem2);//读经度
		mycities[i]._Longitude = tem2;
		
	}
	fclose(fp);
}

void Read_Route_CSV(char* Filename, Routes*& myroutes)
{
	FILE* fp = NULL;
	fp = fopen(Filename, "r");
	if (!fp) { cout << "Error！" << endl; exit(0); }
	//定义缓存区
	char tem1='\0';//字符
	float tem2=0;//坐标
	//根据题目分配 NUM_ROUTE 个空间
	myroutes = new Routes[NUM_ROUTE];
	for (int i = 0; i < NUM_ROUTE;i++) {
		tem1 = fgetc(fp);
		for (; tem1 != ','; tem1 = fgetc(fp)) {
			if (tem1 == '\'')tem1 = '\\\'';
       myroutes[i]._StartCity += tem1;//读起点
		}
		tem1 = fgetc(fp);
		for (; tem1 != ','; tem1 = fgetc(fp)) {
			if (tem1 == '\'')tem1 = '\\\'';
			myroutes[i]._EndCity += tem1;//读终点
		}
		tem1 = fgetc(fp);
		for (; tem1 != ','; tem1 = fgetc(fp)) {
			myroutes[i]._GoWay+= tem1;//读交通方式
		}
		fscanf(fp, "%f", &tem2);//读时间
		myroutes[i]._Time = tem2;
		tem1 = fgetc(fp);//缓冲逗号

		fscanf(fp, "%f", &tem2);//读费用
		myroutes[i]._Cost = tem2;
		tem1 = fgetc(fp);//缓冲逗号
		tem1 = fgetc(fp);//读取下一个
		for (; tem1 != '\n'; tem1 = fgetc(fp)) {
			if (tem1 == '\'')tem1 = '\\\'';
			myroutes[i]._Description += tem1;//读描述信息
		}
	}
	fclose(fp);
}

int Locate(string city, Cities* mycities) {
	for (int i = 0; i < NUM_CITY; i++)
	{
		if (city == mycities[i]._City)
		{//一个个检索

			return i;//找到了返回序列号
		}
	}
	return -1;//没找到返回数值-1
}

void Init_Create_Graph(Graph& mygraph, Cities* mycities, Routes* myroutes) {
	//开始初始化图
	mygraph.arcnum = NUM_CITY * NUM_CITY;
	mygraph.vexnum = NUM_CITY;
	mygraph.vertex = mycities;
	for (int i = 0; i < NUM_CITY; i++) {
		for (int j = 0; j < NUM_CITY; j++) {
			if (i == j) {//邻接矩阵对角线上的处理
				mygraph.arcs[i][j]._Cost = 0;
				mygraph.arcs[i][j]._Time = 0;
			}
			else {//没有读入数据之前全部初始化为无穷大
				mygraph.arcs[i][j]._Cost = INF;
				mygraph.arcs[i][j]._Time = INF;
			}
		}

	}
	//开始录入边的数据
	for (int k = 0; k < NUM_ROUTE; k++) {
		//先确定路线始发地与终点地对应的索引号
		int _Start = -1;
		_Start = Locate(myroutes[k]._StartCity, mycities);
		int _End = -1;
		_End = Locate(myroutes[k]._EndCity, mycities);
		//找到边对应的顶点索引号后
		if ((_Start>=0) && (_End>=0)) {
			/*if (mygraph.arcs[_Start][_End]._Time == NULL) {*/
				//录入路线中的时间与费用
				mygraph.arcs[_Start][_End]._Time = myroutes[k]._Time;
				mygraph.arcs[_Start][_End]._Cost = myroutes[k]._Cost;
				//录入路线的交通方式和描述信息
				mygraph.arcs[_Start][_End]._GoWay = myroutes[k]._GoWay;
				mygraph.arcs[_Start][_End]._Description = myroutes[k]._Description;
			
			//else {//由于交通方式不同，部分路线会有耗时不同的情况，这里我们获得用时短的
			//	//以下代码会覆盖用时长的路径
			//	if (myroutes[k]._Time < mygraph.arcs[_Start][_End]._Time) {
			//		//覆盖录入路线中的时间与费用
			//		mygraph.arcs[_Start][_End]._Time = myroutes[k]._Time;
			//		mygraph.arcs[_Start][_End]._Cost = myroutes[k]._Cost;
			//		//覆盖录入路线的交通方式和描述信息
			//		mygraph.arcs[_Start][_End]._GoWay = myroutes[k]._GoWay;
			//		mygraph.arcs[_Start][_End]._Description = myroutes[k]._Description;

			//	}
			//	//进一步思考，如果两条路线耗时一样，就考虑费用
			//	//以下代码会覆盖费用高的信息
			//	if (myroutes[k]._Cost < mygraph.arcs[_Start][_End]._Cost) {
			//		//覆盖录入路线中的费用
			//		mygraph.arcs[_Start][_End]._Cost = myroutes[k]._Cost;
			//		//覆盖录入路线的交通方式和描述信息
			//		mygraph.arcs[_Start][_End]._GoWay = myroutes[k]._GoWay;
			//		mygraph.arcs[_Start][_End]._Description = myroutes[k]._Description;
			//	}
			//	//得到了最小耗时与最小费用
			//}
		}
	}
}

void DFS(Graph &mygraph, int _Begin, bool*myvisit)
{ 
	myvisit[_Begin] = true;//递归思想，一旦传入即为访问到
	printf("%-40s %-40s %-20f %-20f\n",//打印到控制台上
		mygraph.vertex[_Begin]._Country.c_str(),
		mygraph.vertex[_Begin]._City.c_str(),
		mygraph.vertex[_Begin]._Latitude, 
		mygraph.vertex[_Begin]._Longitude);
	for (int j = 0; j < mygraph.vexnum; j++)
	{//核心代码
		if ((mygraph.arcs[_Begin][j]._Cost < INF)&&(mygraph.arcs[_Begin][j]._Cost >=0) && myvisit[j] == false)
		{//两个顶点有路且没被访问
			DFS(mygraph, j,  myvisit);
		}
	}
}

void BFS(Graph& mygraph, int _Begin, bool* myvisit,SqQueue &Q) {
	for (int v = 0; v < mygraph.vexnum; v++) myvisit[v] = FALSE;
	myvisit[_Begin] = TRUE;
	printf("%-40s %-40s %-20f %-20f\n",//打印到控制台上
		mygraph.vertex[_Begin]._Country.c_str(),
		mygraph.vertex[_Begin]._City.c_str(),
		mygraph.vertex[_Begin]._Latitude,
		mygraph.vertex[_Begin]._Longitude);
	IniQueue(Q);
	for (int v = 0; v < mygraph.vexnum; v++) {
		if ((mygraph.arcs[_Begin][v]._Cost < INF) && myvisit[v] == false) {
		myvisit[v] = TRUE;
		printf("%-40s %-40s %-20f %-20f\n",//打印到控制台上
			mygraph.vertex[v]._Country.c_str(),
			mygraph.vertex[v]._City.c_str(),
			mygraph.vertex[v]._Latitude,
			mygraph.vertex[v]._Longitude);
		EnQueue(Q, v);//同时入队
	}
	}

	int u = 0;
	while (Q.front != Q.rear) {
		   DeQueue(Q, u);
		   for (int k = 0; k < mygraph.vexnum; k++) {
			   if ((mygraph.arcs[u][k]._Cost < INF) && myvisit[k] == false) {
				   myvisit[k] = TRUE;
				   printf("%-40s %-40s %-20f %-20f\n",//打印到控制台上
					   mygraph.vertex[k]._Country.c_str(),
					   mygraph.vertex[k]._City.c_str(),
					   mygraph.vertex[k]._Latitude,
					   mygraph.vertex[k]._Longitude);
				   EnQueue(Q, k);//同时入队
			   }
		   }

	}
}


void Shortest_Path_Cost(const Graph& mygraph, int _Begin, int _End, Way way[][NUM_CITY], float dist[][NUM_CITY])
{
	float mymincost = 0;//最少花费初始化
	//此for循环是初始化
	for (int i = 0; i < mygraph.vexnum; i++)
	{
		mygraph.vertex[i]._Pass = false;//都没有选中
		dist[_Begin][i] = mygraph.arcs[_Begin][i]._Cost;//起点到所有点的费用赋值给dist
		if (dist[_Begin][i] < INF) 
		{
			//有直接路径
			way[i][_Begin]._Path = true;
			way[i][i]._Path = true;
			way[i][i]._Pre = _Begin;//前一个索引就是起点
		}
		else 
		{
			//没有直接路径
			way[i][i]._Pre = -1;
		}
	}
	mygraph.vertex[_Begin]._Pass = true;//起点被选中

	Timer timer1;//计时器
	//核心算法——迪杰斯特拉算法
	timer1.Start();
	for (int j = 0; j < mygraph.vexnum; j++)
	{
		float mincost = INF;//当前值初始化为无穷
		
		int  Cur_Min = 0;//用Cur_Min标记当前最少花费对应的索引号
		//此for循环找最小
		for (int k = 0; k < mygraph.vexnum; k++)
		{   //其它顶点的检查
			if (mygraph.vertex[k]._Pass == false) 
			{   //没有被选中的顶点
				if (dist[_Begin][k] < mincost) {
					Cur_Min = k;
					mincost = dist[_Begin][k];//最小的dist赋给当前值
				}
			}
		}

		mygraph.vertex[Cur_Min]._Pass = true;//上一个循环找到的最少花费对应的顶点被选中

		//此for循环是将最短路径得到顶点Cur_Min并入路径
		for (int w = 0; w < mygraph.vexnum; w++) {
			
			if (mygraph.vertex[w]._Pass == false && (mincost + mygraph.arcs[Cur_Min][w]._Cost < dist[_Begin][w]))
			{//没有被选中的顶点
			 //且_Begin—>Cur_Min->w的_Cost小于_Begin->w的_Cost

				dist[_Begin][w] = mincost + mygraph.arcs[Cur_Min][w]._Cost;//更新起点到顶点w的最少花费
				//更新直接路径
				for (int v = 0; v < mygraph.vexnum; v++) {
					way[w][v]._Path = way[Cur_Min][v]._Path;
					way[w][v]._Pre = way[Cur_Min][v]._Pre;
				}
				//更新二维数组way便于输出
				way[w][w]._Path = true;
				way[w][w]._Pre = Cur_Min;//前一个顶点要更新
				mymincost = dist[_Begin][_End];
			}
		}
	}
	timer1.Stop();
	printf("Elapsed time is: %4.2lf ms\n", timer1.ElapsedTime());
	if (mymincost) {
		cout << "你输入的起终点的最小花费金钱为：" << "$" << mymincost << endl;
	}
	else cout << "没有最短路径（无法到达）！" << endl;
	
}

void Shortest_Path_Time(const Graph& mygraph, int _Begin, int _End, Way way[][NUM_CITY], float dist[][NUM_CITY])
{//与寻找最少花费类似，就没有写注释
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
		cout << "你输入的起终点的最短花费时间为： " << mymintime << "小时" << endl;
	}
	else cout << "没有最短路径（无法到达）！" << endl;
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


	//反斜杠 “\”，表示换行，指长字符串写成多行时的换行
	fprintf(fp, "<!DOCTYPE html><html><head>\
<style type='text/css'>body, html{width: 100%%;height: 100%%;margin:0;font-family:'微软雅黑';}#allmap{height:100%%;width:100%%;}#r-result{width:100%%;}</style>\
<script type='text/javascript' src='http://api.map.baidu.com/api?v=2.0&ak=nSxiPohfziUaCuONe4ViUP2N'></script><title>Shortest path from %s to %s</title></head><body>\
<div id='allmap'></div></div></body></html><script type='text/javascript'>\
var map = new BMap.Map('allmap');\
var point = new BMap.Point(0, 0);\
map.centerAndZoom(point, 2);\
map.enableScrollWheelZoom(true);\n", mycities[_Begin]._City.c_str(), mycities[_End]._City.c_str());


	int j = 0;
	int bycity = _Begin, bycity2;

	while (1)
	{ //第一个点
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
		//第二个点
		fprintf(fp, "var marker%d = new BMap.Marker(new BMap.Point(%.4f, %.4f));\n\
map.addOverlay(marker%d);\n\
var infoWindow%d = new BMap.InfoWindow(\"<p style = 'font-size:14px;'>country: %s<br/>city: %s</p>\");\n\
marker%d.addEventListener(\"click\", function(){\
this.openInfoWindow(infoWindow%d);}); ", j, mycities[bycity]._Longitude, mycities[bycity]._Latitude, j, j, mycities[bycity]._Country.c_str(), mycities[bycity]._City.c_str(), j, j);
		//画弧
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
	if ((Q.rear + 1) % NUM_CITY == Q.front)exit(0);//队列满
	Q.base[Q.rear] = e;
	Q.rear = (Q.rear + 1) % NUM_CITY;
	return;
}

void  DeQueue(SqQueue& Q, int& e) {
	//若队列不空，则删除Q的队头元素，用e返回其值。
	//否则返回ERROR
	if (Q.front == Q.rear)exit(0);
	e = Q.base[Q.front];
	Q.front = (Q.front + 1) % NUM_CITY;
	return;
}