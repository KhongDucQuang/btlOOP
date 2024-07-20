#ifndef VORONOID_H
#define VORONOID_H

#include <vector>
#include <string>
#include <iostream> //cin cout
#include <vector>	//vector
#include <cstdio>	//scanf
#include <sstream>	//ostringstream
#include <cmath>	//pow sqrt
#include <queue>	//priority_queue
#include <stdlib.h> //atoi
#include <tuple>	//get<n> make_tuple
#include <chrono>
#include <string.h>
#include <fstream>
// point type for holding a coordinate
using  namespace std;
struct point
{
	double x;
	double y;
	point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	point()
	{
	}
	bool operator==(const point &other) const
	{
		return (this->x == other.x && this->y == other.y);
	}
};
// Type used in the priority queue in the dijkstra function
typedef std::pair<double, std::pair<int, int>> pq_pair;

// linesegment type holding two points
struct lineSegment
{
	point p;
	point q;
	lineSegment(point p, point q)
	{
		this->p = p;
		this->q = q;
	};
	lineSegment()
	{
	}
	bool operator==(const lineSegment &other) const
	{
		return (this->p == other.p && this->q == other.q) ||
			   (this->p == other.q && this->q == other.p);
	}
};

// Function prototypes
std::vector<int> find_path(point& start, point& end,int size);
extern vector<vector<lineSegment>> polygonss;
extern vector<point> pointss;
void readInput();
double dist(point p, point q);
double rightTurn(point p1, point p2, point p3);
int crosses(lineSegment l1, lineSegment l2);
int numberOfCrossings(std::vector<std::vector<lineSegment>> polygons, lineSegment l);
double dijkstra(std::vector<std::vector<double>> &graphDistance, std::vector<std::vector<int>> &graph, std::vector<int> &route);
int makeVisabilityGraph(std::vector<std::vector<int>> &graph, std::vector<std::vector<double>> &graphDistance, std::vector<std::vector<int>> &crossesNumber, std::vector<point> points);
int calculateNumberOfCrossings(std::vector<std::vector<int>> &crossesNumber, std::vector<std::vector<lineSegment>> polygons, std::vector<point> points);
#endif // VORONOID_H
