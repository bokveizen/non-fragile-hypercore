#include <boost/functional/hash.hpp>
#include <ctime>
#include <cmath>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

#include "utils.h"

#define MAXNODES 10000000
//#define MAXEDGES 100000000

using namespace std;

vector<int> T;
map<int, set<int> > F;
set<int> removed_nodes, removed_edges, dangerous_edges;

inline long long convert_id_hycom(int x, int y)
{
	return x * (1LL << 31) + y;
}

bool F_compare_hycom(int u, int v)
{
	if (F[u].size() != F[v].size())
	    return F[u].size() > F[v].size();
	if (degrees[u] != degrees[v])
	    return degrees[u] > degrees[v];
	return u > v;
}

void keep_memory_hycom(unordered_map<int, tuple<int, int, int> >& memory, int idx, int item1, int item2)
{
	if (not get<0>(memory[idx])){
		memory[idx] = {1, item1, item2};
	}
}

void initialize_candidates_hycom()
{
	int num_nodes = node2index.size();
	int num_edges = edges.size();

	int edge, node_u, node_v, u, v, cnt;
	vector<bool> node_visited(num_nodes, false);

	for (int i = 0; i < core_edges.size(); i++){
		edge = core_edges[i];
		if (not edge_status[edge]) continue;
		if (edge_sizes[edge] - edge_removed[edge] == max((int)ceil(m * (double)edge_sizes[edge]), 2)){
			edge_status[edge] = 1;
			for (int _j = 0; _j < edges[edge].size(); _j++){
				node_u = edges[edge][_j];
				if (not node_status[node_u]) continue;
				for (int _k = _j+1; _k < edges[edge].size(); _k++){
					node_v = edges[edge][_k];
					if (not node_status[node_v]) continue;
					E1[convert_id_hycom(min(node_u,node_v), max(node_u,node_v))]++;
				}
			}
		}
	}

	for (const auto &uv: E1){
		int u = (int)(uv.first >> 31), v = (int)(uv.first & 0x7FFFFFFFLL), cnt = uv.second;
		if (cnt > degrees[u] - k){
			if (not node_visited[v]) {
				node_visited[v] = true; 
				T.push_back(v);
			}
			F[v].insert(u);
		}
		if (cnt > degrees[v] - k){
			if (not node_visited[u]) {
				node_visited[u] = true; 
				T.push_back(u);
			}
			F[u].insert(v);

		}
	}
}

void update_candidates_hycom()
{
	int u, v;
	long long uv_id;

	// Update dangerous edges
	for (const auto &edge: dangerous_edges){
		for (int _j = 0; _j < edges[edge].size(); _j++){
			u = edges[edge][_j];
			if (not node_status[u]) continue;
			for (int _k = _j+1; _k < edges[edge].size(); _k++){
				v = edges[edge][_k];
				if (not node_status[v]) continue;
				uv_id = convert_id_hycom(min(u,v), max(u,v));
				E1[uv_id]++;
			}
		}
	}

	// Update removed edges
	for (const auto &edge: removed_edges){
		for (int _j = 0; _j < edges[edge].size(); _j++){
			u = edges[edge][_j];
			if (not node_status_pre[u]) continue;
			for (int _k = _j+1; _k < edges[edge].size(); _k++){
				v = edges[edge][_k];
				if (not node_status_pre[v]) continue;
				uv_id = convert_id_hycom(min(u,v), max(u,v));
				E1[uv_id]--;
				if (E1[uv_id] == 0) E1.erase(uv_id);
			}
		}
	}

	int num_nodes = node2index.size();
	vector<bool> node_visited(num_nodes, false);

	T.clear(); F.clear();

	for (const auto &uv: E1){
		int u = (int)(uv.first >> 31), v = (int)(uv.first & 0x7FFFFFFFLL), cnt = uv.second;
		if (cnt > degrees[u] - k){
			if (not node_visited[v]){
				node_visited[v] = true;
				T.push_back(v);
			}
			F[v].insert(u);
		}
		if (cnt > degrees[v] - k){
			if (not node_visited[u]){
				node_visited[u] = true;
				T.push_back(u);
			}
			F[u].insert(v);
		}
	}
}

