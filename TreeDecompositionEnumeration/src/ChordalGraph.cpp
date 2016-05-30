/*
 * ChordalGraph.cpp
 *
 *  Created on: May 30, 2016
 *      Author: tmp
 */

#include "ChordalGraph.h"

namespace tdenum {

ChordalGraph::ChordalGraph() {}

ChordalGraph::ChordalGraph(const Graph& g) : Graph(g) {}

ChordalGraph::~ChordalGraph() {}

int ChordalGraph::getFillIn(const Graph& origin) const {
	return getNumberOfEdges() - origin.getNumberOfEdges();
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
