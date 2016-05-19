#include "MinimalTriangulator.h"
#include <map>

namespace tdenum {

MinimalTriangulator::MinimalTriangulator() {}

/*
 * Calculates minimal triangulation by implementing MSC-M
 */
Graph MinimalTriangulator::triangulate(const Graph& g) {
	// initialize structures
	Graph triangulation(g); // holds the result
	map<Node, int> weight; // default is zero
	set< pair<int,Node> > mcsQueue;
	NodeSet visited;
	NodeSet unvisited = g.getNodes();
	for (NodeSetIterator i=unvisited.begin(); i!=unvisited.end(); ++i) {
		mcsQueue.insert(pair<int,Node> (0,*i));
	}
	// start search
	while (!mcsQueue.empty()) {
		// Pop node from queue
		pair<int,Node> current = *mcsQueue.rbegin();
		Node z = current.second;
		mcsQueue.erase(current);
		for (NodeSetIterator i=unvisited.begin(); i!=unvisited.end(); ++i) {
			Node y = *i;
			NodeSet consideredForPath;
			multiset<Node> weightUpdated;
			for (NodeSetIterator j=unvisited.begin(); j!=unvisited.end(); ++j) {
				if (weight[*j] < weight[*i]) {
					consideredForPath.insert(*j);
				}
			}
			consideredForPath.insert(y);
			consideredForPath.insert(z);
			set<NodeSet> components = g.getComponentsOfSubgraph(consideredForPath, NodeSet());
			for (set<NodeSet>::iterator comp=components.begin(); comp!=components.end(); ++comp) {
				if ((*comp).count(y)+(*comp).count(z)==2) {
					weightUpdated.insert(y);
					NodeSet s;
					s.insert(y);
					s.insert(z);
					triangulation.addClique(s);
					break;
				}
			}
			for (multiset<Node>::iterator j=weightUpdated.begin(); j!=weightUpdated.end(); ++j) {
				Node y = *j;
				mcsQueue.erase(pair<int,Node> (weight[y],y));
				weight[y]++;
				mcsQueue.insert(pair<int,Node> (weight[y],y));
			}
		}
		unvisited.erase(z);
		visited.insert(z);
	}
	return triangulation;
}

} /* namespace tdenum */
