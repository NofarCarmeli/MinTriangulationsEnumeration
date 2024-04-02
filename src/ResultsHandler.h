/*
 * ResultsHandler.h
 */

#ifndef RESULTSHANDLER_H_
#define RESULTSHANDLER_H_

#include <ostream>
#include <ctime>
#include <map>
#include "ChordalGraph.h"
using namespace std;

namespace tdenum {

class ResultInformation {
	int number;
	double time;
	int fill;
	int width;
	long long expBagSize;
public:
	ResultInformation() : number(0), time(0), fill(0), width(0), expBagSize(0) {}
	ResultInformation(int index, double time, const Graph& input, const ChordalGraph& result) :
			number(index), time(time) {
		fill = result.getFillIn(input);
		width = result.getTreeWidth();
		expBagSize = result.getExpBagsSize();
	}
	int getTime() { return time; }
	int getFill() { return fill; }
	int getWidth() { return width; }
	long long getExpBagSize() { return expBagSize; }
	void printSummary(ostream& output);
};

enum OutputForm { TRIANGULATIONS, BAGSETS, DECOMPOSITIONS };

class ResultsHandler {
	Graph inputGraph;
	ostream& output;
	OutputForm outputForm;
	clock_t startTime;
	ResultInformation minWidthResult;
	ResultInformation minFillResult;
	ResultInformation minBagExpSizeResult;
	map<int,string> inputNaming;
	int minWidth, maxWidth;
	int minFill, maxFill;
	long long minBagExpSize, maxBagExpSize;
	int resultsFound;
	double getTime() {
		return double(clock() - startTime) / CLOCKS_PER_SEC;
	}
public:
	ResultsHandler(const Graph& g, ostream& o, OutputForm f, map<int,string> n ) :
				inputGraph(g), output(o), outputForm(f), inputNaming(n),
				minWidth(0), maxWidth(0), minFill(0), maxFill(0), minBagExpSize(0),
				maxBagExpSize(0), resultsFound(0) {
		startTime = clock();
	}
	void newResult(const ChordalGraph& triangulation);
	void printReadableSummary(ostream& output);
};


} /* namespace tdenum */

#endif /* RESULTSHANDLER_H_ */
