#ifndef GRAPHREADER_H_
#define GRAPHREADER_H_

#include "Graph.h"

namespace tdenum {

class GraphReader {
public:
	/**
	 * Reads a graph from the standard input. Three supported options:
	 * 1. DIMACS format
	 * 2. a Markov network in the UAI format
	 * 3. First line is expected to have the number of nodes and then the number
	 * of cliques. The next lines represent cliques, start with the number of
	 * nodes in these cliques, and then specify their names. The names are
	 * assumed to start from zero. All values are separated by spaces.
	 */
	static Graph read();
};

} /* namespace tdenum */

#endif /* GRAPHREADER_H_ */
