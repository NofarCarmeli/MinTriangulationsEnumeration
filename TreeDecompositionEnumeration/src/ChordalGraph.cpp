/*
 * ChordalGraph.cpp
 *
 *  Created on: May 30, 2016
 *      Author: tmp
 */

#include "ChordalGraph.h"
#include <map>

namespace tdenum {

ChordalGraph::ChordalGraph() {}

ChordalGraph::ChordalGraph(const Graph& g) : Graph(g) {}

ChordalGraph::~ChordalGraph() {}

int ChordalGraph::getFillIn(const Graph& origin) const {
	return getNumberOfEdges() - origin.getNumberOfEdges();
}

/*
 * Input: a chordal graph
 *
 * Calculates the maximal cliques according to the algorithm described in:
 * A. Berry and R. Pogorelcnik. A simple algorithm to generate the minimal
 * separators and the maximal cliques of a chordal graph.
 *
 * Uses a Maximum Cardinality Search priority queue, where the weight of a node
 * is the number of neighbors it has that were already visited. If this weight
 * is smaller or equal to the previous weight, it with its neighbors form a
 * clique.
 */
set<NodeSet> ChordalGraph::getMaximalCliques() const {
	// initialize structures
	set<NodeSet> cliques; // holds the result
	map<Node, int> weight; // default is zero
	map<Node, bool> isVisited; // default is false
	set< pair<int,Node> > mcsQueue;
	NodeSet gNodes = getNodes();
	for (NodeSetIterator i=gNodes.begin(); i!=gNodes.end(); ++i) {
		mcsQueue.insert(pair<int,Node> (0,*i));
	}
	int previousNumberOfNeighbors = -1;
	Node previousNode = -1;
	// start search
	while (!mcsQueue.empty()) {
		// Pop node from queue
		pair<int,Node> current = *mcsQueue.rbegin();
		Node currentNode = current.second;
		int currentNumberOfNeighbors = current.first;
		mcsQueue.erase(current);
		// Add a new clique if relevant
		if (currentNumberOfNeighbors <= previousNumberOfNeighbors) {
			// add currentNode and  its visited neighbors to cliques
			NodeSet currentClique;
			currentClique.insert(previousNode);
			NodeSet neighbors = getNeighbors(previousNode);
			for (NodeSetIterator i = neighbors.begin(); i!=neighbors.end(); ++i) {
				if (isVisited[*i]) {
					currentClique.insert(*i);
				}
			}
			cliques.insert(currentClique);
		}
		// Update structures
		NodeSet neighbors = getNeighbors(currentNode);
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
		previousNode = currentNode;
	}
	// add the last clique
	NodeSet currentClique;
	currentClique.insert(previousNode);
	NodeSet neighbors = getNeighbors(previousNode);
	for (NodeSetIterator i = neighbors.begin(); i!=neighbors.end(); ++i) {
		currentClique.insert(*i);
	}
	cliques.insert(currentClique);
	return cliques;
}

int ChordalGraph::getTreeWidth() const {
	set<NodeSet> maximalCliques = getMaximalCliques();
	int maxSize = 0;
	for (set<NodeSet>::iterator i=maximalCliques.begin(); i!=maximalCliques.end(); ++i) {
		if (maxSize < (int)(*i).size()) {
			maxSize = (*i).size();
		}
	}
	return maxSize - 1;
}

void ChordalGraph::printTriangulation(const Graph& origin) const {
	NodeSet nodes = getNodes();
	for (NodeSetIterator i=nodes.begin(); i!=nodes.end(); ++i) {
		NodeSet neighbors = getNeighbors(*i);
		for (NodeSetIterator j=neighbors.begin(); j!=neighbors.end(); ++j) {
			if (*i <= *j && origin.getNeighbors(*i).count(*j) == 0) {
				cout << *i << "-" << *j << endl;
			}
		}
	}
}

} /* namespace tdenum */