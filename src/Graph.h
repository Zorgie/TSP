#pragma once

#include <map>

using namespace std;

class Graph
{
public:
	Graph(size_t _size);
	~Graph();
private:
	// Fields
	size_t size;
	map<int, map<int, double>> *edges;
};