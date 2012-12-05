// TSP.cpp : Defines the entry point for the console application.

#include <stdio.h>
#include <fstream>
#include "Solving.h"

int main(int argc, char* argv[])
{

	// testcase 2 optimal solution = 11340
/*
	std::ifstream TSP("testcase2.txt");
	Solving s(TSP);
	TSP.close();
*/
	Solving s(std::cin);
	s.setTime(1.7);
	s.solveTSP();

	//s.solveTSPRandom();
	s.improveTSP();

	s.printTSP();
	return 0;
}

