/*
 * This class was written by Vladimir Zacharov.
 * It implements a separator based approach to extend independent sets of
 * non-crossing minimal separators as described in section 5 of the paper
 * available in https://arxiv.org/abs/1604.02833
 */

#ifndef SRC_EXTENDIND_EXTENDIND_H_
#define SRC_EXTENDIND_EXTENDIND_H_

#include "IndependentSetExtender.h"
#include "Graph.h"
#include "SubGraph.h"
#include <queue>
#include <algorithm>
#include <list>

namespace tdenum {

bool sortNodes(int node1, int node2);
bool isInNodeSet(Node node, const NodeSet& nodes);


class IndSetExtBySeparators: public IndependentSetExtender<MinimalSeparator> {
	Graph graph;

	vector<Node> getUnconnectedNodes(const Graph& graph);

	void includeNodesToMaximalSet(vector<int>& cNodeIndsInMainGraph,
			set<MinimalSeparator>& maximalSet, const NodeSet& cNeighbors,
			const NodeSet& minimalSepInC);

	NodeSet findMinSep(vector<Node>& unconnectedNodes, Graph& graph);

	NodeSet mergeComponentAndNeighbors(const NodeSet& component,
			const NodeSet& compNeighbors);
	queue<SubGraph> decompose(const SubGraph& mainSubGraph,
		const set<MinimalSeparator>& s);
	

public:
	IndSetExtBySeparators(const Graph& graph) :
			graph(graph) {
	}
	;

	IndSetExtBySeparators() {
	}
	;

	

	set<MinimalSeparator> extendToMaxIndependentSet(
			const set<MinimalSeparator>& s) override;
};
}

#endif /* SRC_EXTENDIND_EXTENDIND_H_ */
