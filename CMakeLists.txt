# CMakeList.txt : CMake project for TSP_BNB_OMP, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.9)

project ("TSP_BNB_OMP")

# Add source to this project's executable.
add_executable (TSP_BNB_OMP "TSP_BNB_OMP.cpp" "TSP_BNB_OMP.h")

# TODO: Add tests and install targets if needed.

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
	target_link_libraries(TSP_BNB_OMP PUBLIC OpenMP::OpenMP_CXX)
endif()