/*
 * ResultsHandler.h
 *
 *  Created on: Oct 21, 2016
 *      Author: tmp
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
public:
	ResultInformation() : number(0), time(0), fill(0), width(0) {}
	ResultInformation(int index, double time, const Graph& input, const ChordalGraph& result) :
			number(index), time(time) {
		fill = result.getFillIn(input);
		width = result.getTreeWidth();
	}
	int getFill() { return fill; }
	int getWidth() { return width; }
	void printDetails(ostream& output) {
		output << "Result number " << number << ", Time " << time << " seconds, Width "
				<< width << ", Fill " << fill << endl;
	}
	void printCsvByTime(ostream& output) {
		output << time << ", "
				<< width << ", " << fill << endl;
	}
	static void printCsvHeaderByTime(ostream& output) {
		output << "time(seconds), width, fill" << endl;
	}
};

class ResultsHandler {
	Graph graph;
	ostream& output;
	bool onlyPrintImprovements;
	clock_t startTime;
	ResultInformation firstResult;
	ResultInformation minWidthResult;
	ResultInformation minFillResult;
	int minWidth, maxWidth;
	int minFill, maxFill;
	int resultsFound;
	double getTime() {
		return double(clock() - startTime) / CLOCKS_PER_SEC;
	}
public:
	ResultsHandler(const Graph& g, ostream& o, bool onlyPrintImprovements) :
				graph(g), output(o), onlyPrintImprovements(onlyPrintImprovements),
				minWidth(0), maxWidth(0), minFill(0), maxFill(0), resultsFound(0) {
		startTime = clock();
	}
	void newResult(const ChordalGraph& triangulation) {
		ResultInformation currentResult(++resultsFound, getTime(), graph, triangulation);
		bool print = false;
		int width = currentResult.getWidth();
		int fill = currentResult.getFill();
		if (resultsFound == 1) {
			firstResult = currentResult;
			minWidthResult = currentResult;
			minFillResult = currentResult;
			minWidth = maxWidth = width;
			minFill = maxFill = fill;
			ResultInformation::printCsvHeaderByTime(output);
			print = true;
		} else {
			if (width < minWidth) {
				minWidthResult = currentResult;
				minWidth = width;
				print = true;
			} else if (width > maxWidth) {
				maxWidth = width;
			}
			if (fill < minFill) {
				minFillResult = currentResult;
				minFill = fill;
				print = true;
			} else if (fill > maxFill) {
				maxFill = fill;
			}
		}
		if (!onlyPrintImprovements) {
			print = true;
		}
		if (print) {
			currentResult.printCsvByTime(output);
		}
	}
	void printSummary(ostream& output) {
		output << resultsFound << " results found, ";
		if (minWidth == maxWidth) {
			output << "width " << minWidth << ", ";
		} else {
			output << "width " << minWidth << "-" << maxWidth << ", ";
		}
		if (minFill == maxFill) {
			output << "fill " << minFill << ", ";
		} else {
			output << "fill " << minFill << "-" << maxFill << ", ";
		}
		output << "total time " << getTime() << " seconds." << endl;
		if (resultsFound > 1) {
			output << "First result: ";
			firstResult.printDetails(output);
			output << "Lowest fill: ";
			minFillResult.printDetails(output);
			output << "Lowest width: ";
			minWidthResult.printDetails(output);
		}
	}
};


} /* namespace tdenum */

#endif /* RESULTSHANDLER_H_ */
