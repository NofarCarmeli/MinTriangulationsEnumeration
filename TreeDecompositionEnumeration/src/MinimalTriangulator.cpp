#include "MinimalTriangulator.h"
#include "DataStructures.h"
#include <map>
#include <algorithm>
#include <iterator>

namespace tdenum {

MinimalTriangulator::MinimalTriangulator(TriangulationAlgorithm h) : heuristic(h), time(0) {}

// implementing MSC-M algorithm
ChordalGraph getMinimalTriangulationUsingMSCM(const Graph& g) {
	// initialize structures
	ChordalGraph triangulation(g); // holds the result
	IncreasingWeightNodeQueue queue(g.getNumberOfNodes());
	vector<bool> handled (g.getNumberOfNodes(), false);
	// start search
	while (!queue.isEmpty()) {
		// Pop node from queue
		Node v = queue.pop();
		handled[v] = true;
		// Find nodes to update
		NodeSet nodesToUpdate;
		vector<bool> reached(g.getNumberOfNodes(), false);
		vector<NodeSet> reachedByMaxWeight(g.getNumberOfNodes(), vector<Node>());
		for (set<Node>::iterator i=g.getNeighbors(v).begin(); i!=g.getNeighbors(v).end(); ++i) {
			Node u = *i;
			if (!handled[u]) {
				nodesToUpdate.push_back(u);
				reached[u] = true;
				reachedByMaxWeight[queue.getWeight(u)].push_back(u);
			}
		}
		for (int maxWeight=0; maxWeight<g.getNumberOfNodes(); maxWeight++) {
			while (!reachedByMaxWeight[maxWeight].empty()) {
				Node w = reachedByMaxWeight[maxWeight].back();
				reachedByMaxWeight[maxWeight].pop_back();
				for (set<Node>::iterator i=g.getNeighbors(w).begin(); i!=g.getNeighbors(w).end(); ++i) {
					Node u = *i;
					if (!handled[u] && !reached[u]) {
						if (queue.getWeight(u) > maxWeight) {
							nodesToUpdate.push_back(u);
						}
						reached[u] = true;
						reachedByMaxWeight[max(queue.getWeight(u), maxWeight)].push_back(u);
					}
				}
			}
		}
		// Update nodes
		for (NodeSet::iterator j=nodesToUpdate.begin(); j!=nodesToUpdate.end(); ++j) {
			Node u = *j;
			queue.increaseWeight(u);
			triangulation.addEdge(u, v);
		}
	}
	return triangulation;
}


int getFill(const Graph& g, Node v) {
	const set<Node>& neighborsSet = g.getNeighbors(v);
	int twiceFillEdges = 0;
	// for every node in the neighborhood, add the number of non-neighbors
	for (set<Node>::iterator it=neighborsSet.begin(); it!=neighborsSet.end(); ++it) {
		vector<bool> notNeighborsOfCurrentNode = g.getNeighborsMap(v);
		notNeighborsOfCurrentNode[*it] = false;
		const set<Node>& neighborsOfCurrentNode = g.getNeighbors(*it);
		for (set<Node>::iterator jt=neighborsOfCurrentNode.begin(); jt!=neighborsOfCurrentNode.end(); ++jt) {
			notNeighborsOfCurrentNode[*jt] = false;
		}
		for (int i=0; i<g.getNumberOfNodes(); i++) {
			if (notNeighborsOfCurrentNode[i]) {
				twiceFillEdges++;
			}
		}
	}
	return twiceFillEdges/2;
}

class NodeSetStaturator {
	set<NodeSet> toSaturate;
public:
	void markForSaturation(const vector<Node>& nodeSet) {
		toSaturate.insert(nodeSet);
	}
	void saturate(Graph& g) {
		g.saturateNodeSets(toSaturate);
	}
};

// Returns the minimal separators included in the neighborhood of v
NodeSetStaturator getSubstars(const Graph& g, const Graph& gi, Node v) {
	set<Node> removedNodes = gi.getNeighbors(v);
	removedNodes.insert(v);
	vector<NodeSet> components = g.getComponents(removedNodes);
	NodeSetStaturator saturator;
	for (vector<NodeSet>::iterator it=components.begin(); it!=components.end(); ++it) {
		saturator.markForSaturation(g.getNeighbors(*it));
	}
	return saturator;
}

// Saturates the minimal separators included in the neighborhood of v
// g is the original graph, and gi is the graph in the last phase.
void makeNodeLBSimplicial(const Graph& g, Graph& gi, Node v) {
	NodeSetStaturator saturator =  getSubstars(g, gi, v);
	saturator.saturate(gi);
}

class NodeQueue {
	set< pair<int,Node> > queue;
	const Graph& graph;
	TriangulationAlgorithm heuristic;
	int score(Node v) {
		if (heuristic == MIN_DEGREE_LB_TRIANG || heuristic == INITIAL_DEGREE_LB_TRIANG) {
			return graph.getNeighbors(v).size();
		} else if (heuristic == MIN_FILL_LB_TRIANG || heuristic == INITIAL_FILL_LB_TRIANG || heuristic == COMBINED) {
			return getFill(graph, v);
		}
		return 0;
	}
public:
	NodeQueue(const Graph& g, TriangulationAlgorithm h) : graph(g), heuristic(h) {
		for (Node v=0; v<g.getNumberOfNodes(); v++) {
			queue.insert(make_pair(score(v), v));
		}
	}
	Node pop() {
		Node top = queue.begin()->second;
		// check if score was updates in relevant heuristics
		if (heuristic == MIN_DEGREE_LB_TRIANG || heuristic == MIN_FILL_LB_TRIANG || heuristic == COMBINED) {
			int savedScore = queue.begin()->first;
			int currentScore = score(top);
			while (currentScore > savedScore) {
				queue.erase(queue.begin());
				queue.insert(make_pair(currentScore,top));
				top = queue.begin()->second;
				savedScore = queue.begin()->first;
				currentScore = score(top);
			}
		}
		queue.erase(queue.begin());
		return top;
	}
	bool isEmpty() {
		return queue.empty();
	}
};


ChordalGraph getMinimalTriangulationUsingLBTriang(const Graph& g, TriangulationAlgorithm heuristic) {
	Graph result(g);
	if (heuristic == LB_TRIANG) {
		for (Node v=0; v<g.getNumberOfNodes(); v++) {
			makeNodeLBSimplicial(g, result, v);
		}
	} else {
		NodeQueue queue(result, heuristic);
		while (!queue.isEmpty()) {
			makeNodeLBSimplicial(g, result, queue.pop());
		}
	}
	return result;
}


ChordalGraph MinimalTriangulator::triangulate(const Graph& g) {
	time++;
	if (heuristic == MCS_M || (heuristic == COMBINED && time % 2 == 0)) {
		return getMinimalTriangulationUsingMSCM(g);
	}
	return getMinimalTriangulationUsingLBTriang(g, heuristic);
}


} /* namespace tdenum */
