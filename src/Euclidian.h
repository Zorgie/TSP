#pragma once

#include <map>
#include <list>
#include <stdlib.h>
#include <iostream>
#include <cmath>

using namespace std;

class Euclidian
{
	struct Point{
		double x, y;
	};
public:
	// Constructors, Destructors
	Euclidian(std::istream& in);
	~Euclidian(void);

	// Methods
	void solve();
	void print();
	int* getSolution();
private:
	// Methods
	void initialize();
	bool opt2Cycle();
	bool opt2Step(int origin);
	bool isOpt2Improvement(int* fourTuple);
	void reverse(int from, int to);
	double dist(int first, int second);
	void greedy();
	// Fields
	Point* points;
	map<int, int> solution;
	map<int, int> backSolution;
	int** pointsByDistance;
	size_t size;
	int K;
	static const int K_MAX = 15;
	static const int TWO_OPT_COUNT = 5;
};

