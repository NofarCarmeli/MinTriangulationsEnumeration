/*
 * TreeDecomposition.h
 */

#ifndef TREEDECOMPOSITION_H_
#define TREEDECOMPOSITION_H_

#include "Graph.h"
#include <map>

namespace tdenum {

class TreeDecomposition {
    vector<NodeSet> bags;
    vector<NodeSet> edges;
public:
	TreeDecomposition();
	TreeDecomposition(const set<NodeSet>& inputBags);
	virtual ~TreeDecomposition();
	void print(ostream& output, const map<int,string>& inputNaming) const;
};

} /* namespace tdenum */

#endif /* TREEDECOMPOSITION_H_ */