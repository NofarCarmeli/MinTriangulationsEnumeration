#ifndef MAXIMALINDEPENDENTSETSENUMERATOR_H_
#define MAXIMALINDEPENDENTSETSENUMERATOR_H_

#include "SuccinctGraphRepresentation.h"
#include "IndependentSetExtender.h"
#include "IndependentSetScorer.h"

namespace tdenum {

enum AlgorithmStep {BEGINNING, ITERATING_NODES, ITERATING_SETS};

/**
 * Enumerates the maximal independent sets of a graph given by a succinct graph
 * representation with an independent set expansion.
 * If the representation and the expansion are polynomial, this algorithm runs
 * in incremental polynomial delay.
 */
template<class T>
class MaximalIndependentSetsEnumerator {

	// Tools
	SuccinctGraphRepresentation<T>& graph;
	IndependentSetExtender<T>& extender;
	IndependentSetScorer<T>& scorer;

	// State
	set<T> nodesGenerated;
	set< set<T> > setsExtended;
	set< set<T> > setsNotExtended;
	set< pair<int, set<T> > > extendingQueue;
	bool nextSetReady;
	set<T> nextIndependentSet;
	AlgorithmStep step;
	// State for case ITERATING_NODES
	typename set<T>::iterator nodesIterator;
	set<T> currentSet;
	// State for case ITERATING_SETS
	typename set< set<T> >::iterator setsIterator;
	T currentNode;

