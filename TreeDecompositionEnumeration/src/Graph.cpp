#include "Graph.h"
#include "DataStructures.h"
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
			if (u < v) {
				addEdge(u, v);
			}
		}
	}
}

void Graph::addEdge(Node u, Node v) {
	if (!isValidNode(u) || !isValidNode(v) || neighborSets[u].count(v)>0) {
		return;
	}
	neighborSets[u].insert(v);
	neighborSets[v].insert(u);
	numberOfEdges++;
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
const NodeSet& Graph::getNeighbors(Node v) const {
	if (!isValidNode(v)) {
		cout << "Error: Requesting access to invalid node" << endl;
		return neighborSets[0];
	}
	return neighborSets[v];
}

vector<bool> Graph::getNeighborsMap(Node v) const {
	vector<bool> result(numberOfNodes, false);
	for (NodeSet::iterator j = neighborSets[v].begin(); j != neighborSets[v].end(); ++j) {
		result[*j] = true;
	}
	return result;
}

/*
 * Returns the set of neighbors of nodes in the given node set without returning
 * nodes that are in the input node set
 */
NodeSet Graph::getNeighbors(const NodeSet& inputSet) const {
	NodeSetProducer neighborsProducer(numberOfNodes);
	for (set<Node>::const_iterator i = inputSet.begin(); i != inputSet.end(); ++i) {
		Node v = *i;
		if (!isValidNode(v)) {
			return set<Node>();
		}
		for (NodeSet::iterator j = neighborSets[v].begin(); j != neighborSets[v].end(); ++j) {
			neighborsProducer.insert(*j);
		}
	}
	for (set<Node>::const_iterator i = inputSet.begin(); i != inputSet.end(); ++i) {
		neighborsProducer.remove(*i);
	}
	return neighborsProducer.produce();
}

bool Graph::areNeighbors(Node u, Node v) const {
	return neighborSets[u].find(v) != neighborSets[u].end();
}

vector< set<Node> > Graph::getComponents(const NodeSet& removedNodes) const {
	vector<int> visitedList(numberOfNodes, 0);
	for (NodeSetIterator i = removedNodes.begin(); i != removedNodes.end(); ++i) {
		Node v = *i;
		if (!isValidNode(v)) {
			return vector< set<Node> >();
		}
		visitedList[v] = -1;
	}
	int numberOfUnhandeledNodes = numberOfNodes - removedNodes.size();
	return getComponentsAux(visitedList, numberOfUnhandeledNodes);
}

vector< set<Node> > Graph::getComponentsAux(vector<int> visitedList, int numberOfUnhandeledNodes) const {
	vector< set<Node> > components;
	// Finds a new component in each iteration
	while (numberOfUnhandeledNodes > 0) {
		queue<Node> bfsQueue;
		NodeSetProducer componentProducer(visitedList.size());
		// Initialize the queue to contain a node not handled
		for (Node i=0; i<numberOfNodes; i++) {
			if (visitedList[i]==0) {
				bfsQueue.push(i);
				visitedList[i]=1;
				componentProducer.insert(i);
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
					componentProducer.insert(u);
					numberOfUnhandeledNodes--;
				}
			}
		}
		components.push_back(componentProducer.produce());
	}
	return components;
}

vector<int> Graph::getComponentsMap(const NodeSet& removedNodes) const {
	vector<int> visitedList(numberOfNodes, 0);
	for (NodeSetIterator i = removedNodes.begin(); i != removedNodes.end(); ++i) {
		Node v = *i;
		if (!isValidNode(v)) {
			return vector<int>();
		}
		visitedList[v] = -1;
	}
	int numberOfUnhandeledNodes = numberOfNodes - removedNodes.size();
	int currentComponent = 1;
	while (numberOfUnhandeledNodes > 0) {
			queue<Node> bfsQueue;
			// Initialize the queue to contain a node not handled
			for (Node i=0; i<numberOfNodes; i++) {
				if (visitedList[i]==0) {
					bfsQueue.push(i);
					visitedList[i]=currentComponent;
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
						visitedList[u]=currentComponent;
						numberOfUnhandeledNodes--;
					}
				}
			}
			currentComponent++;
		}
	return visitedList;
}

void Graph::print() {
	for (Node v=0; v<getNumberOfNodes(); v++) {
		cout << v << " has neighbors: {";
		for (NodeSetIterator jt = getNeighbors(v).begin(); jt!=getNeighbors(v).end(); ++jt) {
			cout << *jt << " ";
		}
		cout << "}" << endl;
	}
}

} /* namespace tdenum */

