#include "SeparatorGraph.h"
#include <algorithm>

namespace tdenum {

SeparatorGraph::SeparatorGraph(const Graph& g, SeparatorsScoringCriterion c) :
		graph(g), nodesEnumerator(g, c) {}

bool SeparatorGraph::hasNextNode() {
	return nodesEnumerator.hasNext();
}

MinimalSeparator SeparatorGraph::nextNode() {
	MinimalSeparator res = nodesEnumerator.next();
	return res;
}

// check if the nodes of v are in different components of the graph obtained by removing u
bool SeparatorGraph::hasEdge(const MinimalSeparator& u, const MinimalSeparator& v) {
	set<NodeSet> components = graph.getComponents(u);
	int componentsContainingV = 0;
	for (set<NodeSet>::iterator i=components.begin(); i!=components.end(); ++i) {
		bool thisComponentFound = false;
		for (MinimalSeparator::iterator j = v.begin(); j!=v.end(); ++j) {
			if ((*i).count(*j)>0) {
				thisComponentFound = true;
			}
		}
		if (thisComponentFound) {
			componentsContainingV++;
		}
	}
	return (componentsContainingV>1);
}

} /* namespace tdenum */

