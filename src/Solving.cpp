#include "Solving.h"
#include <math.h>

Solving::Solving(std::istream& in)
{
	// Reads the size
	int temp;
	in >> temp;
	size = temp;
	map.resize(size);
	roads.resize(size);
	for(int i=0; i<size; i++)
	{
		double x;
	    in >> x;
		double y;
	    in >> y;
		map[i].set(i,x,y);
		roads[i].resize(size);
	}
}


Solving::~Solving(void)
{
}


void Solving::solveTSP()
{
	route.resize(size);
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
			if(!used[j] && (best == -1 || distance(route[i-1], j) < distance(route[i-1], best)))
				best = j;
		}
		route[i] = best;
		used[best] = true;
	}

	pathStart = new Route(-1,NULL,NULL);
	Route* path = pathStart;
	for(int i = 0; i<size;i++)
	{
	path->nextr = new Route(route[i],path,NULL);
	path = path->next(path);
	map[route[i]].pos = path; 
	}
	
	pathEnd = new Route(-1,path,NULL);
	(*path).nextr = pathEnd;
}

void Solving::twoOpt(){
	Route* tmp = pathStart->next(pathStart);
	int lastid ;
	while(tmp->ID()!=-1)
	{
		lastid=tmp->ID();
		tmp = tmp->next(tmp);
		if(tmp == pathEnd)
		{
				std::cout << "what!?!?!?!?" << std::endl;
		}

		Route* tmp2 = pathStart->next(pathStart);
			while(tmp2->ID()!=-1)
			{
				int lastinnerid=tmp2->ID();
				tmp2 = tmp2->next(tmp2);


				if((distance(lastid,tmp->ID())+distance(lastinnerid,tmp2->ID())>distance(lastid,lastinnerid)+distance(tmp->ID(),tmp2->ID())) && okayCombo(lastid,tmp->ID(),lastinnerid,tmp2->ID()))
				{
						switchRoads(lastid,tmp->ID(),lastinnerid,tmp2->ID());
						return;
				}

			}

	}
}

bool Solving::okayCombo(int a, int b, int c, int d)
{
		if(a != b && a != c && a != d && b != c && b != d && c != d)
				return true;
		return false;

}

void Solving::switchRoads(int A,int B, int C, int D)
{// switches the road A-B, C-D to A-C, B-D
		std::cout << A << " "<< B << " "<< C << " "<< D << " " << std::endl;
Route* a = map[A].pos;
Route* b = map[B].pos;
Route* c = map[C].pos;
Route* d = map[D].pos;
a->replace(b,c);
c->replace(d,a);
b->replace(a,d);
d->replace(c,b);

}

void Solving::improveTSP()
{
		twoOpt();
	

}

void Solving::printTSP()
{
		Route* tmp = pathStart->next(pathStart);
		while(tmp->ID() != -1)
		{
			printf("%d\n",tmp->ID());		
			tmp = tmp->next(tmp);
		}
/*
	for(int i=0; i<size; i++)
	{
		printf("%d\n",route[i]);		
	}
	*/

	std::cout << "totalt avstånd: " << totalDistance()  << std::endl;
}


int Solving::distance(int c1, int c2)
{
	return distance(map[c1],map[c2]);
}
int Solving::distance(const City ID1,const City ID2)
{

		if(roads[ID1.ID][ID2.ID] != 0)
		{
			return roads[ID1.ID][ID2.ID];
		}

	int tmp = round(sqrt(pow(ID1.coordX-ID2.coordX,2) + pow(ID1.coordY-ID2.coordY,2)));

	roads[ID1.ID][ID2.ID] = tmp;
	roads[ID2.ID][ID1.ID] = tmp;
	return tmp;
}
int Solving::totalDistance()
{
	int sum = 0;
	Route* tmp = pathStart->next(pathStart);
	int lastid = tmp->ID();
	while(tmp->ID()!=-1)
	{
		sum += distance(lastid,tmp->ID());
		lastid=tmp->ID();
		tmp = tmp->next(tmp);
		//std::cout << sum << " - " << lastid << tmp->ID() << std::endl;
	}
	sum += distance(lastid,pathStart->next(pathStart)->ID());
	return sum;
}
