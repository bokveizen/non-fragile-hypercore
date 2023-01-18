import sys; sys.path.append('../..')
from pathlib import Path
import argparse
import pickle
import matplotlib.pyplot as plt
from tqdm import tqdm
from util import ds_names, step_plot
from fractions import Fraction
from scipy.stats import linregress
import matplotlib

parser = argparse.ArgumentParser()
parser.add_argument('-d', type=int, help='dataset index')
parser.add_argument('-b', type=float, help='beta')
parser.add_argument('-g', type=float, help='gamma')
parser.add_argument('-r', type=int, help='reps')
parser.add_argument('-c', type=int, help='number of CPU cores')
args = parser.parse_args()

dataset_index_list = [3, 4, 1, 2, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16]
# LCC_size_list = [1654109, 898648, 628, 3065, 327, 242, 143, 979, 3021, 1627, 49931, 82075, 152702, 2301070]
dataset_index = dataset_index_list[args.d]
ds_name = ds_names[args.d]

v2influence = dict()

with open('results.txt') as f:
    lines = f.readlines()
for line in lines:
    if line[-1] == '\n':
        line = line[:-1]
    if not line:
        continue
    v, inf_v = line.split(' ')
    v, inf_v = int(v), float(inf_v)
    v2influence[v] = inf_v

nodes = sorted(v2influence)
influence_list = [v2influence[v] for v in nodes]

Path('figures').mkdir(exist_ok=True)
plt.rcParams.update({'font.size': 30, 'text.usetex': False, 'lines.linewidth': 5})

# t-hypercoreness
t2pearsonr = dict()
p_core_ds = Path('../../data/core') / ds_name
for c_t in tqdm(list(p_core_ds.glob('c*.pkl'))):
    t = Fraction(c_t.name[2:-4].replace('-', '/'))
    with c_t.open('rb') as f:
        v2c_t = pickle.load(f)
    c_t_list = [v2c_t[v] for v in nodes]
    plt.scatter(c_t_list, influence_list)
    slope, intercept, r_value, _, _ = linregress(c_t_list, influence_list)
    t2pearsonr[t] = r_value
    r2 = r_value * r_value
    plt.axline((0, intercept), slope=slope, color='r', linestyle='--')
    plt.xlabel('$t$-hypercoreness')
    plt.ylabel('final $|R|$')
    # plt.title('$R^2= %.2f$' % r2, fontsize=fontsize_title, color='r', fontweight='bold')
    plt.title('$t = {}, R^2= {:.2f}$'.format(t, r2))
    plt.savefig('figures/t_{}.png'.format(c_t.name[2:-4]), bbox_inches='tight')
    plt.savefig('figures/t_{}.pdf'.format(c_t.name[2:-4]), bbox_inches='tight')
    plt.clf()

centrality_name2list = dict()
centrality2pearsonr = dict()

# degree
p_incident_ds = Path('../../data/incident') / ds_name
with (p_incident_ds / 'v2edges.pkl').open('rb') as f:
    v2edges = pickle.load(f)
centrality_name2list['degree'] = [len(v2edges[v]) for v in nodes]

# clique expansions
p_CE_ds = Path('../../data/clique_expansion') / ds_name
with (p_CE_ds / 'v2cU.pkl').open('rb') as f:
    v2cU = pickle.load(f)
with (p_CE_ds / 'v2cW.pkl').open('rb') as f:
    v2cW = pickle.load(f)
centrality_name2list['coreness-U'] = [v2cU[v] for v in nodes]
centrality_name2list['coreness-W'] = [v2cW[v] for v in nodes]

# eigen-centralities
p_EC_ds = Path('../../data/eigenvector_centrality') / ds_name
with (p_EC_ds / 'EC_CE_U.pkl').open('rb') as f:
    EC_CE_U = pickle.load(f)
with (p_EC_ds / 'EC_CE_W.pkl').open('rb') as f:
    EC_CE_W = pickle.load(f)
centrality_name2list['eigencentrality-U'] = [EC_CE_U[v] for v in nodes]
centrality_name2list['eigencentrality-W'] = [EC_CE_W[v] for v in nodes]

# hyper-eigen-centralities
centrality_types = ['linear', 'log-exp', 'max']
centrality_types_short = ['hyperEC-L', 'hyperEC-LE', 'hyperEC-M']
for i, centrality_type in enumerate(centrality_types):
    with (p_EC_ds / 'hyperEC_{}_V.pkl'.format(centrality_type)).open('rb') as f:
        centrality_dict_V = pickle.load(f)
    centrality_name2list[centrality_types_short[i]] = [centrality_dict_V[v] for v in nodes]

for centrality_name, centrality_list in centrality_name2list.items():
    plt.scatter(centrality_list, influence_list)
    slope, intercept, r_value, _, _ = linregress(centrality_list, influence_list)
    centrality2pearsonr[centrality_name] = r_value
    r2 = r_value * r_value
    plt.axline((0, intercept), slope=slope, color='r', linestyle='--')
    plt.xlabel(centrality_name)
    plt.ylabel('final $|R|$')
    plt.title('{}, $R^2$ = {:.2f}'.format(centrality_name, r2))
    plt.savefig('figures/{}.png'.format(centrality_name), bbox_inches='tight')
    plt.savefig('figures/{}.pdf'.format(centrality_name), bbox_inches='tight')
    plt.clf()

colors = ['r', 'g', 'c', 'm', 'y', 'k', 'darkorange', 'lime']
markers = ['o', 'v', '^', '<', '>', 's', 'P', 'D']
linestyles = [
    ('solid', 'solid'),  # Same as (0, ()) or '-'
    ('dotted', 'dotted'),  # Same as (0, (1, 1)) or '.'
    ('dashed', 'dashed'),  # Same as '--'
    ('dashdot', 'dashdot')]  # Same as '-.'
linestyles = linestyles + linestyles

centrality_names = ['degree', 'coreness-U', 'coreness-W',
                    'hyperEC-L', 'hyperEC-LE', 'hyperEC-M',
                    'eigencentrality-U', 'eigencentrality-W']

step_plot(t2pearsonr, label='$t$-hypercoreness')
for centrality_name, r in centrality2pearsonr.items():
    plt.axhline(y=r, color=colors.pop(), linestyle=linestyles.pop()[1], label=centrality_name)
plt.xlabel('t')
plt.ylabel('Pearson\'s r')
# plt.subplots_adjust(left=0.2, bottom=0.2)
plt.title(r'$\beta = {beta}$, $\gamma = {gamma}$'.format(beta=args.b, gamma=args.g))
ax = plt.gca()
loc = matplotlib.ticker.MultipleLocator(base=0.2)  # this locator puts ticks at regular intervals
ax.yaxis.set_major_locator(loc)
plt.ylim(top=1.0)
plt.savefig('summary.png', bbox_inches='tight')
plt.savefig('summary.pdf', bbox_inches='tight')
plt.clf()
