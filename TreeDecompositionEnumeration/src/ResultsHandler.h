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

class ResultsHandler {
	Graph graph;
	ostream& output;
	bool onlyPrintImprovements;
	clock_t startTime;
	ResultInformation firstResult;
	ResultInformation minWidthResult;
	ResultInformation minFillResult;
	ResultInformation minBagExpSizeResult;
	int minWidth, maxWidth;
	int minFill, maxFill;
	long long minBagExpSize, maxBagExpSize;
	int firstWidth;
	int resultsWithMinWidthCount;
	int resultsWithGoodWidthCount; // Good = Width at least as good as the first one
	int resultsFound;
	double getTime() {
		return double(clock() - startTime) / CLOCKS_PER_SEC;
	}
public:
	ResultsHandler(const Graph& g, ostream& o, bool onlyPrintImprovements) :
				graph(g), output(o), onlyPrintImprovements(onlyPrintImprovements),
				minWidth(0), maxWidth(0), minFill(0), maxFill(0), minBagExpSize(0),
				maxBagExpSize(0), firstWidth(0), resultsWithMinWidthCount(0),
				resultsWithGoodWidthCount(0), resultsFound(0) {
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
			minFill = maxFill = fill;
			minBagExpSize = maxBagExpSize = bagExpSize;
			resultsWithGoodWidthCount = resultsWithMinWidthCount = 1;
			ResultInformation::printCsvHeaderByTime(output);
			print = true;
		} else {
			if (width <= firstWidth) {
				resultsWithGoodWidthCount++;
			}
			if (width < minWidth) {
				minWidthResult = currentResult;
				minWidth = width;
				resultsWithMinWidthCount = 1;
				print = true;
			} else if (width == minWidth) {
				resultsWithMinWidthCount ++;
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
			if (bagExpSize < minBagExpSize) {
				minBagExpSizeResult = currentResult;
				minBagExpSize = bagExpSize;
				print = true;
			} else if (bagExpSize > maxBagExpSize) {
				maxBagExpSize = bagExpSize;
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
		output << resultsWithMinWidthCount << " results with minimal width, ";
		output << resultsWithGoodWidthCount << " results with width at least as good as the first found." << endl;
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
