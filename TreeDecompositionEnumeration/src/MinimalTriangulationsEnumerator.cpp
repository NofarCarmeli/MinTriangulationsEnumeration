#include "MinimalTriangulationsEnumerator.h"
#include "Converter.h"

namespace tdenum {

/*
 * Initialization
 */
MinimalTriangulationsEnumerator::MinimalTriangulationsEnumerator(const Graph& g) :
		graph(g), separatorGraph(graph), extender(graph, MinimalTriangulator()),
		scorer(graph, NONE), setsEnumerator(separatorGraph, extender, scorer) {}

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

