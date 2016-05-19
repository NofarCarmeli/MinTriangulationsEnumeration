#ifndef MAXIMALINDEPENDENTSETSENUMERATOR_H_
#define MAXIMALINDEPENDENTSETSENUMERATOR_H_

#include "SuccinctGraphRepresentation.h"
#include "IndependentSetExtender.h"

namespace tdenum {

/**
 * Enumerates the maximal independent sets of a graph given by a succinct graph
 * representation with an independent set expansion.
 * If the representation and the expansion are polynomial, this algorithm runs
 * in incremental polynomial delay.
 */
template<class T>
class MaximalIndependentSetsEnumerator {
	SuccinctGraphRepresentation<T>& graph;
	IndependentSetExtender<T>& extender;
	set<T> nodesGenerated;
	set< set<T> > maxIndependentSetsReturned;
	set< set<T> > maxIndependentSetsNotReturned;

	set<T> extendSetInDirectionOfNode(const set<T>& set, const T& node);
	void maxIndependentSetFound(const set<T>& set);
public:
	/**
	 * Initialization. Receives a succinct graph representation with an
	 * independent set expansion.
	 */
	MaximalIndependentSetsEnumerator(SuccinctGraphRepresentation<T>& graph,
			IndependentSetExtender<T>& extender);
	/**
	 * Checks whether there is another maximal independent set.
	 */
	bool hasNext();
	/**
	 * Returns another maximal independent set.
	 */
	set<T> next();
};





/*
 * Initialization
 */
template<class T>
MaximalIndependentSetsEnumerator<T>::MaximalIndependentSetsEnumerator(
		SuccinctGraphRepresentation<T>& g,
		IndependentSetExtender<T>& e) : graph(g), extender(e){
	set<T> firstIndSet = extender.extendToMaxIndependentSet(set<T>());
	maxIndependentSetsNotReturned.insert(firstIndSet);
}

/*
 * Outputs whether there is a maximal independent set not yet returned.
 */
template<class T>
bool MaximalIndependentSetsEnumerator<T>::hasNext() {
	return !maxIndependentSetsNotReturned.empty();
}

/*
 * Input: maximal independent set and node.
 * Creates a maximal independent set containing the given node and the nodes
 * from the given set that do not share an edge with the given node.
 */
template<class T>
set<T> MaximalIndependentSetsEnumerator<T>::extendSetInDirectionOfNode(
		const set<T>& s, const T& node) {
	set<T> baseNodes;
	baseNodes.insert(node);
	for (typename set<T>::iterator i=s.begin(); i!=s.end(); ++i) {
		if (!graph.hasEdge(node, *i)) {
			baseNodes.insert(*i);
		}
	}
	return extender.extendToMaxIndependentSet(baseNodes);
}

/*
 * Input: maximal independent set.
 * If this set was not found before, it is inserted to the set of not
 * returned independent sets.
 */
template<class T>
void MaximalIndependentSetsEnumerator<T>::maxIndependentSetFound(const set<T>& s) {
	if (maxIndependentSetsReturned.find(s) == maxIndependentSetsReturned.end()) {
		maxIndependentSetsNotReturned.insert(s);
	}
}

/*
 * Processes a maximal independent set that was not yet processed, transfers it
 * to the list of returned sets, and returns it.
 */
template<class T>
set<T> MaximalIndependentSetsEnumerator<T>::next() {
	// Verify that there is another set
	if (!hasNext()) {
		return set<T>();
	}
	// Choose set to extend
	set<T> currentSet = *maxIndependentSetsNotReturned.begin();
	// Transfer set to the list of returned sets
	maxIndependentSetsReturned.insert(currentSet);
	maxIndependentSetsNotReturned.erase(currentSet);
	// Process set to create new sets
	for (typename set<T>::iterator i = nodesGenerated.begin(); i!=nodesGenerated.end(); ++i) {
		set<T> generatedSet = extendSetInDirectionOfNode(currentSet, *i);
		maxIndependentSetFound(generatedSet);
	}
	while(maxIndependentSetsNotReturned.empty() && graph.hasNextNode()) {
		// generate a new node and extend returned sets in this direction
		T generatedNode = graph.nextNode();
		nodesGenerated.insert(generatedNode);
		for (typename set< set<T> >::iterator i=maxIndependentSetsReturned.begin();
				i!=maxIndependentSetsReturned.end(); ++i) {
			set<T> generatedSet = extendSetInDirectionOfNode(*i, generatedNode);
			maxIndependentSetFound(generatedSet);
		}
	}
	return currentSet;
}

} /* namespace tdenum */

#endif /* MAXIMALINDEPENDENTSETSENUMERATOR_H_ */
