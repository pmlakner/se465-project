#ifndef COMMON_H
#define COMMON_H

#include "callgraph.h"
#include <string>
#include <memory>


// Useful Classes
// ---------------------------------

class UnorderedPair {
public:
    UnorderedPair(NodePtr a, NodePtr b);
    bool operator==(const UnorderedPair&) const;
    friend std::ostream& operator<<(std::ostream&, const UnorderedPair&);

    NodePtr a;
    NodePtr b;
};

class Bug {
public:
    Bug(std::string func, std::string parent, UnorderedPair pair, int support, float confidence);
    friend std::ostream& operator<<(std::ostream&, const Bug&);

private:
    std::string func;
    std::string parent;
    UnorderedPair pair;
    int support;
    float confidence;
};


// Input / Output operators
// ---------------------------------

std::ostream& operator<<(std::ostream&, const UnorderedPair&);
std::ostream& operator<<(std::ostream&, const Bug&);


// Make UnorderedPair hashable
// ---------------------------------

namespace std {
    template <>
    struct hash<UnorderedPair> {
        std::size_t operator()(const UnorderedPair& p) const {
            return hash<string>()(p.a->name) ^ hash<string>()(p.b->name);
        }
    };
}

#endif
