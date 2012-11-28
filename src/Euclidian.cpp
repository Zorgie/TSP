#include "Euclidian.h"

Euclidian::Euclidian(std::istream& in)
{
	int temp;
	in >> temp;
	size = temp;
	points = new Point[size];
	for(int i=0; i<size; i++)
	{
		in >> points[i].x;
		in >> points[i].y;
	}
	initialize();
}


Euclidian::~Euclidian(void)
{
	//delete[] points;
	//for(int i=0; i<size; i++)
	//	delete[] pointsByDistance[i];
	//delete[] pointsByDistance;
}

void Euclidian::solve()
{
	greedy();
	for(int i=0; i<TWO_OPT_COUNT; i++)
	{
		if(!opt2Cycle())
			break;
	}
}

void Euclidian::print()
{
	int cur = 0;
	int count = 0;
	do{
		printf("%d\n", cur);
		cur = solution[cur];
	}while(cur != 0);
}

int* Euclidian::getSolution()
{
	int cur = 0;
	int count = 0;
	int* sol = new int[size];
	do{
		sol[count++] = cur;
		cur = solution[cur];
	}while(cur != 0);
	return sol;
}

void Euclidian::initialize()
{
	K = min((int)size-1, K_MAX);
	pointsByDistance = new int*[size];
	for(int i=0; i<size; i++)
	{
		pointsByDistance[i] = new int[K];
		bool* used = new bool[size];
		for(int ii = 0; ii<size;ii++){used[ii]=false;}
		for(int j = 0; j<K; j++)
		{
			double minDist = 100000000; // A large number.
			int closest = -1;
			for(int ii = 0; ii < size; ii ++)
			{
				if(used[ii] || ii == i)
					continue;
				double distance = dist(i, ii);
				if(distance < minDist)
				{
					minDist = distance;
					closest = ii;
				}
			}
			used[closest] = true;
			pointsByDistance[i][j] = closest;
		}
		delete[] used;
	}
}

bool Euclidian::opt2Cycle()
{
	bool swapped = false;
	for(int i=0; i<size; i++)
	{
		if(opt2Step(i))
		{
			swapped = true;
		}
	}
	return swapped;
}

bool Euclidian::opt2Step(int origin)
{
	int* tuple = new int[4];
	tuple[0] = origin;
	tuple[1] = solution[origin];
	for(int i=0; i<K; i++)
	{
		tuple[2] = pointsByDistance[tuple[1]][i];
		tuple[3] = backSolution[tuple[2]];
		if(tuple[1] != tuple[3] && isOpt2Improvement(tuple))
		{
			reverse(tuple[1], tuple[3]);
			solution[tuple[0]]=tuple[3];
			solution[tuple[1]]=tuple[2];
			backSolution[tuple[3]] = tuple[0];
			backSolution[tuple[2]] = tuple[1];
			return true;
		}
	}
	delete[] tuple;
	return false;
}

bool Euclidian::isOpt2Improvement(int* fourTuple)
{
	double dist1 = dist(fourTuple[0], fourTuple[1]);
	double dist2 = dist(fourTuple[2], fourTuple[3]);
	double dist3 = dist(fourTuple[0], fourTuple[3]);
	double dist4 = dist(fourTuple[1], fourTuple[2]);
	return (dist1 + dist2 > dist3 + dist4);
}

void Euclidian::reverse(int from, int to)
{
	if(from == to)
		return;
	int next = solution[from];
	do{
		int tempNext = solution[next];
		solution[next] = from;
		backSolution[from] = next;
		from = next;
		next = tempNext;
	}while(from != to);
}

double Euclidian::dist(int first, int second)
{
	Point p1 = points[first];
	Point p2 = points[second];
	double d = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	return d;
}

void Euclidian::greedy()
{
	int* tour = new int[size];
	bool* used = new bool[size];
	for(int i=0; i<size; i++)
	{
		tour[i] = 0;
		used[i] = false;
	}

	tour[0] = 0;
	used[0] = true;
	for(int i=1; i<size; i++)
	{
		int best = -1;
		for(int j=0; j<size; j++)
		{
			if(!used[j] && (best == -1 || dist(tour[i-1], j) < dist(tour[i-1], best)))
				best = j;
		}
		tour[i] = best;
		used[best] = true;
	}
	for(int i=0; i<size; i++)
	{
		int cur = tour[i];
		int next = tour[(i+1)%size];
		solution[cur]=next;
		backSolution[next] = cur;
	}
}