#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <boost/functional/hash.hpp>

#include "utils.h"
#include "read_data.cpp"
#include "hypersir.cpp"

#define MAXNODES 10000000

using namespace std;

unordered_map<int,int> node2index;
unordered_map<int,double> node2infection;
vector<int> index2node, edge_sizes, degrees, nodes_cc;
vector< vector<int> > edges, incident;

int main(int argc, char *argv[])
{
	clock_t start_time;

	// Configuration
	/*
	 * Dataset Index
	 * 0: DAWN / 1: NDC-classes / 2: NDC-substances / 3: coauth-DBLP / 4: coauth-MAG-Geology /
	 * 5: coauth-MAG-History / 6: congress-biils / 7: contact-high-school / 8: contact-primary-school /
	 * 9: email-Enron / 10: email-Eu / 11: tags-ask-ubuntu / 12: tags-math-sx / 13: tags-stack-overflow /
	 * 14: threads-ask-ubuntu / 15: threads-math-sx / 16: threads-stack-overflow
	 */
	int dataset_index = stoi(argv[1]);
	double beta = stod(argv[2]);
	double gamma = stod(argv[3]);
	int seed_node = stoi(argv[4]);
	int rep = stoi(argv[5]);

	// Read data
	start_time = clock();

	read_data(dataset_index);

	int num_nodes = node2index.size();
	int num_edges = edges.size();

	// HyperSIR
	start_time = clock();
	seed_node = nodes_cc[seed_node];
	hypersir(beta, gamma, seed_node, rep);

	double runtime_method =  (double)(clock() - start_time) / CLOCKS_PER_SEC;

	// cout << runtime_method << endl;

	for (const int &u: nodes_cc){
		if (node2infection[u] > 1)
		cout << index2node[u] << " " << node2infection[u] << endl;
	}

	return 0;
}
