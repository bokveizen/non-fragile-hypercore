#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <boost/functional/hash.hpp>

#include "utils.h"
#include "read_data.cpp"
#include "hycom.cpp"
#include "hyperckc.cpp"
#include "hypercore.cpp"

#define MAXNODES 10000000
//#define MAXEDGES 1000000000

using namespace std;

int k, b, n = MAXNODES;
double m;
unordered_map<int,int> node2index;
vector<int> index2node, edge_sizes, degrees, core_nodes, core_edges, nodes_to_remove, node_status, edge_removed, edge_status, node_status_pre;
vector< vector<int> > edges, incident;
//unordered_map<pair<int, int>, int, boost::hash<pair<int, int> > > E1;
unordered_map<long long, int> E1;
unordered_map<int, unordered_map<int, int> > E1_adj;
unordered_map<pair<int, int>, vector<int>, boost::hash<pair<int, int> > > E2;
unordered_map<int, unordered_set<int> > E3;

int main(int argc, char *argv[])
{
	clock_t start_time;
	unordered_map<int, string> index2method = {{0, "HyCoM+inf"}, {1, "HyCoM+"}};

	// Configuration
	/*
	 * Dataset Index
	 * 0: DAWN / 1: NDC-classes / 2: NDC-substances / 3: coauth-DBLP / 4: coauth-MAG-Geology /
	 * 5: coauth-MAG-History / 6: congress-biils / 7: contact-high-school / 8: contact-primary-school /
	 * 9: email-Enron / 10: email-Eu / 11: tags-ask-ubuntu / 12: tags-math-sx / 13: tags-stack-overflow /
	 * 14: threads-ask-ubuntu / 15: threads-math-sx / 16: threads-stack-overflow
	 */
	int dataset_index = stoi(argv[1]);
	// Here, the variable "m" is what is called "t" in the paper, "n" <-> "n_c"
	/*
	 * Method Index
	 * 0: hyperCKC  k, t, b
	 * 1: HyCoM     k, t, b, n_c
	 * 2: HyCoM+    k, t, b, n_c
	 */
	int method_index = stoi(argv[2]); 

	k = stoi(argv[3]);
	m = stod(argv[4]);
	b = stoi(argv[5]);
	if (method_index != 0) n = stoi(argv[6]);

	// Read data
	read_data(dataset_index);

	int num_nodes = node2index.size();
	int num_edges = edges.size();

	// Hypercore decomposition
    hypercore_decomposition();


    // Hypercore minimization

	vector<int> collapsers;
    collapsers = (method_index == 2)? HyCoM() : hyperCKC();

	int rem_nodes_cnt = 0;
	for (int i = 0; i < num_nodes; i++){
		if (node_status[i] != 0) rem_nodes_cnt++;
	}

//	// File output
//	string filename = "results/result_" + index2method[method_index];
//	filename += "_" + to_string(dataset_index);
//	filename += "_" + to_string(k);
//	filename += "_" + to_string(m);
//	filename += "_" + to_string(b);
//	if (method_index == 1) filename += "_" + to_string(n);
//	filename += ".txt";
//
//	ofstream resultFile(filename.c_str(), fstream::out | fstream::app);
//
//	resultFile << fixed << runtime_decomp << " " << runtime_method << " " << runtime_decomp + runtime_method << endl;
//	resultFile << core_nodes.size() << " " << rem_nodes_cnt << endl;
//
//	resultFile.close();

	return 0;
}
