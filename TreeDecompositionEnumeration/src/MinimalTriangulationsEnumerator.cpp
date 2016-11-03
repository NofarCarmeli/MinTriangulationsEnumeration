#include "MinimalTriangulationsEnumerator.h"
#include "Converter.h"

namespace tdenum {

/*
 * Initialization
 */
MinimalTriangulationsEnumerator::MinimalTriangulationsEnumerator(
			const Graph& g, TriangulationScoringCriterion triC,
			SeparatorsScoringCriterion sepC, TriangulationAlgorithm heuristic) :
		graph(g), separatorGraph(graph, sepC), triangulator(heuristic),
		extender(graph, triangulator), scorer(graph, triC),
		setsEnumerator(separatorGraph, extender, scorer) {}

/*
 * Checks whether there is another minimal triangulation
 */
bool MinimalTriangulationsEnumerator::hasNext(){
	return setsEnumerator.hasNext();
}

/*
 * Returns another minimal triangulation
 */
ChordalGraph MinimalTriangulationsEnumerator::next() {
	return Converter::minimalSeparatorsToTriangulation(graph, setsEnumerator.next());
}

int MinimalTriangulationsEnumerator::getNumberOfMinimalSeperatorsGenerated() {
	return separatorGraph.getNumberOfNodesGenerated();
}

} /* namespace tdenum */

