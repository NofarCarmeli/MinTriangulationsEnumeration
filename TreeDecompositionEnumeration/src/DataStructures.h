/*
 * DataStructures.h
 *
 *  Created on: Oct 17, 2016
 *      Author: tmp
 */

#ifndef DATASTRUCTURES_H_
#define DATASTRUCTURES_H_

#include "Graph.h"
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

namespace tdenum {

/*
 * A maximum heap. Can be used for Maximum Cardinality Search.
 */
class IncreasingWeightNodeQueue {
	vector<int> weight;
	set< pair<int,Node> > queue;
public:
	IncreasingWeightNodeQueue(int numberOfNodes);
	// Increase the weight of the node by 1.
	// Node is assumed to be an integer between 0 and numberOfNodes-1.
	void increaseWeight(Node v);
	// Returns the weight of the node. This is also available after pop.
	// Node is assumed to be an integer between 0 and numberOfNodes-1.
	int getWeight(Node v);
	// True if the structure is empty.
	bool isEmpty();
	// Returns and removes the node with the maximal weight.
	Node pop();
};


/*
 * Constructs a subset of nodes in linear time in the size of the original set.
 */
class NodeSetProducer {
	vector<bool> isMember;
public:
	NodeSetProducer(int sizeOfOriginalNodeSet);
	// Adds the given node to the subset.
	// Node is assumed to be an integer between 0 and sizeOfOriginalNodeSet-1.
	void insert(Node v);
	// Removes the given node from the subset.
	// Node is assumed to be an integer between 0 and sizeOfOriginalNodeSet-1.
	void remove(Node v);
	// Returns the subset containing all added nodes.
	set<Node> produce();
};

} /* namespace tdenum */

#endif /* DATASTRUCTURES_H_ */
