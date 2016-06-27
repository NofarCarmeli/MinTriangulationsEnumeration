#ifndef INDSETSCORERBYTRIANGULATION_H_
#define INDSETSCORERBYTRIANGULATION_H_

#include "IndependentSetScorer.h"
#include "Graph.h"

namespace tdenum {

enum TriangulationScoringCriterion { NONE, WIDTH, FILL, MAX_SEP_SIZE, DIFFERENECE };

/**
 * An implementation of the IndependentSetScorer interface for the case where
 * the nodes are minimal separators.
 * Uses the conversion to a minimal triangulation to evaluate it.
 * Lower score is better.
 */
class IndSetScorerByTriangulation :
		public IndependentSetScorer<MinimalSeparator> {
	Graph graph;
	TriangulationScoringCriterion criterion;
	set< set<Node> > seenFillEdges;
public:
	IndSetScorerByTriangulation(const Graph& g, TriangulationScoringCriterion c);
	virtual ~IndSetScorerByTriangulation();
	int scoreIndependentSet(const set<MinimalSeparator>& s);
};

} /* namespace tdenum */

#endif /* INDSETSCORERBYTRIANGULATION_H_ */
