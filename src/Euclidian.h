#pragma once
#include <iostream>
#include <cmath>
#include "time.h"
#include <limits>

using namespace std;

class Euclidian
{
        struct Point{
                double x, y;
        };
public:
        Euclidian(std::istream&);
        ~Euclidian(void);
        void initDistances();
        void solve();
        void greedy();
        void ruinEverything(int count);
		void opt2Best(clock_t start, int timeout);
        void opt2Cycle(clock_t start, int timeout);
        void opt25Cycle(clock_t start, int timeout);
        void opt3Cycle(clock_t start, int timeout);
        void print();
        long totalDistance();

private:
        // Methods
        double dist(int first, int second);
        double swapGood(int from, int to);
        bool insertGood(int from, int to);
        int tripleSwapGood(int* tuple);
        void reverse(int* a, int from, int to);
        void insert(int* a, int from, int to);
        // Fields
        int K;
        static const int K_MAX = 30;
        static const int OPT2_COUNT = 10;
        size_t size;
        int* solution;
        int** nodeDistance;
        double** distanceMatrix;
        Point* points;
};