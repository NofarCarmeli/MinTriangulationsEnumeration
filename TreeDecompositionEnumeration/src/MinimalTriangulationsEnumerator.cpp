#include "MinimalTriangulationsEnumerator.h"
#include "Converter.h"

namespace tdenum {

/*
 * Initialization
 */
MinimalTriangulationsEnumerator::MinimalTriangulationsEnumerator(
			const Graph& g, ScoringCriterion c) :
		graph(g), separatorGraph(graph), extender(graph, MinimalTriangulator()),
		scorer(graph, c), setsEnumerator(separatorGraph, extender, scorer) {}

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

} /* namespace tdenum */

