#include <boost/functional/hash.hpp>
#include <random>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include "utils.h"

using namespace std;

void hypersir(double beta, double gamma, int seed_node, int rep)
{
	int num_nodes = node2index.size();
	int num_edges = edges.size();

	unordered_set<int> S, I, R;
	unordered_map<int, double> P;
	vector<bool> edge_visited(num_edges);
	vector<int> node_status(num_nodes);

	random_device rd;
	default_random_engine eng(rd());
	uniform_real_distribution<double> dist(0,1);

	vector<int> results;

	clock_t start_time;

	for (int i = 0; i < rep; i++){

		S.clear(); I.clear(); R.clear();
		for (const int &u: nodes_cc) S.insert(u); S.erase(seed_node);
		I.insert(seed_node);

		fill(node_status.begin(), node_status.end(), 0);
		node_status[seed_node] = 1;

		while (I.size()){
			start_time = clock();
			P.clear();
			fill(edge_visited.begin(), edge_visited.end(), false);

			vector<int> incident_edges;

			for (const int &u: I){
				for (const int &edge: incident[u]){
					if (edge_visited[edge]) continue;
					edge_visited[edge] = true;

					bool valid = false;
					for (const int &v: edges[edge]){
						if (node_status[v] == 0){
							valid = true; break;
						}
					}
					if (not valid) continue;

					int I_e = 0, S_e = 0;
					for (const int &v: edges[edge]){
						if (node_status[v] == 0) S_e++;
						else if (node_status[v] == 1) I_e++;
					}

					for (const int &v: edges[edge]){
						if (node_status[v] == 0){
							if (P.find(v) == P.end()) P[v] = 1.0;
							P[v] = P[v] * (1.0 - 2.0 * beta * I_e / edge_sizes[edge]);
						}
					}
				}
			}

			if (gamma == 1.0){
				for (const int &u: I){
					R.insert(u);
					node_status[u] = 2;
				}
				I.clear();
			} else{
				vector<int> I2R;
				for (const int &u: I){
					if (dist(eng) < gamma){
						I2R.push_back(u);
						R.insert(u);
						node_status[u] = 2;
					}
				}
				for (const int &u: I2R) I.erase(u);
			}

			for (const auto &p: P){
				int u = p.first; double prob = p.second;
				if (dist(eng) < 1.0 - prob){
					S.erase(u);
					I.insert(u);
					node_status[u] = 1;
				}
			}

		}
		// cout << R.size() << endl;
		results.push_back(R.size());
	}

	int cnt = 0;
	for (const int &x: results) cnt += x;
	// cout << (double)cnt / results.size() << endl;
	node2infection[seed_node] = (double)cnt / results.size();
}

