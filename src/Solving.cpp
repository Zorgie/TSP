#include "Solving.h"
#include <math.h>

Solving::Solving(std::istream& in)
{
	// Reads the size
	int temp;
	in >> temp;
	size = temp;
	coord = new double*[size];
	for(int i=0; i<size; i++)
	{
		coord[i] = new double[2];
		in >> coord[i][0];
		in >> coord[i][1];
	}
}


Solving::~Solving(void)
{
}


void Solving::solveTSP()
{
	route = new int[size];
	bool* used = new bool[size];
	for(int i=0; i<size; i++)
	{
		route[i] = 0;
		used[i] = false;
	}

	route[0] = 0;
	used[0] = true;
	for(int i=1; i<size; i++)
	{
		int best = -1;
		for(int j=0; j<size; j++)
		{
			if(!used[j] && (best == -1 || dist(route[i-1], j) < dist(route[i-1], best)))
				best = j;
		}
		route[i] = best;
		used[best] = true;
	}
}

void Solving::printTSP()
{
	for(int i=0; i<size; i++)
	{
		printf("%d\n",route[i]);		
	}
}

double Solving::dist(const int& _first, const int& _second) const
{
	// Euclidian distance
	double x1, y1, x2, y2;
	x1 = coord[_first][0];
	y1 = coord[_first][1];
	x2 = coord[_second][0];
	y2 = coord[_second][1];
	return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}