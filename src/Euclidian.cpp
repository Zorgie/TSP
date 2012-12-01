#include "Euclidian.h"


Euclidian::Euclidian(std::istream& in)
{
	// Read input
	int temp;
	in >> temp;
	size = temp;
	// Reads the point data.
	points = new Point[size];
	for(int i=0; i<size; i++)
	{
		in >> points[i].x;
		in >> points[i].y;
	}

	// Calculates the K closest cities for each blah blah.
	initDistances();
}

void Euclidian::initDistances()
{
	K = min(K_MAX, (int)size-1);
	distanceMatrix = new double*[size];
}


Euclidian::~Euclidian(void)
{
}


void Euclidian::solve()
{
	greedy();
}

void Euclidian::greedy()
{
	solution = new int[size];
	bool* used = new bool[size];
	for(int i=0; i<size; i++)
	{
		solution[i] = 0;
		used[i] = false;
	}

	solution[0] = 0;
	used[0] = true;
	for(int i=1; i<size; i++)
	{
		int best = -1;
		for(int j=0; j<size; j++)
		{
			if(!used[j] && (best == -1 || dist(solution[i-1], j) < dist(solution[i-1], best)))
				best = j;
		}
		solution[i] = best;
		used[best] = true;
	}
}

void Euclidian::ruinEverything(int count)
{
	if(size < 4)
		return;
	for(int i=0; i<count; i++)
	{
		int from = rand() % size;
		int to = from;
		while(to == from)
		{
			to = rand() % size;
		}
		reverse(solution, from, to);
	}
}

void Euclidian::opt2Cycle(clock_t start, int timeout)
{
	if(size < 4)
		return;
	while(true){
		for(int i=0; i<size-3; i++)
		{
			time_t now = clock();
			int time = (((double)now-start)/CLOCKS_PER_SEC*1000);
			if(time > timeout)
				return;
			for(int j=2; j< (size-1); j++)
			{
				int to = (i + j)%size;
				if(swapGood(i, to))
				{
					reverse(solution, i+1, to);
					if(i > 0 && time < timeout * 0.65)
					{
						i--;
					}
				}
			}
		}
	}
}
void Euclidian::opt25Cycle(clock_t start, int timeout)
{
	if(size < 4)
		return ;
	for(int i=0; i<size; i++)
	{
		time_t now = clock();
		int time = (((double)now-start)/CLOCKS_PER_SEC*1000);
		if(time > timeout)
			return ;
		// i is the node that will be moved.
		for(int j=1; j<size-1; j++)
		{
			if(insertGood(i, j))
			{
				insert(solution, i, j);
			}
		}
	}
}

void Euclidian::opt3Cycle(clock_t start, int timeout)
{
	if(size < 6)
		return;
	int* tuple = new int[6];
	for(int i=0; i<size-4; i++)
	{
		time_t now = clock();
		int time = (((double)now-start)/CLOCKS_PER_SEC*1000);
		if(time > timeout)
		{
			delete[] tuple;
			return ;
		}
		tuple[0] = i;
		tuple[1] = (i+1)%size;
		for(int j=2; j<size-i-2; j++)
		{
			tuple[2] = (i+j)%size;
			tuple[3] = (i+j+1)%size;
			for(int k=2; k<size-i-j; k++)
			{
				tuple[4] = (i+j+k)%size;
				tuple[5] = (i+j+k+1)%size;
				if(tripleSwapGood(tuple))
				{
					reverse(solution, tuple[1], tuple[4]);
					reverse(solution, tuple[1], tuple[2]);
				}
			}
		}
	}
	delete [] tuple;
}


void Euclidian::print()
{
	for(int i=0; i<size; i++)
	{
		printf("%d\n", solution[i]);
	}
}

long Euclidian::totalDistance()
{
	long distance = 0;
	for(int i=0; i<size; i++)
	{
		distance += dist(solution[i], solution[(i+1)%size]);
	}
	return distance;
}

double Euclidian::dist(int first, int second)
{
	Point p1 = points[first];
	Point p2 = points[second];
	double d = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
	return d;
}

bool Euclidian::swapGood(int from, int to)
{
	int from1 = (from+1)%size;
	int to1 = (to+1)%size;
	if(from % size == to % size)
		return false;
	double dist2 = dist(solution[to], solution[to1]);
	double dist3 = dist(solution[from], solution[to]);
	if(dist2 < dist3)
		return false;
	double dist1 = dist(solution[from], solution[from1]);
	double dist4 = dist(solution[from1], solution[to1]);
	return (dist1 + dist2 > dist3 + dist4);
}

bool Euclidian::insertGood(int from, int to)
{
	int from1, from2;
	int to2;
	from1 = (from-1+size)%size;
	from2 = (from+1)%size;
	to2 = (to+1)%size;
	double* distance = new double[6];
	distance[0] = dist(solution[from1], solution[from]);
	distance[1] = dist(solution[from], solution[from2]);
	distance[2] = dist(solution[to], solution[to2]);
	distance[3] = dist(solution[from1], solution[from2]);
	distance[4] = dist(solution[to], solution[from]);
	distance[5] = dist(solution[from], solution[to2]);
	bool whaat = distance[0] + distance[1] + distance[2] > distance[3] + distance[4] + distance[5];
	delete[] distance;
	return whaat;
}

bool Euclidian::tripleSwapGood(int* tuple)
{
	double* distance = new double[6];
	distance[0] = dist(solution[tuple[0]], solution[tuple[1]]);
	distance[1] = dist(solution[tuple[2]], solution[tuple[3]]);
	distance[2] = dist(solution[tuple[4]], solution[tuple[5]]);

	distance[3] = dist(solution[tuple[0]], solution[tuple[3]]);
	distance[4] = dist(solution[tuple[4]], solution[tuple[2]]);
	distance[5] = dist(solution[tuple[1]], solution[tuple[5]]);
	
	bool whaat = distance[0] + distance[1] + distance[2] > distance[3] + distance[4] + distance[5];
	delete[] distance;
	return whaat;
}

void Euclidian::reverse(int* a, int from, int to)
{
	if(from > to)
	{
		int temp = from;
		from = (to+1) % size;
		//from = to+1;
		to = (temp-1 + size) % size;
		//to = temp -1;
	}
	while(from < to)
	{
		int temp = a[from];
		a[from] = a[to];
		a[to] = temp;
		from++;
		to--;
	}
}

void Euclidian::insert(int* a, int from, int to)
{
	int temp = a[from];
	while(from != to)
	{
		int next = (from + 1)%size;
		a[from] = a[next];
		from = next;
	}
	a[from] = temp;
}