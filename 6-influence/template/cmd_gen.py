import argparse
from pathlib import Path
import numpy as np

np.random.seed(42)

parser = argparse.ArgumentParser()
parser.add_argument('-d', type=int, help='dataset index')
parser.add_argument('-b', type=float, help='beta')
parser.add_argument('-g', type=float, help='gamma')
parser.add_argument('-r', type=int, help='reps')
parser.add_argument('-c', type=int, help='number of CPU cores')
args = parser.parse_args()

# // Configuration
# /*
#  * Dataset Index
#  * 0: DAWN / 1: NDC-classes / 2: NDC-substances / 3: coauth-DBLP / 4: coauth-MAG-Geology /
#  * 5: coauth-MAG-History / 6: congress-biils / 7: contact-high-school / 8: contact-primary-school /
#  * 9: email-Enron / 10: email-Eu / 11: tags-ask-ubuntu / 12: tags-math-sx / 13: tags-stack-overflow /
#  * 14: threads-ask-ubuntu / 15: threads-math-sx / 16: threads-stack-overflow
#  */

# coauth-DBLP, total = 1831126, LCC = 1654109
# coauth-MAG-Geology, total = 1087111, LCC = 898648
# NDC-classes, total = 1149, LCC = 628
# NDC-substances, total = 3438, LCC = 3065
# contact-high-school, total = 327, LCC = 327
# contact-primary-school, total = 242, LCC = 242
# email-Enron, total = 143, LCC = 143
# email-Eu, total = 979, LCC = 979
# tags-ask-ubuntu, total = 3021 LCC = 3021
# tags-math-sx, total = 1627 LCC = 1627
# tags-stack-overflow, total = 49945 LCC = 49931
# threads-ask-ubuntu, total = 90054 LCC = 82075
# threads-math-sx, total = 153806 LCC = 152702
# threads-stack-overflow, total = 2321751 LCC = 2301070

dataset_index_list = [3, 4, 1, 2, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
LCC_size_list = [1654109, 898648, 628, 3065, 327, 242, 143, 979, 3021, 1627, 49931, 82075, 152702, 2301070]

# LCC_size = 1654109
# file_num = 6000
# core_num = 20

dataset_index = dataset_index_list[args.d]
LCC_size = LCC_size_list[args.d]
core_num = args.c
file_num = int((LCC_size * core_num) ** 0.5)

node_i_list = np.random.permutation(LCC_size)
# node_i_list = np.array_split(node_i_list, 10)[0]  # 10% nodes
# cmd_fmt = './run 3 0.05 1 {node_i} 100 >> results.txt;'
cmd_fmt = './run {ds_i} {beta} {gamma} {node_i} {reps} >> results.txt;'
# int dataset_index = stoi(argv[1]);
# double beta = stod(argv[2]);
# double gamma = stod(argv[3]);
# int seed_node = stoi(argv[4]);
# int rep = stoi(argv[5]);

Path('cmd').mkdir(exist_ok=True)
cmd_list = []
for c in range(file_num):
    cmd = ''
    nodes_c = np.array_split(node_i_list, file_num)[c]
    for ni in nodes_c:
        cmd += cmd_fmt.format(ds_i=dataset_index, beta=args.b, gamma=args.g, node_i=ni, reps=args.r)
    cmd_list.append(cmd)
    with open('cmd/{}.sh'.format(c), 'w+') as f:
        f.write(cmd)

cmd_fmt = 'cmd/{file_i}.sh;'
for c in range(core_num):
    cmd = ''
    files_c = np.array_split(range(file_num), core_num)[c]
    for fi in files_c:
        cmd += cmd_fmt.format(file_i=fi)
    with open('{}.sh'.format(c+1), 'w+') as f:
        f.write(cmd)
