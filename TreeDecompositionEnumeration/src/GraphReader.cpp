#include "GraphReader.h"
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

namespace tdenum {

Graph GraphReader::read() {

	string line;
	getline(cin, line);
	istringstream lineStream(line);
	int numberOfNodes, numberOfCliques;
	lineStream >> numberOfNodes;
	lineStream >> numberOfCliques;

	Graph g(numberOfNodes);

	for (int i=0; i<numberOfCliques; i++) {
		getline(cin, line);
		istringstream lineStream(line);
		NodeSet clique;
		Node v;
		while(lineStream >> v) {
			clique.insert(v);
		}
		g.addClique(clique);
	}

	return g;
}

Graph GraphReader::readCnf(bool isWeighted) {
	string line;
	getline(cin, line);
	// Skip comments
	while (line[0] == 'c') {
		getline(cin, line);
	}
	// Skip beginning of line ("p wcnf "), and get numbers of variables and clauses
	istringstream lineStream(line);
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

} /* namespace tdenum */

