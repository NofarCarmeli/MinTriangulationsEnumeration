/*
 * ChordalGraph.cpp
 *
 *  Created on: May 30, 2016
 *      Author: tmp
 */

#include "ChordalGraph.h"
#include "DataStructures.h"
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
	map<Node, bool> isVisited; // default is false
	IncreasingWeightNodeQueue queue(getNumberOfNodes());
	int previousNumberOfNeighbors = -1;
	Node previousNode = -1;
	// start search
	while (!queue.isEmpty()) {
		// Pop node from queue
		Node currentNode = queue.pop();
		int currentNumberOfNeighbors = queue.getWeight(currentNode);
		// Add a new clique if relevant
		if (currentNumberOfNeighbors <= previousNumberOfNeighbors) {
			// add currentNode and  its visited neighbors to cliques
			NodeSetProducer cliqueProducer(getNumberOfNodes());
			cliqueProducer.insert(previousNode);
			NodeSet neighbors = getNeighbors(previousNode);
			for (NodeSetIterator i = neighbors.begin(); i!=neighbors.end(); ++i) {
				if (isVisited[*i]) {
					cliqueProducer.insert(*i);
				}
			}
			cliques.insert(cliqueProducer.produce());
		}
		// Update structures
		NodeSet neighbors = getNeighbors(currentNode);
		for (NodeSetIterator i = neighbors.begin(); i!=neighbors.end(); ++i) {
			if (!isVisited[*i]) {
				queue.increaseWeight(*i);
			}
		}
		isVisited[currentNode] = true;
		previousNumberOfNeighbors = currentNumberOfNeighbors;
		previousNode = currentNode;
	}
	// add the last clique
	NodeSetProducer cliqueProducer(getNumberOfNodes());
	cliqueProducer.insert(previousNode);
	NodeSet neighbors = getNeighbors(previousNode);
	for (NodeSetIterator i = neighbors.begin(); i!=neighbors.end(); ++i) {
		cliqueProducer.insert(*i);
	}
	cliques.insert(cliqueProducer.produce());
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

vector< set<Node> > ChordalGraph::getFillEdges(const Graph& origin) const {
	vector< set<Node> > edges;
	for (Node v=0; v<getNumberOfNodes(); v++) {
		NodeSet neighbors = getNeighbors(v);
		for (NodeSetIterator j=neighbors.begin(); j!=neighbors.end(); ++j) {
			Node u = *j;
			if (v <= u && !origin.areNeighbors(v,u)) {
				set<Node> edge;
				edge.insert(v);
				edge.insert(u);
				edges.push_back(edge);
			}
		}
	}
	return edges;
}

void ChordalGraph::printTriangulation(const Graph& origin) const {
	vector< set<Node> > fillEdges = getFillEdges(origin);
	for (vector< set<Node> >::iterator it=fillEdges.begin(); it!=fillEdges.end(); ++it) {
		Node u = *(it->begin());
		Node v = *(++it->begin());
		cout << u << "-" << v << endl;
	}
}

} /* namespace tdenum */
