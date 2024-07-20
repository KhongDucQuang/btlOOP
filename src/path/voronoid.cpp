#include "voronoid.h"
// So we don't need to write std:: everywhere
using namespace std;

vector<vector<lineSegment>> polygonss;
vector<point> pointss;

void readInput()
{
	// Add the start point to the pointss vector
	// Open the input file
	ifstream file("data/mapfor_path.txt");
	if (!file.is_open())
	{
		cerr << "Error: Unable to open input file" << endl;
		return; // Exit the function if file opening fails
	}
	int numberOfPolygons;
	file >> numberOfPolygons;
	polygonss.resize(numberOfPolygons, vector<lineSegment>());

	polygonss.resize(numberOfPolygons, vector<lineSegment>());

	// Iterate through the polygonss
	for (int i = 0; i < numberOfPolygons; i++)
	{

		// Get the number of sides
		int numberOfSides;
		file >> numberOfSides;
		// Get the first point and remember it
		// so we can make the last linesegment after the loop
		point firstPoint;
		file >> firstPoint.x;
		file >> firstPoint.y;

		// Create a variable for the last point we saw
		point lastPoint = firstPoint;

		// Add the first point
		pointss.push_back(firstPoint);

		for (int j = 1; j < numberOfSides; j++)
		{

			// Get the next point
			point currentPoint;
			file >> currentPoint.x;
			file >> currentPoint.y;
			// Add point to list of pointss
			pointss.push_back(currentPoint);

			// create linesegment
			lineSegment l;

			// Set the linesegment
			l.p = lastPoint;
			l.q = currentPoint;

			// push it to the list of linesegments
			polygonss[i].push_back(l);

			// and update the lastPoint
			lastPoint = currentPoint;
		}

		// Construct the missing linesegment
		lineSegment l;
		l.p = lastPoint;
		l.q = firstPoint;

		// and push it to the vector
		polygonss[i].push_back(l);
	}

	// Add the end point to the pointss vector
}

// Function for calculating the distance between two points
double dist(point p, point q)
{
	// calculate euclidean distance sqrt( (p.x-q.x)^2+(p.y-q.y)^2 )
	return (double)sqrt(pow(p.x - q.x, 2.0) + pow(p.y - q.y, 2.0));
}

double rightTurn(point p1, point p2, point p3)
{
	return (p1.y - p2.y) * (p2.x - p3.x) - (p2.y - p3.y) * (p1.x - p2.x);
}

int crosses(lineSegment l1, lineSegment l2)
{
	if (l1 == l2)
		return -1;

	int returnValue = 0;
	if (l1.p == l2.p)
		returnValue++;
	if (l1.p == l2.q)
		returnValue++;
	if (l1.q == l2.p)
		returnValue++;
	if (l1.q == l2.q)
		returnValue++;

	if (returnValue > 0)
		return returnValue;

	double rt_1 = rightTurn(l1.p, l1.q, l2.p);
	double rt_2 = rightTurn(l1.p, l1.q, l2.q);
	double rt_3 = rightTurn(l2.p, l2.q, l1.p);
	double rt_4 = rightTurn(l2.p, l2.q, l1.q);

	double r1 = rt_1 * rt_2;
	double r2 = rt_3 * rt_4;

	if ((r1 == 0 && r2 <= 0) || (r2 == 0 && r1 <= 0))
	{
		returnValue = 10;
	}

	if ((r1 <= 0) && (r2 <= 0))
		returnValue = 10;
	return returnValue;
}

// Takes a line segment and returns the number of polygon edges it crosses
int numberOfCrossings(vector<vector<lineSegment>> polygons, lineSegment l)
{
	int n = 0;
	for (size_t i = 0; i < polygons.size(); i++)
	{
		int numberOfvaolation = 0;
		for (size_t j = 0; j < polygons[i].size(); j++)
		{
			int result = crosses(l, polygons[i][j]);
			if (result == -1)
			{
				return 0;
			}
			else if (result == 10)
			{
				numberOfvaolation = 10;
			}
			else
			{
				numberOfvaolation += result;
			}
		}
		if (numberOfvaolation > 2)
		{
			n++;
		}
	}
	return n;
}

