#include <iostream>
#include <ctime>
#include "GraphReader.h"
#include "MinimalTriangulationsEnumerator.h"
using namespace std;
using namespace tdenum;

double clocksToMillis(double timeElapsed) {
	return (1000.0 * timeElapsed)/CLOCKS_PER_SEC;
}

double millisToclocks(double time) {
	return (time * CLOCKS_PER_SEC) / 1000.0;
}

class Result {
	int number;
	double time;
	int fill;
	int treewidth;
	Graph origin;
	ChordalGraph triangulation;
public:
	Result() {}
	Result(int index, double t, Graph g, ChordalGraph triangulation) :
			number(index), origin(g), triangulation(triangulation) {
		time = clocksToMillis(t);
		fill = triangulation.getFillIn(g);
		treewidth = triangulation.getTreeWidth();
	}
	int getFill() { return fill; }
	int getWidth() { return treewidth; }
	void print() {
		cout << number << ") Time " << time << " millis, Tree-width "
				<< treewidth << ", Fill-in " << fill << endl;
		//triangulation.printTriangulation(origin); cout << endl;
	}
};

int main() {
	Graph g = GraphReader::read();
	clock_t startTime, endTime;
	MinimalTriangulationsEnumerator enumerator(g);

	int resultNumber = 1;
	double totalTime = 0;
	vector<Result> results;
	Result minWidth;
	Result minFill;

	double timeLimitInMillis = 10000;
	bool timeLimitExceeded = false;

	startTime = clock();
	while (enumerator.hasNext()) {
		ChordalGraph triangulation = enumerator.next();

		endTime = clock();
		double lastTime = difftime(endTime, startTime);
		totalTime += lastTime;

		Result res(resultNumber, totalTime, g, triangulation);
		results.push_back(res);
		res.print();

		if (resultNumber == 1) {
			minWidth = res;
			minFill = res;
		} else {
			if (res.getWidth() < minWidth.getWidth()) {
				minWidth = res;
			}
			if (res.getFill() < minFill.getFill()) {
				minFill = res;
			}
		}

		if (clocksToMillis(totalTime) >= timeLimitInMillis) {
			timeLimitExceeded = true;
			break;
		}

		resultNumber++;
		startTime = clock();
	}

	if (timeLimitExceeded) {
		cout << "Time limit reached" << endl << endl;
	} else {
		cout << "All minimal triangulations were printed!" << endl << endl;
	}
	cout << "First result: ";
	results[0].print();
	cout << "Lowest fill: ";
	minFill.print();
	cout << "Lowest tree-width: ";
	minWidth.print();
	cout << "Total time " << clocksToMillis(totalTime) << " milliseconds" << endl;
	return 0;
}
