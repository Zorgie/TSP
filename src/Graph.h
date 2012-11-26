#pragma once

#include <map>

using namespace std;

class Graph
{
public:
	// Constructors and destructors
	Graph(size_t _size);
	~Graph();
	// Methods
	void addEdge(int from, int to, double weight);
	map<int, double> getNeighbours(int from);
	double getEdge(int from, int to);
private:
	// Fields
	size_t size;
	map<int, map<int, double>> *edges;
};
