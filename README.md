# TreeDecompositionEnumeration

Implements enumeration of all minimal triangulations of a graph as described in the paper http://arxiv.org/abs/1604.02833
The full list of triangulations will be saved to a file next to the input file, while a summary is printed to the screen.

Specify input file and configuration using commant line arguments:
./TreeDecompositionEnumeration <input_file> <time_limit> <triangulations_order> <separators_order>
Only the input file is required
Currently the heuristic used for generating a minimal triangulation is MSC-M

1. input_file
Supported file formats are: hg, sp, wcnf, uai, csv.
csv - each line holds name of variables in a clique.

2. time_limit
Time in seconds after which no more results are extended. The current one will finish extending, and all results that where found but not extended yet will be printed too.

3. triangulations_order
The order of extending the generated minimal triangulations.
Options are: width, fill, difference, sepsize, none. Default is none.
width - lowest width first.
fill - lowest number of fill edges first.
difference - lowest number of fill edges that were seen in previously generated triangulations.
sepsize - lowest size of maximal separator size.

4. separators_order
The order of extending the minimal separators.
Options are: size, fill, none. Default is none.
size - lowest size first.
fill - lowest number of pairs in the separator that don't share an edge.