int compute_followers_hycom(int u, vector<bool>& is_follower, bool simulation=true)
{
	unordered_map<int, tuple<int, int, int> > node_memory, edge_memory;

	if (not simulation){
		removed_nodes.clear();
		removed_edges.clear();
		dangerous_edges.clear();
	}

	nodes_to_remove.clear();
	nodes_to_remove.push_back(u);
	vector<int> org_edge_status = edge_status;
	
	for (int i = 0; i < nodes_to_remove.size(); i++){
		int idx = nodes_to_remove[i];
		// Keep node info
		keep_memory_hycom(node_memory, idx, node_status[idx], degrees[idx]);
		node_status[idx] = 0;
		degrees[idx] = 0;
		//if (not simulation) removed_nodes.insert(idx);
		removed_nodes.insert(idx);

		for (int j = 0; j < incident[idx].size(); j++){
			int edge = incident[idx][j];
			if (edge_status[edge] == 0) continue;
			// Keep edge info
			keep_memory_hycom(edge_memory, edge, edge_status[edge], edge_removed[edge]);
			edge_removed[edge]++;
			if ((edge_sizes[edge] - edge_removed[edge]) < max((int)ceil(m * edge_sizes[edge]), 2)){
				for (const int &_idx: edges[edge]){
					if (node_status[_idx] == 1){
						// Keep node info
						keep_memory_hycom(node_memory, _idx, node_status[_idx], degrees[_idx]);
						degrees[_idx]--;
						if (degrees[_idx] < k){
							nodes_to_remove.push_back(_idx);
							node_status[_idx] = 2;
							if (_idx != u) is_follower[_idx] = true;
						}
					}
				}
				if (org_edge_status[edge] == 1)
					removed_edges.insert(edge);
				dangerous_edges.erase(edge);
				edge_status[edge] = 0;
				edge_removed[edge] = edge_sizes[edge];
			} 
			
			if (edge_sizes[edge] - edge_removed[edge] == max((int)ceil(m * edge_sizes[edge]), 2)){
				edge_status[edge] = 1;
				dangerous_edges.insert(edge);
			}
		}
	}

	// Count the followers
	int num_followers = 0;
	for (const auto &memory: node_memory){
		int node = memory.first;
		if (node_status[node] == 0 and node != u)
			num_followers++;
	}

	// Recover the memory
	if (simulation){
		for (const auto &memory: node_memory){
			int node = memory.first;
			node_status[node] = get<1>(node_memory[node]);
			degrees[node] = get<2>(node_memory[node]);
		}
		for (const auto &memory: edge_memory){
			int edge = memory.first;
			edge_status[edge] = get<1>(edge_memory[edge]);
			edge_removed[edge] = get<2>(edge_memory[edge]);
		}
	} 

	return num_followers;
}

int best_collapser_hycom()
{
	if (n == 1) return *min_element(T.begin(), T.end(), F_compare_hycom);
	int num_nodes = node2index.size();
	int num_edges = edges.size();

	// Sort T
	sort(T.begin(), T.end(), F_compare_hycom);
	
	// Find the best collapser
	int best_node = -1, best_followers = -1;
	vector<bool> is_follower(num_nodes, false);

	int checked_num = 0;
	for (int i = 0; i < T.size(); i++){
		int u = T[i];
		if (is_follower[u]) continue;
		int num_followers = compute_followers_hycom(u, is_follower);
		if (num_followers > best_followers){
			best_followers = num_followers;
			best_node = u;
		}
		checked_num++;
		if (checked_num == n) break;
	}	

	return best_node;
}

vector<int> HyCoM()
{
	clock_t start_time, total_start_time;
	int num_nodes = node2index.size();
	int num_edges = edges.size();

	vector<int> collapsers;
       	vector<bool> visited(num_nodes, false), is_follower(num_nodes, false);

	// Initialize
	initialize_candidates_hycom();
	node_status_pre = node_status;

	// total_start_time = clock();
	for (int round = 0; round < b; round++){
		start_time = clock();
		// Search collapser
		int best_node = best_collapser_hycom();
		if (best_node == -1) continue;
		if (not visited[best_node]){
			visited[best_node] = true;
			collapsers.push_back(best_node);
		}

		// Collapse
        int num_fol = compute_followers_hycom(best_node, is_follower, false);

		// Update T
		update_candidates_hycom();
		node_status_pre = node_status;

		cout << (double)(clock() - start_time) / CLOCKS_PER_SEC << ' ';
		cout << index2node[best_node] << ' ' << num_fol << endl;
	}
	// cout << "collapsers are:";
	// for (auto i: collapsers) cout << index2node[i] << ' ';
	// cout << endl;
	// cout << (double)(clock() - total_start_time) / CLOCKS_PER_SEC << endl;

	return collapsers;
}
