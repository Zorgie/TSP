#include "Graph.h"


Graph::Graph(size_t _size)
{
	size = _size;
	edges = new map<int, map<int, double>>();
}

Graph::~Graph()
{
}

void Graph::addEdge(int from, int to, double weight)
{
	if(from < 0 || from >= size || to < 0 || to >= size)
	{
		throw std::out_of_range("Edge nodes out of range");
	}
	edges[from][to] = weight;	
}

map<int, double> Graph::getNeighbours(int from)
{
	return &(edges[from]);
}

double Graph::getEdge(int from, int to)
{
	return edges[from][to];
}
