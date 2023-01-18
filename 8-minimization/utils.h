#include <boost/functional/hash.hpp>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set> 

using namespace std;

extern int k, b, n;
extern double m;
extern unordered_map<int,int> node2index;
extern vector<int> index2node, edge_sizes, degrees, core_nodes, core_edges, nodes_to_remove, node_status, edge_removed, edge_status, node_status_pre;
extern vector< vector<int> > edges, incident;
extern map<int, set<int> > F;
extern unordered_map<int, unordered_set<int> > F_adj;
extern unordered_map<int, int> F_adj_cnt;
//extern unordered_map<pair<int, int>, int, boost::hash<pair<int, int> > > E1;
extern unordered_map<long long, int> E1;
extern unordered_map<int, unordered_map<int, int> > E1_adj;
extern unordered_map<pair<int, int>, vector<int>, boost::hash<pair<int, int> > > E2;
extern unordered_map<int, unordered_set<int> > E3;
