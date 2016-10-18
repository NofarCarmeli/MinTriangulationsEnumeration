#ifndef GRAPH_H_
#define GRAPH_H_

#include <set>
#include <vector>
#include <iostream>
using namespace std;

namespace tdenum {

typedef int Node;
typedef set<Node> NodeSet;
typedef set<Node>::iterator NodeSetIterator;
typedef NodeSet MinimalSeparator;

class Graph {
	int numberOfNodes;
	int numberOfEdges;
	vector<NodeSet> neighborSets;

	bool isValidNode(Node v) const;
	vector< set<Node> > getComponentsAux(vector<int> visitedList, int numberOfUnhandeledNodes) const;

public:
	// Constructs an empty graph
	Graph();
	// Constructs a graph with nodes and without edges
	Graph(int numberOfNodes);
	// Connects the given two nodes by a edge
	void addEdge(Node u, Node v);
	// Adds edges that will make that given node set a clique
	void addClique(const NodeSet& s);
	// Adds edges that will make the given node sets cliques
	void saturateNodeSets(const set<NodeSet>& s);

	// Returns the nodes of the graph
	NodeSet getNodes() const;
	// Returns the number of edges in the graph
	int getNumberOfEdges() const;
	// Returns the number of nodes in the graph
	int getNumberOfNodes() const;
	// Returns the neighbors of the given node
	NodeSet getNeighbors(Node v) const;
	// Returns the neighbors of the given node set
	NodeSet getNeighbors(const NodeSet& s) const;
	// Returns whether there is an edge between the given two nodes
	bool areNeighbors(Node u, Node v) const;
	// Returns the set of components obtained by removing the given node set
	vector< set<Node> > getComponents(const NodeSet& removedNodes) const;
	// Prints the graph
	void print();
};

} /* namespace tdenum */

#endif /* GRAPH_H_ */
