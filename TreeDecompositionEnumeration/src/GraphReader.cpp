#include "GraphReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

namespace tdenum {

Graph readCliques(ifstream& input) {
	// Get numbers of variables and clauses
	string line;
	getline(input, line);
	istringstream lineStream(line);
	int numberOfNodes, numberOfCliques;
	lineStream >> numberOfNodes;
	lineStream >> numberOfCliques;
	// Get cliques
	Graph g(numberOfNodes);
	for (int i=0; i<numberOfCliques; i++) {
		getline(input, line);
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

Graph readCnf(ifstream& input) {
	string line;
	getline(input, line);
	// Skip comments
	while (line[0] == 'c') {
		getline(input, line);
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
		getline(input, line);
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

Graph readUAI(ifstream& input) {
	// Ignore the line "MARKOV"
	string line;
	getline(input, line);
	// Get numbers of variables
	getline(input, line);
	istringstream variableLineStream(line);
	int numberOfNodes;
	variableLineStream >> numberOfNodes;
	// Ignore the cardinalities
	getline(input, line);
	// Get number of cliques
	getline(input, line);
	istringstream cliquesLineStream(line);
	int numberOfCliques;
	cliquesLineStream >> numberOfCliques;
	// Get cliques
	Graph g(numberOfNodes);
	for (int i=0; i<numberOfCliques; i++) {
		getline(input, line);
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

Graph readCSV(ifstream& input) {
	map <string,int> nodeNames;
	vector < set<int> > edges;
	// Get nodes and edges
	string line;
	while (getline(input, line)) {
		istringstream lineStream(line);
		string nodeName;
		set<int> edge;
		while (getline(lineStream, nodeName, ',' )) {
			map<string,int>::iterator nameSearchResult = nodeNames.find(nodeName);
			int nodeNumber;
			if (nameSearchResult == nodeNames.end()) {
				nodeNumber = nodeNames.size();
				nodeNames[nodeName] = nodeNumber;
			} else {
				nodeNumber = nameSearchResult->second;
			}
			edge.insert(nodeNumber);
		}
		edges.push_back(edge);
	}
	// Construct graph
	Graph g(nodeNames.size());
	for (int i=0; i<(int)edges.size(); i++) {
		g.addClique(edges[i]);
	}
	return g;
}

Graph readBliss(ifstream& input) {
	string line;
	getline(input, line);
	// Skip comments
	while (line[0] == 'c') {
		getline(input, line);
	}

	istringstream lineStream(line);
	// Skip beginning of line ("p edge "), and get numbers of nodes and edges.
	while (lineStream.peek() > '9' || lineStream.peek() < '0') {
		lineStream.get();
	}
	int numberOfNodes, numberOfEdges;
	lineStream >> numberOfNodes >> numberOfEdges;
	// Get clauses and create graph
	Graph g(numberOfNodes);
	for (int i=0; i<numberOfEdges; i++) {
		getline(input, line);
		istringstream lineStream(line);
		while (lineStream.peek() > '9' || lineStream.peek() < '0') {
			lineStream.get();
		}
		NodeSet clique;
		Node v;
		while(lineStream >> v) {
			clique.insert(v-1);
		}
		g.addClique(clique);
	}
	return g;
}

string GetFileExtension(const string& fileName) {
    if(fileName.find_last_of(".") != string::npos)
        return fileName.substr(fileName.find_last_of(".")+1);
    return "";
}

Graph GraphReader::read(const string& fileName) {
	ifstream input (fileName.c_str());
	if (!input.is_open()) {
		cout << "Unable to open file" << endl;
		return Graph();
	}
	string extension = GetFileExtension(fileName);
	if ( extension == "hg" || extension == "sp") {
		return readCliques(input);
	} else if ( extension == "wcnf") {
		return readCnf(input);
	} else if ( extension == "uai" ) {
		return readUAI(input);
	} else if ( extension == "csv" ) {
		return readCSV(input);
	} else if ( extension == "bliss") {
		return readBliss(input);
	}
	cout << "Unrecognized file extension" << endl;
	return Graph();
}

} /* namespace tdenum */

