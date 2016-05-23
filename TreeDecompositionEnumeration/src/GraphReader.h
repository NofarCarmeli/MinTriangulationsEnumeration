#ifndef GRAPHREADER_H_
#define GRAPHREADER_H_

#include "Graph.h"

namespace tdenum {

class GraphReader {
public:
	/**
	 * Reads a graph from the standard input.
	 * First line is expected to have the number of nodes and then the number of
	 * cliques. The next lines represent cliques, and specify the names of the
	 * nodes in these cliques. The names are assumed to start from zero.
	 * All values are separated by spaces.
	 */
	static Graph read();
	/**
	 * Reads a graph specified in the DIMACS format.
	 */
	static Graph readCnf(bool isWeighted);
};

} /* namespace tdenum */

#endif /* GRAPHREADER_H_ */
