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
	summaryOutput << "Field, Type, Graph, Nodes, Edges, Finished, Time, Algorithm, Separators generated, ";
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
	// Parse input
	if (argc < 2) {
		cout << "No graph file specified" << endl;
		return 0;
	}
	InputFile inputFile(argv[1]);
	bool isTimeLimited = false;
	int timeLimitInSeconds = -1;
	if (argc >= 3) {
		timeLimitInSeconds = atoi(argv[2]);
		if (timeLimitInSeconds >= 0) {
			isTimeLimited = true;
		}
	}
	string algorithm = "mcs";
	TriangulationAlgorithm heuristic = MCS_M;
	if (argc >=4) {
		string heuristicName = argv[3];
		algorithm = heuristicName;
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
		} else if (heuristicName == "combined") {
			heuristic = COMBINED;
		} else {
			cout << "Triangulation algorithm not recognized" << endl;
			return 0;
		}
	}
	TriangulationScoringCriterion criterion = NONE;
	if (argc >=5) {
		string criterionName = argv[4];
		algorithm = algorithm + "." + criterionName;
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
		algorithm = algorithm + "." + criterionName;
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
	string outputFileName = inputFile.getField() + "." + inputFile.getType()
			+ "." + inputFile.getName() + "." + algorithm + ".csv";

	// Manage files and initialize variables
	Graph g = GraphReader::read(inputFile.getPath());
	ofstream detailedOutput;
	detailedOutput.open(outputFileName.c_str());
	string summaryFileName = "summary.csv";
	ofstream summaryOutput;
	if (!ifstream(summaryFileName.c_str()).good()) { // summary file doesn't exist
		summaryOutput.open(summaryFileName.c_str());
		printSummaryHeader(summaryOutput);
	} else  {
		summaryOutput.open(summaryFileName.c_str(), ios::app);
	}
	cout << setprecision(2);
	cout << "Starting enumeration for " << inputFile.getField() << "\\"
			<< inputFile.getType() << "\\" << inputFile.getName() << endl;
	clock_t startTime = clock();
	ResultsHandler results(g, detailedOutput, NEVER);
	bool timeLimitExceeded = false;

	// generate and print the results to detailedOutput file
	MinimalTriangulationsEnumerator enumerator(g, criterion, separatorsOrder, heuristic);
	while (enumerator.hasNext()) {
		ChordalGraph triangulation = enumerator.next();
		results.newResult(triangulation);
		double totalTimeInSeconds = double(clock() - startTime) / CLOCKS_PER_SEC;
		if (isTimeLimited && totalTimeInSeconds >= timeLimitInSeconds) {
			timeLimitExceeded = true;
			break;
		}
	}
	detailedOutput.close();

	// Print summary to file
	double totalTimeInSeconds = double(clock() - startTime) / CLOCKS_PER_SEC;
	int separators = enumerator.getNumberOfMinimalSeperatorsGenerated();
	printSummary(summaryOutput, inputFile, g, timeLimitExceeded, totalTimeInSeconds, algorithm, separators, results);
	// Output summary to standard detailedOutput
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
