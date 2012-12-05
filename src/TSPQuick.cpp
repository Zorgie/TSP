// TSPQuick.cpp : Defines the entry point for the console application.
//
#include "Euclidian.h"
#include "time.h"

int main(int argc, char* argv[])
{
	clock_t t1, t2;
	t1 = clock();

	Euclidian e(std::cin);
	t2 = clock();
	e.solve();
	e.opt2Best(t1, 600);
	int time = (((double)t2-t1)/CLOCKS_PER_SEC*1000);
	cerr << "Setup time: " << time << "\n";
	time = min(time, 1150);
	e.opt2Cycle(t1, 200 + time);
	e.opt25Cycle(t1, 400 + time);
	e.opt2Cycle(t1, 600 + time);
	e.opt25Cycle(t1, 800 + time);
	cerr << "Total: " << e.totalDistance() << "\n";
	e.opt3Cycle(t1, 1950);
	e.opt2Best(t1, 1950);
	t2 = clock();
	e.print();
	time = (((double)t2-t1)/CLOCKS_PER_SEC*1000);
	cerr << "Total distance: " << e.totalDistance() << ", time: " << time;
	return 0;
}