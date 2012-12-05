#pragma once
#include <iostream>
#include <cmath>
#include "time.h"
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

class Euclidian
{
        struct Point{
                double x, y;
        };
		struct Edge{
			int from;
			double distance;
			int to;
			Edge(){}
			Edge(int a, int b, int c)
			{
				from = a;
				distance = b;
				to = c;
			}	
			bool operator()(const Edge* x, const Edge* y) const
			{// funktor för att jag är en douche som använder sånt
				return x->distance < y->distance;
			}
		};

		struct List{
			int node;
			List* child;

			List(){}
			List(int a)
			{
				child = NULL;
				node = a;
			}
			bool find(int element)
			{
			if(node == element)
				return true;
			if(child == NULL)
				return false;

			return child->find(element);
			}
			void unite(List* b)
			{
				if(child == NULL)
					child = b;
				else
					child->unite(b);
			}
			int size()
			{
				if(child == NULL)
					return 1;

				return 1+child->size();
			}
		};
public:
        Euclidian(std::istream&);
        ~Euclidian(void);
        void initDistances();
        void solve();
        void greedy();
		void christofides();
		void MST(vector<int>& ocs, vector<Edge*>&  MSTE);
		void matching(vector<int>& odd, vector<Edge*>& match);
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
		std::vector<Edge*> edges;
        Point* points;
};
