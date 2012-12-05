// TSPQuick.cpp : Defines the entry point for the console application.
//
#include "Euclidian.h"
#include "time.h"

int main(int argc, char* argv[])
{
	clock_t t1, t2;
	t1 = clock();

	Euclidian e(std::cin);
	e.solve();
	e.opt2Cycle(t1, 200);
	e.opt25Cycle(t1, 600);
	//cerr << "Total distance: " << e.totalDistance() << "\n";
	e.opt2Cycle(t1, 800);
	e.opt25Cycle(t1, 1200);
	//cerr << "Total distance: " << e.totalDistance() << "\n";
	e.opt2Cycle(t1, 1400);
	e.opt25Cycle(t1, 1900);
	t2 = clock();
	e.print();
	//int time = (((double)t2-t1)/CLOCKS_PER_SEC*1000);
	//cerr << "Total distance: " << e.totalDistance() << ", time: " << time << "\n";
	return 0;
}