// Implementation of dijkstra
// Takes a graph and a start and end point in the graph
// returns the distance
double dijkstra(vector<vector<double>> &graphDistance, vector<vector<int>> &graph, vector<int> &route)
{
	int start = 0;
	size_t end = graph.size() - 1;

	route.resize(graph.size());

	// Create a vector to see if we already visited the point
	vector<bool> visited(graph.size());

	// Create a priority queue where pq_pair consists of
	// the distance to the point and the point index in points vector
	priority_queue<tuple<double, int, int>> pq;

	// Put the start point in the queue
	// TODO: tuple instead of pair?
	pq.push(make_tuple(0, start, -1));

	// While there a still points we haven't visited we run
	while (!pq.empty())
	{

		// Get the top point
		tuple<double, int, int> t = pq.top();

		// Remove it
		pq.pop();

		// How far have we travelled until now
		double distanceSoFar = -1 * get<0>(t);

		// What point are we at
		int current = get<1>(t);

		int whereFrom = get<2>(t);

		// If we already visited the current continue
		if (visited[current])
			continue;

		route[current] = whereFrom;

		// We we have reached the distination return the distance
		if (current == end)
			return distanceSoFar;

		// Set the current to true in the visited vector
		visited[current] = true;

		// Go through every current we have an edge to and haven't visited
		for (size_t i = 0; i < graph[current].size(); i++)
		{
			int next = graph[current][i];
			if (visited[next])
				continue;

			// calculate the complete distance to that current
			double newdistance = distanceSoFar + graphDistance[current][i];

			// And push it to the queue
			tuple<double, int, int> newTuple = make_tuple(-1 * newdistance, next, current);

			pq.push(newTuple);
		}
	}
	return -1;
}

int makeVisabilityGraph(vector<vector<int>> &graph, vector<vector<double>> &graphDistance, vector<vector<int>> &crossesNumber, vector<point> points)
{

	// Get how many points we have
	size_t numberOfPoints = points.size();

	// Go through all pairs of points and calculate the distance
	for (size_t i = 0; i < graph.size(); i++)
	{
		for (size_t j = 0; j < numberOfPoints; j++)
		{

			size_t from = i;

			size_t from_point_index = from % numberOfPoints;

			size_t to_point_index = j;

			size_t to = (i / numberOfPoints) * numberOfPoints + crossesNumber[from_point_index][j] * numberOfPoints + j;

			// If it is the same point don't make an edge
			if (graph.size() > to)
			{
				// if(crossesNumber[from][to]==0){
				// Call dist function to calculate the distance
				double distance = dist(points[from_point_index], points[to_point_index]);

				graphDistance[from].push_back(distance);
				graph[from].push_back(to);
			}
		}
	}

	return 0;
}

int calculateNumberOfCrossings(vector<vector<int>> &crossesNumber, vector<vector<lineSegment>> polygons, vector<point> points)
{
	crossesNumber.resize(points.size(), vector<int>(points.size()));
	for (size_t i = 0; i < points.size(); i++)
	{
		for (size_t j = 0; j < points.size(); j++)
		{
			lineSegment l;
			l.p = points[i];
			l.q = points[j];

			// Call numberOfCrossings, which
			// suprise suprise counts the number of crossings
			crossesNumber[i][j] = numberOfCrossings(polygons, l);
		}
	}
	return 0;
}

std::vector<int>find_path(point &start, point &end,int size)
{

	// Create variables for holding start and endpoint plus the test title
	point temp = start;
	start = end;
	end = temp;
	// Create vector for containing the linesegments of the polygons
	vector<vector<lineSegment>> polygons;
	// Create vector for containing the points of the polygons
	vector<point> points;

	// Call function that parses the file input
	points.push_back(start);
	for (int i = 0; i <= pointss.size() - 1; i++)
	{
		points.push_back(pointss[i]);
	}
	for (int i = 0; i <= polygonss.size() - 1; i++)
	{
		polygons.push_back(polygonss[i]);
	}
	points.push_back(end);
	vector<vector<int>> graph;
	vector<vector<double>> graphDistance;

	// Vector so we can backtrack the route
	vector<int> route;
	vector<vector<int>> crossesNumber;

	// Get how many points we have
	size_t numberOfPoints = size* 4 + 2;

	// Create a two dimenstional vector for the graph

	size_t dimension = numberOfPoints;
	graph.resize(dimension, vector<int>());
	graphDistance.resize(dimension, vector<double>());

	// Call function that calculate the distance
	calculateNumberOfCrossings(crossesNumber, polygons, points);
	makeVisabilityGraph(graph, graphDistance, crossesNumber, points);

	// The graph is constructed call dijksta to calculate the distance
	double distance = dijkstra(graphDistance, graph, route);

	// Output the distance

	std::vector<int> path;

	int i = route.size() - 1;
	while (route[i] != -1)
	{	
		if(i!=route.size()-1){
		path.push_back(points[i].x);
		path.push_back(points[i].y);}
		i = route[i];
	}


	return path;
}
