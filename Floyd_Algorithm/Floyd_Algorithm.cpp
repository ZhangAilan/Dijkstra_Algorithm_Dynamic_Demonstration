/*-----------------------------------------------------------------
*Author: ZhangYuehao
**Email: yuehaozhang@njtech.edu.cn
**Zhihu: https://www.zhihu.com/people/bu-meng-cheng-kong-46/posts
*GitHub: https://github.com/ZhangAilan
*------------------------------------------------------------------
*Time:
*  2024/1/5
*Funcion:
*	Floyd Algorithm
*-------------------------------------------------------------------
//                .-~~~~~~~~~-._       _.-~~~~~~~~~-.
//            __.'              ~.   .~              `.__
//          .'//         Nanjing Tech University        \\`.
//        .'//                     |                     \\`.
//      .'// .-~"""""""~~~~-._     |     _,-~~~~"""""""~-. \\`.
//    .'//.-"                 `-.  |  .-'                 "-.\\`.
//  .'//______.============-..   \ | /   ..-============.______\\`.
//.'______________________________\|/______________________________`.
*/
#include <iostream>
#include <vector>
#include "DSGraphic.h"
#define INF 0x3f3f3f3f
using namespace std;

float startX_Distance = 100;
float startY_Distance = 50;
float startX_Next = 450;
float startY_Next = 50;
void printSolution(vector<vector<int>> dist, vector<vector<int>> next, int V);

void Folyd(vector<vector<int>> graph, int V)
{
	vector<vector<int>> dist(V, vector<int>(V));  //Two-dimensional array to store the shortest distance between two points
	vector<vector<int>> next(V, vector<int>(V, 0));  //Two-dimensional array to store the next point of the shortest path

	vector<vector<void*>>p_Distance(V, vector<void*>(V, 0));  //save the location of the data needed to be modified
	vector<vector<void*>>p_Next(V, vector<void*>(V, 0));  //save the location of the data needed to be modified
	for (int i = 0; i < V; i++)
	{
		for (int j = 0; j < V; j++)
		{
			dist[i][j] = graph[i][j];  //Initialize the distance between two points
			if (dist[i][j] != INF)
			{
				p_Distance[i][j]=drawText(dist[i][j], startX_Distance + 20+ i * 50, startY_Distance + 20+ j * 50, 24, Gdiplus::Color::Green);
			}
			else
			{
				p_Distance[i][j]=drawText("INF", startX_Distance  + i * 50, startY_Distance + 20 + j * 50, 24, Gdiplus::Color::Green);
			}
			if (graph[i][j] != INF && i != j)
			{
				next[i][j] = j;  //Initialize the next point of the shortest path
				p_Next[i][j] = drawText(j, startX_Next + 20 + i * 50, startY_Next + 20 + j * 50, 24, Gdiplus::Color::Green);
			}
		}
	}

	for (int k = 0; k < V; k++)
	{
		for (int i = 0; i < V; i++)
		{
			for (int j = 0; j < V; j++)
			{
				if (dist[i][k] + dist[k][j] < dist[i][j])  //If the distance between two points is greater than the distance between two points through the intermediate point, update the distance between two points
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					clearObject(p_Distance[i][j]);
					p_Distance[i][j] = drawText(dist[i][j], startX_Distance + 10 + i * 50, startY_Distance + 20 + j * 50, 24, Gdiplus::Color::Red);
					
					next[i][j] = next[i][k];  //Update the next point of the shortest path
					clearObject(p_Next[i][j]);
					p_Next[i][j] = drawText(next[i][j], startX_Next + 20 + i * 50, startY_Next + 20 + j * 50, 24, Gdiplus::Color::Red);
				}
			}
		}
	}
	printSolution(dist, next, V);
}

void drawTable(int rows, int columns, float startX, float startY, float cellWidth, float cellHeight, Gdiplus::Color lineColor)
{
	float endX = startX + cellWidth * columns; float endY = startY + cellHeight * rows;
	// Draw horizontal lines
	for (int i = 0; i <= rows; ++i)
	{
		float y = startY + i * cellHeight;
		drawLine(startX, y, endX, y, 1, lineColor);
	}
	// Draw vertical lines
	for (int i = 0; i <= columns; ++i)
	{
		float x = startX + i * cellWidth;
		drawLine(x, startY, x, endY, 1, lineColor);
	}
}

