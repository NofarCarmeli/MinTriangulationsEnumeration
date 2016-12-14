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

class InputFile {
	string name;
	string wholename;
	string innerContainingFolder;
	string outerContainingFolder;
public:
	InputFile(string path) : wholename(path) {
		string::size_type lastSlash = path.find_last_of("\\/");
		if(lastSlash == string::npos) {
			name = path;
			innerContainingFolder = "None";
			outerContainingFolder = "None";
		} else {
			string nameWithExtension = path.substr(lastSlash+1);
			name = nameWithExtension.substr(0, nameWithExtension.find_last_of("."));
			path = path.substr(0, lastSlash);
			lastSlash = path.find_last_of("\\/");
			if(lastSlash == string::npos) {
				innerContainingFolder = "None";
				outerContainingFolder = "None";
			} else {
				innerContainingFolder = path.substr(lastSlash+1);
				path = path.substr(0, lastSlash);
				lastSlash = path.find_last_of("\\/");
				if(lastSlash == string::npos) {
					outerContainingFolder = "None";
				} else {
					outerContainingFolder = path.substr(lastSlash+1);
				}
			}
		}
	}
	string getName(){ return name; }
	string getPath(){ return wholename; }
	string getField(){ return outerContainingFolder; }
	string getType(){ return innerContainingFolder; }
};

void printSummaryHeader(ofstream& summaryOutput) {
	summaryOutput << "Field,Type,Graph,Nodes,Edges,Finished,Time,Algorithm,Separators generated,";
	ResultsHandler::printTableSummaryHeader(summaryOutput);
}

void printSummary(ofstream& summaryOutput, InputFile& input, Graph& graph, bool timeLimitExceeded,
		double time, string algorithm, int separators, ResultsHandler& results) {
	string finished = timeLimitExceeded ? "No" : "Yes";
	summaryOutput << input.getField() << ", " << input.getType() << ", " << input.getName()
			<< ", " << graph.getNumberOfNodes() << ", " << graph.getNumberOfEdges() << ", "
			<< finished << ", " << time << ", " << algorithm
			<< ", " << separators << ", ";
	results.printTableSummary(summaryOutput);
}


/**
 * First parameter is the graph file path. Second is timeout in seconds.
 * Third is the order of extending triangulations. Options are: width, fill,
 * difference, sepsize, none.
 * Fourth is the order of extending minimal separators. Options are: size
 * (ascending), fill or none.
 */
int main(int argc, char* argv[]) {
	// Parse input graph file
	if (argc < 2) {
		cout << "No graph file specified" << endl;
		return 0;
	}
	InputFile inputFile(argv[1]);
	Graph g = GraphReader::read(inputFile.getPath());

	// Define default parameters
	bool isTimeLimited = false;
	int timeLimitInSeconds = -1;
	WhenToPrint print = NEVER;
	string algorithm = "";
	TriangulationAlgorithm heuristic = MCS_M;
	TriangulationScoringCriterion triangulationsOrder = NONE;
	SeparatorsScoringCriterion separatorsOrder = UNIFORM;

	// Replace parameter specified
	for (int i=2; i<argc; i++) {
		string argument = argv[i];
		string flagName = argument.substr(0, argument.find_last_of("="));
		string flagValue = argument.substr(argument.find_last_of("=")+1);
		if (flagName == "time_limit") {
			timeLimitInSeconds = atoi(flagValue.c_str());
			if (timeLimitInSeconds >= 0) {
				isTimeLimited = true;
			}
		} else if (flagName == "print") {
			if (flagValue == "all") {
				print = ALWAYS;
			} else if (flagValue == "none") {
				print = NEVER;
			} else if (flagValue == "improved") {
				print = IF_IMPROVED;
			}
		} else if (flagName == "alg") {
			algorithm = algorithm + "." + flagValue;
			if (flagValue == "mcs") {
				heuristic = MCS_M;
			} else if (flagValue == "degree") {
				heuristic = MIN_DEGREE_LB_TRIANG;
			} else if (flagValue == "fill") {
				heuristic = MIN_FILL_LB_TRIANG;
			}  else if (flagValue == "initialDegree") {
				heuristic = INITIAL_DEGREE_LB_TRIANG;
			} else if (flagValue == "initialFill") {
				heuristic = INITIAL_FILL_LB_TRIANG;
			} else if (flagValue == "lb") {
				heuristic = LB_TRIANG;
			} else if (flagValue == "combined") {
				heuristic = COMBINED;
			} else {
				cout << "Triangulation algorithm not recognized" << endl;
				return 0;
			}
		} else if (flagName == "t_order") {
			algorithm = algorithm + "." + flagValue;
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
			algorithm = algorithm + "." + flagValue;
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

	// Open output files
	ofstream detailedOutput;
	if (print != NEVER) {
		algorithm = algorithm != "" ? algorithm : "mcs";
		string outputFileName = inputFile.getField() + "." + inputFile.getType()
				+ "." + inputFile.getName() + "." + algorithm + ".csv";
		detailedOutput.open(outputFileName.c_str());
	}
	string summaryFileName = "summary.csv";
	ofstream summaryOutput;
	if (!ifstream(summaryFileName.c_str()).good()) { // summary file doesn't exist
		summaryOutput.open(summaryFileName.c_str());
		printSummaryHeader(summaryOutput);
	} else  {
		summaryOutput.open(summaryFileName.c_str(), ios::app);
	}

	// Initialize variables
	cout << setprecision(2);
	cout << "Starting enumeration for " << inputFile.getField() << "\\"
			<< inputFile.getType() << "\\" << inputFile.getName() << endl;
	clock_t startTime = clock();
	ResultsHandler results(g, detailedOutput, print);
	bool timeLimitExceeded = false;

	// Generate the results and print details if asked for
	MinimalTriangulationsEnumerator enumerator(g, triangulationsOrder, separatorsOrder, heuristic);
	while (enumerator.hasNext()) {
		ChordalGraph triangulation = enumerator.next();
		results.newResult(triangulation);
		double totalTimeInSeconds = double(clock() - startTime) / CLOCKS_PER_SEC;
		if (isTimeLimited && totalTimeInSeconds >= timeLimitInSeconds) {
			timeLimitExceeded = true;
			break;
		}
	}
	if (print != NEVER) {
		detailedOutput.close();
	}

	// Print summary to file
	double totalTimeInSeconds = double(clock() - startTime) / CLOCKS_PER_SEC;
	int separators = enumerator.getNumberOfMinimalSeperatorsGenerated();
	printSummary(summaryOutput, inputFile, g, timeLimitExceeded,
			totalTimeInSeconds, algorithm, separators, results);
	summaryOutput.close();

	// Print summary to standard output
	if (timeLimitExceeded) {
		cout << "Time limit reached." << endl;
	} else {
		cout << "All minimal triangulations were generated!" << endl;
	}
	results.printReadableSummary(cout);
	cout << "The graph has " << g.getNumberOfNodes() << " nodes and " << g.getNumberOfEdges() << " edges. ";
	cout << separators << " minimal separators were generated in the process." << endl;

	return 0;
}
