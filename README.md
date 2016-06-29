# TreeDecompositionEnumeration

Implements enumeration of all minimal triangulations of a graph as described in the paper [On the Enumeration of all Minimal Triangulations](http://arxiv.org/abs/1604.02833).

## Input

Specify input file and configuration using command line arguments:
```
./TreeDecompositionEnumeration <input_file> <time_limit> <triangulations_order> <separators_order>
```
Only the input file is required.
Currently the heuristic used for generating a minimal triangulation is MSC-M.

### input_file
Supported file formats are: hg, sp, wcnf, uai, csv.
* csv - each line holds name of variables in a clique.

### time_limit
Time in seconds after which no more results are extended. The current one will finish extending, and all results that where found but not extended yet will be printed as well.
-1 means no time limit.
Default is no time limit.

### triangulations_order
The order of extending the generated minimal triangulations.
Options are: width, fill, difference, sepsize, none. Default is none.
* width - lowest width first.
* fill - lowest number of fill edges first.
* difference - lowest number of fill edges that were seen in previously generated triangulations.
* sepsize - lowest size of maximal separator size.

### separators_order
The order of extending the minimal separators.
Options are: size, fill, none. Default is none.
* size - lowest size first.
* fill - lowest number of pairs in the separator that don't share an edge.

## Output

The full list of triangulations is saved to a file next to the input file, while a summary is printed to the screen.
The output file is \<input_file\>.output.\<triangulations_order\>.\<separators_order\>.csv (or \<input-name\>.output.csv if no order heuristics are specified).
