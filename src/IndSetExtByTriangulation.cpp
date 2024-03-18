#include "IndSetExtByTriangulation.h"
#include "Converter.h"
#include "MinimalTriangulator.h"

namespace tdenum {

IndSetExtByTriangulation::IndSetExtByTriangulation(const Graph& g, const MinimalTriangulator& t) :
	graph(g), triangulator(t) {}


set<MinimalSeparator> IndSetExtByTriangulation::extendToMaxIndependentSet(
		const set<MinimalSeparator>& s) {
	Graph saturatedGraph(graph);
	saturatedGraph.saturateNodeSets(s);
	Graph minimalTriangulation = triangulator.triangulate(saturatedGraph);
	set<MinimalSeparator> minimalSeparators =
			Converter::triangulationToMinimalSeparators(minimalTriangulation);
	return minimalSeparators;
}

} /* namespace tdenum */
