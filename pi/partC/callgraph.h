#ifndef CALLGRAPH_H
#define CALLGRAPH_H

#include <iostream>
#include <set>
#include <list>
#include <string>
#include <unordered_map>
#include <memory>

#define NodePtr std::shared_ptr<Node>

class Bug;

struct Node {
    std::string id;
    std::string name;
    std::set<NodePtr> calledByFunctions;
    std::set<NodePtr> callsFunctions;
};

class CallGraph {
public:
    CallGraph();
    friend std::istream& operator>>(std::istream&, CallGraph&);

    void expandFunctions();
    void createNodeIfNotExists(std::string id);
    std::list<Bug> getBugs(int t_support, float t_confidence);

private:
    std::string rootId;
    std::unordered_map<std::string, NodePtr> nodes;
};

std::istream& operator>>(std::istream&, CallGraph&);

#endif
