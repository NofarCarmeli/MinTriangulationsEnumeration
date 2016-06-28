#include "MinimalSeparatorsEnumerator.h"

namespace tdenum {

/*
 * Initialization
 */
MinimalSeparatorsEnumerator::MinimalSeparatorsEnumerator(const Graph& g, SeparatorsScoringCriterion c) :
	graph(g), scorer(g,c) {
	// Initialize separatorsNotReturned according to the initialization phase
	NodeSet nodes = graph.getNodes();
	for (NodeSetIterator i = nodes.begin(); i != nodes.end(); ++i) {
		Node v = *i;
		NodeSet vAndNeighbors = graph.getNeighbors(v);
		vAndNeighbors.insert(v);
		set<NodeSet> components = graph.getComponents(vAndNeighbors);
		for (set<NodeSet>::iterator it=components.begin(); it!=components.end();
				++it) {
			NodeSet potentialSeparator = graph.getNeighbors(*it);
			if (potentialSeparator.size() > 0) {
				int score = scorer.scoreSeparator(potentialSeparator);
				pair<int,MinimalSeparator> scoredSeparator = make_pair(score, potentialSeparator);
				separatorsNotReturned.insert(scoredSeparator);
			}
		}
	}
}

/*
 * Outputs whether there is a separator not yet returned.
 */
bool MinimalSeparatorsEnumerator::hasNext() {
	return !separatorsNotReturned.empty();
}

/*
 * Input: minimal separator.
 * If this separator was not found before, it is inserted to the set of not
 * returned separators.
 */
void MinimalSeparatorsEnumerator::minimalSeparatorFound(const MinimalSeparator& s) {
	int score = scorer.scoreSeparator(s);
	pair<int,MinimalSeparator> scoredSeparator = make_pair(score, s);
	if (s.size() > 0 && separatorsReturned.find(scoredSeparator) == separatorsReturned.end()) {
		separatorsNotReturned.insert(scoredSeparator);
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
	// Choose separator
	pair<int,MinimalSeparator> scoredSeparator = *separatorsNotReturned.begin();
	MinimalSeparator s = scoredSeparator.second;
	// Process separator according to the generation phase
	for (NodeSetIterator i = s.begin(); i != s.end(); ++i) {
		Node x = *i;
		NodeSet xNeighborsAndS = graph.getNeighbors(x);
		xNeighborsAndS.insert(s.begin(),s.end());
		set<NodeSet> components = graph.getComponents(xNeighborsAndS);
		for (set<NodeSet>::iterator j = components.begin();
				j != components.end(); ++j) {
			NodeSet c = *j;
			MinimalSeparator separator = graph.getNeighbors(c);
			minimalSeparatorFound(separator);
		}
	}
	// Transfer separator to the list of returned separators
	separatorsReturned.insert(scoredSeparator);
	separatorsNotReturned.erase(scoredSeparator);
	return s;
}

} /* namespace tdenum */
