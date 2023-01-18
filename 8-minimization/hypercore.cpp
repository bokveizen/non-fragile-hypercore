#include <boost/functional/hash.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include "utils.h"

//#define MAXNODES 10000000
//#define MAXEDGES 100000000

using namespace std;

void hypercore_decomposition()
{
	int num_nodes = node2index.size();
	int num_edges = edges.size();

	// Hypercore decomposition
	edge_removed.resize(num_edges, 0);
	edge_status.resize(num_edges, 2);

	for (int i = 0; i < num_nodes; i++){
		if (degrees[i] < k){
			nodes_to_remove.push_back(i);
			node_status.push_back(2);
		} else{
			node_status.push_back(1);
		}
	}

	for (int i = 0; i < nodes_to_remove.size(); i++){
		int idx = nodes_to_remove[i];
		node_status[idx] = 0; // Expired
		degrees[idx] = 0;
		for (int j = 0; j < incident[idx].size(); j++){
			int edge = incident[idx][j];
			if (edge_status[edge] == 0) continue;
			edge_removed[edge]++;
			if ((double)(edge_sizes[edge] - edge_removed[edge]) < max(m * edge_sizes[edge], 2.0)){
				for (const int &_idx: edges[edge]){
					if (node_status[_idx] == 1){
						degrees[_idx]--;
						if (degrees[_idx] < k){
							nodes_to_remove.push_back(_idx);
							node_status[_idx] = 2;
						}
					}
				}
				edge_status[edge] = 0; // Expired
				edge_removed[edge] = edge_sizes[edge];
			}
		}
	}

	for (int i = 0; i < num_nodes; i++){
		if (node_status[i] != 0)
			core_nodes.push_back(i);
	}
	for (int i = 0; i < num_edges; i++){
		if (edge_status[i] != 0)
			core_edges.push_back(i);
	}
}

