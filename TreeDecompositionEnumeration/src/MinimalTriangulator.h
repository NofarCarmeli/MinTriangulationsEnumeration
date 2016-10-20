#ifndef MINIMALTRIANGULATOR_H_
#define MINIMALTRIANGULATOR_H_

#include "ChordalGraph.h"

namespace tdenum {

enum TriangulationAlgorithm { MCS_M, MIN_FILL_LB_TRIANG, INITIAL_FILL_LB_TRIANG, MIN_DEGREE_LB_TRIANG, INITIAL_DEGREE_LB_TRIANG, LB_TRIANG };

/**
 * Calculates a minimal triangulation of the graph
 * Implements MSC-M or LB-Triang with min-fill or min-dgree heuristics
 */
class MinimalTriangulator {
	TriangulationAlgorithm heuristic;
public:
	MinimalTriangulator(TriangulationAlgorithm h);
	ChordalGraph triangulate(const Graph& g);
};

} /* namespace tdenum */

#endif /* MINIMALTRIANGULATOR_H_ */
