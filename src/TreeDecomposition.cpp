#include "TreeDecomposition.h"
#include "DataStructures.h"

namespace tdenum {

    // assumes that NodeSet contains a sorted vector
    int getIntersectionSize(const NodeSet& b1, const NodeSet& b2) {
        int weight = 0;
        auto it1 = b1.begin();
        auto it2 = b2.begin();
        while (it1!=b1.end() && it2!=b2.end()) {
            if (*it1<*it2) ++it1;
            else if (*it2<*it1) ++it2;
            else {
                weight++;
                ++it1; ++it2;
            }
        }
        return weight;
    }

    TreeDecomposition::TreeDecomposition() {}
	TreeDecomposition::TreeDecomposition(const set<NodeSet>& inputBags) : bags() {
        if (inputBags.size() == 0) return;
        for (auto b : inputBags) {
            bags.push_back(b);
        }
        // Construct edges between bags using Prim's algorithm for MST
        vector<bool> covered(bags.size(),false);
        WeightedNodeSetQueue queue;
        int b1 = 0;
        covered[b1]=true;
        for (int b2=1; b2<bags.size(); b2++) {
            queue.insert(vector<int>{b1,b2}, -getIntersectionSize(bags[b1],bags[b2]));
        }
        while(!queue.isEmpty()) {
            vector<int> e = queue.pop();
            for (int b1 : e) {
                if (!covered[b1]) {
                    edges.push_back(e);
                    covered[b1]=true;
                    for (int b2=0; b2<bags.size(); b2++) {
                        if (!covered[b2]) {
                            queue.insert(vector<int>{min(b1,b2),max(b1,b2)}, -getIntersectionSize(bags[b1],bags[b2]));
                        }
                    }
                }
            }
        }
    }

	TreeDecomposition::~TreeDecomposition() {}
	void TreeDecomposition::print(ostream& output, const map<int,string>& inputNaming) const {
        // print bags
        output << "bags:" << endl;
        for (int i=0; i<bags.size(); i++) {
            output << i+1 << ":";
            for (auto v : bags[i]) {
                output << " " << inputNaming.at(v);
            }
            output << endl;
        }
        // print edges
        output << "edges:" << endl;
        for (auto e : edges) {
            for (auto i : e) {
                output << i+1 << " ";
            }
            output << endl;
        }
    }

}