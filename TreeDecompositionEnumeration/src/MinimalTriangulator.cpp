#include "MinimalTriangulator.h"
#include <map>
#include <algorithm>

namespace tdenum {

MinimalTriangulator::MinimalTriangulator(TriangulationAlgorithm h) : heuristic(h) {}

// implementing MSC-M algorithm
ChordalGraph getMinimalTriangulationUsingMSCM(const Graph& g) {
	// initialize structures
	ChordalGraph triangulation(g); // holds the result
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

// assumes options is not empty
Node getMinDegree(const Graph& g, set<Node>& options) {
	set<Node>::iterator it = options.begin();
	Node bestNode = *it;
	int minDegree = g.getNeighbors(*it).size();
	for (;it!=options.end(); ++it) {
		int currentDegree = g.getNeighbors(*it).size();
		if (currentDegree < minDegree) {
			bestNode = *it;
			minDegree = currentDegree;
		}
	}
	return bestNode;
}

int getFill(const Graph& g, Node v) {
	set<Node> neighborsSet = g.getNeighbors(v);
	int twiceFillEdges = 0;
	// for every node in the neighborhood, add the number of non-neighbors
	for (set<Node>::iterator it=neighborsSet.begin(); it!=neighborsSet.end(); ++it) {
		set<Node> neighborsOfCurrentNode = g.getNeighbors(*it);
		set<Node> notNeighborsOfCurrentNode;
		set_difference(neighborsSet.begin(), neighborsSet.end(),
				neighborsOfCurrentNode.begin(), neighborsOfCurrentNode.end(),
				inserter(notNeighborsOfCurrentNode, notNeighborsOfCurrentNode.begin()));
		twiceFillEdges+= (notNeighborsOfCurrentNode.size()-1);
	}
	return twiceFillEdges/2;
}

// assumes options is not empty
Node getMinFill(const Graph& g, const set<Node>& options) {
	set<Node>::iterator it = options.begin();
	Node bestNode = *it;
	int minFill = getFill(g,*it);
	for (;it!=options.end(); ++it) {
		int currentFill = getFill(g,*it);
		if (currentFill < minFill) {
			bestNode = *it;
			minFill = currentFill;
		}
	}
	return bestNode;
}

// Returns the minimal separators included in the neighborhood of v
set<NodeSet> getSubstars(const Graph& g, Node v) {
	NodeSet removedNodes = g.getNeighbors(v);
	removedNodes.insert(v);
	set<NodeSet> components = g.getComponents(removedNodes);
	set<NodeSet> substars;
	for (set<NodeSet>::iterator it=components.begin(); it!=components.end(); ++it) {
		substars.insert(g.getNeighbors(*it));
	}
	return substars;
}

// Saturates the minimal separators included in the neighborhood of v
void makeNodeLBSimplicial(Graph& g, Node v) {
	set<NodeSet> substars =  getSubstars(g,v);
	g.saturateNodeSets(substars);
}

//FIXME can be done faster (don't compute the score from scratch each time)
ChordalGraph getMinimalTriangulationUsingLBTriang(const Graph& g, TriangulationAlgorithm heuristic) {
	Graph result(g);
	set<Node> unhandledNodes = g.getNodes();
	for (int i=0; i<g.getNumberOfNodes(); i++) {
		Node v = i;
		if (heuristic == MIN_DEGREE_LB_TRIANG) {
			v = getMinDegree(result,unhandledNodes);
		} else if (heuristic ==MIN_FILL_LB_TRIANG) {
			v = getMinFill(result,unhandledNodes);
		}
		makeNodeLBSimplicial(result,v);
		unhandledNodes.erase(v);
	}
	return result;
}


ChordalGraph MinimalTriangulator::triangulate(const Graph& g) {
	if (heuristic == MSC_M) {
		return getMinimalTriangulationUsingMSCM(g);
	}
	return getMinimalTriangulationUsingLBTriang(g, heuristic);
}


} /* namespace tdenum */
