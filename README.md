# MinTriangulationsEnumeration

Implements enumeration of all minimal triangulations of a graph as described in the paper [On the Enumeration of all Minimal Triangulations](http://arxiv.org/abs/1604.02833).

## Compilation
Run:
```
make
```

## Input

Specify the input file as the first argument, and possibly other configuration options using command line arguments. e.g:
```
enumerator.exe example_input_TPCH9.bliss time_limit=1 alg=mcs
```
Only the input file is required.

### Input File
Supported file formats are: hg, sp, wcnf, uai, bliss, and:
* txt - each line holds names of vertices in a clique separated by spaces (specifying an edge in every line also works). A vertex name can be any string without whitespaces.
* csv - the same as txt but the values are separated by commas.

### Output Form
Flag name: out.
Specifies the type of results that the algorithm enumerates.
Options are: triangulations, bags. Default is triangulations.
* triangulations - enumerates the minimal triangulations (a.k.a. chordal completions) of the input graph; each result is a list of fill edges (the edges to be added to the input graph).
* bags - enumerates the equivalence classes of proper tree decompositions by bag sets; each result is a list of bags.

### Time Limit
Flag name: time_limit.
The next result after this specified time will be the last. Time is specified in seconds.
-1 means no time limit.
Default is no time limit.

### Triangualtion Heuristic
Flag name: alg.
The heuristic used for generating a single minimal triangulation.
Options are: mcs, lb, degree, initialDegree, fill, initalFill, combined and separators. Default is mcs.
* mcs - using MCS-M algorithm.
* lb - using LB-Triang algorithm with no heuristic for the order of vertices (faster than the following options).
* degree - using LB-Triang algorithm with min-degree heuristic for the order of vertices.
* initialDegree - using LB-Triang algorithm, determining the order of vertices up-front by the min-degree.
* fill - using LB-Triang algorithm with min-fill heuristic for the order of vertices.
* initalFill - using LB-Triang algorithm, determining the order of vertices up-front by the min-fill.
* combined - runs fill and mcs alternatively in each iteration.
* separators - using a separator based approch instead of a triangualtion, as described in section 5 of the [paper](http://arxiv.org/abs/1604.02833).

### Triangulations Order
Flag name: t_order.
The order of extending the generated minimal triangulations.
Options are: width, fill, difference, sepsize, none. Default is none.
* width - lowest width first.
* fill - lowest number of fill edges first.
* difference - lowest number of fill edges that were seen in previously extended triangulations.
* sepsize - lowest size of maximal separator size.

### Separators Order
Flag name: s_order.
The order of extending the minimal separators.
Options are: size, fill, none. Default is none.
* size - lowest size first.
* fill - lowest number of pairs in the separator that don't share an edge.

## Output

The list of results is stored in an output file with the name of the input file followed by ".out".
A summary of the triangulations found is printed to the screen.
