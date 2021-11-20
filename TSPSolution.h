#pragma once
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iterator>
#include <cmath>
class TSPSolution
{
public:
	TSPSolution();
	~TSPSolution();
	void parse_file(std::ifstream &input_stream);

private:
	std::vector<int> matrix;
	std::map < std::string , std::string > metadata;
};

TSPSolution::TSPSolution()
{
}

TSPSolution::~TSPSolution()
{
}

inline void TSPSolution::parse_file(std::ifstream &input_stream)
{
	using namespace std;
	string key;
	string value;
	input_stream >> key;
	while(key.compare("EDGE_WEIGHT_SECTION") != 0)
	{
		input_stream >> ws >> value;
		const char* tabu = " \t\n:";
		int first = key.find_first_not_of(tabu);
		int last = key.find_last_not_of(tabu);
		key = key.substr(first, last - first + 1);
		first = value.find_first_not_of(tabu);
		last = value.find_first_not_of(tabu);
		metadata.insert(make_pair(key, value));
		input_stream >> key;
	}
	istream_iterator<int> is_it(input_stream);
	int dim = stoi(metadata.at("DIMENSION"));
	int val_cnt = floor((dim * dim - dim) / 2) + dim;
	copy_n(is_it, val_cnt, back_inserter(matrix));
}
