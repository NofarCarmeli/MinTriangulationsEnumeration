#include "GraphReader.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace tdenum {

Graph readCliques() {
	// Get numbers of variables and clauses
	string line;
	getline(cin, line);
	istringstream lineStream(line);
	int numberOfNodes, numberOfCliques;
	lineStream >> numberOfNodes;
	lineStream >> numberOfCliques;
	// Get cliques
	Graph g(numberOfNodes);
	for (int i=0; i<numberOfCliques; i++) {
		getline(cin, line);
		istringstream lineStream(line);
		int nodesInClique;
		lineStream >> nodesInClique;
		NodeSet clique;
		Node v;
		while(lineStream >> v) {
			clique.insert(v);
		}
		g.addClique(clique);
	}
	return g;
}

Graph readCnf() {
	string line;
	getline(cin, line);
	// Skip comments
	while (line[0] == 'c') {
		getline(cin, line);
	}
	// Determine if weighted
	bool isWeighted;
	istringstream lineStream(line);
	if (line.find("p cnf") != string::npos) {
		isWeighted = false;
	} else if (line.find("p wcnf") != string::npos) {
		isWeighted = true;
	} else {
		cout << "Parsing error" << endl;
		return Graph();
	}
	// Skip beginning of line ("p wcnf "), and get numbers of variables and clauses.
	while (lineStream.peek() > '9' || lineStream.peek() < '0') {
		lineStream.get();
	}
	int numberOfNodes, numberOfCliques;
	lineStream >> numberOfNodes >> numberOfCliques;
	// Get clauses and create graph
	Graph g(numberOfNodes);
	for (int i=0; i<numberOfCliques; i++) {
		getline(cin, line);
		istringstream lineStream(line);
		if (isWeighted) {
			int weight;
			lineStream >> weight;
		}
		NodeSet clique;
		Node v;
		while(lineStream >> v && v!=0) {
			v = v>0 ? v : -v;
			v = v == numberOfNodes ? 0 : v;
			clique.insert(v);
		}
		g.addClique(clique);
	}
	return g;
}

Graph GraphReader::read() {
	if (cin.peek() == 'c' || cin.peek() == 'p') {
		return readCnf();
	} else {
		return readCliques();
	}
}

} /* namespace tdenum */

