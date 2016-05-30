#ifndef CONVERTER_H_
#define CONVERTER_H_

#include "Graph.h"
#include "ChordalGraph.h"
#include <set>

namespace tdenum {

/**
 * Converts between minimal triangulations and maximal sets of non-crossing
 * minimal separators.
 *
 * Based on the bijection described in the paper:
 * Characterizations and algorithmic applications of chordal graph embeddings.
 * A. Parra and P. Scheffler, 1997.
 */
class Converter {
public:
	/**
	 * Receives a graph and a set of maximal non crossing minimal separators,
	 * and returns the minimal triangulation obtained by saturating its bags.
	 */
	static ChordalGraph minimalSeparatorsToTriangulation(const Graph& g,
			const set<MinimalSeparator>& minimalSeparators);
	/**
	 * Receives a chordal graph, and returns its minimal separators.
	 * If this is a minimal triagulation of some other graph, the result is a
	 * maximal set of not crossing minimal separators.
	 */
	static set<MinimalSeparator> triangulationToMinimalSeparators(
			const ChordalGraph& triangulation);
};

} /* namespace tdenum */

#endif /* CONVERTER_H_ */
