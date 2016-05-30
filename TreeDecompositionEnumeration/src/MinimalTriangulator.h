#ifndef MINIMALTRIANGULATOR_H_
#define MINIMALTRIANGULATOR_H_

#include "ChordalGraph.h"

namespace tdenum {

/**
 * Calculates a minimal triangulation of the graph
 */
class MinimalTriangulator {
public:
	MinimalTriangulator();
	ChordalGraph triangulate(const Graph& g);
};

} /* namespace tdenum */

#endif /* MINIMALTRIANGULATOR_H_ */
