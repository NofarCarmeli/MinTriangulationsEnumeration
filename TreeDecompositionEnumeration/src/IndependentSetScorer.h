#ifndef INDEPENDENTSETSCORER_H_
#define INDEPENDENTSETSCORER_H_

#include <set>
using namespace std;

namespace tdenum {

/**
 * An abstract class defining the interface needed by the Maximal Independent
 * Set Enumerator.
 * Provides a function that scores an independent set that holds objects of type T.
 */
template<class T>
class IndependentSetScorer {
public:
	IndependentSetScorer() {}
	virtual ~IndependentSetScorer() {}
	/**
	 * receives an independent set and returns a score.
	 */
	virtual int scoreIndependentSet(const set<T>& s) = 0;
	/**
	 * Marks the given set as used. Useful is case of scoring relative to used sets.
	 */
	virtual void independentSetUsed(const set<T>& s) = 0;
};

} /* namespace tdenum */

#endif /* INDEPENDENTSETSCORER_H_ */
