# Hypercore Decomposition for Non-Fragile Hyperedges

Source code for the paper **Hypercore Decomposition for Non-Fragile Hyperedges: Concepts, Algorithms, Observations, and Applications**, where we propose a novel substructure model for hypergraphs, called the *(k,t)*-hypercore, together with some related concepts.
Using them, we investigate real-world hypergraphs from diverse perspectives and discover interesting structural patterns.
We also demonstrate the usefulness of the proposed concepts by their successful applications to influential node identification, dense substructure discovery, and vulnerability detection in hypergraphs.

Note: if a preview of the supplementary materials PDF file does not appear properly, please download the file.

## Requirements
All python packages required for this repo are included in the *requirements* text file. Please run the following to install them.

    pip install -r requirements.txt

## Datasets

Please download the datasets from [https://www.cs.cornell.edu/~arb/data/](https://www.cs.cornell.edu/~arb/data/) and put the tar.gz files in the "data/raw_data" subfolder. Then untar all the compressed files (and delete the compressed files).
For example, for the *coauth-DBLP* dataset, download "coauth-DBLP.tar.gz" at [https://www.cs.cornell.edu/~arb/data/coauth-DBLP/](https://www.cs.cornell.edu/~arb/data/coauth-DBLP/). 
Note that we do **NOT** use the "FULL" version. The final structure would be like:

	raw_data
	┣ coauth-DBLP
	┃ ┣ DATA-DESCRIPTION.txt
	┃ ┣ README.txt
	┃ ┣ coauth-DBLP-node-labels.txt
	┃ ┣ coauth-DBLP-nverts.txt
	┃ ┣ coauth-DBLP-simplices.txt
	┃ ┗ coauth-DBLP-times.txt
	...

## Code
We divide the code into different parts according to the "topics". Please run the parts in order. The production of the plots is also included in the code.

### 0: Data Preprocessing
We filter parallel hyperedges and singleton hyperedges. We also print out the basic statistics for each dataset.

### 1: Computation
For each dataset and each possible *t* value, we compute the *t*-hypercoreness for each node.

### 2: Hypercore Sizes
For each dataset and each *(k, t)* pair, we compute the size of the *(k, t)*-hypercore as well as the hypercore-size-mean-difference (HSMD) distance between each pair of datasets. We also measure the similarity w.r.t the HSDM distance among the datasets.

### 3: Distributions
For each dataset, we compute the detailed distributions of the *t*-hypercoreness sequence for each possible *t* value. We also operate tests on heavy-tailed distributions.

### 4: Correlations
For each dataset, we compute the Pearson correlation coefficients between *t*-hypercoreness with each possible *t* value and degree, coreness in unweighted and weighted clique expansions. We also compute the Pearson's r for each pair of *t*-hypercoreness sequences. The corresponding plots are also produced.

### 5: Information Gain
For each dataset and each possible *t* value, we compute the information gain of the *t*-hypercoreness sequence over the degree sequence. We also compute the Pearson's r between the information gain sequences of each pair of datasets.

### 6: Influence
We extract the largest connect component in each dataset and simulate the *hyperSIR* process. Please do the following to do the simulation:

	# make sure that you have finished the Jupyter notebook
	# copy the template folder for each independent experiment and enter the new folder
	cp -r template [experiment_name]; cd [experiment_name]
	# make the script executable
	chmod +x start
	# run the script
	# [dataset_index]
	# 0/1: coauth-DBLP/Geology; 2/3: NDC-classes/substances; 4/5: contact-high/primary; 6/7: email-Enron/Eu;
	# 8/9/10: tags-ubuntu/math/SO; 11/12/13: threads-ubuntu/math/SO
	./start -d [dataset_index] -b [beta] -g [gamma] -r [reps_for_each_node] -c [num_of_CPU_cores_to_use]
	# an example (use 20 CPU cores on the email-Eu dataset with beta=0.01 and gamma=1.0; 10000 times for each node)
	# ./start -d 7 -b 0.01 -g 1 -r 10000 -c 20

After the simulation finishes, the script will automatically analyze the results and produce the plots.

### 7: density
For each dataset and each possible *t* value, we compute the density of the non-empty *(k, t)*-hypercore with maximum *k* value as well as the relative-density-mean-difference (RDMD) distance between each pair of datasets. We also consider the generalized vertex cover problem.

### 8: minimization
We consider the **collpased *(k,t)-hypercore problem*** to detect the vulnerabilities in hypergraphs. Please do the following to run the experiments:

	# complile the c++ files
	g++ -O3 -std=c++11 main.cpp -o run
	# run the compiled file
	# [dataset_index]
	# 0/1: coauth-DBLP/Geology; 2/3: NDC-classes/substances; 4/5: contact-high/primary; 6/7: email-Enron/Eu;
	# 8/9/10: tags-ubuntu/math/SO; 11/12/13: threads-ubuntu/math/SO
	# [method_index]
	# 0: hyperCKC; 1: HyCoM; 2: HyCoM+
	./run [dataset_index] [method_index] [k] [t] [b] [n_c]
	# an example (run HyCoM+ with n_c = 1 on the tags-SO dataset to minimize the (10, 0.6)-hypercore with budget b = 100)
	# ./run 10 2 10 0.6 100 1
 
The output of each trial would be *b* lines, where each line consists of *(1) time used in this round*, *(2) the chosen collapser*, *(3) the number of followers*.
