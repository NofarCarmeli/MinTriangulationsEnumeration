#include <iostream>
#include "Graph.h"
#include "MinimalSeparatorsEnumerator.h"
#include "SeparatorGraph.h"
#include "MinimalTriangulationsEnumerator.h"
#include "IndependentSetExtender.h"
#include "converter.h"
#include "GraphReader.h"
#include "IndSetExtByTriangulation.h"
using namespace std;
using namespace tdenum;

int main() {
	Graph g = GraphReader::read();
	MinimalTriangulationsEnumerator enumerator(g);

	int resultNumber = 1;
	while (enumerator.hasNext()) {
		cout << "Triangulation #" << resultNumber << ":" << endl;
		enumerator.next().print();
		resultNumber++;
		cout << endl;
	}
	cout << "All minimal triangulations were printed!" << endl;
	return 0;
}
