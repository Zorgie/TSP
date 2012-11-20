// TSP.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "Graph.h"
#include "Solving.h"

int main(int argc, char* argv[])
{
	Solving s(std::cin);
	s.solveTSP();
	s.printTSP();
	return 0;
}