void printSolution(vector<vector<int>> dist, vector<vector<int>> next, int V)
{
	cout<<"The following lines shows the actual shortest path between every pair of vertices \n";
	for (int i = 0; i < V; i++)
	{
		for (int j = i; j < V; j++)
		{
			if (i != j)
			{
				cout<<"The shortest path from "<<i<<" to "<<j<<" is ("<<i<<" ";
				int k = next[i][j];
				while (k != j)
				{
					cout << "->" << k;
					k = next[k][j];
				}
				cout << "->" << j << ")" << endl;
			}
		}
	}
}


void main()
{
	showWindow();
	setPlayingSpeed(200);

	//draw the graph
	//connect 0 to 1
	drawLine(400, 350, 200, 450, 1, Gdiplus::Color::Black);
	//connect 0 to 2
	drawLine(400, 350, 500, 450, 1, Gdiplus::Color::Black);
	//connect 1 to 2
	drawLine(200, 450, 500, 450, 1, Gdiplus::Color::Black);
	//connect 1 to 3
	drawLine(200, 450, 400, 550, 1, Gdiplus::Color::Black);
	//connect 2 to 3
	drawLine(500, 450, 400, 550, 1, Gdiplus::Color::Black);
	//connect 3 to 4
	drawLine(400, 550, 600, 550, 1, Gdiplus::Color::Black);
	drawPoint(400, 350, 10, Gdiplus::Color::Red);  //0
	drawPoint(200, 450, 10, Gdiplus::Color::Red);  //1
	drawPoint(500, 450, 10, Gdiplus::Color::Red);  //2
	drawPoint(400, 550, 10, Gdiplus::Color::Red);  //3
	drawPoint(600, 550, 10, Gdiplus::Color::Red);  //4
	//add the number
	drawText(0, 400, 350, 24, Gdiplus::Color::Black);
	drawText(1, 200, 450, 24, Gdiplus::Color::Black);
	drawText(2, 500, 450, 24, Gdiplus::Color::Black);
	drawText(3, 400, 550, 24, Gdiplus::Color::Black);
	drawText(4, 600, 550, 24, Gdiplus::Color::Black);
	//add the weight
	drawText(2, 300, 400, 24, Gdiplus::Color::Green);
	drawText(1, 450, 400, 24, Gdiplus::Color::Green);
	drawText(5, 450, 500, 24, Gdiplus::Color::Green);
	drawText(5, 350, 450, 24, Gdiplus::Color::Green);
	drawText(4, 350, 500, 24, Gdiplus::Color::Green);
	drawText(6, 500, 550, 24, Gdiplus::Color::Green);



	drawTable(5, 5, startX_Distance, startY_Distance, 50, 50, Gdiplus::Color::Black);
	drawTable(5, 5, startX_Next, startY_Next, 50, 50, Gdiplus::Color::Black);
	drawText("Distance Matrix", startX_Distance+20, startY_Distance+260, 24, Gdiplus::Color::Black);
	drawText("Next Matrix", startX_Next+20, startY_Distance+260, 24, Gdiplus::Color::Black);
	for (int i = 0; i < 5; i++)
	{
		drawText(i, startX_Distance-30, startY_Distance+20 + i * 50, 24, Gdiplus::Color::Black);
		drawText(i, startX_Distance+20+i*50, startY_Distance-30 , 24, Gdiplus::Color::Black);
		drawText(i, startX_Next-30, startY_Next+20 + i * 50, 24, Gdiplus::Color::Black);
		drawText(i, startX_Next + 20 + i * 50, startY_Next-30, 24, Gdiplus::Color::Black);
	}

	vector<vector<int>> graph = { 
		{0,2,1,INF,INF},
		{2,0,5,4,INF},
		{1,5,0,5,INF},
		{INF,4,5,0,6},
		{INF,INF,INF,6,0}
	};
	Folyd(graph, 5);

	system("pause");
}