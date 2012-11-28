// TSP2.cpp : Defines the entry point for the console application.
//

#include "Euclidian.h"

int main(int argc, char* argv[])
{
	Euclidian e(std::cin);
	e.solve();
	e.print();
	return 0;
}