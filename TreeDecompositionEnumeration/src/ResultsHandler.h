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
	int resultsFound;
	double getTime() {
		return double(clock() - startTime) / CLOCKS_PER_SEC;
	}
public:
	ResultsHandler(const Graph& g, ostream& o, bool onlyPrintImprovements) :
				graph(g), output(o), onlyPrintImprovements(onlyPrintImprovements), resultsFound(0) {
		startTime = clock();
	}
	void newResult(const ChordalGraph& triangulation) {
		ResultInformation currentResult(++resultsFound, getTime(), graph, triangulation);
		bool print = false;
		if (resultsFound == 1) {
			firstResult = currentResult;
			minWidthResult = currentResult;
			minFillResult = currentResult;
			ResultInformation::printCsvHeaderByTime(output);
			print = true;
		} else {
			if (currentResult.getWidth() < minWidthResult.getWidth()) {
				minWidthResult = currentResult;
				print = true;
			}
			if (currentResult.getFill() < minFillResult.getFill()) {
				minFillResult = currentResult;
				print = true;
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
		output << "Total time " << getTime() << " seconds." << endl;
		output << "First result: ";
		firstResult.printDetails(output);
		output << "Lowest fill: ";
		minFillResult.printDetails(output);
		output << "Lowest width: ";
		minWidthResult.printDetails(output);
	}
};


} /* namespace tdenum */

#endif /* RESULTSHANDLER_H_ */
