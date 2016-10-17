/*
 * DataStructures.cpp
 *
 *  Created on: Oct 17, 2016
 *      Author: tmp
 */

#include "DataStructures.h"

namespace tdenum {

IncreasingWeightNodeQueue::IncreasingWeightNodeQueue(int numberOfNodes) :
		weight(numberOfNodes, 0) {
	vector< pair<int,Node> > temporaryQueue;
	for (Node v = 0; v<numberOfNodes; v++) {
		temporaryQueue.push_back(pair<int,Node> (0,v));
	}
	queue = set< pair<int,Node> >(temporaryQueue.begin(), temporaryQueue.end());
}

void IncreasingWeightNodeQueue::increaseWeight(Node v) {
	queue.erase(pair<int,Node> (weight[v],v));
	weight[v]++;
	queue.insert(pair<int,Node> (weight[v],v));
}

int IncreasingWeightNodeQueue::getWeight(Node v) {
	return weight[v];
}

bool IncreasingWeightNodeQueue::isEmpty() {
	return queue.empty();
}

Node IncreasingWeightNodeQueue::pop() {
	pair<int,Node> current = *queue.rbegin();
	Node v = current.second;
	queue.erase(current);
	return v;
}


NodeSetProducer::NodeSetProducer(int sizeOfOriginalNodeSet) :
		isMember(sizeOfOriginalNodeSet, false){}

void NodeSetProducer::insert(Node v) {
	isMember[v] = true;
}

void NodeSetProducer::remove(Node v) {
	isMember[v] = false;
}

set<Node> NodeSetProducer::produce() {
	vector<Node> members;
	for (unsigned int i=0; i<isMember.size(); i++) {
		if (isMember[i]) {
			members.push_back(i);
		}
	}
	return set<Node> (members.begin(), members.end());
}

} /* namespace tdenum */
