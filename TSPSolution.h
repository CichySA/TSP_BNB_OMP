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
#include <set>
#include <iostream>
#include <fstream>
#include <stack>
#include <queue>
#define DEBUG 1

class Node {
public:
	std::list<int> path;
	std::set<int> unvisited;
	int lower_bound;
private:
};
class TSPSolution
{
public:
	TSPSolution();
	~TSPSolution();
	void parse_file(std::ifstream &input_stream);
	void heuristic_solve();
	void brute_force_solve();
	int get_score();
	void print_path(const std::list<int> & path);
	std::list<int> two_opt_solve();
	int mst() { std::list<int> some_path = this->path; some_path.push_front(0); return mst_weight_prism(some_path); }
	void bnb_solve() {
		using namespace std;
		this->visited_nodes_count = 0;
		list<int> best_path = this->path;
		int best_score = this->get_score(this->path);
		Node start_node;
		int i = 0;
		generate_n(inserter(start_node.unvisited, start_node.unvisited.end()), dimension - 1, [&] {return ++i; });
		//start_node.lower_bound = mst_weight_prism(list<int>(start_node.unvisited.begin(), start_node.unvisited.end()));

		start_node.lower_bound = prism(start_node.unvisited);
		//stack<Node> node_queue;
		//node_queue.push(start_node);
		list<Node> node_queue;
		node_queue.push_back(start_node);
		while (!node_queue.empty()) {

			//Node front_node = node_queue.top();
			auto min = min_element(node_queue.begin(), node_queue.end(), [](Node a, Node b) {return a.lower_bound / a.path.size() < b.lower_bound / b.path.size(); });
			//node_queue.pop();

			Node front_node = *min;
			node_queue.erase(min);
			if (front_node.lower_bound >= best_score) continue;
			//branch
			for (auto it = front_node.unvisited.begin(); it != front_node.unvisited.end(); ++it) {
				Node new_node;
				new_node.path = front_node.path;
				new_node.unvisited = front_node.unvisited;
				new_node.path.push_back(*it);
				new_node.unvisited.erase(*it);
				//new_node.lower_bound = get_score(new_node.path) + mst_weight_prism(list<int>(new_node.unvisited.begin(), new_node.unvisited.end()));
				new_node.lower_bound = partial_score(new_node.path) + prism(new_node.unvisited);
				//node_queue.push(new_node);
				node_queue.push_back(new_node);
			}
			if (front_node.path.size() == dimension - 1) {
				int score = get_score(front_node.path);
				this->visited_nodes_count++;
				if (score < best_score) {
					best_path = front_node.path;
					best_score = score;
					this->print_path(best_path);
				}
			}
		}
		this->best_path = best_path;
		this->best_path_score = get_score(best_path);
		this->print_path(best_path);

	}
	void bnb_foo() {
		using namespace std;
		// upper bound
		this->heuristic_solve();
		list<int> best_path = this->path;
		int best_score = get_score(this->path);
		this->visited_nodes_count = 0;
		Node start_node;
		for (int i = 1; i < dimension; ++i) start_node.unvisited.insert(i);
		//start_node.lower_bound = mst_weight_prism(list<int>(start_node.unvisited.begin(), start_node.unvisited.end()));

		start_node.lower_bound = prism(start_node.unvisited);
		list<Node> stack;
		stack.push_back(start_node);
		while (!stack.empty()) {
			Node some_node = stack.back();
			stack.pop_back();
			// full?
			if (some_node.path.size() != dimension - 1)
			{
				// branch
				for (auto&& i : some_node.unvisited) {
					Node new_node;
					new_node.path = some_node.path;
					new_node.path.push_back(i);
					new_node.unvisited = some_node.unvisited;
					new_node.unvisited.erase(i);
					int p = new_node.path.size();
					int u = new_node.unvisited.size();
					
					new_node.lower_bound = get_score(new_node.path) + prism(new_node.unvisited);

					//new_node.lower_bound = get_score(new_node.path) + mst_weight_prism(list<int>(new_node.unvisited.begin(), new_node.unvisited.end()));
					stack.push_back(new_node);
				}
			}
			else{
				int score = get_score(some_node.path);
				if (score < best_score) {
					best_score = score;
					best_path = some_node.path;
				}
				this->visited_nodes_count++;
			}
			remove_if(stack.begin(), stack.end(), [&](Node n) {
				return n.lower_bound >= best_score; });
			
		}
		this->best_path = best_path;
		this->best_path_score = best_score;;
		this->print_path(best_path);

	}

private:
	std::vector<int> matrix;
	int visited_nodes_count;
	int dimension = 0;
	std::map < std::string , std::string > metadata;
	std::list<int> path;
	std::list<int> best_path;
	int best_path_score = INT_MAX;
	int get_edge_weight(int v1, int v2) const;
	int get_index(int v1, int v2) const;
	void brute_force_solve(std::list<int> path, std::set<int> unvisited);
	int get_score(const std::list<int> & path);
	int partial_score(const std::list<int>& path);
	int mst_weight_prism(std::list<int> vertices) {
		if (vertices.size() < 2) return 0;
		std::list<int> visited;
		int acc = 0;
		visited.push_back(vertices.front());
		vertices.pop_front();
		/*
		while (!vertices.empty()) {
			int min_edge_weight = INT_MAX;
			int next_vertex_added;
			for (auto&& next : visited) {
				std::list<int>::iterator min = std::min_element(vertices.begin(), vertices.end(), \
					[=](int a, int b) {return get_edge_weight(next, a) < get_edge_weight(next, b); });
				int score = get_edge_weight(next, *min);
				if (score < min_edge_weight) {
					next_vertex_added = *min;
					min_edge_weight = score;
				}

			}
			vertices.erase(find(vertices.begin(), vertices.end(), next_vertex_added));
			acc += min_edge_weight;

			visited.push_back(next_vertex_added);
		}
		*/

		return acc;
	}
	int prism(std::set<int> unvisited) {
		if (unvisited.size() < 1) return 0;
		std::set<int> visited;
		auto it = unvisited.begin();
		visited.insert(*it);
		unvisited.erase(it);
		int acc = 0;
		while (!unvisited.empty()) {
			int min_edge = INT_MAX;
			int min_visited_v;
			int min_unvisited_v;
			for (auto&& v : visited) {
				for (auto&& u : unvisited) {
					int weight = get_edge_weight(v, u);
					if (weight < min_edge) {
						min_edge = weight;
						min_visited_v = v;
						min_unvisited_v = u;
					}
				}
			}
			unvisited.erase(min_unvisited_v);
			visited.insert(min_unvisited_v);
			acc += min_edge;
		}
		return acc;
	}
};

