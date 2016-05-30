#include "Converter.h"
#include <queue>
#include <map>
#include <utility>

namespace tdenum {

/*
 * Returns the triangulation of g obtained by saturating the given minimal separators
 */
ChordalGraph Converter::minimalSeparatorsToTriangulation(const Graph& g,
		const set<MinimalSeparator>& minimalSeparators) {
	ChordalGraph triangulation(g);
	triangulation.saturateNodeSets(minimalSeparators);
	return triangulation;
}

/*
 * Input: a chordal graph
 *
 * Calculates the minimal separators according to the algorithm described in:
 * P. Kumar and C. Madhavan. Minimal vertex separators of chordal graphs.
 * Discrete Applied Mathematics, 89(1-3): 155-168, 1998.
 *
 * Uses a Maximum Cardinality Search priority queue, where the weight of a node
 * is the number of neighbors it has that were already visited. If this weight
 * is smaller or equal to the previous weight, these neighbors form a minimal
 * separator.
 */
set<MinimalSeparator> Converter::triangulationToMinimalSeparators(
		const ChordalGraph& g) {
	// initialize structures
	set<MinimalSeparator> minimalSeparators; // holds the result
	map<Node, int> weight; // default is zero
	map<Node, bool> isVisited; // default is false
	set< pair<int,Node> > mcsQueue;
	NodeSet gNodes = g.getNodes();
	for (NodeSetIterator i=gNodes.begin(); i!=gNodes.end(); ++i) {
		mcsQueue.insert(pair<int,Node> (0,*i));
	}
	int previousNumberOfNeighbors = -1;
	// start search
	while (!mcsQueue.empty()) {
		// Pop node from queue
		pair<int,Node> current = *mcsQueue.rbegin();
		Node currentNode = current.second;
		int currentNumberOfNeighbors = current.first;
		mcsQueue.erase(current);
		// Add a new minimal separator if relevant
		if (currentNumberOfNeighbors <= previousNumberOfNeighbors) {
			// add visited neighbors of currentNode to minimalSeparators
			MinimalSeparator currentSeparator;
			NodeSet neighbors = g.getNeighbors(currentNode);
			for (NodeSetIterator i = neighbors.begin(); i!=neighbors.end(); ++i) {
				if (isVisited[*i]) {
					currentSeparator.insert(*i);
				}
			}
			if (!currentSeparator.empty()) {
				minimalSeparators.insert(currentSeparator);
			}
		}
		// Update structures
		NodeSet neighbors = g.getNeighbors(currentNode);
		for (NodeSetIterator i = neighbors.begin(); i!=neighbors.end(); ++i) {
			if (!isVisited[*i]) {
				int oldWeight = weight[*i];
				mcsQueue.erase(pair<int,Node> (oldWeight,*i));
				mcsQueue.insert(pair<int,Node> (oldWeight+1,*i));
				weight[*i]++;
			}
		}
		isVisited[currentNode] = true;
		previousNumberOfNeighbors = currentNumberOfNeighbors;
	}
	return minimalSeparators;
}

} /* namespace tdenum */

