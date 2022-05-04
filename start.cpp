#include "graph.h"
Cities* CITY=NULL;//城市变量
Routes* ROUTE=NULL;//路径变量
Graph GRAPH;//存储图
Way way[NUM_CITY][NUM_CITY];//存储最短路径前后索引号
float dist[NUM_CITY][NUM_CITY];//存储最短路径的长度（时间/花费）
bool Visited[NUM_CITY];//实现的辅助visit
int main() {
	

	char File1[] = "cities.csv";//城市文件名
	char File2[] = "routes.csv";//路线文件名

	Read_City_CSV(File1, CITY);//读入城市
	cout << "所有城市坐标已经读入，进入下一步......" << endl << endl;

	Read_Route_CSV(File2, ROUTE);//读入路线
	cout << "所有路径信息已经读入，进入下一步......" << endl << endl;

	cout << "执行到此处，已完成实习的第一个任务" << endl << endl;

	Init_Create_Graph(GRAPH, CITY, ROUTE);
	cout << "图的构建（邻接矩阵）已完成，进入下一步......" << endl << endl;

	cout << "执行到此处，已完成实习的第二个任务" << endl << endl;
	
	int method = 0;
	int BeginNum = 0;
	int BeginNum2 = 0;
    start0:
	cout << "执行图的遍历，请先输入你想要遍历的方式：1.深度优先 2.广度优先。请选择：";
	cin >> method;//选择你的遍历方式
	system("cls");
	switch (method)
	{case 1:
		cout << "请输入开始遍历的索引号：";
		cin >> BeginNum;
		
		while (BeginNum > NUM_CITY - 1) {
			cout << "输出错误重新输入" << endl;
			BeginNum = 0;
			cin >> BeginNum;
			if (BeginNum < NUM_CITY - 1 && BeginNum >= 0)break;
		}
		system("cls");
		cout << "深度优先遍历中，请稍候......" << endl << endl;
		printf("%-40s %-40s %-20s %-20s\n", "国家", "首都", "纬度", "经度");
		DFS(GRAPH, BeginNum, Visited);//深度优先遍历实现
		cout << "深度优先遍历已完成" << endl << endl;
		break;
	case 2:
		cout << "请输入开始遍历的索引号：";
		cin >> BeginNum2;
		while (BeginNum2 > NUM_CITY - 1) {
			cout << "输出错误重新输入:" << endl;
			BeginNum2 = 0;
			cin >> BeginNum2;
			if (BeginNum2 < NUM_CITY - 1 && BeginNum2 >= 0)break;
		}
		system("cls");
		SqQueue myqueue;
		cout << "广度优先遍历中，请稍候......" << endl << endl;
		printf("%-40s %-40s %-20s %-20s\n", "国家", "首都", "纬度", "经度");
		BFS(GRAPH, BeginNum2, Visited, myqueue);//广度度优先遍历实现
		cout << "广度优先遍历已完成" << endl << endl;
		break;

	default:cout << "错误，请重新输入："; goto start0; break;
		break;
	}
	
	cout << "执行到此处，已完成实习的第三个任务" << endl << endl;
	cout << "下面开始执行最短路径算法,在此之前请考虑起终点城市名，扣1表示已经准备好：";
	int ready = 0;
	cin >> ready;
	system("cls");
	cout << "执行最短路径算法，请输入寻找的路径的起终点（换行隔开,名字有'的一律改为\\，间接后续可视化错误）" << endl;
	string Mybegincity;
	string Myendcity;
	int Mybegin_Num;
	int Myend_Num;
	cin.ignore();
	start1:
	cout << "起点：";
	getline(cin, Mybegincity);
	Mybegin_Num = Locate(Mybegincity, CITY);
	if (Mybegin_Num <0) { cout << "输入错误，重新输入"; goto start1; }
    start2:
	cout << "终点：";
	getline(cin, Myendcity);
	Myend_Num = Locate(Myendcity, CITY);
	if (Myend_Num<0) { cout << "输入错误，重新输入"; goto start2; }
	system("cls");
    start3:
	int choice = 0;//选择你的考虑方面
	cout << "你所要考虑的方面：1.花费时间 2.花费费用。请选择：";
	cin >> choice;
	switch (choice)
	{
	case 1:
		system("cls");
		Shortest_Path_Time(GRAPH, Mybegin_Num, Myend_Num, way, dist); 
		break;
	case 2:
		system("cls");
		Shortest_Path_Cost(GRAPH, Mybegin_Num, Myend_Num, way, dist);
		break;
	default:cout << "错误，请重新输入："; goto start3; break;
	}

	char File3[] = "mymingraph.htm";
	Output_Into_Htm(File3, GRAPH, Mybegin_Num, Myend_Num, CITY, way);
	cout << "已经创建好了可视化地图" << endl << endl;
	cout << "执行到此处，已完成实习的第四、五个任务" << endl << endl;
	delete[]CITY;
	delete[]ROUTE;
	cout << "实习的第六个任务绘制时间复杂度图形完毕" << endl << endl;
	cout << "运行到此处没有报错，说明程序正确,请您查看mymingraph.htm检查结果";
	return 0;

}

