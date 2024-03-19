/*
 * ResultsHandler.cpp
 */

#include "ResultsHandler.h"
#include <ostream>
#include <ctime>
using namespace std;

namespace tdenum {

	void ResultInformation::printSummary(ostream& output) {
		output << "#" << number << " (width " << width << ", fill " << fill
				<< ", sum of exponents of bag sizes " << expBagSize << "), obtained after " << time << " seconds." << endl;
	}

	void ResultsHandler::newResult(const ChordalGraph& triangulation) {
		ResultInformation currentResult(++resultsFound, getTime(), inputGraph, triangulation);
		int width = currentResult.getWidth();
		int fill = currentResult.getFill();
		long long bagExpSize = currentResult.getExpBagSize();
		
		// maintain metrics ranges
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

		// Print result
		currentResult.printSummary(output);
		if (outputForm == TRIANGULATIONS) {
			// Print fill edges
			vector< set<Node> > fillEdges = triangulation.getFillEdges(inputGraph);
			for (vector< set<Node> >::iterator it=fillEdges.begin(); it!=fillEdges.end(); ++it) {
				Node u = *(it->begin());
				Node v = *(++it->begin());
				output << inputNaming[u] << " " << inputNaming[v] << endl;
			}
		} else if (outputForm == BAGSETS) {
			// Print bags
			set<NodeSet> maximalCliques = triangulation.getMaximalCliques();
			for (set<NodeSet>::const_iterator it=maximalCliques.begin(); it!=maximalCliques.end(); ++it) {
				for (NodeSet::const_iterator jt = it->begin(); jt!=it->end(); ++jt) {
					output << inputNaming[*jt] << " ";
				}
				output << endl;
			}
		}
		output << " " << endl;
	}

	void ResultsHandler::printReadableSummary(ostream& output) {
		// Print count and metrics ranges
		output << resultsFound << " triangulations were found in " << getTime() << " seconds (";
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

		// Recommend results
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

} /* namespace tdenum */
