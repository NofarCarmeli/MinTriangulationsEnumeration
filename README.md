# TreeDecompositionEnumeration

Implements enumeration of all minimal triangulations of a graph as described in the paper [On the Enumeration of all Minimal Triangulations](http://arxiv.org/abs/1604.02833).

## Input

Specify input file as the first argument, and possibly other configuration options using command line arguments. e.g:
```
./TreeDecompositionEnumeration C:\Graph\1.uai time_limit=30 alg=fill
```
Only the input file is required.

### input_file
Supported file formats are: hg, sp, wcnf, uai, csv, bliss.
* csv - each line holds name of variables in a clique.

### Time Limit
Flag name: time_limit.
Time in seconds after which no more results are extended. The current one will finish extending, and all results that were found but not extended yet will be printed as well.
-1 means no time limit.
Default is no time limit.

### Triangualtion Heuristic
Flag name: alg.
The heuristic used for generating a single minimal triangulation.
Options are: mcs, degree, fill. Default is mcs.
* msc - using MCS-M algorithm.
* lb - using LB-Triang algorithm with no heuristic for the order of vertices (faster than the following two).
* degree - using LB-Triang algorithm with min-degree heuristic for the order of vertices.
* initalDegree - using LB-Triang algorithm, determining the order of vertices up-front by the min-degree.
* fill - using LB-Triang algorithm with min-fill heuristic for the order of vertices.
* initalFill - using LB-Triang algorithm, determining the order of vertices up-front by the min-fill.
* combined - runs msc and fill alternatively.

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

### Detailed Output
Flag name: print.
The results to print to a details file.
Options are: all, none, improved. Default is none.
* none - does not generate a details file.
* all - prints all results.
* improved - prints only results that show improvement in one of the measures printed.

## Output

A summary of the results is printed to the screen and saved to a table in summary.csv in the current directory.
Depending on the value of the Print flag, The full list of triangulations may be saved to a csv file in the current directory, with the name of the input file followed by the configuration options.
