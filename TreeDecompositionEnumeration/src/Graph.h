#ifndef GRAPH_H_
#define GRAPH_H_

#include <set>
#include <vector>
#include <iostream>
using namespace std;

namespace tdenum {

typedef int Node;
typedef vector<Node> NodeSet; // sorted vector of node names
typedef NodeSet MinimalSeparator;

class Graph {
	int numberOfNodes;
	int numberOfEdges;
	vector< set<Node> > neighborSets;

	bool isValidNode(Node v) const;
	vector< vector<Node> > getComponentsAux(vector<int> visitedList, int numberOfUnhandeledNodes) const;

public:
	// Constructs an empty graph
	Graph();
	// Constructs a graph with nodes and without edges
	Graph(int numberOfNodes);
	// Connects the given two nodes by a edge
	void addEdge(Node u, Node v);
	// Adds edges that will make that given node set a clique
	void addClique(const set<Node>& s);
	// Adds edges that will make that given node set a clique
	void addClique(const vector<Node>& s);
	// Adds edges that will make the given node sets cliques
	void saturateNodeSets(const set< set<Node> >& s);
	// Adds edges that will make the given node sets cliques
	void saturateNodeSets(const set< vector<Node> >& s);

	// Returns the nodes of the graph
	set<Node> getNodes() const;
	// Returns the number of edges in the graph
	int getNumberOfEdges() const;
	// Returns the number of nodes in the graph
	int getNumberOfNodes() const;
	// Returns the neighbors of the given node
	const set<Node>& getNeighbors(Node v) const;
	// Returns a vector the size of the number of nodes in the graph, stating
	// whether the index nodes are neighbors of the input node.
	vector<bool> getNeighborsMap(Node v) const;
	// Returns the neighbors of the given node set
	NodeSet getNeighbors(const vector<Node>& s) const;
	// Returns the neighbors of the given node set
	NodeSet getNeighbors(const set<Node>& s) const;
	// Returns whether there is an edge between the given two nodes
	bool areNeighbors(Node u, Node v) const;
	// Returns a map from the Nodes numbers to the number of the component they are in.
	// -1 if they are in the input set.
	vector<int> getComponentsMap(const vector<Node>& removedNodes) const;
	// Returns the set of components obtained by removing the given node set
	vector<NodeSet> getComponents(const set<Node>& removedNodes) const;
	// Returns the set of components obtained by removing the given node set
	vector<NodeSet> getComponents(const NodeSet& removedNodes) const;
	// Returns all the nodes reachable from the node after removing
	// the removedNodes from the graph
	set<Node> getComponent(Node v, const set<Node>& removedNodes);
	// Prints the graph
	void print() const;
};

} /* namespace tdenum */

#endif /* GRAPH_H_ */
