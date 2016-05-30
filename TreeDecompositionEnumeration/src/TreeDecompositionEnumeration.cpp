#include <iostream>
#include <ctime>
#include "GraphReader.h"
#include "MinimalTriangulationsEnumerator.h"
using namespace std;
using namespace tdenum;

void printTimeElapsed(double timeElapsed) {
	cout.precision(2);
	cout << "Computation took " << fixed << (1000.0 * timeElapsed)/CLOCKS_PER_SEC
			<< " milliseconds" << endl;
}

int main() {
	Graph g = GraphReader::read();
	clock_t startTime, endTime;
	MinimalTriangulationsEnumerator enumerator(g);

	int resultNumber = 1;
	double totalTime = 0;

	startTime = clock();
	while (enumerator.hasNext()) {
		ChordalGraph triangulation = enumerator.next();
		endTime = clock();
		double lastTime = difftime(endTime, startTime);
		totalTime += lastTime;

		cout << "Triangulation #" << resultNumber << ":" << endl;
		triangulation.printTriangulation(g);
		printTimeElapsed(lastTime);
		cout << endl;

		resultNumber++;
		startTime = clock();
	}
	cout << "All minimal triangulations were printed!" << endl;
	printTimeElapsed(totalTime);
	return 0;
}
