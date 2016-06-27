#ifndef MINIMALSEPARATORSENUMERATOR_H_
#define MINIMALSEPARATORSENUMERATOR_H_

#include "Graph.h"

namespace tdenum {

enum SeparatorsScoringCriterion { UNIFORM, ASCENDING_SIZE };

/**
 * Enumerates the minimal separators of a graph with polynomial delay.
 *
 * Implementing the paper:
 * Generating all the minimal separators of a graph.
 * A. Berry, J. P. Bordat, and O. Cogis, 1999.
 */
class MinimalSeparatorsEnumerator {
	Graph graph;
	SeparatorsScoringCriterion order;
	set< pair<int,MinimalSeparator> > separatorsReturned;
	set< pair<int,MinimalSeparator> > separatorsNotReturned;
	void minimalSeparatorFound(const MinimalSeparator& s);
public:
	// Initialization
	MinimalSeparatorsEnumerator(const Graph& g, SeparatorsScoringCriterion c);
	// Checks whether there is another minimal separator
	bool hasNext();
	// Returns another minimal separator
	MinimalSeparator next();
};

} /* namespace tdenum */

#endif /* MINIMALSEPARATORSENUMERATOR_H_ */
