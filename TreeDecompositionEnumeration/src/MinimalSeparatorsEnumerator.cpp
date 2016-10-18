#include "MinimalSeparatorsEnumerator.h"

namespace tdenum {

/*
 * Initialization
 */
MinimalSeparatorsEnumerator::MinimalSeparatorsEnumerator(const Graph& g, SeparatorsScoringCriterion c) :
	graph(g), scorer(g,c) {
	// Initialize separatorsNotReturned according to the initialization phase
	for (Node v = 0; v < g.getNumberOfNodes(); v++) {
		NodeSet vAndNeighbors = graph.getNeighbors(v);
		vAndNeighbors.insert(v);
		vector<NodeSet> components = graph.getComponents(vAndNeighbors);
		for (vector<NodeSet>::iterator it=components.begin(); it!=components.end(); ++it) {
			NodeSet potentialSeparator = graph.getNeighbors(*it);
			if (potentialSeparator.size() > 0) {
				int score = scorer.scoreSeparator(potentialSeparator);
				separatorsToExtend.insert(potentialSeparator, score);
			}
		}
	}
}

/*
 * Outputs whether there is a separator not yet returned.
 */
bool MinimalSeparatorsEnumerator::hasNext() {
	return !separatorsToExtend.isEmpty();
}

/*
 * Input: minimal separator.
 * If this separator was not found before, it is inserted to the set of not
 * returned separators.
 */
void MinimalSeparatorsEnumerator::minimalSeparatorFound(const MinimalSeparator& s) {
	if (s.size() > 0 && !separatorsExtended.isMember(s)) {
		int score = scorer.scoreSeparator(s);
		separatorsToExtend.insert(s, score);
	}
}

/*
 * Processes a separator that was not yet processed, transfers it to the list of
 * returned separators, and returns it.
 */
MinimalSeparator MinimalSeparatorsEnumerator::next() {
	// Verify that there is another separator
	if (!hasNext()) {
		return MinimalSeparator();
	}
	// Choose separator and transfer to list of returned
	MinimalSeparator s = separatorsToExtend.pop();
	separatorsExtended.insert(s);
	// Process separator according to the generation phase
	for (NodeSetIterator i = s.begin(); i != s.end(); ++i) {
		Node x = *i;
		NodeSet xNeighborsAndS = graph.getNeighbors(x);
		xNeighborsAndS.insert(s.begin(),s.end());
		vector<NodeSet> components = graph.getComponents(xNeighborsAndS);
		for (vector<NodeSet>::iterator j = components.begin(); j != components.end(); ++j) {
			minimalSeparatorFound(graph.getNeighbors(*j));
		}
	}
	return s;
}

} /* namespace tdenum */
