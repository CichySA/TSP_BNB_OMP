// TSP_BNB_OMP.cpp : Defines the entry point for the application.
//

#include "TSP_BNB_OMP.h"
#include "omp.h"

using namespace std;
int main()
{
	cout << "Hello CMake." << endl;
#pragma omp parallel
	{
		printf("Hello world from thread = %d\n", omp_get_thread_num());

	}
	return 0;
}
