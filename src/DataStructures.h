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
 * A minimum heap.
 */
class WeightedNodeSetQueue {
	set< pair<int, NodeSet > > queue;
public:
	// True if the structure is empty.
	bool isEmpty();
	// True if the structure contains the given node set with the given weight.
	bool isMember(const NodeSet& nodeSet, int weight);
	// Adds the given node set to the structure with the given weight.
	void insert(const NodeSet& nodeSet, int weight);
	// Returns the maximal weighted node set, and removes it from the structure.
	NodeSet pop();
};

class NodeSetSet {
	set< NodeSet > sets;
public:
    // True if the structure contains the given node set.
	bool isMember(const NodeSet& nodeSet);
	// Adds the given node set to the structure.
	void insert(const NodeSet& nodeSet);
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
	// Returns the subset containing all added nodes, sorted ascending.
	NodeSet produce();
};

} /* namespace tdenum */

#endif /* DATASTRUCTURES_H_ */
