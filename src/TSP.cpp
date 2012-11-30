// TSP.cpp : Defines the entry point for the console application.

#include <stdio.h>
#include <fstream>
#include "Solving.h"

int main(int argc, char* argv[])
{
		std::ifstream TSP("TSP.txt");
	Solving s(TSP);
	TSP.close();

	//Solving s(std::cin);
	s.solveTSP();
	s.improveTSP(); // när man har en lösning... börja ösa optimeringar
	//s.printTSP();
	return 0;
}

