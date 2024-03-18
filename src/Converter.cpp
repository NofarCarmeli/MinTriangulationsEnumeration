#include "Converter.h"
#include "DataStructures.h"
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
	map<Node, bool> isVisited; // default is false
	IncreasingWeightNodeQueue queue(g.getNumberOfNodes());
	int previousNumberOfNeighbors = -1;
	// start search
	while (!queue.isEmpty()) {
		// Pop node from queue
		Node currentNode = queue.pop();
		int currentNumberOfNeighbors = queue.getWeight(currentNode);
		// Add a new minimal separator if relevant
		if (currentNumberOfNeighbors <= previousNumberOfNeighbors) {
			// add visited neighbors of currentNode to minimalSeparators
			NodeSetProducer separatorProducer(g.getNumberOfNodes());
			for (set<Node>::iterator i = g.getNeighbors(currentNode).begin();
					i!=g.getNeighbors(currentNode).end(); ++i) {
				if (isVisited[*i]) {
					separatorProducer.insert(*i);
				}
			}
			MinimalSeparator currentSeparator = separatorProducer.produce();
			if (!currentSeparator.empty()) {
				minimalSeparators.insert(currentSeparator);
			}
		}
		// Update structures
		for (set<Node>::iterator i = g.getNeighbors(currentNode).begin();
				i!=g.getNeighbors(currentNode).end(); ++i) {
			if (!isVisited[*i]) {
				queue.increaseWeight(*i);
			}
		}
		isVisited[currentNode] = true;
		previousNumberOfNeighbors = currentNumberOfNeighbors;
	}
	return minimalSeparators;
}

} /* namespace tdenum */

