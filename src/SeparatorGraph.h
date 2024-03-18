#ifndef SEPARATORGRAPH_H_
#define SEPARATORGRAPH_H_
#include "MinimalSeparatorsEnumerator.h"
#include "SuccinctGraphRepresentation.h"

namespace tdenum {

/**
 * A polynomial succinct graph representation of a separator graph;
 * A node for every minimal separator of the provided graph,
 * An edge appears between two minimal separators if they are crossing.
 */
class SeparatorGraph : public SuccinctGraphRepresentation<MinimalSeparator> {
	Graph graph;
	MinimalSeparatorsEnumerator nodesEnumerator;
	int nodesGenerated;
public:
	// Initialization
	SeparatorGraph(const Graph& g, SeparatorsScoringCriterion c);
	// Checks whether there is another minimal separator
	virtual bool hasNextNode();
	// Returns another minimal separator
	virtual MinimalSeparator nextNode();
	// Checks whether the minimal separators are crossing
	virtual bool hasEdge(const MinimalSeparator& u, const MinimalSeparator& v);
	// Returns how many nodes were already generated
	int getNumberOfNodesGenerated();
};

} /* namespace tdenum */

#endif /* SEPARATORGRAPH_H_ */
