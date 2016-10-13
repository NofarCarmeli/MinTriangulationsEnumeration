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
	vector<bool> handled (g.getNumberOfNodes(), false);
	for (Node v = 0; v<g.getNumberOfNodes(); v++) {
		mcsQueue.insert(pair<int,Node> (0,v));
	}
	// start search
	while (!mcsQueue.empty()) {
		// Pop node from queue
		pair<int,Node> current = *mcsQueue.rbegin();
		Node v = current.second;
		mcsQueue.erase(current);
		handled[v] = true;
		// Find nodes to update
		multiset<Node> nodesToUpdate;
		vector<bool> reached(g.getNumberOfNodes(), false);
		vector< vector<Node> > reachedByMaxWeight(g.getNumberOfNodes(), vector<Node>());
		NodeSet neighborsOfV = g.getNeighbors(v);
		for (NodeSetIterator i=neighborsOfV.begin(); i!=neighborsOfV.end(); ++i) {
			Node u = *i;
			if (!handled[u]) {
				nodesToUpdate.insert(u);
				reached[u] = true;
				reachedByMaxWeight[weight[u]].push_back(u);
			}
		}
		for (int maxWeight=0; maxWeight<g.getNumberOfNodes(); maxWeight++) {
			while (!reachedByMaxWeight[maxWeight].empty()) {
				Node w = reachedByMaxWeight[maxWeight].back();
				reachedByMaxWeight[maxWeight].pop_back();
				NodeSet neighborsOfW = g.getNeighbors(w);
				for (NodeSetIterator i=neighborsOfW.begin(); i!=neighborsOfW.end(); ++i) {
					Node u = *i;
					if (!handled[u] && !reached[u]) {
						if (weight[u] > maxWeight) {
							nodesToUpdate.insert(u);
						}
						reached[u] = true;
						reachedByMaxWeight[max(weight[u], maxWeight)].push_back(u);
					}
				}
			}
		}
		// Update nodes
		for (multiset<Node>::iterator j=nodesToUpdate.begin(); j!=nodesToUpdate.end(); ++j) {
			Node u = *j;
			// Update weight
			mcsQueue.erase(pair<int,Node> (weight[u],u));
			weight[u]++;
			mcsQueue.insert(pair<int,Node> (weight[u],u));
			// Add edge
			NodeSet s;
			s.insert(u);
			s.insert(v);
			triangulation.addClique(s);
		}
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
