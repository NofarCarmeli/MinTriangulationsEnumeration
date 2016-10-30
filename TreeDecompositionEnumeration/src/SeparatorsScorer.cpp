#include "SeparatorsScorer.h"

namespace tdenum {

SeparatorsScorer::SeparatorsScorer(
		const Graph& g, SeparatorsScoringCriterion c) : graph(g), criterion(c){}

SeparatorsScorer::~SeparatorsScorer() {}

int SeparatorsScorer::scoreSeparator(const MinimalSeparator& s) {
	if (criterion == UNIFORM) {
		return 0;
	} else if (criterion == ASCENDING_SIZE) {
		return s.size();
	} else if (criterion == FILL_EDGES) {
		int fill_edges = 0;
		for (MinimalSeparator::const_iterator it=s.begin(); it!=s.end(); ++it) {
			for (MinimalSeparator::const_iterator jt=s.begin(); jt!=it; ++jt) {
				if (!graph.areNeighbors(*it, *jt)) {
					fill_edges++;
				}
			}
		}
		return fill_edges;
	}
	return 0;
}

} /* namespace tdenum */