TSPSolution::TSPSolution()
{
}


TSPSolution::~TSPSolution()
{
}

inline void TSPSolution::print_path(const std::list<int> & path) {
	using namespace std;
	for (auto it = path.begin(); it != path.end(); ++it) {
		cout << *it << ' ';
	}
	cout << endl << "Score: " << this->get_score(path) << endl;
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
	this->dimension = dim;
	copy_n(is_it, val_cnt, back_inserter(matrix));
}

inline void TSPSolution::heuristic_solve()
{
	using namespace std;
	this->path.clear();
	//this->best_path.clear();
	best_path_score = INT_MAX;
	list<int> unassigned_vertices;
	generate_n(back_inserter(unassigned_vertices), stoi(metadata.at("DIMENSION")) - 1, [n = 1]() mutable {return n++; });
	int last_vertex = 0;
	while (!unassigned_vertices.empty())
	{
		auto it = min_element(unassigned_vertices.begin(), unassigned_vertices.end(), [&](int a, int b) { 
			return get_edge_weight(last_vertex, a) < get_edge_weight(last_vertex, b); });
		this->path.push_back(*it);
		last_vertex = *it;
		unassigned_vertices.erase(it);
	}
	print_path(this->path);
}

inline std::list<int> two_opt_swap(std::list<int> path, int i, int k) {
	using namespace std;
	auto start = path.begin();
	auto ith = start;
	advance(ith, i);
	auto kth = start;
	advance(kth, k);
	reverse(ith, kth);
	return path;
}

