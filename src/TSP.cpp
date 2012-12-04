// TSP.cpp : Defines the entry point for the console application.

#include <stdio.h>
#include <fstream>
#include "Solving.h"

int main(int argc, char* argv[])
{


	std::ifstream TSP("testcase.txt");
	Solving s(TSP);
	TSP.close();

//	Solving s(std::cin);
	s.setTime(1.6);
	s.solveTSP();
	//s.solveTSPRandom();
	s.improveTSP();

	s.printTSP();
	return 0;
}

