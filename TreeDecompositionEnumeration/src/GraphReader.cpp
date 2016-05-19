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

} /* namespace tdenum */

