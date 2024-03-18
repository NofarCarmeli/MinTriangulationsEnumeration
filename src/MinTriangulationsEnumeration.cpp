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
 * The first parameter is the graph file path.
 * Further parameters are optional flags.
 */
int main(int argc, char* argv[]) {
	// Parse input graph file
	if (argc < 2) {
		cout << "No graph file specified" << endl;
		return 0;
	}
	string inputFile = argv[1];
	Graph g = GraphReader::read(inputFile);

	// Define the default parameters
	bool isTimeLimited = false;
	int timeLimitInSeconds = -1;
	TriangulationAlgorithm heuristic = MCS_M;
	TriangulationScoringCriterion triangulationsOrder = NONE;
	SeparatorsScoringCriterion separatorsOrder = UNIFORM;
	OutputForm outputForm = TRIANGULATIONS;

	// Read the parameters specified
	for (int i=2; i<argc; i++) {
		string argument = argv[i];
		string flagName = argument.substr(0, argument.find_last_of("="));
		string flagValue = argument.substr(argument.find_last_of("=")+1);
		if (flagName == "time_limit") {
			timeLimitInSeconds = atoi(flagValue.c_str());
			if (timeLimitInSeconds >= 0) {
				isTimeLimited = true;
			}
		} else if (flagName == "output") {
			if (flagValue == "triangulations") {
				outputForm = TRIANGULATIONS;
			} else if (flagValue == "bags") {
				outputForm = BAGSETS;
			} else {
				cout << "Output form not recognized" << endl;
				return 0;
			}
		} else if (flagName == "alg") {
			if (flagValue == "mcs") {
				heuristic = MCS_M;
			} else if (flagValue == "degree") {
				heuristic = MIN_DEGREE_LB_TRIANG;
			} else if (flagValue == "fill") {
				heuristic = MIN_FILL_LB_TRIANG;
			} else if (flagValue == "initialDegree") {
				heuristic = INITIAL_DEGREE_LB_TRIANG;
			} else if (flagValue == "initialFill") {
				heuristic = INITIAL_FILL_LB_TRIANG;
			} else if (flagValue == "lb") {
				heuristic = LB_TRIANG;
			} else if (flagValue == "combined") {
				heuristic = COMBINED;
			} else if (flagValue == "separators") {
				heuristic = SEPARATORS;
			} else {
				cout << "Triangulation algorithm not recognized" << endl;
				return 0;
			}
		} else if (flagName == "t_order") {
			if (flagValue == "fill") {
				triangulationsOrder = FILL;
			} else if (flagValue == "width") {
				triangulationsOrder = WIDTH;
			} else if (flagValue == "difference") {
				triangulationsOrder = DIFFERENECE;
			} else if (flagValue == "sepsize") {
				triangulationsOrder = MAX_SEP_SIZE;
			} else if (flagValue == "none") {
				triangulationsOrder = NONE;
			} else {
				cout << "Triangulation scoring criterion not recognized" << endl;
				return 0;
			}
		} else if (flagName == "s_order") {
			if (flagValue == "size") {
				separatorsOrder = ASCENDING_SIZE;
			} else if (flagValue == "none") {
				separatorsOrder = UNIFORM;
			} else if (flagValue == "fill") {
				separatorsOrder = FILL_EDGES;
			} else {
				cout << "Seperators scoring criterion not recognized" << endl;
				return 0;
			}
		}
	}

	// Open output file
	ofstream detailedOutput;
	string outputFileName = inputFile + ".out";
	detailedOutput.open(outputFileName.c_str());

	// Initialize variables
	cout << setprecision(2);
	cout << "Starting enumeration for " << inputFile << endl;
	clock_t startTime = clock();
	ResultsHandler results(g, detailedOutput, outputForm);
	bool timeLimitExceeded = false;

	// Generate the results and print details if asked for
	MinimalTriangulationsEnumerator enumerator(g, triangulationsOrder, separatorsOrder, heuristic);
	int i=1;
	while (enumerator.hasNext()) {
		ChordalGraph triangulation = enumerator.next();
		results.newResult(triangulation);
		double totalTimeInSeconds = double(clock() - startTime) / CLOCKS_PER_SEC;
		if (isTimeLimited && totalTimeInSeconds >= timeLimitInSeconds) {
			timeLimitExceeded = true;
			break;
		}
	}
	
	// Close output file
	detailedOutput.close();

	// Print summary to standard output
	if (timeLimitExceeded) {
		cout << "Time limit reached." << endl;
	} else {
		cout << "All minimal triangulations were generated!" << endl;
	}
	results.printReadableSummary(cout);

	return 0;
}
