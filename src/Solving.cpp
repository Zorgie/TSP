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

	bool* used = new bool[size];
	for(int i=0;i<size;i++)
	{used[i] = false;}

	pathStart = new Route(0,NULL,NULL);
	Route* path = pathStart;
	map[0].pos = pathStart;
	Route* lastRoute = pathStart;
	used[0] = true;

	for(int i=1; i<size; i++)
	{
			int best = -1;
		for(int j=0; j<size; j++)
		{
			if(!used[j] && (best == -1 || distance(lastRoute->ID(), j) < distance(lastRoute->ID(), best)))
				best = j;
		}

	path->nextr = new Route(best,path,NULL);
	used[best] = true;
	path = path->next(path);
	map[best].pos = path; 
	lastRoute = path;
	}
	
	path->nextr = pathStart;
	pathStart->prevr = path;

}

void Solving::twoOpt(){

	Route* lastRoute = pathStart;
	Route* tmp = pathStart->next(pathStart);
	
	do {
				
		
		Route* lastInnerRoute=pathStart;
		Route* tmp2 = pathStart->next(pathStart);
			do{
				

	
				//std::cout << lastRoute->ID()<<"-"<<tmp->ID() << " + " << lastInnerRoute->ID()<<"-"<< tmp2->ID()<< " > " << lastRoute->ID()<<"-"<<lastInnerRoute->ID() << " + " << tmp->ID()<<"-"<<tmp2->ID() << std::endl;
				//std::cout << distance(lastRoute->ID(),tmp->ID()) << " + " << distance(lastInnerRoute->ID(),tmp2->ID()) << " > " << distance(lastRoute->ID(),lastInnerRoute->ID()) << " + " << distance(tmp->ID(),tmp2->ID()) << std::endl;

				if(((distance(lastRoute->ID(),tmp->ID())+distance(lastInnerRoute->ID(),tmp2->ID()))>(distance(lastRoute->ID(),lastInnerRoute->ID())+distance(tmp->ID(),tmp2->ID()))) && okayCombo(lastRoute->ID(),tmp->ID(),lastInnerRoute->ID(),tmp2->ID()))
				{
						//std::cout << distance(lastRoute->ID(),tmp->ID()) << " + " << distance(lastInnerRoute->ID(),tmp2->ID()) << " > " << distance(lastRoute->ID(),lastInnerRoute->ID()) << " + " << distance(tmp->ID(),tmp2->ID()) << std::endl;
						
						switchRoads(lastRoute->ID(),tmp->ID(),lastInnerRoute->ID(),tmp2->ID());
						return;
				}
			Route* lol2 = tmp2;
			tmp2 = tmp2->next(lastInnerRoute);
			lastInnerRoute = lol2;

			}while(tmp2->ID()!=pathStart->nextr->ID());

	Route* lol = tmp;
	tmp = tmp->next(lastRoute);
	lastRoute = lol;

	}while(tmp->ID() != pathStart->nextr->ID());
}

bool Solving::okayCombo(int a, int b, int c, int d)
{
		if(a != b && a != c && a != d && b != c && b != d && c != d)
				return true;
		return false;

}

void Solving::switchRoads(int A,int B, int C, int D)
{// switches the road A-B, C-D to A-C, B-D
		//std::cout << "SWITCHING "<< A << "-"<< B << ","<< C << "-"<< D << " to " << A << "-"<< C << ","<< B << "-"<< D << std::endl;

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
		Route* lastRoute = pathStart;
		while(tmp->ID() != pathStart->ID())
		{
			printf("%d\n",lastRoute->ID());

			Route* lol = tmp;		
			tmp = tmp->next(lastRoute);
			lastRoute = lol;

		}
		printf("%d\n",lastRoute->ID());

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
		//std::cout << ID1.ID << "-" << ID2.ID << std::endl;
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
	Route* lastRoute = pathStart;
	Route* tmp = pathStart->next(pathStart);
	while(tmp->ID()!=pathStart->ID())
	{
	
		sum += distance(lastRoute->ID(),tmp->ID());
		Route* lol = tmp;		
		tmp = tmp->next(lastRoute);
		lastRoute = lol;

		//std::cout << sum << " - " << lastid << tmp->ID() << std::endl;
	}
		sum += distance(lastRoute->ID(),tmp->ID());

	return sum;
}
