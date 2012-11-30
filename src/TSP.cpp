// TSP.cpp : Defines the entry point for the console application.

#include <stdio.h>
#include <fstream>
#include "Solving.h"
#include "time.h"

int main(int argc, char* argv[])
{

	clock_t t1,t2;
	t1 = clock();
/*
	std::ifstream TSP("TSP.txt");
	Solving s(TSP);
	TSP.close();
*/
	Solving s(std::cin);
	s.solveTSP();
//	s.printTSP();
	t2 = clock();
	double time = 0;
	while(time<1.9)	
	{
		s.improveTSP(); 
		t2 = clock();
		time = ((double)t2-t1)/CLOCKS_PER_SEC;
	}

	s.printTSP();
	return 0;
}

