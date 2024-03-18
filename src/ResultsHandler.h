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
	void printDetails(ostream& output) {
		output << "Result number " << number << ", Time " << time
				<< " seconds, Width " << width << ", Fill " << fill
				<< ", Exponential Bag Size " << expBagSize << "." << endl;
	}
	void printCsvByTime(ostream& output) {
		output << time << ", "
				<< width << ", " << fill << ", " << expBagSize << endl;
	}
	static void printCsvHeaderByTime(ostream& output) {
		output << "time(seconds), width, fill, exponential bags size" << endl;
	}
};

enum WhenToPrint { NEVER, ALWAYS, IF_IMPROVED };

class ResultsHandler {
	Graph graph;
	ostream& output;
	WhenToPrint whenToPrint;
	clock_t startTime;
	ResultInformation firstResult;
	ResultInformation minWidthResult;
	ResultInformation minFillResult;
	ResultInformation minBagExpSizeResult;
	int minWidth, maxWidth;
	int minFill, maxFill;
	long long minBagExpSize, maxBagExpSize;
	int firstWidth, firstFill;
	int minWidthCount, minFillCount;
	int goodWidthCount, goodFillCount; // good = at least as good as the first one
	int resultsFound;
	double getTime() {
		return double(clock() - startTime) / CLOCKS_PER_SEC;
	}
public:
	ResultsHandler(const Graph& g, ostream& o, WhenToPrint p) :
				graph(g), output(o), whenToPrint(p),
				minWidth(0), maxWidth(0), minFill(0), maxFill(0), minBagExpSize(0),
				maxBagExpSize(0), firstWidth(0), firstFill(0), minWidthCount(0),
				minFillCount(0), goodWidthCount(0), goodFillCount(0), resultsFound(0) {
		startTime = clock();
	}
	void newResult(const ChordalGraph& triangulation) {
		ResultInformation currentResult(++resultsFound, getTime(), graph, triangulation);
		bool print = false;
		int width = currentResult.getWidth();
		int fill = currentResult.getFill();
		long long bagExpSize = currentResult.getExpBagSize();
		if (resultsFound == 1) {
			minBagExpSizeResult = minFillResult = minWidthResult = firstResult = currentResult;
			firstWidth = minWidth = maxWidth = width;
			firstFill = minFill = maxFill = fill;
			minBagExpSize = maxBagExpSize = bagExpSize;
			goodWidthCount = minWidthCount = 1;
			goodFillCount = minFillCount = 1;
			ResultInformation::printCsvHeaderByTime(output);
			print = true;
		} else {
			if (width <= firstWidth) {
				goodWidthCount++;
			}
			if (width < minWidth) {
				minWidthResult = currentResult;
				minWidth = width;
				minWidthCount = 1;
				print = true;
			} else if (width == minWidth) {
				minWidthCount ++;
			} else if (width > maxWidth) {
				maxWidth = width;
			}
			if (fill <= firstFill) {
				goodFillCount++;
			}
			if (fill < minFill) {
				minFillResult = currentResult;
				minFill = fill;
				minFillCount = 1;
				print = true;
			} else if (fill == minFill) {
				minFillCount ++;
			}  else if (fill > maxFill) {
				maxFill = fill;
			}
			if (bagExpSize < minBagExpSize) {
				minBagExpSizeResult = currentResult;
				minBagExpSize = bagExpSize;
				print = true;
			} else if (bagExpSize > maxBagExpSize) {
				maxBagExpSize = bagExpSize;
			}
		}
		if (whenToPrint == ALWAYS) {
			print = true;
		} else if (whenToPrint == NEVER){
			print = false;
		}
		if (print) {
			currentResult.printCsvByTime(output);
		}
	}
	static void printTableSummaryHeader(ostream& output) {
		output << "Results,First Width,Min Width,Max Width,Best Width Time,";
		output << "Best Width Count,Good width Count,First Fill,Min Fill,";
		output << "Max Fill,Best Fill Time,Best Fill Count,Good Fill Count,";
		output << "First ExpBags,Min ExpBags,Max ExpBags,Best ExpBags Time" << endl;
	}
	void printTableSummary(ostream& output) {
		output << resultsFound << "," << firstWidth << "," << minWidth << ","
				<< maxWidth << "," << minWidthResult.getTime() << ","
				<< minWidthCount << "," << goodWidthCount
				<< "," << firstFill << "," << minFill << ","
				<< maxFill << "," << minFillResult.getTime() << ","
				<< minFillCount << "," << goodFillCount << ","
				<< firstResult.getExpBagSize() << "," << minBagExpSize
				<< "," << maxBagExpSize << "," << minBagExpSizeResult.getTime() << endl;
	}
	void printReadableSummary(ostream& output) {
		output << resultsFound << " results found, ";
		output << "total time " << getTime() << " seconds." << endl;
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
			output << "exponential bags size " << minBagExpSize << ".";
		} else {
			output << "exponential bags size " << minBagExpSize
					<< "-" << maxBagExpSize << ".";
		}
		output <<endl;
		output << minWidthCount << " results with minimal width, ";
		output << goodWidthCount << " results with width at least as good as the first found." << endl;
		if (resultsFound > 1) {
			output << "First result: ";
			firstResult.printDetails(output);
			output << "Lowest fill: ";
			minFillResult.printDetails(output);
			output << "Lowest width: ";
			minWidthResult.printDetails(output);
			output << "Lowest exp.b.size: ";
			minBagExpSizeResult.printDetails(output);
		}
	}
};


} /* namespace tdenum */

#endif /* RESULTSHANDLER_H_ */
