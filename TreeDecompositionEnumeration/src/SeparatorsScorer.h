#ifndef SEPARATORSSCORER_H_
#define SEPARATORSSCORER_H_

#include "Graph.h"

namespace tdenum {

enum SeparatorsScoringCriterion { UNIFORM, ASCENDING_SIZE, FILL_EDGES };

class SeparatorsScorer {
	Graph graph;
	SeparatorsScoringCriterion criterion;
public:
	SeparatorsScorer(const Graph& g, SeparatorsScoringCriterion c);
	virtual ~SeparatorsScorer();
	int scoreSeparator(const MinimalSeparator& s);
};

} /* namespace tdenum */

#endif /* SEPARATORSSCORER_H_ */
