#include <boost/functional/hash.hpp>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set> 

using namespace std;

extern unordered_map<int,int> node2index;
extern unordered_map<int,double> node2infection;
extern vector<int> index2node, edge_sizes, degrees, nodes_cc;
extern vector< vector<int> > edges, incident;
