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

/**
 * Returns all the results that were generated as part of the calculations
 * but not returned yet. Useful in case of partial enumeration when you
 * don't want to invest more time in generating more results.
 */
vector<ChordalGraph> MinimalTriangulationsEnumerator::getGeneratedNotReturned() {
	vector< set<MinimalSeparator> > sets = setsEnumerator.getGeneratedNotReturned();
	vector<ChordalGraph> result(sets.size());
	for (unsigned int i=0; i<sets.size(); i++) {
		result[i] = Converter::minimalSeparatorsToTriangulation(graph, sets[i]);
	}
	return result;
}

} /* namespace tdenum */

