#ifndef MINIMALTRIANGULATIONSENUMERATOR_H_
#define MINIMALTRIANGULATIONSENUMERATOR_H_

#include "Graph.h"
#include "SeparatorGraph.h"
#include "IndSetExtByTriangulation.h"
#include "MaximalIndependentSetsEnumerator.h"

namespace tdenum {

/**
 * Enumerates the minimal triangulations of a graph
 */
class MinimalTriangulationsEnumerator {
	Graph graph;
	SeparatorGraph separatorGraph;
	IndSetExtByTriangulation extender;
	MaximalIndependentSetsEnumerator<MinimalSeparator> setsEnumerator;
public:
	// initialization
	MinimalTriangulationsEnumerator(const Graph& g);
	// Checks whether there is another minimal triangulation
	bool hasNext();
	// Returns another minimal triangulation
	ChordalGraph next();
};

} /* namespace tdenum */

#endif /* MINIMALTRIANGULATIONSENUMERATOR_H_ */
