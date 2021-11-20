// TSP_BNB_OMP.cpp : Defines the entry point for the application.
//

#include "TSP_BNB_OMP.h"
#include "omp.h"
#include <sstream>;
#include <iostream>
#include <fstream>
#include "TSPSolution.h"

using namespace std;
int main()
{
	TSPSolution solution;
	
	ifstream input_file("C:\\Users\\szymo\\source\\repos\\TSP_BNB_OMP\\tsplib\\gr17.tsp");
	if (input_file.is_open()) {

		solution.parse_file(input_file);
	}

	return 0;
}
