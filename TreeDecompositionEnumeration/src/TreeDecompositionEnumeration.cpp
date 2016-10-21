#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <string>
#include "GraphReader.h"
#include "MinimalTriangulationsEnumerator.h"
#include "ResultsHandler.h"
using namespace std;
using namespace tdenum;


/**
 * First parameter is the graph file path. Second is timeout in seconds.
 * Third is the order of extending triangulations. Options are: width, fill,
 * difference, sepsize, none.
 * Fourth is the order of extending minimal separators. Options are: size
 * (ascending), fill or none.
 */
int main(int argc, char* argv[]) {
	// Parse input
	if (argc < 2) {
		cout << "No graph file specified" << endl;
		return 0;
	}
	string inputFileName = argv[1];
	string outputFileName = inputFileName + ".output";
	bool isTimeLimited = false;
	int timeLimitInSeconds = -1;
	if (argc >= 3) {
		timeLimitInSeconds = atoi(argv[2]);
		if (timeLimitInSeconds >= 0) {
			isTimeLimited = true;
		}
	}
	TriangulationAlgorithm heuristic = MCS_M;
	if (argc >=4) {
		string heuristicName = argv[3];
		outputFileName = outputFileName + "." + heuristicName;
		if (heuristicName == "mcs") {
			heuristic = MCS_M;
		} else if (heuristicName == "degree") {
			heuristic = MIN_DEGREE_LB_TRIANG;
		} else if (heuristicName == "fill") {
			heuristic = MIN_FILL_LB_TRIANG;
		}  else if (heuristicName == "initialDegree") {
			heuristic = INITIAL_DEGREE_LB_TRIANG;
		} else if (heuristicName == "initialFill") {
			heuristic = INITIAL_FILL_LB_TRIANG;
		} else if (heuristicName == "lb") {
			heuristic = LB_TRIANG;
		} else {
			cout << "Triangulation algorithm not recognized" << endl;
			return 0;
		}
	}
	TriangulationScoringCriterion criterion = NONE;
	if (argc >=5) {
		string criterionName = argv[4];
		outputFileName = outputFileName + "." + criterionName;
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
		} else {
			cout << "Triangulation scoring criterion not recognized" << endl;
			return 0;
		}
	}
	SeparatorsScoringCriterion separatorsOrder = UNIFORM;
	if (argc >=6) {
		string criterionName = argv[5];
		outputFileName = outputFileName + "." + criterionName;
		if (criterionName == "size") {
			separatorsOrder = ASCENDING_SIZE;
		} else if (criterionName == "none") {
			separatorsOrder = UNIFORM;
		} else if (criterionName == "fill") {
			separatorsOrder = FILL_EDGES;
		} else {
			cout << "Seperators scoring criterion not recognized" << endl;
			return 0;
		}
	}
	outputFileName = outputFileName + ".csv";

	// Manage files and initialize variables
	Graph g = GraphReader::read(inputFileName);
	ofstream output;
	output.open(outputFileName.c_str());
	cout << setprecision(2);
	cout << "Starting enumeration for "  << inputFileName << endl;
	clock_t startTime = clock();
	ResultsHandler results(g, output, true);
	bool timeLimitExceeded = false;

	// generate and print the results to output file
	MinimalTriangulationsEnumerator enumerator(g, criterion, separatorsOrder, heuristic);
	while (enumerator.hasNext()) {
		ChordalGraph triangulation = enumerator.next();
		results.newResult(triangulation);
		double totalTimeInSeconds = double(clock() - startTime) / CLOCKS_PER_SEC;
		if (isTimeLimited && totalTimeInSeconds >= timeLimitInSeconds) {
			timeLimitExceeded = true;
			vector<ChordalGraph> moreResults = enumerator.getGeneratedNotReturned();
			for (unsigned int i=0; i<moreResults.size(); i++) {
				results.newResult(moreResults[i]);
			}
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
	results.printSummary(cout);

	return 0;
}
