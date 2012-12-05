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
	K = min(30, (int)size-1);
	distanceMatrix = new double*[size];
	Point p1;
	Point p2;
for(int i=0; i<size; i++)
	{
		distanceMatrix[i] = new double[size];
	}

	for(int i=0; i<size; i++)
	{
		for(int j=0; j<size; j++)
		{
			p1 = points[i];
			p2 = points[j];
			distanceMatrix[i][j] = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
			if(distanceMatrix[j][i] == 0 && i != j)
				edges.push_back(new Edge(i,distanceMatrix[i][j],j));
		}
	}

	// Init the sorting stuff
	bool* used = new bool[size];
	nodeDistance = new int*[size];
	for(int i=0; i<size; i++)
	{
		nodeDistance[i] = new int[K];
		// clear used
		for(int j = 0; j<size; j++)
		{
			used[j] = false;
		}
		used[i] = true;
		// Find lowest nonused distance.
		for(int k = 0; k<K; k++)
		{
			double minDist = numeric_limits<double>::max();
			int minId = 0;
			for(int j=0; j<size; j++)
			{
				if(used[j])
					continue;
				if(dist(i, j) < minDist)
				{
					minDist = dist(i, j);
					minId = j;
				}
			}
			nodeDistance[i][k] = minId;
			used[minId] = true;
		}
	}
}


Euclidian::~Euclidian(void)
{
}


void Euclidian::solve()
{
	//greedy();
	christofides();
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

void Euclidian::christofides()
{
	vector<int> ocs;
	ocs.resize(size);
	vector<Edge*> MSTE;

	MST(ocs,MSTE);
	cout << "MST done" << endl;

	/*	
		for(int i=0;i<MSTE.size();i++)
		{
			cout << "Tree " << MSTE[i]->from << " - " << MSTE[i]->to << endl;
		}*/
	vector<int> odd;
	for(int i=0;i<size;i++)
	{
		if(ocs[i]%2 != 0)
			odd.push_back(i);
	}
	
	vector<Edge*> match;
	matching(odd,match);

	cout << "Matching done " << endl;

	for(int i=0;i<match.size();i++)
		{
			MSTE.push_back(match[i]);
			//cout << "Matchning "<< match[i]->from << " - " << match[i]->to << endl;
		}

	solution = new int[size];

	bool* used = new bool[size];
	int lastNode = MSTE[0]->from;
	MSTE.erase(MSTE.begin()+0);
	used[lastNode] = true;
	solution[0] = lastNode;
	int usednr = 1;
	int tmp;

	while(usednr<size)
	{
			tmp = -1;
		for(int i=0;i<MSTE.size();i++)
		{
			Edge* e = MSTE[i];
			//cout << "e: " << e->from << " -> " << e->to << " from " << lastNode << endl;
			if(e->from == lastNode && !used[e->to])
			{
			//cout << "e: " << e->from << " -> " << e->to << " from " << lastNode << endl;
				solution[usednr] = e->to;
				used[e->to] = true;
				lastNode = e->to;
				MSTE.erase(MSTE.begin()+i);
				usednr++;
				tmp = -1;
				break;
			}
			else if(e->from == lastNode && used[e->to])
			{
				tmp = i;
			}
			else if(e->to == lastNode && !used[e->from])
			{
			//cout << "e: " << e->from << " -> " << e->to << " from " << lastNode << endl;
				solution[usednr] = e->from;
				used[e->from] = true;
				lastNode = e->from;
				MSTE.erase(MSTE.begin()+i);
				usednr++;
				tmp = -1;
				break;
			}
			else if(e->to == lastNode && used[e->from])
			{
				tmp = i;
			}
		}
		if( tmp != -1 && MSTE[tmp]->to == lastNode)
		{
			lastNode = MSTE[tmp]->from;
			MSTE.erase(MSTE.begin()+tmp);
		}else if( tmp != -1 && MSTE[tmp]->from == lastNode)
		{
			lastNode = MSTE[tmp]->to;
			MSTE.erase(MSTE.begin()+tmp);
		}
	}
	cout << "the last crap done" << endl;
	
		

}

void Euclidian::MST(vector<int>& ocs, vector<Edge*>&  MSTE)
{
	vector<List*> vec;
		for(int i=0;i<size;i++)
		{
			vec.push_back(new List(i));
		}
			
		std::make_heap(edges.begin(),edges.end(),Edge());
	
		
		while(MSTE.size()<size-1)
		{

			  pop_heap(edges.begin(),edges.end(),Edge()); 
				Edge* e = edges.front();
				edges.pop_back();

		for(int i=0;i<vec.size();i++)
		{
			for(int j=0;j<vec.size();j++)
			{
				if((vec[i]->find(e->from) && vec[j]->find(e->to))||(vec[j]->find(e->from) && vec[i]->find(e->to)) )
					{
						if(i==j)
							continue;
						if(i>j)
						{
							vec[j]->unite(vec[i]);
							vec.erase(vec.begin()+i);
						}else
						{
							vec[i]->unite(vec[j]);
							vec.erase(vec.begin()+j);
						}
						ocs[e->from]++;
						ocs[e->to]++;
						MSTE.push_back(e);
					}
			}
		}
		}
		return ; 

}

void Euclidian::matching(vector<int>& odd, vector<Edge*>&  match)
{
	/*
	for(int i=0;i<odd.size();i++)
	{
			cout << odd[i];
	}
	cout << endl << "total odds " << odd.size() << endl;
*/

	// en "greedy" matchning
	int odds = odd.size();
	while(match.size()<odds/2)
	{
		int bestIndex = 1;
		for(int j=1;j<odd.size();j++)
		{
			if(distanceMatrix[odd[0]][odd[j]]<distanceMatrix[odd[0]][odd[bestIndex]])
				bestIndex = j;
		}
		match.push_back(new Edge(odd[0],distanceMatrix[odd[0]][odd[bestIndex]],odd[bestIndex]));
		odd.erase(odd.begin()+bestIndex);
		odd.erase(odd.begin());
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

void Euclidian::opt2Best(clock_t start, int timeout)
{
	time_t now = clock();
	int begin = (((double)now-start)/CLOCKS_PER_SEC*1000);
	if(size < 4)
		return;
	bool running = true;
	while(running)
	{
		running = false;
		double best = 0;
		int bestFrom = 0;
		int bestTo = 0;
		for(int i=0; i<size-3; i++)
		{
			now = clock();
			int time = (((double)now-start)/CLOCKS_PER_SEC*1000);
			if(time > timeout)
				return;
			for(int j=2; j< (size-1); j++)
			{
				int to = (i + j)%size;
				double current = swapGood(i, to);
				if(current > best)
				{
					best = current;
					bestTo = to;
					bestFrom = i+1;
				}
			}
		}
		if(best > 0)
		{
			reverse(solution, bestFrom, bestTo);
			running = true;
		}
	}
}

void Euclidian::opt2Cycle(clock_t start, int timeout)
{
	time_t now = clock();
	int begin = (((double)now-start)/CLOCKS_PER_SEC*1000);
	if(size < 4)
		return;
	bool running = true;
	while(running){
		running = false;
		for(int i=0; i<size-3; i++)
		{
			now = clock();
			int time = (((double)now-start)/CLOCKS_PER_SEC*1000);
			if(time > timeout)
				return;
			for(int j=2; j< (size-1); j++)
			{
				int to = (i + j)%size;
				if(swapGood(i, to) > 0)
				{
					reverse(solution, i+1, to);
					if(i > 0 && time < begin + (timeout-begin) * 0.65)
					{
						i--;
					}
					running = true;
				}
			}
		}
	}
}
void Euclidian::opt25Cycle(clock_t start, int timeout)
{
	if(size < 4)
		return ;
	time_t now = clock();
	int begin = (((double)now-start)/CLOCKS_PER_SEC*1000);
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
				if( i > 0 && time < begin + (timeout - begin)*0.65)
				{
					i--;
				}
			}
		}
	}
}

