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
		triExtender(graph, triangulator), sepExtender(graph), scorer(graph, triC),
		setsEnumerator(separatorGraph, triExtender, scorer) {
	if (heuristic == SEPARATORS) {
		setsEnumerator = MaximalIndependentSetsEnumerator<MinimalSeparator>(
				separatorGraph, sepExtender, scorer);
	}
}

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

