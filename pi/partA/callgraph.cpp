#include "callgraph.h"
#include "common.h"

#include <unordered_map>
#include <queue>
#include <algorithm>
#include <sstream>

using namespace std;

CallGraph::CallGraph() {}

list<Bug> CallGraph::getBugs(int t_support, float t_confidence) {
    list<Bug> bugs;

    // Populate SUPPORT calculations for all function pairs
    // -------------------------------------------------------
    unordered_map<UnorderedPair, int> support;

    // Iterate through all nodes
    for (auto it : nodes) {
        NodePtr cur = it.second;

        // For all pairs (A, B), add 1 to support(A, B)
        set<NodePtr> children = cur->callsFunctions;
        for (auto i = children.begin(); i != children.end(); i++) {
            for (auto j = next(i); j != children.end(); j++) {
                if ((*i)->name != "" && (*j)->name != "")
                    support[UnorderedPair(*i, *j)]++;
            }
        }
    }

    // For all pairs w/ SUPPORT > T_SUPPORT, find confidence
    // -------------------------------------------------------

    for (auto it : support) {
        NodePtr a = it.first.a;
        NodePtr b = it.first.b;
        int support_a_b = it.second;

        if (support_a_b >= t_support) {
            // A & B are often called together
            int support_a = a->calledByFunctions.size();
            int support_b = b->calledByFunctions.size();
            float confidence_a_b_a = 100.00 * support_a_b / support_a;
            float confidence_a_b_b = 100.00 * support_a_b / support_b;

            if (confidence_a_b_a >= t_confidence) {
                // BUGS EXIST
                // > A is almost never called without B

                // Of all times A is called, find times when it is not
                // called together with B
                for (NodePtr caller : a->calledByFunctions) {
                    set<NodePtr> others = caller->callsFunctions;
                    if (others.find(b) == others.end()) {
                        // Bug found
                        bugs.push_back(
                            Bug(
                                a->name,
                                caller->name,
                                it.first,
                                support_a_b,
                                confidence_a_b_a
                            )
                        );
                    }
                }
            }

            if (confidence_a_b_b >= t_confidence) {
                // BUGS EXIST
                // > B is almost never called without A

                // Of all times B is called, find times when it is not
                // called together with A
                for (NodePtr caller : b->calledByFunctions) {
                    set<NodePtr> others = caller->callsFunctions;
                    if (others.find(a) == others.end()) {
                        // Bug found
                        bugs.push_back(
                            Bug(
                                b->name,
                                caller->name,
                                it.first,
                                support_a_b,
                                confidence_a_b_b
                            )
                        );
                    }
                }
            }
        }
    }

    return bugs;
}

void CallGraph::createNodeIfNotExists(string id) {
    if (nodes.find(id) == nodes.end()) {
        NodePtr root = NodePtr(new Node());
        root->id = id;
        nodes[id] = root;
    }
}

istream& operator>> (istream &in, CallGraph &cg) {
    // Parse from 'dot' format

    // First jump to the first node
    string line;
    getline(in, line);
    while (line.length() < 40 ||
            line.substr(line.length()-39, 39) != "[shape=record,label=\"{external node}\"];") {
        getline(in, line);
    }

    // Line-by-line, read in edges & nodes from the callgraph
    bool ignore_cur_node = true;
    string curFunctionId, tmp;
    getline(in, line);

    while (line != "}") {
        // Parse line token-by-token
        stringstream ss(line);
        ss >> curFunctionId;
        ss >> tmp;

        if (tmp == "->") {
            // New EDGE
            // ------------------------------------------
            if (!ignore_cur_node) {
                string refFunctionId;
                ss >> refFunctionId;

                // Trim trailing semicolon
                refFunctionId = refFunctionId.substr(0, refFunctionId.size()-1);

                //cout << "Adding edge from " << curFunctionId << " to " << refFunctionId << endl;

                // Add forward & backward edge
                cg.createNodeIfNotExists(refFunctionId);
                cg.nodes[curFunctionId]->callsFunctions.insert(cg.nodes[refFunctionId]);
                cg.nodes[refFunctionId]->calledByFunctions.insert(cg.nodes[curFunctionId]);
            }
        } else {
            // New NODE
            // ------------------------------------------
            string curFunctionDescription = tmp;
            unsigned first = curFunctionDescription.find('{');
            unsigned last = curFunctionDescription.find('}');
            string curFunctionName = curFunctionDescription.substr(first+1, last-first-1);
            //cout  << "Setting name " << curFunctionId << " to " << curFunctionName << endl;

            if (curFunctionName == "" || curFunctionName == "external node") {
                // Ignore this node
                ignore_cur_node = true;
            } else {
                ignore_cur_node = false;

                // We now know the name, so set it
                cg.createNodeIfNotExists(curFunctionId);
                cg.nodes[curFunctionId]->name = curFunctionName;

                if (curFunctionName == "main")
                    cg.rootId = curFunctionId;
            }
        }

        // Next line
        getline(in, line);
    }
    return in;
}

// istream& operator>> (istream &in, CallGraph &cg) {
//     // Parse regular 'opt' output
// 
//     bool ignore_edges = false;
//     string line, curFunctionId, curFunctionName;
// 
//     // Line-by-line, create nodes & edges of call graph
//     while (getline(in, line)) {
//         if (line.compare(0, 15, "Call graph node")) {
//             // New NODE
//             // ------------------------------------------
//         } else if (!ignore_edges && line.compare(16, 14, "calls function")) {
//             // New EDGE
//             // ------------------------------------------
//         } else if (line == "") {
//             // Done with current node
//             // ------------------------------------------
//             ignore_edges = false;
//         }
//     }
// 
//     return in;
// }
