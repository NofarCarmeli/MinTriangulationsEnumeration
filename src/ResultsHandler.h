/*
 * ResultsHandler.h
 */

#ifndef RESULTSHANDLER_H_
#define RESULTSHANDLER_H_

#include <ostream>
#include <ctime>
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
	void printSummary(ostream& output) {
		output << "#" << number << " (width " << width << ", fill " << fill
				<< ", sum of exponents of bag sizes " << expBagSize << "), obtained after " << time << " seconds." << endl;
	}
};

enum OutputForm { TRIANGULATIONS, BAGSETS };

class ResultsHandler {
	Graph inputGraph;
	ostream& output;
	OutputForm outputForm;
	clock_t startTime;
	ResultInformation minWidthResult;
	ResultInformation minFillResult;
	ResultInformation minBagExpSizeResult;
	int minWidth, maxWidth;
	int minFill, maxFill;
	long long minBagExpSize, maxBagExpSize;
	int resultsFound;
	double getTime() {
		return double(clock() - startTime) / CLOCKS_PER_SEC;
	}
public:
	ResultsHandler(const Graph& g, ostream& o, OutputForm f) :
				inputGraph(g), output(o), outputForm(f),
				minWidth(0), maxWidth(0), minFill(0), maxFill(0), minBagExpSize(0),
				maxBagExpSize(0), resultsFound(0) {
		startTime = clock();
	}
	void newResult(const ChordalGraph& triangulation) {
		ResultInformation currentResult(++resultsFound, getTime(), inputGraph, triangulation);
		int width = currentResult.getWidth();
		int fill = currentResult.getFill();
		long long bagExpSize = currentResult.getExpBagSize();
		
		// store metrics ranges
		if (resultsFound == 1) {
			minBagExpSizeResult = minFillResult = minWidthResult = currentResult;
			minWidth = maxWidth = width;
			minFill = maxFill = fill;
			minBagExpSize = maxBagExpSize = bagExpSize;
		} else {
			if (width < minWidth) {
				minWidthResult = currentResult;
				minWidth = width;
			} else if (width > maxWidth) {
				maxWidth = width;
			}
			if (fill < minFill) {
				minFillResult = currentResult;
				minFill = fill;
			} else if (fill > maxFill) {
				maxFill = fill;
			}
			if (bagExpSize < minBagExpSize) {
				minBagExpSizeResult = currentResult;
				minBagExpSize = bagExpSize;
			} else if (bagExpSize > maxBagExpSize) {
				maxBagExpSize = bagExpSize;
			}
		}

		// Print triangulation
		currentResult.printSummary(output);
		if (outputForm == TRIANGULATIONS) {
			triangulation.printTriangulation(output, inputGraph);
		} else if (outputForm == BAGSETS) {
			triangulation.printMaximalCliques(output);
		}
		output << " " << endl;
	}
	
	void printReadableSummary(ostream& output) {
		// Print count and metrics ranges
		output << resultsFound << " triangulation were found in " << getTime() << " seconds (";
		if (minWidth == maxWidth) {
			output << "Width " << minWidth << ", ";
		} else {
			output << "Width " << minWidth << "-" << maxWidth << ", ";
		}
		if (minFill == maxFill) {
			output << "fill " << minFill << ", ";
		} else {
			output << "fill " << minFill << "-" << maxFill << ", ";
		}
		if (minBagExpSize == maxBagExpSize) {
			output << "sum of exponents of bag sizes " << minBagExpSize << ".";
		} else {
			output << "sum of exponents of bag sizes " << minBagExpSize
					<< "-" << maxBagExpSize << ").";
		}
		output <<endl;

		// Recommend triangulations
		if (resultsFound > 1) {
			output << "Recommended results:" << endl;
			minBagExpSizeResult.printSummary(output);
			if (minFill < minBagExpSizeResult.getFill()) {
				minFillResult.printSummary(output);
			}
			if (minWidth < minBagExpSizeResult.getWidth()) {
				minWidthResult.printSummary(output);
			}
		}
	}
};


} /* namespace tdenum */

#endif /* RESULTSHANDLER_H_ */
