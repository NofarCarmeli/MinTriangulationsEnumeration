#include "TreeDecomposition.h"
#include "DataStructures.h"
#include "Graph.h"
#include <deque>

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

    int TreeDecomposition::getEdgeWeight(const Edge& e) {
        return -getIntersectionSize(bags[e.first], bags[e.second]);
    }

    // Using Prim's algorithm for MST
	void TreeDecomposition::makeTreeEdges() {
        vector<Edge> edges;
        vector<bool> covered(bags.size(),false);
        set< pair<int, Edge > > queue;
        int b1 = 0;
        covered[b1]=true;
        for (int b2=1; b2<bags.size(); b2++) {
            queue.insert(pair<int,Edge>{getEdgeWeight(Edge{b1,b2}),Edge{b1,b2}});
        }
        while(!queue.empty()) {
            Edge e = queue.begin()->second;;
	        queue.erase(queue.begin());
            int b1 = e.second;
            if (!covered[b1]) {
                edges.push_back(e);
                covered[b1]=true;
                for (int b2=0; b2<bags.size(); b2++) {
                    if (!covered[b2]) {
                        queue.insert(pair<int,Edge>{getEdgeWeight(Edge{b1,b2}), Edge{b1,b2}});
                    }
                }
            }
        }
        edgesOptions.push_back(edges);
    }

    TreeDecomposition::TreeDecomposition() {}
    
    TreeDecomposition::TreeDecomposition(const set<NodeSet>& inputBags) : bags() {
        if (inputBags.size() == 0) return;
        for (auto b : inputBags) {
            bags.push_back(b);
        }
        makeTreeEdges();
    }

	TreeDecomposition::~TreeDecomposition() {}

    void TreeDecomposition::print(ostream& output, const map<int,string>& inputNaming) {
        // print bags
        output << "Bags:" << endl;
        for (int i=0; i<bags.size(); i++) {
            output << i+1 << ":";
            for (auto v : bags[i]) {
                output << " " << inputNaming.at(v);
            }
            output << endl;
        }
        // print edges
        if (edgesOptions.size()==1) {
            output << "Edges:" << endl;
            for (auto e : edgesOptions[0]) {
                output << e.first+1 << " " << e.second+1 << endl;
            }
        } else {
            for (int i=0; i<edgesOptions.size(); i++) {
                output << "Edges option " << i+1 << ":" << endl;
                for (auto e : edgesOptions[i]) {
                    output << e.first+1 << " " << e.second+1 << endl;
                }
            }
        }
    }

	void TreeDecomposition::printSingleTree(ostream& output, const map<int,string>& inputNaming) {
        print(output, inputNaming);
    }

    vector<NodeSet> getConnectedComponentAfterRemovingEdge(const vector<Edge>& tree, int removedIndex) {
        Graph g(tree.size()+1);
        for (int i=0; i<tree.size(); i++) if (i != removedIndex) {
            g.addEdge(tree[i].first, tree[i].second);
        }
        return g.getComponents(NodeSet());
    }

    void TreeDecomposition::makeAllTreeEdgeOptionsAux(vector<Edge> tree, int minModifiableIndex, set<Edge> forbiddenEdges) {
        for (int modifiedIndex = minModifiableIndex; modifiedIndex < tree.size(); modifiedIndex++) {
            Edge activeEdge = tree[modifiedIndex];
            int targetWeight = getEdgeWeight(activeEdge);
            // A replacement edge should connect the two components obtained by removing the active edge  
            vector<NodeSet> components = getConnectedComponentAfterRemovingEdge(tree, modifiedIndex);
            for (int v : components[0]) for (int u : components[1]) {
                Edge candidateToAdd {v,u};
                // A replacement edge should be different than the active edge but with the sam weight and not in the forbidden set
                if (activeEdge != candidateToAdd and !forbiddenEdges.count(candidateToAdd) and getEdgeWeight(candidateToAdd)==targetWeight) {
                    // A new tree was found
                    vector<Edge> updatedTree = tree;
                    updatedTree[modifiedIndex]=candidateToAdd;
                    edgesOptions.push_back(updatedTree);
                    // Look for extensions of the new tree
                    set<Edge> updatedForbiddenEdges = forbiddenEdges;
                    updatedForbiddenEdges.insert(activeEdge);
                    updatedForbiddenEdges.insert(Edge{activeEdge.second,activeEdge.first});
                    makeAllTreeEdgeOptionsAux(updatedTree, modifiedIndex+1, updatedForbiddenEdges);
                }
            }
        }
    }

    void TreeDecomposition::printAllEdgeOptions(ostream& output, const map<int,string>& inputNaming) {
        if (edgesOptions.size()==1) {
            makeAllTreeEdgeOptionsAux(vector<Edge>(edgesOptions[0]), 0, set<Edge>());
        }
        print(output, inputNaming);
    }

}