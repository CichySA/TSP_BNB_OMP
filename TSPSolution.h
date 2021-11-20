#pragma once
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <iterator>
#include <cmath>
#include <utility>
#include <list>
#include <numeric>
class TSPSolution
{
public:
	TSPSolution();
	~TSPSolution();
	void parse_file(std::ifstream &input_stream);
	void heuristic_solve();
	void brute_force_solve();
	TSPSolution get_solution();
	static int get_score(const TSPSolution& solution);

private:
	std::vector<int> matrix;
	std::map < std::string , std::string > metadata;
	std::list<int> path;
	int get_edge_weight(int v1, int v2) const;
	int get_index(int v1, int v2) const;
};

TSPSolution::TSPSolution()
{
}


TSPSolution::~TSPSolution()
{
}

inline int TSPSolution::get_edge_weight(int v1, int v2) const {
	
	int ind = get_index(v1, v2);
	return matrix.at(ind);
}

inline int TSPSolution::get_index(int v1, int v2) const
{
	if (v2 > v1) {
		int temp = v1;
		v1 = v2;
		v2 = temp;
	}
	return (v1 * v1 - v1) / 2 + v1 + v2;
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
	int val_cnt = get_index(dim-1, dim-1) + 1;
	copy_n(is_it, val_cnt, back_inserter(matrix));
}

inline void TSPSolution::heuristic_solve()
{
	using namespace std;
	this->path.clear();
	list<int> unassigned_vertices;
	generate_n(back_inserter(unassigned_vertices), stoi(metadata.at("DIMENSION")) - 1, [n = 0]() mutable {return ++n; });
	path.push_back(0);
	while (!unassigned_vertices.empty())
	{
		int last_vertice = path.back();
		auto it = min_element(unassigned_vertices.begin(), unassigned_vertices.end(), [&](int a, int b) { 
			return get_edge_weight(last_vertice, a) < get_edge_weight(last_vertice, b); });
		path.push_back(*it);
		unassigned_vertices.erase(it);
	}
}

inline TSPSolution TSPSolution::get_solution()
{
	return *this;
}

inline int TSPSolution::get_score(const TSPSolution &solution)
{
	auto it = solution.path.cbegin();
	int acc = 0;
	int previous_vertice = *it;
	while (++it != solution.path.cend())
	{
		acc += solution.get_edge_weight(previous_vertice, *it);
		previous_vertice = *it;
	}
	acc += solution.get_edge_weight(previous_vertice, 0);
	return acc;

}
