#include "IndSetScorerByTriangulation.h"
#include "ChordalGraph.h"
#include "Converter.h"

namespace tdenum {

IndSetScorerByTriangulation::IndSetScorerByTriangulation(const Graph& g, TriangulationScoringCriterion c) :
		graph(g), criterion(c) {}

IndSetScorerByTriangulation::~IndSetScorerByTriangulation() {}

int IndSetScorerByTriangulation::scoreIndependentSet(const set<MinimalSeparator>& s) {
	if (criterion == NONE) {
		return 0;
	} else if (criterion == WIDTH) {
		ChordalGraph g = Converter::minimalSeparatorsToTriangulation(graph, s);
		return g.getTreeWidth();
	} else if (criterion == FILL) {
		ChordalGraph g = Converter::minimalSeparatorsToTriangulation(graph, s);
		return g.getFillIn(graph);
	} else if (criterion == MAX_SEP_SIZE) {
		int maxSeparatorSize = 0;
		for (set<MinimalSeparator>::iterator it = s.begin(); it!=s.end(); ++it) {
			if (it->size() > (unsigned int)maxSeparatorSize) {
				maxSeparatorSize = it->size();
			}
		}
		return maxSeparatorSize;
	} else if (criterion == DIFFERENECE) {
		int score = 0;
		ChordalGraph g = Converter::minimalSeparatorsToTriangulation(graph, s);
		vector< set<Node> > fillEdges = g.getFillEdges(graph);
		for (vector< set<Node> >::iterator it=fillEdges.begin(); it!=fillEdges.end(); ++it) {
			if (seenFillEdges.find(*it) != seenFillEdges.end()) {
				score++;
			}
		}
		//FIXME seenFillEdges should be updated.
		// MaximalIndependentSetsEnumerator assumes the score is invariant over time.
		return score;
	}
	return 0;
}

} /* namespace tdenum */
