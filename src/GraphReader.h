#ifndef GRAPHREADER_H_
#define GRAPHREADER_H_

#include "Graph.h"

namespace tdenum {

class GraphReader {
public:
	/**
	 * Reads a graph from the specified file. Supported options:
	 * 1. DIMACS format
	 * 2. a Markov network in the UAI format
	 * 3. First line is expected to have the number of nodes and then the number
	 * of cliques. The next lines represent cliques, start with the number of
	 * nodes in these cliques, and then specify their names. The names are
	 * assumed to start from zero. All values are separated by spaces.
	 * 4. CSV files. Each line containing names of nodes in a clique, seperated
	 * by commas.
	 * 5. bliss files. May begin with comments, lines starting with "c".
	 * Then starts with a line "p edge <number-of-nodes> <number-of-edges>".
	 * Other lines specify edges "e <node> <another-node>".
	 * Where all nodes are numbers between 1 and <number-of-nodes>.
	 */
	static Graph read(const string& fileName);
};

} /* namespace tdenum */

#endif /* GRAPHREADER_H_ */
