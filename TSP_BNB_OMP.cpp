// TSP_BNB_OMP.cpp : Defines the entry point for the application.
//

#include "TSP_BNB_OMP.h"
#include "omp.h"
#include <sstream>;
#include <iostream>
#include <fstream>
#include "TSPSolution.h"
#include <list>
using namespace std;
int main()
{
	list<int> l{ 0,1,2,3,4,5 };
	auto start = l.begin();
	auto ith = start;
	advance(ith, 2);
	TSPSolution solution;
	
	ifstream input_file("C:\\Users\\szymo\\source\\repos\\TSP_BNB_OMP\\tsplib\\gr10.tsp");
	if (input_file.is_open()) {

		solution.parse_file(input_file);
		solution.heuristic_solve();
		solution.bnb_solve();
		//solution.brute_force_solve();
		cout << "MST WEIGHT: " << solution.mst() << endl;


	}

	return 0;
}
