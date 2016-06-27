#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include "GraphReader.h"
#include "MinimalTriangulationsEnumerator.h"
using namespace std;
using namespace tdenum;


class Result {
	int number;
	double time;
	int fill;
	int width;
	Graph origin;
	ChordalGraph triangulation;
public:
	Result() : number(0), time(0), fill(0), width(0) {}
	Result(int index, double t, Graph g, ChordalGraph triangulation) :
			number(index), time(t), origin(g), triangulation(triangulation) {
		fill = triangulation.getFillIn(g);
		width = triangulation.getTreeWidth();
	}
	int getFill() { return fill; }
	int getWidth() { return width; }
	void printDetails() {
		cout << "Result number " << number << ", Time " << time << " seconds, Width "
				<< width << ", Fill " << fill << endl;
	}
	void printEdges() {
		triangulation.printTriangulation(origin);
		cout << endl;
	}
	void printCsv(ofstream& output) {
		output << number << ", " << time << ", "
				<< width << ", " << fill << endl;
	}
	static void printCsvHeader(ofstream& output) {
		output << "result number, time in seconds, width, fill" << endl;
	}
};

/**
 * First parameter is the graph file path. Second is timeout in seconds.
 * Third is the order of extending triangulations. Options are: width, fill,
 * difference, sepsize, none.
 * Fourth is the order of extending minimal separators. Options are: size
 * (ascending) or none.
 */
int main(int argc, char* argv[]) {
	// Parse input
	if (argc < 2) {
		cout << "No graph file specified" << endl;
		return 0;
	}
	bool isTimeLimited = false;
	int timeLimitInSeconds = -1;
	if (argc >= 3) {
		timeLimitInSeconds = atoi(argv[2]);
		if (timeLimitInSeconds >= 0) {
			isTimeLimited = true;
		}
	}
	TriangulationScoringCriterion criterion = NONE;
	if (argc >=4) {
		string criterionName = argv[3];
		if (criterionName == "fill") {
			criterion = FILL;
		} else if (criterionName == "width") {
			criterion = WIDTH;
		} else if (criterionName == "difference") {
			criterion = DIFFERENECE;
		} else if (criterionName == "sepsize") {
			criterion = MAX_SEP_SIZE;
		} else if (criterionName == "none") {
			criterion = NONE;
		}
	}
	SeparatorsScoringCriterion separatorsOrder = UNIFORM;
	if (argc >=5) {
		string criterionName = argv[4];
		if (criterionName == "size") {
			separatorsOrder = ASCENDING_SIZE;
		} else if (criterionName == "none") {
			separatorsOrder = UNIFORM;
		}
	}

	// Manage files
	clock_t startTime = clock();
	string inputFileName = argv[1];
	Graph g = GraphReader::read(inputFileName);
	string outputFileName = inputFileName + ".output.csv";
	ofstream output;
	output.open(outputFileName.c_str());
	cout << setprecision(2);
	if (double(clock() - startTime) > 0) {
		cout << "Reading the graph took " << double(clock() - startTime) / CLOCKS_PER_SEC
				<< " seconds" << endl;
	}
	cout << "Starting enumeration for "  << inputFileName << endl;

	// initialize variables
	int resultNumber = 1;
	double totalTimeInSeconds = 0;
	vector<Result> results;
	Result minWidth, minFill;
	bool timeLimitExceeded = false;
	startTime = clock();

	// generate and print the results to output file
	MinimalTriangulationsEnumerator enumerator(g, criterion, separatorsOrder);
	while (enumerator.hasNext()) {
		ChordalGraph triangulation = enumerator.next();

		totalTimeInSeconds = double(clock() - startTime) / CLOCKS_PER_SEC;
		Result res(resultNumber, totalTimeInSeconds, g, triangulation);
		results.push_back(res);
		if (resultNumber == 1) {
			minWidth = res;
			minFill = res;
			Result::printCsvHeader(output);
		} else {
			if (res.getWidth() < minWidth.getWidth()) {
				minWidth = res;
			}
			if (res.getFill() < minFill.getFill()) {
				minFill = res;
			}
		}
		res.printCsv(output);
		resultNumber++;

		if (isTimeLimited && totalTimeInSeconds >= timeLimitInSeconds) {
			timeLimitExceeded = true;
			break;
		}
	}
	output.close();

	// Output summary to standard output
	if (timeLimitExceeded) {
		cout << "Time limit reached, ";
	} else {
		cout << "All minimal triangulations were generated! ";
	}
	cout << resultNumber-1 << " results found, ";
	cout << "Total time " << totalTimeInSeconds << " seconds." << endl;
	cout << "First result: ";
	results[0].printDetails();
	cout << "Lowest fill: ";
	minFill.printDetails();
	cout << "Lowest width: ";
	minWidth.printDetails();

	return 0;
}
