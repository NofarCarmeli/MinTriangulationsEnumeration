#ifndef MINIMALTRIANGULATIONSENUMERATOR_H_
#define MINIMALTRIANGULATIONSENUMERATOR_H_

#include "Graph.h"
#include "SeparatorGraph.h"
#include "IndSetExtByTriangulation.h"
#include "MaximalIndependentSetsEnumerator.h"
#include "IndSetScorerByTriangulation.h"

namespace tdenum {

/**
 * Enumerates the minimal triangulations of a graph
 */
class MinimalTriangulationsEnumerator {
	Graph graph;
	SeparatorGraph separatorGraph;
	MinimalTriangulator triangulator;
	IndSetExtByTriangulation extender;
	IndSetScorerByTriangulation scorer;
	MaximalIndependentSetsEnumerator<MinimalSeparator> setsEnumerator;
public:
	// initialization
	MinimalTriangulationsEnumerator(const Graph& g, TriangulationScoringCriterion triC,
			SeparatorsScoringCriterion sepC, TriangulationAlgorithm heuristic);
	// Checks whether there is another minimal triangulation
	bool hasNext();
	// Returns another minimal triangulation
	ChordalGraph next();
	/**
	 * Returns all the results that were generated as part of the calculations
	 * but not returned yet. Useful in case of partial enumeration when you
	 * don't want to invest more time in generating more results.
	 */
	vector<ChordalGraph> getGeneratedNotReturned();
};

} /* namespace tdenum */

#endif /* MINIMALTRIANGULATIONSENUMERATOR_H_ */
