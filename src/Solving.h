#pragma once

#include <iostream>

class Solving
{
public:
	Solving(std::istream&);
	~Solving(void);
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

