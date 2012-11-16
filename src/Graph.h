#pragma once

#include <iostream>

class Graph
{
public:
	Graph(std::istream&);
	~Graph(void);
	void solveTSP();
	void printTSP();
private:
	// Methods
	double dist(const int&, const int&) const;
	// Fields
	size_t size;
	double** coord;
	int* route;
	
};

