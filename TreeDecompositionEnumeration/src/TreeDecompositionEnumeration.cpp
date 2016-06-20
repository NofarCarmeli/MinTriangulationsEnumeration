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
	void printDetails() {
		cout << number << ") Time " << time << " millis, Tree-width "
				<< treewidth << ", Fill-in " << fill << endl;
	}
	void printEdges() {
		triangulation.printTriangulation(origin);
		cout << endl;
	}
	void printCsv() {
		cout << number << ", " << time << ", "
				<< treewidth << ", " << fill << endl;
	}
	static void printCsvHeader() {
		cout << "result number, time in millis, tree-width, fill-in" << endl;
	}
};

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "No graph file specified" << endl;
		return 0;
	}

	string fileName = argv[1];
	Graph g = GraphReader::read(fileName);
	clock_t startTime, endTime;
	MinimalTriangulationsEnumerator enumerator(g);

	int resultNumber = 1;
	double totalTime = 0;
	vector<Result> results;
	Result minWidth;
	Result minFill;

	double timeLimitInMillis = 360000;
	bool timeLimitExceeded = false;

	startTime = clock();
	while (enumerator.hasNext()) {
		ChordalGraph triangulation = enumerator.next();

		endTime = clock();
		double lastTime = difftime(endTime, startTime);
		totalTime += lastTime;

		Result res(resultNumber, totalTime, g, triangulation);
		results.push_back(res);

		if (resultNumber == 1) {
			minWidth = res;
			minFill = res;
			Result::printCsvHeader();
			res.printCsv();
		} else {
			if (res.getWidth() < minWidth.getWidth()) {
				minWidth = res;
			}
			if (res.getFill() < minFill.getFill()) {
				minFill = res;
			}
			res.printCsv();
		}

		if (clocksToMillis(totalTime) >= timeLimitInMillis) {
			timeLimitExceeded = true;
			break;
		}

		resultNumber++;
		startTime = clock();
	}

	if (timeLimitExceeded) {
		cout << endl << "Time limit reached" << endl;
	} else {
		cout << endl << "All minimal triangulations were printed!" << endl;
	}
	cout << "First result: ";
	results[0].printDetails();
	cout << "Lowest fill: ";
	minFill.printDetails();
	cout << "Lowest tree-width: ";
	minWidth.printDetails();
	cout << "Total time " << clocksToMillis(totalTime) << " milliseconds" << endl;
	return 0;
}