	void getNextSetToExtend();
	set<T> extendSetInDirectionOfNode(const set<T>& set, const T& node);
	bool newSetFound(const set<T>& set);
	bool runFullEnumeration();
public:
	/**
	 * Initialization. Receives a succinct graph representation with an
	 * independent set expansion.
	 */
	MaximalIndependentSetsEnumerator(SuccinctGraphRepresentation<T>& graph,
			IndependentSetExtender<T>& extender, IndependentSetScorer<T>& scorer);
	/**
	 * Checks whether there is another maximal independent set.
	 */
	bool hasNext();
	/**
	 * Returns another maximal independent set.
	 */
	set<T> next();
};



/**
 * Saves the next set to extend to currentSet.
 */
template<class T>
void MaximalIndependentSetsEnumerator<T>::getNextSetToExtend() {
	pair<int, set<T> > currentScoredSet = *extendingQueue.begin();
	// Support for changing scores: Maybe choose a different set if the score has changed
	int currentScore = scorer.scoreIndependentSet(currentScoredSet.second);
	while (currentScore > currentScoredSet.first) {
		// Update weight
		pair<int, set<T> > rescoredSet = make_pair(currentScore, currentScoredSet.second);
		extendingQueue.erase(currentScoredSet);
		extendingQueue.insert(rescoredSet);
		// Choose new set
		currentScoredSet = *extendingQueue.begin();
		currentScore = scorer.scoreIndependentSet(currentScoredSet.second);
	}
	currentSet = currentScoredSet.second;
	// Update that this set is being extended
	scorer.independentSetUsed(currentSet);
	setsExtended.insert(currentSet);
	setsNotExtended.erase(currentSet);
	extendingQueue.erase(currentScoredSet);
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
 * If this set is new, it is inserted to setsNotExtended and extendingQueue,
 * and saved to nextIndependentSet.
 * Returns whether this set was new.
 */
template<class T>
bool MaximalIndependentSetsEnumerator<T>::newSetFound(const set<T>& generatedSet) {
	if (setsExtended.find(generatedSet) == setsExtended.end()) {
		pair<typename set< set<T> >::iterator, bool> insertionReturnValue = setsNotExtended.insert(generatedSet);
		if (insertionReturnValue.second) {
			extendingQueue.insert(make_pair(scorer.scoreIndependentSet(generatedSet), generatedSet));
			nextIndependentSet = generatedSet;
			nextSetReady = true;
			return true;
		}
	}
	return false;
}


template<class T>
bool MaximalIndependentSetsEnumerator<T>::runFullEnumeration() {
	while (!extendingQueue.empty()) {
		getNextSetToExtend();
		for (nodesIterator = nodesGenerated.begin();
				nodesIterator != nodesGenerated.end(); ++nodesIterator) {
			set<T> generatedSet = extendSetInDirectionOfNode(currentSet, *nodesIterator);
			if (newSetFound(generatedSet)) {
				step = ITERATING_NODES;
				return true;
			}
		}
		while(setsNotExtended.empty() && graph.hasNextNode()) {
			// generate a new node and extend returned sets in this direction
			currentNode = graph.nextNode();
			nodesGenerated.insert(currentNode);
			for (setsIterator = setsExtended.begin();
					setsIterator != setsExtended.end(); ++setsIterator) {
				set<T> generatedSet = extendSetInDirectionOfNode(*setsIterator, currentNode);
				if (newSetFound(generatedSet)) {
					step = ITERATING_SETS;
					return true;
				}
			}
		}
	}
	return false;
}


/*
 * Initialization
 */
template<class T>
MaximalIndependentSetsEnumerator<T>::MaximalIndependentSetsEnumerator(
		SuccinctGraphRepresentation<T>& g, IndependentSetExtender<T>& e,
		IndependentSetScorer<T>& s) : graph(g), extender(e), scorer (s) {
	newSetFound(extender.extendToMaxIndependentSet(set<T>()));
	step = BEGINNING;
}


/*
 * Outputs whether there is a maximal independent set not yet returned.
 */
template<class T>
bool MaximalIndependentSetsEnumerator<T>::hasNext() {
	if (nextSetReady) {
		return true;
	} else {
		if (step == BEGINNING) {
			return runFullEnumeration();
		} else if (step == ITERATING_NODES) {
			for (; nodesIterator != nodesGenerated.end(); ++nodesIterator) {
				set<T> generatedSet = extendSetInDirectionOfNode(currentSet, *nodesIterator);
				if (newSetFound(generatedSet)) {
					step = ITERATING_NODES;
					return true;
				}
			}
			while(setsNotExtended.empty() && graph.hasNextNode()) {
				// generate a new node and extend returned sets in this direction
				currentNode = graph.nextNode();
				nodesGenerated.insert(currentNode);
				for (setsIterator = setsExtended.begin(); setsIterator != setsExtended.end(); ++setsIterator) {
					set<T> generatedSet = extendSetInDirectionOfNode(*setsIterator, currentNode);
					if (newSetFound(generatedSet)) {
						step = ITERATING_SETS;
						return true;
					}
				}
			}
			return runFullEnumeration();
		} else if (step == ITERATING_SETS) {
			for (; setsIterator != setsExtended.end(); ++setsIterator) {
				set<T> generatedSet = extendSetInDirectionOfNode(*setsIterator, currentNode);
				if (newSetFound(generatedSet)) {
					step = ITERATING_SETS;
					return true;
				}
			}
			while(setsNotExtended.empty() && graph.hasNextNode()) {
				// generate a new node and extend returned sets in this direction
				currentNode = graph.nextNode();
				nodesGenerated.insert(currentNode);
				for (setsIterator = setsExtended.begin(); setsIterator != setsExtended.end(); ++setsIterator) {
					set<T> generatedSet = extendSetInDirectionOfNode(*setsIterator, currentNode);
					if (newSetFound(generatedSet)) {
						step = ITERATING_SETS;
						return true;
					}
				}
			}
			return runFullEnumeration();
		}
	}
	// We covered all cases so we should never get here
	return false;
}


/*
 * Processes a maximal independent set that was not yet processed, transfers it
 * to the list of returned sets, and returns it.
 */
template<class T>
set<T> MaximalIndependentSetsEnumerator<T>::next() {
	if (nextSetReady || hasNext()) {
		nextSetReady = false;
		return nextIndependentSet;
	}
	return set<T>();
}

} /* namespace tdenum */

#endif /* MAXIMALINDEPENDENTSETSENUMERATOR_H_ */
