/*-----------------------------------------------------------------
*Author: ZhangYuehao
**Email: yuehaozhang@njtech.edu.cn
**Zhihu: https://www.zhihu.com/people/bu-meng-cheng-kong-46/posts
*GitHub: https://github.com/ZhangAilan
*------------------------------------------------------------------
*Time:
*  2023/12/31
*Funcion:
*	Dijkstra algorithm access the shortest path.
*   The result is displayed in the SFML window.
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
#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <string>
#include <cmath>
#include <tuple>
#include <iomanip>
#include <unordered_map>
#include <SFML/Graphics.hpp>	//use the SFML library
using namespace std;
using namespace sf;

struct Edge
{
	string edgeName;
	string startNode;
	string endNode;
	double length;
};

struct Node
{
	string nodeName;
	double x;
	double y;
};


//read nodes from file
vector<Node> readNodes(string filepath)
{
	ifstream file(filepath);
	string nodeName;
	double x, y;
	vector<Node> nodes;
	while (file >> nodeName >> x >> y)
	{
		nodes.push_back({nodeName, x, y });
	}
	return nodes;
}


//read edges from file
vector<Edge> readEdges(string filepath)
{
	ifstream file(filepath);
	string edgeName, startNode, endNode;
	double length;
	vector<Edge> edges;
	while (file >> edgeName >> startNode >> endNode >> length)
	{
		edges.push_back({ edgeName, startNode, endNode, length });
	}
	return edges;
}

//define the graph
typedef unordered_map<string,vector<pair<string,double>>> Graph;

//dijkstra algorithm
tuple<vector<string>,vector<tuple<string,string,string>>> dijkstra(const Graph& graph, const string& start, const string& end)
{
	priority_queue<pair<double,string>, vector<pair<double,string>>, greater<pair<double,string>>> pq;  
	unordered_map<string, double> distances;	//distance from start to each node
	unordered_map<string, string> previous;	    //previous node in optimal path from start

	vector<tuple<string, string,string>> pathUpdates;   //save the progress of the algorithm

	//initialize the distances
	for (const auto& entry : graph)
	{
		distances[entry.first]=numeric_limits<double>::infinity();
		previous[entry.first] = "";
	}
	distances[start] = 0;
	pq.push({ 0, start });  //push the start node into the priority queue

	//dijkstra algorithm
	while (!pq.empty())
	{
		string current = pq.top().second;  //the node with the shortest distance
		pq.pop();  //remove the node from the priority queue

		for (const auto& neighbor : graph.at(current))  
		{
			string next= neighbor.first;  //the neighbor of current node
			double weight= neighbor.second;  //the distance between current node and its neighbor

			double new_distance = distances[current] + weight;  //the distance from start to next node

			if (new_distance < distances[next])
			{
				distances[next] = new_distance;
				previous[next] = current;  
				pq.push({ new_distance, next });  

				pathUpdates.push_back({ current, next ,"True" });  //save the progress of the algorithm
			}
			else
			{
				pathUpdates.push_back({current, next ,"False"});
			}
		}
	}

	//reconstruct the optimal path
	vector<string> path;
	string current = end;
	while (!current.empty())
	{
		path.insert(path.begin(), current);
		current = previous[current];
	}

	return make_tuple(path,pathUpdates);
}


int main()
{
	//read nodes and edges from file
	vector<Node> nodes = readNodes("nodes.txt");
	vector<Edge> edges = readEdges("edges.txt");

	//create the coordinates mapping
	map<string,Vector2f> nodeCoordinates;
	for (const Node& node : nodes)
	{
		nodeCoordinates[node.nodeName]=Vector2f(node.x, node.y);
	}

	//create the graph
	Graph graph; 
	for (const auto& edge : edges)
	{
		graph[edge.startNode].emplace_back(edge.endNode, edge.length);
		graph[edge.endNode].emplace_back(edge.startNode, edge.length);  
	}

	//find the shortest path
	string startNode="N1";
	string endNode="N5";
	auto result = dijkstra(graph, startNode, endNode);
	vector<string> path = get<0>(result);
	vector<tuple<string, string,string>> pathUpdates = get<1>(result); 
	
	//print the progress of the algorithm
	cout << "The progress of the algorithm is: " << endl;
	int count = 0;
	for (const auto& update : pathUpdates)
	{
		cout << std::setw(3) << std::left << std::get<0>(update) << " -> "
			<< std::setw(5) << std::left << std::get<1>(update) << "  "
			<< std::setw(5) << std::left << std::get<2>(update);
		count++;
		if (count % 4 == 0)
		{
			cout << endl;
		}
		else
		{
			cout << "        ";
		}
	}
	cout << endl;


	//print the shortest path
	cout<< "The shortest path from " << startNode << " to " << endNode << " is: " << endl;
	for (const auto& node : path)
	{
		cout << node << " ";
	}
	cout << endl;

	//add the nodes to the window
	vector<CircleShape> shapes;  //nodes
	for (const auto& node : nodes)  
	{
		CircleShape shape(3);
		shape.setFillColor(Color::Red);
		shape.setPosition(node.x, node.y);
		shapes.push_back(shape);
	}


	//create SFML window
	RenderWindow window(VideoMode(1500, 900), "Dijkstra Algorithm Dynamic Demonstration");
	//create the font
	Font font;
	if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
	{
		cout<<"Error loading font"<<endl;
	}

	//create the button for display the path
	RectangleShape button_path(Vector2f(215, 50));
	button_path.setPosition(60, 830);
	button_path.setFillColor(Color(255, 255, 255, 300));	//the last parameter is the transparency of the color
	//create the text for the button
	Text buttonText("Display All The Paths", font, 20);
	buttonText.setFillColor(Color::Black);
	buttonText.setPosition(70, 840);
	//create the bool variable for the visibility of the path
	bool isVisiable_path = false; 

	//create the button for display the shortest path
	RectangleShape button_shortest(Vector2f(215, 50));
	button_shortest.setPosition(300, 830);
	button_shortest.setFillColor(Color(255, 255, 255, 300));
	//create the text for the button
	Text buttonText_shortest("Display The Shortest Path", font, 20);
	buttonText_shortest.setFillColor(Color::Black);
	buttonText_shortest.setPosition(310, 840);
	//create the bool variable for the visibility of the path
	bool isVisiable_shortest = false;

	//create the button for display the progress of the algorithm
	RectangleShape button_progress(Vector2f(215, 50));
	button_progress.setPosition(620, 830);
	button_progress.setFillColor(Color(255, 255, 255, 300));	
	Text buttonText_progress("Display The Progress", font, 20);
	buttonText_progress.setFillColor(Color::Black);
	buttonText_progress.setPosition(630, 840);
	bool isVisiable_progress = false;

	//main loop
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}

		//white background
		window.clear(Color(255, 255, 255));

		//draw the button
		window.draw(button_path);
		window.draw(buttonText);
		window.draw(button_shortest);
		window.draw(buttonText_shortest);
		window.draw(button_progress);
		window.draw(buttonText_progress);

		//draw the nodes
		for (const auto& shape : shapes)
		{
			window.draw(shape);
		}

		//draw the font
		for (const auto& node : nodes)
		{
			Text text;
			text.setFont(font);
			text.setString(node.nodeName);
			text.setCharacterSize(12);
			text.setFillColor(Color::Black);
			text.setPosition(node.x ,node.y );
			window.draw(text);
		}


		//draw the text of the shortest path
		Text text_shortest;
		text_shortest.setFont(font);
		text_shortest.setCharacterSize(20);
		text_shortest.setFillColor(Color::Black);
		string  pathStr;
		pathStr += "The shortest path is: \n";
		for (size_t i = 0; i < path.size(); ++i)
		{
			pathStr += path[i];
			if (i != path.size() - 1)
			{
				pathStr += " -> ";
			}
		}
		text_shortest.setString(pathStr);
		text_shortest.setPosition(1000, 820);
		window.draw(text_shortest);


		//mouse click event (draw all the paths)
		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) 
		{
			//check if the mouse click is within the button's area
			if (button_path.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window))))
			{
				isVisiable_path = !isVisiable_path;  
			}
		}
		if (isVisiable_path)
		{
			//Toggle the visibility of the path
			for (const Edge& edge : edges)
			{
				VertexArray line(Lines, 2);
				line[0].position = nodeCoordinates[edge.startNode]; //set the start point of the line
				line[0].color = Color::Black;
				line[1].position = nodeCoordinates[edge.endNode];
				line[1].color = Color::Black;
				window.draw(line);
			}
		}


		//mouse click event (draw the progress of the algorithm)
		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
		{
			if (button_progress.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window))))
			{
				isVisiable_progress = !isVisiable_progress;
			}
		}
		vector<VertexArray> lines_progress_disp;
		if (isVisiable_progress)
		{
			VertexArray line(Lines, 2);
			for (const auto& update : pathUpdates)
			{
				if (get<2>(update) == "True")
				{
					line[0].position = nodeCoordinates[get<0>(update)];
					line[0].color = Color::Blue;
					line[1].position = nodeCoordinates[get<1>(update)];
					line[1].color = Color::Blue;
					lines_progress_disp.push_back(line);
				}
			}
		}
		for (const auto& line : lines_progress_disp)
		{
			window.draw(line);
		}


		//mouse click event (draw the shortest path)
		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
		{
			if (button_shortest.getGlobalBounds().contains(window.mapPixelToCoords(Mouse::getPosition(window))))
			{
				isVisiable_shortest = !isVisiable_shortest;
			}
		}
		vector<VertexArray> lines;
		if (isVisiable_shortest)
		{
			for (int i = 0; i < path.size() - 1; i++)
			{
				VertexArray line(Lines, 4);
				Vector2f offset(0.5f, 0.5f);  //offset to make the line more clear

				line[0].position = nodeCoordinates[path[i]] - offset;
				line[0].color = Color::Red;
				line[1].position = nodeCoordinates[path[i + 1]] - offset;
				line[1].color = Color::Red;
				line[2].position = nodeCoordinates[path[i]] + offset;
				line[2].color = Color::Red;
				line[3].position = nodeCoordinates[path[i + 1]] + offset;
				line[3].color = Color::Red;
				lines.push_back(line);
			}
		}
		for (const auto& line : lines)
		{
			window.draw(line);
		}


		//display the content from the back buffer
		window.display();
		sleep(milliseconds(100));
	}
	return 0;
}