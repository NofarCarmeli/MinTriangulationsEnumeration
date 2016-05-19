#ifndef INDEPENDENTSETEXTENDER_H_
#define INDEPENDENTSETEXTENDER_H_

#include <set>
using namespace std;

namespace tdenum {

/**
 * An abstract class defining the interface needed by the Maximal Independent
 * Set Enumerator.
 * Provides a function that extends an independent set to a maximal independent
 * set, where the sets hold objects of type T.
 * Can be used as part of a polynomial expansion of independent sets for a
 * succinct graph representation.
 */
template<class T>
class IndependentSetExtender {
public:
	IndependentSetExtender() {}
	virtual ~IndependentSetExtender() {}
	/**
	 * receives an independent set and returns a maximal independent set that
	 * contains it.
	 */
	virtual set<T> extendToMaxIndependentSet(const set<T>& s) = 0;
};

} /* namespace tdenum */

#endif /* INDEPENDENTSETEXTENDER_H_ */
