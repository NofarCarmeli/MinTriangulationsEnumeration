#include "GraphReader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <utility>

using namespace std;

namespace tdenum {

map<int,string> getIdentityMapping(int size) {
	map<int,string> m;
	for (int i=0; i<size; i++) {
		m[i]=to_string(i);
	}
	return m;
}

pair<Graph,map<int,string> > readCliques(ifstream& input) {
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
		vector<Node> clique;
		Node v;
		while(lineStream >> v) {
			clique.push_back(v);
		}
		g.addClique(clique);
	}
	return pair<Graph,map<int,string> > {g,getIdentityMapping(numberOfNodes)};
}

pair<Graph,map<int,string> > readCnf(ifstream& input) {
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
		return pair<Graph,map<int,string> > ();
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
		vector<Node> clique;
		Node v;
		while(lineStream >> v && v!=0) {
			v = v>0 ? v : -v;
			v = v == numberOfNodes ? 0 : v;
			clique.push_back(v);
		}
		g.addClique(clique);
	}
	return pair<Graph,map<int,string> > {g,getIdentityMapping(numberOfNodes)};
}

pair<Graph,map<int,string> > readUAI(ifstream& input) {
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
		vector<Node> clique;
		Node v;
		while(lineStream >> v) {
			clique.push_back(v);
		}
		g.addClique(clique);
	}
	return pair<Graph,map<int,string> > {g,getIdentityMapping(numberOfNodes)};
}

pair<Graph,map<int,string> > readCSV(ifstream& input, char separator) {
	map <string,int> nodeNameToNumber;
	map <int,string> nodeNumberToName;
	vector < vector<int> > edges;
	// Get nodes and edges
	string line;
	while (getline(input, line)) {
		istringstream lineStream(line);
		string nodeName;
		vector<int> edge;
		while (getline(lineStream, nodeName, separator)) {
			if ( nodeName.size() && nodeName[nodeName.size()-1] == '\r' ) {
				nodeName = nodeName.substr( 0, nodeName.size() - 1 );
			}
			map<string,int>::iterator nameSearchResult = nodeNameToNumber.find(nodeName);
			int nodeNumber;
			if (nameSearchResult == nodeNameToNumber.end()) {
				nodeNumber = nodeNameToNumber.size();
				nodeNameToNumber[nodeName] = nodeNumber;
				nodeNumberToName[nodeNumber] = nodeName;
			} else {
				nodeNumber = nameSearchResult->second;
			}
			edge.push_back(nodeNumber);
		}
		edges.push_back(edge);
	}
	// Construct graph
	Graph g(nodeNameToNumber.size());
	for (int i=0; i<(int)edges.size(); i++) {
		g.addClique(edges[i]);
	}
	return pair<Graph,map<int,string> > {g,nodeNumberToName};
}

pair<Graph,map<int,string> > readBliss(ifstream& input) {
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
		vector<Node> clique;
		Node v;
		while(lineStream >> v) {
			clique.push_back(v-1);
		}
		g.addClique(clique);
	}
	return pair<Graph,map<int,string> > {g,getIdentityMapping(numberOfNodes)};
}

string GetFileExtension(const string& fileName) {
    if(fileName.find_last_of(".") != string::npos)
        return fileName.substr(fileName.find_last_of(".")+1);
    return "";
}

pair<Graph,map<int,string> > GraphReader::read(const string& fileName) {
	ifstream input (fileName.c_str());
	if (!input.is_open()) {
		cout << "Unable to open file" << endl;
		return pair<Graph,map<int,string> > ();;
	}
	string extension = GetFileExtension(fileName);
	if ( extension == "hg" || extension == "sp") {
		return readCliques(input);
	} else if ( extension == "wcnf") {
		return readCnf(input);
	} else if ( extension == "uai" ) {
		return readUAI(input);
	} else if ( extension == "csv" ) {
		return readCSV(input, ',');
	} else if ( extension == "txt" ) {
		return readCSV(input, ' ');
	} else if ( extension == "bliss") {
		return readBliss(input);
	}
	cout << "Unrecognized file extension" << endl;
	return pair<Graph,map<int,string> > ();
}

} /* namespace tdenum */

