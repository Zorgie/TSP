// TSP.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include "Graph.h"

int main(int argc, char* argv[])
{
	Graph g(std::cin);
	g.solveTSP();
	g.printTSP();
	return 0;
}