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

	twoOptDone = false;
	threeOptDone = false;
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
		if(out_of_time())
			return;
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




bool Solving::twoOpt(){

	Route* lastRoute = pathStart;
	Route* tmp = pathStart->next(pathStart);
	
	do {
			Route* lastInnerRoute=tmp;
		Route* tmp2 = tmp->next(lastRoute);
			do{
				if(out_of_time())
					return false;

				if(okayCombo(lastRoute->ID(),tmp->ID(),lastInnerRoute->ID(),tmp2->ID()))
				{
					if(switchRoads(lastRoute->ID(),tmp->ID(),lastInnerRoute->ID(),tmp2->ID()))
					{
					return true;
					}
				}
			Route* lol2 = tmp2;
			tmp2 = tmp2->next(lastInnerRoute);
			lastInnerRoute = lol2;

			}while(tmp2->ID()!=pathStart->nextr->ID());

	Route* lol = tmp;
	tmp = tmp->next(lastRoute);
	lastRoute = lol;

	}while(tmp->ID() != pathStart->nextr->ID());
	return false;

}

bool Solving::twoOptReverse(){

	Route* lastRoute = pathStart;
	Route* tmp = pathStart->nextr;
	
	do {
		Route* lastInnerRoute=pathStart;
		Route* tmp2 = pathStart->prevr;
			do{
				if(out_of_time())
					return false;

				if(okayCombo(lastRoute->ID(),tmp->ID(),tmp2->ID(),lastInnerRoute->ID()))
				{
					if(switchRoads(lastRoute->ID(),tmp->ID(),tmp2->ID(),lastInnerRoute->ID()))
					{
					return true;
					}
				}
			Route* lol2 = tmp2;
			tmp2 = tmp2->next(lastInnerRoute);
			lastInnerRoute = lol2;

			}while(tmp2->ID()!=tmp->ID());

	Route* lol = tmp;
	tmp = tmp->next(lastRoute);
	lastRoute = lol;

	}while(tmp->ID() != pathStart->nextr->ID());
	return false;

}






bool Solving::okayCombo(int a, int b, int c, int d)
{
		if(a != b && a != c && a != d && b != c && b != d && c != d)
				return true;
		return false;

}

bool Solving::switchRoads(int A,int B, int C, int D)
{// switches the road A-B, C-D to A-C, B-D
	if((distance(A,B)+distance(C,D))<= (distance(A,C)+distance(B,D)))
	{
		return false;
	}
Route* a = map[A].pos;
Route* b = map[B].pos;
Route* c = map[C].pos;
Route* d = map[D].pos;
a->replace(b,c);
c->replace(d,a);
b->replace(a,d);
d->replace(c,b);
return true;
}

void Solving::improveTSP()
{
	while(!out_of_time())
	{
	
		twoOptReverse();
		//std::cout << "2-opt " << totalDistance() << std::endl;
		twoHalfOpt();
		//std::cout << "2.5-opt " << totalDistance() << std::endl;
		//threeOpt();
		//std::cout << "3-opt " << totalDistance() << std::endl;
/*
	if(!twoOptReverse())
	{
		if(out_of_time())
			return ;

			twoHalfOpt();
		if(!threeOpt())
		{
		std::cout << "3-opt " << totalDistance() << std::endl;
			if(out_of_time())
				return ;

			twoHalfOpt();
		}

		if(out_of_time())
			return ;

	}
	*/
	}

}

void Solving::printTSP()
{

		Route* tmp = pathStart->next(pathStart);
		Route* lastRoute = pathStart;
		while(tmp->ID() != pathStart->ID())
		{
			std::cout << lastRoute->ID() << std::endl;
			lastRoute->printInfo();	
			Route* lol = tmp;		
			tmp = tmp->next(lastRoute);
			lastRoute = lol;

		}
			std::cout << lastRoute->ID() << std::endl;
			lastRoute->printInfo();	

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

	}
		sum += distance(lastRoute->ID(),tmp->ID());

	return sum;
}



bool Solving::threeOpt(){
	/*
	 * threeopt cases
	 * Given A-B, C-D, E-F
	 * Working:
	 * A-C, B-E, D-F
	 * A-D, B-E, C-F
	 * A-D, B-F, C-E
	 * A-E, B-D, C-F
	 */
	
	Route* fix1 = pathStart;
	Route* fix2 = pathStart;
	Route* temp = pathStart->next(pathStart);

	for(int i=0;i<4;i++)
	{
		Route* lol = fix1;
		fix1 = fix1->next(temp);
		temp = lol;
		if(i==1)
			fix2 = fix1;

	}

	Route* lr = pathStart;
	Route* tmp = pathStart->nextr;
	do {
		Route* lir=tmp->next(lr);
		Route* tmp2 = lir->next(tmp);
		do{
			
			Route* liir=tmp2->next(lir);
			Route* tmp3 = liir->next(tmp2);
			do{
				if(out_of_time())
					return false;
				if(threeOkayCombo(lr->ID(),tmp->ID(),lir->ID(),tmp2->ID(),liir->ID(),tmp3->ID()))
				{
					if(threeSwitchRoads(lr->ID(),tmp->ID(),lir->ID(),tmp2->ID(),liir->ID(),tmp3->ID()))
						{
							return true;
						}
					}
				
				Route* lol3 = tmp3;
				tmp3 = tmp3->next(liir);
				liir = lol3;
			}while(tmp3->ID()!=pathStart->ID());

			Route* lol2 = tmp2;
			tmp2 = tmp2->next(lir);
			lir = lol2;
		}while(tmp2->ID()!=fix2->ID());

	Route* lol = tmp;
	tmp = tmp->next(lr);
	lr = lol;
	}while(tmp->ID() != fix1->ID());

	return false;
}






