#include "SeparatorGraph.h"
#include <algorithm>

namespace tdenum {

SeparatorGraph::SeparatorGraph(const Graph& g, SeparatorsScoringCriterion c) :
		graph(g), nodesEnumerator(g, c), nodesGenerated(0) {}

bool SeparatorGraph::hasNextNode() {
	return nodesEnumerator.hasNext();
}

MinimalSeparator SeparatorGraph::nextNode() {
	nodesGenerated++;
	return nodesEnumerator.next();
}

// check if the nodes of v are in different components of the graph obtained by removing u
bool SeparatorGraph::hasEdge(const MinimalSeparator& s, const MinimalSeparator& t) {
	vector<int> componentsMap = graph.getComponentsMap(s);
	int componentContainingT = 0;
	for (MinimalSeparator::const_iterator it = t.begin(); it != t.end(); ++it) {
		int componentContainingCurrentNode = componentsMap[*it];
		if (componentContainingT == componentContainingCurrentNode) {
			continue;
		} else if (componentContainingCurrentNode == -1) {
			continue;
		} else if (componentContainingT == 0) {
			componentContainingT = componentContainingCurrentNode;
			continue;
		} else {
			return true;
		}
	}
	return false;
}

int SeparatorGraph::getNumberOfNodesGenerated() {
	return nodesGenerated;
}

} /* namespace tdenum */

