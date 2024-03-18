#ifndef INDSETEXTBYTRIANGULATION_H_
#define INDSETEXTBYTRIANGULATION_H_

#include "IndependentSetExtender.h"
#include "MinimalTriangulator.h"
#include "Graph.h"

namespace tdenum {

/**
 * An implementation of the IndependentSetExtender interface for the case where
 * the nodes are minimal separators.
 * Uses a provided MinimalTriangulator to perform the steps of finding a minimal
 * triangulation.
 */
class IndSetExtByTriangulation :
		public IndependentSetExtender<MinimalSeparator>  {
	Graph graph;
	MinimalTriangulator triangulator;
public:
	IndSetExtByTriangulation(const Graph& g, const MinimalTriangulator& t);
	virtual set<MinimalSeparator> extendToMaxIndependentSet(const set<MinimalSeparator>& s);
};

} /* namespace tdenum */

#endif /* INDSETEXTBYTRIANGULATION_H_ */
