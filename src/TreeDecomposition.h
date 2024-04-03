/*
 * TreeDecomposition.h
 */

#ifndef TREEDECOMPOSITION_H_
#define TREEDECOMPOSITION_H_

#include "Graph.h"
#include <map>
#include <deque>

namespace tdenum {

typedef pair<int,int> Edge;

class TreeDecomposition {
    vector<NodeSet> bags;
    vector<vector<pair<int,int>>> edgesOptions;
	void makeTreeEdges();
	void makeAllTreeEdgeOptions();
	void makeAllTreeEdgeOptionsAux(vector<Edge> tree, int minModifiableIndex, set<Edge> forbiddenEdges);
	int getEdgeWeight(const pair<int,int>& edge);
	void print(ostream& output, const map<int,string>& inputNaming);
public:
	TreeDecomposition();
	TreeDecomposition(const set<NodeSet>& inputBags);
	virtual ~TreeDecomposition();
	void printSingleTree(ostream& output, const map<int,string>& inputNaming);
	void printAllEdgeOptions(ostream& output, const map<int,string>& inputNaming);
};

} /* namespace tdenum */

#endif /* TREEDECOMPOSITION_H_ */