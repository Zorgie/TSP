#include "Graph.h"


Graph::Graph(size_t _size)
{
	size = _size;
	edges = new map<int, map<int, double>>();
}

Graph::~Graph()
{
}