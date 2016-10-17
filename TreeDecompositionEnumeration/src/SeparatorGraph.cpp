#include "SeparatorGraph.h"
#include <algorithm>

namespace tdenum {

SeparatorGraph::SeparatorGraph(const Graph& g, SeparatorsScoringCriterion c) :
		graph(g), nodesEnumerator(g, c) {}

bool SeparatorGraph::hasNextNode() {
	return nodesEnumerator.hasNext();
}

MinimalSeparator SeparatorGraph::nextNode() {
	return nodesEnumerator.next();
}

// check if the nodes of v are in different components of the graph obtained by removing u
bool SeparatorGraph::hasEdge(const MinimalSeparator& u, const MinimalSeparator& v) {
	vector<bool> inV (graph.getNumberOfNodes(), false);
	for (NodeSet::iterator it = v.begin(); it!=v.end(); ++it) {
		inV[*it] = true;
	}
	vector<NodeSet> componentsofU = graph.getComponents(u);
	int componentsContainingV = 0;
	for (vector<NodeSet>::iterator i=componentsofU.begin(); i!=componentsofU.end(); ++i) {
		for (NodeSet::iterator j = i->begin(); j != i->end(); ++j) {
			if (inV[*j]) {
				componentsContainingV++;
				if (componentsContainingV > 1) {
					return true;
				}
				break;
			}
		}
	}
	return false;
}

} /* namespace tdenum */

