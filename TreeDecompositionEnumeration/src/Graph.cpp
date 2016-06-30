#include "Graph.h"
#include <queue>
#include <iostream>

namespace tdenum {

Graph::Graph() : numberOfNodes(0), numberOfEdges(0) {}

Graph::Graph(int numberOfNodes) : numberOfNodes(numberOfNodes), numberOfEdges(0),
		neighborSets(numberOfNodes) {}

void Graph::addClique(const NodeSet& newClique) {
	for (NodeSetIterator i = newClique.begin(); i != newClique.end(); ++i) {
		Node v = *i;
		for (NodeSetIterator j = newClique.begin(); j != newClique.end(); ++j) {
			Node u = *j;
			if (!isValidNode(u) || !isValidNode(v)) {
				return;
			}
			if (u < v) {
				if (neighborSets[u].count(v) == 0) {
					neighborSets[u].insert(v);
					neighborSets[v].insert(u);
					numberOfEdges++;
				}
			}
		}
	}
}

void Graph::saturateNodeSets(const set<NodeSet>& s) {
	for (set<NodeSet>::iterator i = s.begin(); i != s.end(); ++i) {
		addClique(*i);
	}
}

bool Graph::isValidNode(Node v) const {
	if (v<0 || v>=numberOfNodes) {
		cout << "Invalid input" << endl;
		return false;
	}
	return true;
}


/*
 * Returns the set of nodes in the graph
 */
NodeSet Graph::getNodes() const {
	NodeSet nodes;
	for (Node i=0; i<numberOfNodes; i++) {
		nodes.insert(i);
	}
	return nodes;
}

/*
 * Returns the number of edges in the graph
 */
int Graph::getNumberOfEdges() const {
	return numberOfEdges;
}

/*
 * Returns the number of nodes in the graph
 */
int Graph::getNumberOfNodes() const {
	return numberOfNodes;
}

/*
 * Returns the set of neighbors of the given node
 */
NodeSet Graph::getNeighbors(Node v) const {
	if (!isValidNode(v)) {
		return NodeSet();
	}
	return neighborSets[v];
}


/*
 * Returns the set of neighbors of nodes in the given node set without returning
 * nodes that are in the input node set
 */
NodeSet Graph::getNeighbors(const NodeSet& inputSet) const {
	NodeSet neighbors;
	for (NodeSetIterator i = inputSet.begin(); i != inputSet.end(); ++i) {
		Node v = *i;
		if (!isValidNode(v)) {
			return NodeSet();
		}
		neighbors.insert(neighborSets[v].begin(),neighborSets[v].end());
	}
	for (NodeSetIterator i = inputSet.begin(); i != inputSet.end(); ++i) {
		Node v = *i;
		neighbors.erase(v);
	}
	return neighbors;
}


/*
 * Returns sets representing the connected components received when removing the
 * given set from the graph
 *
 * Initializes visitedList and calls getcomponentAux
 */
set<NodeSet> Graph::getComponents(const NodeSet& inputSet) const {
	vector<int> visitedList(numberOfNodes, 0);
	for (NodeSetIterator i = inputSet.begin(); i != inputSet.end(); ++i) {
		Node v = *i;
		if (!isValidNode(v)) {
			return set<NodeSet>();
		}
		visitedList[v] = -1;
	}
	int numberOfUnhandeledNodes = numberOfNodes - inputSet.size();
	return getComponentsAux(visitedList, numberOfUnhandeledNodes);
}

/*
 * Returns the components obtained by removing the given node set from the subgraph
 *
 * Initializes visitedList and calls getcomponentAux
 */
set<NodeSet> Graph::getComponentsOfSubgraph(const NodeSet& subgraph, const NodeSet& removedSet) const {
	vector<int> visitedList(numberOfNodes, -1);
	for (NodeSetIterator i = subgraph.begin(); i != subgraph.end(); ++i) {
		Node v = *i;
		if (!isValidNode(v)) {
			return set<NodeSet>();
		}
		visitedList[v] = 0;
	}
	int numberOfUnhandeledNodes = subgraph.size();
	for (NodeSetIterator i = removedSet.begin(); i != removedSet.end(); ++i) {
		Node v = *i;
		if (!isValidNode(v)) {
			return set<NodeSet>();
		}
		if (visitedList[v]==0) {
			visitedList[v] = -1;
			numberOfUnhandeledNodes--;
		}
	}
	return getComponentsAux(visitedList, numberOfUnhandeledNodes);
}

/*
 * Receives a list with a flag for each node: -1 if removed, 0 if not handled.
 * Returns the components of the nodes marked by 0.
 * Marks nodes in the list by 1 when they are added to some component.
 */
set<NodeSet> Graph::getComponentsAux(vector<int> visitedList, int numberOfUnhandeledNodes) const {
	set<NodeSet> components;
	// Finds a new component in each iteration
	while (numberOfUnhandeledNodes > 0) {
		queue<Node> bfsQueue;
		NodeSet currentComponent;
		// Initialize the queue to contain a node not handled
		for (Node i=0; i<numberOfNodes; i++) {
			if (visitedList[i]==0) {
				bfsQueue.push(i);
				visitedList[i]=1;
				currentComponent.insert(i);
				numberOfUnhandeledNodes--;
				break;
			}
		}
		// BFS through the component
		while (!bfsQueue.empty()) {
			Node v = bfsQueue.front();
			bfsQueue.pop();
			for (NodeSetIterator i = neighborSets[v].begin();
					i != neighborSets[v].end(); ++i) {
				Node u = *i;
				if (visitedList[u]==0) {
					bfsQueue.push(u);
					visitedList[u]=1;
					currentComponent.insert(u);
					numberOfUnhandeledNodes--;
				}
			}
		}
		components.insert(currentComponent);
	}
	return components;
}

void Graph::print() {
	NodeSet nodes = getNodes();
	for (NodeSetIterator i=nodes.begin(); i!=nodes.end(); ++i) {
		cout << *i << " has neighbors: ";
		NodeSet s = getNeighbors(*i);
		cout << "{ ";
		for (NodeSetIterator jt = s.begin(); jt!=s.end(); ++jt) {
			cout << *jt << " ";
		}
		cout << "}" << endl;
	}
}

} /* namespace tdenum */

