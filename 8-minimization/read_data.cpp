#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#include "utils.h"

#define MAXNODES 10000000
#define MAXEDGES 1000000000

using namespace std;

//vector<string> dataset_names {"DAWN", "NDC-classes", "NDC-substances", "coauth-DBLP", "coauth-MAG-Geology", "coauth-MAG-History", "congress-bills", "contact-high-school", "contact-primary-school", "email-Enron", "email-Eu", "tags-ask-ubuntu", "tags-math-sx", "tags-stack-overflow", "threads-ask-ubuntu", "threads-math-sx", "threads-stack-overflow", "tags-stack-overflow-1", "tags-stack-overflow-2", "tags-stack-overflow-4", "tags-stack-overflow-8", "tags-stack-overflow-16", "tags-stack-overflow-32", "tags-stack-overflow-64", "tags-stack-overflow-128", "tags-stack-overflow-256"};
vector<string> dataset_names {
	"coauth-DBLP", "coauth-MAG-Geology",
	"NDC-classes", "NDC-substances",
	"contact-high-school", "contact-primary-school",
	"email-Enron", "email-Eu",
	"tags-ask-ubuntu", "tags-math-sx", "tags-stack-overflow",
	"threads-ask-ubuntu", "threads-math-sx", "threads-stack-overflow"};
//"tags-stack-overflow-1", "tags-stack-overflow-2", "tags-stack-overflow-4",
//"tags-stack-overflow-8", "tags-stack-overflow-16", "tags-stack-overflow-32",
//"tags-stack-overflow-64", "tags-stack-overflow-128", "tags-stack-overflow-256"};

vector<string> split(string str, char delimiter)
{
	vector<string> internal;
	stringstream ss(str);
	string temp;
	while (getline(ss, temp, delimiter)){
		internal.push_back(temp);
	}
	return internal;
}

void read_data(int dataset_index)
{
	string line;
	vector<int> nodes;
	degrees.resize(MAXNODES, -1);

	string dataset_name = dataset_names[dataset_index];
//	cout << dataset_name << endl << endl;

	string edge_filename = "../data_cpp/" + dataset_name + "_edges.txt";
	ifstream edge_file(edge_filename.c_str());
	while (getline(edge_file, line)){
		vector<string> terms = split(line, '\t');
		nodes.clear();
		for (int i = 0; i < terms.size(); i++){
			int node = stoi(terms[i]);
			if (node2index.find(node) == node2index.end()){
				node2index[node] = node2index.size();
				index2node.push_back(node);
			}
			nodes.push_back(node2index[node]);
		}
		edges.push_back(nodes);
	}

	for (int i = 0; i < edges.size(); i++){
		edge_sizes.push_back(edges[i].size());
	}

	string degree_filename = "../data_cpp/" + dataset_name + "_degrees.txt";
	ifstream degree_file(degree_filename.c_str());
	while (getline(degree_file, line)){
		vector<string> terms = split(line, '\t');
		int node = stoi(terms[0]);
		int deg = stoi(terms[1]);
		degrees[node2index[node]] = deg;
	}

	string incident_filename = "../data_cpp/" + dataset_name + "_incident.txt";
	ifstream incident_file(incident_filename.c_str());
	while (getline(incident_file, line)){
		vector<string> terms = split(line, '\t');
		int node = stoi(terms[0]);
		vector<string> incident_edges = split(terms[1], ',');
		while (node2index[node] >= incident.size()){
			incident.push_back(vector<int>());
		}
		for (int i = 0; i < incident_edges.size(); i++){
			int edge = stoi(incident_edges[i]);
			incident[node2index[node]].push_back(edge);
		}
	}
}