inline std::list<int> TSPSolution::two_opt_solve() {
	using namespace std;
	bool improved = false;
start:
	do {
		improved = false;
		int best_distance = this->get_score(path);
		int eligible = path.size();
		for (int i = 0; i <= eligible; ++i) {
			for (int k = i + 1; k <= eligible; ++k)
			{
				list<int> new_path = two_opt_swap(path, i, k);
				int new_distance = this->get_score(new_path);
				if (new_distance < best_distance) {
					improved = true;
					this->path = new_path;
					best_distance = new_distance;
					print_path(this->path);
					goto start;
				}
			}
		}
	} while (improved);
	return this->path;
}

inline void TSPSolution::brute_force_solve() {
	using namespace std;
	this->path.clear();
	this->best_path.clear();
	this->visited_nodes_count = 0;
	best_path_score = INT_MAX;
	set<int> unvisited;
	for (int i = 1; i < dimension; ++i) unvisited.insert(i);
#pragma omp parallel for
	for (int i = 1; i < dimension; ++i)
	{
		list<int> some_path = path;
		some_path.push_back(i);
		set<int> some_unvisited = unvisited;
		some_unvisited.erase(i);
		brute_force_solve(some_path, some_unvisited);
	}
	this->print_path(best_path);

}

inline void TSPSolution::brute_force_solve(std::list<int> path, std::set<int> unvisited)
{
	if (!unvisited.empty())
	{
		for (int v : unvisited) {
			std::set<int> next_unvisited = unvisited;
			next_unvisited.erase(v);
			std::list<int> next_path = path;
			next_path.push_back(v);
			brute_force_solve(next_path, next_unvisited);
		}
	}
	else {
		int score = get_score(path);
		this->visited_nodes_count++;

#pragma omp critical
		if (best_path_score > score)
		{
			best_path = path;
			best_path_score = score;
			//std::cout << "MST WEIGHT: " << mst_weight_prism(path) << std::endl;
			//print_path(path);
		}
	}

}
/*
inline void TSPSolution::brute_force_solve(std::list<int> path, std::set<int> unvisited)
{
	using namespace std;
	int last_vertice = 0;
	if (!path.empty()) unvisited.erase(path.back());

	if (path.size() == stoi(metadata.at("DIMENSION")) - 1) {
		int path_score = get_score(path);
		if (path_score <= best_path_score) { 
			best_path = list<int>(path);
			best_path_score = path_score;
			cout << "MST WEIGHT: " << mst_weight_prism(best_path) << endl;
		}
	}
	else {
		for_each(unvisited.begin(), unvisited.end(), [&](int i) {
			path.push_back(i);
			unvisited.erase(i);
			brute_force_solve(path, unvisited);
			path.pop_back();
			unvisited.insert(i);

			});
	}

	if (!path.empty()) unvisited.insert(path.back());
}*/
inline int TSPSolution::get_score(const std::list<int> &path)
{
	auto it = path.cbegin();
	int acc = 0;
	int previous_vertex = 0;
	while (it != path.cend())
	{
		acc += get_edge_weight(previous_vertex, *it);
		previous_vertex = *it;
		it++;
	}
	acc += get_edge_weight(previous_vertex, 0);
	return acc;
}
inline int TSPSolution::partial_score(const std::list<int>& path)
{
	auto it = path.cbegin();
	int acc = 0;
	int previous_vertex = 0;
	while (it != path.cend())
	{
		acc += this->get_edge_weight(previous_vertex, *it);
		previous_vertex = *it;
		it++;
	}
	return acc;
}


inline int TSPSolution::get_score()
{
	return get_score(this->best_path);
}
