/*
 * ChordalGraph.h
 *
 *  Created on: May 30, 2016
 *      Author: tmp
 */

#ifndef CHORDALGRAPH_H_
#define CHORDALGRAPH_H_

#include "Graph.h"

namespace tdenum {

class ChordalGraph: public Graph {
public:
	ChordalGraph();
	// Saves the graph object as a chordal graph object (no triangulation is performed)
	ChordalGraph(const Graph& g);
	virtual ~ChordalGraph();
	// If this is a triangulation of origin, returns the number of fill edges
	int getFillIn(const Graph& origin) const;
	// Prints the edges in this graph and not in the origin
	void printTriangulation(const Graph& origin) const;
};

} /* namespace tdenum */

#endif /* CHORDALGRAPH_H_ */