void Euclidian::opt3Cycle(clock_t start, int timeout)
{
	if(size < 6)
		return;
	int* tuple = new int[6];
	bool running = true;
	while(running)
	{
		running = false;
		for(int i=0; i<size-5; i++)
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
			for(int j=0; j<K; j++)
			{
				tuple[2] = nodeDistance[i][j];
				tuple[3] = tuple[2] + 1;
				if(tuple[2] < i+2 || tuple[2] > size-4)
					continue;
				for(int k=0; k<K; k++)
				{
					tuple[4] = nodeDistance[i][k];
					tuple[5] = tuple[4] + 1;
					if(tuple[4] < tuple[2] + 2 || tuple[4] > size-2)
						continue;
					int swap = tripleSwapGood(tuple);
					if(swap == 1)
					{
						// Case 1: 0-3, 2-4, 1-5
						reverse(solution, tuple[3], tuple[4]);
						reverse(solution, tuple[1], tuple[4]);
						running = true;
					}
					else if(swap == 2)
					{
						// Case 2: 0-3, 1-4, 2-5
						reverse(solution, tuple[1], tuple[2]);
						reverse(solution, tuple[3], tuple[4]);
						reverse(solution, tuple[1], tuple[4]);
						running = true;
					}
					if(swap == 3)
					{
						// Case 1: 0-4, 1-3, 2-5
						reverse(solution, tuple[1], tuple[2]);
						reverse(solution, tuple[1], tuple[4]);
						running = true;
					}
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
	return distanceMatrix[first][second];
}

double Euclidian::swapGood(int from, int to)
{
	int from1 = (from+1)%size;
	int to1 = (to+1)%size;
	if(from % size == to % size)
		return false;
	double dist2 = dist(solution[to], solution[to1]);
	double dist3 = dist(solution[from], solution[to]);
	if(dist2 < dist3)
		return 0;
	double dist1 = dist(solution[from], solution[from1]);
	double dist4 = dist(solution[from1], solution[to1]);
	return (dist1 + dist2 - dist3 - dist4);
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

int Euclidian::tripleSwapGood(int* tuple)
{
	double* distance = new double[10];
	distance[0] = dist(solution[tuple[0]], solution[tuple[1]]);
	distance[1] = dist(solution[tuple[2]], solution[tuple[3]]);
	distance[2] = dist(solution[tuple[4]], solution[tuple[5]]);

	// Version 1
	distance[3] = dist(solution[tuple[0]], solution[tuple[3]]);
	distance[4] = dist(solution[tuple[1]], solution[tuple[5]]);
	distance[5] = dist(solution[tuple[2]], solution[tuple[4]]);

	// Version 2
	distance[6] = dist(solution[tuple[1]], solution[tuple[4]]);
	distance[7] = dist(solution[tuple[2]], solution[tuple[5]]);

	// Version 3
	distance[8] = dist(solution[tuple[0]], solution[tuple[4]]);
	distance[9] = dist(solution[tuple[1]], solution[tuple[3]]);


	if( distance[0] + distance[1] + distance[2] > distance[3] + distance[4] + distance[5])
	{
		delete[] distance;
		return 1;
	}
	else if( distance[0] + distance[1] + distance[2] > distance[3] + distance[6] + distance[7])
	{
		delete[] distance;
		return 2;
	}
	else if( distance[0] + distance[1] + distance[2] > distance[7] + distance[8] + distance[9])
	{
		delete[] distance;
		return 3;
	}
	delete[] distance;
	return 0;
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
