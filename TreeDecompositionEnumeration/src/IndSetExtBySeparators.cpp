/*
 * This class was written by Vladimir Zacharov.
 * It implements a separator based approach to extend independent sets of
 * non-crossing minimal separators as described in section 5 of the paper
 * available in https://arxiv.org/abs/1604.02833
 */

#include "IndSetExtBySeparators.h"

namespace tdenum {

// stops at the first unconnected vertices it finds
vector<Node> IndSetExtBySeparators::getUnconnectedNodes(const Graph& graph) {
	set<Node> allNodes = graph.getNodes();
	vector<Node> unconnectedNodes(2);

	for (int node = 0; node < graph.getNumberOfNodes(); node++) {
		for (int otherNode = node + 1; otherNode < graph.getNumberOfNodes();
				otherNode++) {

			if (!graph.areNeighbors(node, otherNode)) {

				unconnectedNodes[0] = node;
				unconnectedNodes[1] = otherNode;
				if (unconnectedNodes[0] > unconnectedNodes[1]) {
					int t = unconnectedNodes[0];
					unconnectedNodes[0] = unconnectedNodes[1];
					unconnectedNodes[1] = t;
				}

				return unconnectedNodes;
			}
		}
	}

	return {-1,-1};

}

bool sortNodes(int node1, int node2) {
	return node1 < node2;
}

bool isInNodeSet(Node node, const NodeSet& sortedNodes) {

	int n = sortedNodes.size();
	int i = 0;
	while (n > 0) {
		int j = i + n / 2;
		if (sortedNodes[j] == node) {
			return true;
		} else if (node < sortedNodes[j]) {
			n = n / 2;
		} else {
			i = j + 1;
			n = n - n / 2 - 1;
		}
	}

	return false;
}

void IndSetExtBySeparators::includeNodesToMaximalSet(
		vector<int>& cNodesMapInMainGraph, set<MinimalSeparator>& maximalSet,
		const NodeSet& cComponentNeighbors, const NodeSet& minimalSepInC) {

	NodeSet newSep;
	set<Node> newSepSet;
	for (Node neighbor : cComponentNeighbors) {

		if (isInNodeSet(neighbor, minimalSepInC)) {
			// found the neighbor

			int neighborInMain = cNodesMapInMainGraph[neighbor];

			// put in the group the index of the node in the main graph
			newSepSet.insert(neighborInMain);

			continue;
		}

	}

	int i = 0;
	newSep = vector<Node>(newSepSet.size());
	for (Node n : newSepSet) {
		newSep[i] = n;
		i++;
	}

	maximalSet.insert(newSep);

}

queue<SubGraph> IndSetExtBySeparators::decompose(const SubGraph& mainSubGraph,
		const set<MinimalSeparator>& s) {

	list<SubGraph> Q;

	queue<SubGraph> resultComponents;

	// create a component sg which will be equall to mainSubGraph and
	// save its min seps group s as a variable
	SubGraph sg = SubGraph(mainSubGraph, s);
	Q.push_front(sg);

	vector<NodeSet> cComponents;

	while (!Q.empty()) {
		SubGraph& c = Q.front();

		set<MinimalSeparator> cSeps = c.getSeps();

		// if no seps for component c
		if (cSeps.empty()) {
			resultComponents.push(c);
		} else {
			MinimalSeparator S = *cSeps.begin();

			c.addClique(S); // saturate

			// getting components of graph after removing S group
			cComponents = c.getComponents(S);

			for (NodeSet& cComponent : cComponents) {

				// for each component of Cs
				NodeSet cComponentNeighbors = c.getNeighbors(cComponent);

				// merge to create the component merged with its neighbors
				NodeSet cComponentNeighborsMerged = mergeComponentAndNeighbors(
						cComponent, cComponentNeighbors);

				// create a sub graph for the sub comp(which is a component of Cs)
				// that is subCompNeighborsMerged node sets local to c component
				SubGraph cComponentSubGraph = SubGraph(c,
						cComponentNeighborsMerged);

				// creating seps for the sub component
				set<MinimalSeparator> cComponentSeps = c.createNewSepGroup(
						cComponentNeighborsMerged, S, cSeps);

				cComponentSubGraph.setSeps(cComponentSeps);

				Q.push_back(cComponentSubGraph);
			}
		}

		Q.pop_front();
	}

	return resultComponents;

}

NodeSet IndSetExtBySeparators::findMinSep(vector<Node>& unconnectedNodes,
		Graph& graph) {
	Node v = unconnectedNodes[0];
	Node u = unconnectedNodes[1];

	set<Node> uNeighbors = graph.getNeighbors(u);

	set<Node> vComponent = graph.getComponent(v, uNeighbors);

	NodeSet vCompNeighbors = graph.getNeighbors(vComponent);

	return vCompNeighbors;
}

NodeSet IndSetExtBySeparators::mergeComponentAndNeighbors(
		const NodeSet& component, const NodeSet& compNeighbors) {

	// neighbors and comp nodes are different sets
	vector<Node> mergedComp(compNeighbors.size() + component.size());

	for (unsigned int i = 0; i < component.size(); i++) {

		mergedComp[i] = component[i];
	}

	for (unsigned int i = 0; i < compNeighbors.size(); i++) {

		mergedComp[i + component.size()] = compNeighbors[i];
	}

	return mergedComp;

}

set<MinimalSeparator> IndSetExtBySeparators::extendToMaxIndependentSet(
		const set<MinimalSeparator>& minSeps) {

	queue<SubGraph> Q;

	// create a copy of minSeps
	set<MinimalSeparator> maximalSet = minSeps;

	SubGraph sg = SubGraph(graph);
	if (maximalSet.empty()) {
		Q.push(sg);
	} else {
		Q = decompose(sg, minSeps);
	}

	vector<NodeSet> cComponents;

	while (!Q.empty()) {
		SubGraph& c = Q.front();

		vector<Node> unconnectedNodes = getUnconnectedNodes(c);

		if (unconnectedNodes[0] >= 0) { // <=> not clique

			NodeSet minSepInC = findMinSep(unconnectedNodes, c);

			c.addClique(minSepInC); // saturate

			// c components when removing minSepInC group
			cComponents = c.getComponents(minSepInC);

			for (NodeSet& cComponent : cComponents) {
				NodeSet cComponentNeighbors = c.getNeighbors(cComponent);

				// mapping of the node indices in the component c to the node
				// indices in the main graph
				vector<int>& cNodeMapInMainGraph = c.getNodeMapToMainGraph();

				includeNodesToMaximalSet(cNodeMapInMainGraph, maximalSet,
						cComponentNeighbors, minSepInC);

				// subComp nodes merged with its neighbors
				NodeSet cComponentNeighborsMerged = mergeComponentAndNeighbors(
						cComponent, cComponentNeighbors);

				// create a sub graph out of c component and subCompMerged nodes
				SubGraph cComponentSubGraph = SubGraph(c,
						cComponentNeighborsMerged);
				Q.push(cComponentSubGraph);

			}

		}

		Q.pop();
	}

	return maximalSet;
}
}
