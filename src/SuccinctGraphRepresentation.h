#ifndef SUCCINCTGRAPHREPRESENTATION_H_
#define SUCCINCTGRAPHREPRESENTATION_H_

namespace tdenum {

/**
 * An abstract class defining the interface of a succinct graph representation
 * where the nodes are of type T.
 */
template<class T>
class SuccinctGraphRepresentation {
public:
	SuccinctGraphRepresentation(){}
	virtual ~SuccinctGraphRepresentation(){}
	// Checks whether there is another node
	virtual bool hasNextNode() = 0;
	// Returns another node
	virtual T nextNode() = 0;
	// Checks whether an edge exists
	virtual bool hasEdge(const T& u, const T& v) = 0;
};

} /* namespace tdenum */

#endif /* SUCCINCTGRAPHREPRESENTATION_H_ */