bool Solving::threeOkayCombo(int a, int b, int c, int d, int e, int f)
{
		if(a != b && a != c && a != d && a != e && a != f && b != c && b != d && b != e && b != f && c != d && c != e && c != f && d != e && d != f && e != f)
				return true;
		return false;

}

bool Solving::threeSwitchRoads(int A,int B, int C, int D, int E, int F)
{
	/*
	 * returns true if change is made, false otherwise
	 * threeopt cases
	 * Given A-B, C-D, E-F
	 * Working:
	 * A-C, B-E, D-F
	 * A-D, B-E, C-F
	 * A-D, B-F, C-E
	 * A-E, B-D, C-F
	 */
	int compare = distance(A,B) + distance(C,D)+distance(E,F);
	int min = compare;
	int index = 0;

	
	int tmp=0;
	if((tmp = distance(A,C)+distance(B,E)+distance(D,F))<min)
	{
		index = 1;
		min = tmp;
	 }
	if((tmp = distance(A,D)+distance(B,E)+distance(C,F))<min)
	{
		index = 2;
		min = tmp;
	 }
	if((tmp = distance(A,D)+distance(B,F)+distance(C,E))<min)
	{
		index = 3;
		min = tmp;
	 }
	if((tmp = distance(A,E)+distance(B,D)+distance(C,F))<min)
	{
		index = 4;
		min = tmp;
	 }
	if(min == compare)
		return false;

	Route* a = map[A].pos;
	Route* b = map[B].pos;
	Route* c = map[C].pos;
	Route* d = map[D].pos;
	Route* e = map[E].pos;
	Route* f = map[F].pos;
	switch(index)
	{
		case 1: 
			//std::cout << "case 1" << std::endl;
			a->replace(b,c);
			b->replace(a,e);
			c->replace(d,a);
			d->replace(c,f);
			e->replace(f,b);
			f->replace(e,d);
			break;
		case 2:  
			//std::cout << "case 2 " << A << "-" << D << "," << B <<"-" <<E << "," << C << "-" << F << std::endl;

			a->replace(b,d);
			b->replace(a,e);
			c->replace(d,f);
			d->replace(c,a);
			e->replace(f,b);
			f->replace(e,c);
			break;
		case 3:  
			//std::cout << "case 3 " << A << "-" << D << "," << B <<"-" <<F << "," << C << "-" << E << std::endl;
			a->replace(b,d);
			b->replace(a,f);
			c->replace(d,e);
			d->replace(c,a);
			e->replace(f,c);
			f->replace(e,b);
			break;
		case 4:  
			//std::cout << "case 4" << std::endl;
			a->replace(b,e);
			b->replace(a,d);
			c->replace(d,f);
			d->replace(c,b);
			e->replace(f,a);
			f->replace(e,c);
			break;
	}
	return true;

}

void Solving::setTime(double limit)
{
startTime = clock();
timeLimit = limit*CLOCKS_PER_SEC;

}

bool Solving::out_of_time()
{
	clock_t tmp = clock();
if(((double)tmp-startTime)>timeLimit)
	return true;
return false;
}

bool Solving::twoHalfOpt(){

	Route* lastRoute = pathStart;
	Route* tmp = pathStart->nextr;
	
	do {
		Route* lastInnerRoute=tmp;
		Route* tmp2 = tmp->next(lastRoute);
			do{
				if(out_of_time())
					return false;

				if(moveCity(tmp,lastInnerRoute,tmp2))
					return true;	

			
			Route* lol2 = tmp2;
			tmp2 = tmp2->next(lastInnerRoute);
			lastInnerRoute = lol2;

			}while(tmp2->ID()!=pathStart->nextr->ID());

	Route* lol = tmp;
	tmp = tmp->next(lastRoute);
	lastRoute = lol;

	}while(tmp->ID() != pathStart->nextr->ID());
	return false;

}
bool Solving::moveCity(Route* city, Route* r1, Route* r2)
{
	if(city->ID() == r1->ID() || city->ID() == r2->ID() || r1->ID() == r2->ID())
		return false;

		int oldD= distance(city->ID(),city->prevr->ID()) + distance(city->ID(),city->nextr->ID()) + distance(r1->ID(),r2->ID());

	int newD= distance(r1->ID(),city->ID()) + distance(r2->ID(),city->ID()) + distance(city->nextr->ID(),city->prevr->ID());

	if(oldD<=newD)
		return false;


	//std::cout << "flyttar: " << city->ID() << " till mellan " << r1->ID() << " och " << r2->ID() << std::endl;

	city->nextr->replace(city,city->prevr);
	city->prevr->replace(city,city->nextr);
	r1->replace(r2,city);
	r2->replace(r1,city);
	city->nextr = r1;
	city->prevr = r2;

	return true;
}
