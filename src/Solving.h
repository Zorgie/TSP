#pragma once

#include <iostream>
#include <vector>
#include "time.h"

class Solving
{

public:
	Solving(std::istream&);
	~Solving(void);
	void solveTSP();
	void printTSP();
	void improveTSP();
private:
	
std::vector<std::vector<int>> roads;
struct Route;
struct City
{
		int ID;
		double coordX;
		double coordY;
		Route* pos;

	City(){}
	City(int id,double x,double y)
	{
		ID = id;
		coordX = x;
		coordY = y;
	}
	void set(int id,double x,double y)
	{
		ID = id;
		coordX = x;
		coordY = y;
	}

};

struct Route
{
		// typ en dubbellänkad lista 
		Route* prevr;
		Route* nextr;
		int IDD;


		Route(){}
		Route(int i, Route* r, Route* n){
				IDD = i;
				prevr = r ;
				nextr = n;
		}
		int ID(){return IDD;}

		Route* next(Route* asker)
		{
			if (asker == nextr)
			{
					//std::cout << "asker: " << asker << " returning: " << prevr << std::endl;
					return prevr;
			}
			else
					return nextr;
		}

		void replace(Route* r,Route* p)
		{
				//std::cout << "ID: " << ID() << " Address: " << this << " nextr: " << nextr << " prevr: " << prevr << " replacing: " << r << " with: " << p << std::endl;
			if(r == prevr)
					prevr = p;
			else
					nextr = p;
		}
		void printInfo()
		{
				//std::cout << "ID: " << ID() << " Address: " << this << " nextr: " << nextr << " prevr: " << prevr << std::endl;
		}
};


	// Fields
	size_t size;
	std::vector<int> route;
	Route* pathStart;
	std::vector<City> map;
	bool twoOptDone, threeOptDone;
	clock_t startTime;
	double timeLimit;

	// Methods
	int distance(const int ID1,const int ID2);
	int distance(const City ID1,const City ID2);

	bool okayCombo(int a, int b, int c, int d);
	bool twoOpt();
	bool twoOptReverse();
	bool twoOptRandom();
	bool switchRoads(int A,int B, int C, int D);
	
	bool threeOkayCombo(int a, int b, int c, int d, int e, int f);
	bool threeOpt();

	bool threeSwitchRoads(int A,int B, int C, int D, int E, int F);

	bool twoHalfOpt();
	bool moveCity(Route* city, Route* r1, Route* r2);
	
	public:
	void setTime(double limit);
	int totalDistance();
	private:
		bool out_of_time();


};


