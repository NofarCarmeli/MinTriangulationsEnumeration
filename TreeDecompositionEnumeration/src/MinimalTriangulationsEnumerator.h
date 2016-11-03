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
	// Returns the number of minimal separators generated
	int getNumberOfMinimalSeperatorsGenerated();
};

} /* namespace tdenum */

#endif /* MINIMALTRIANGULATIONSENUMERATOR_H_ */
