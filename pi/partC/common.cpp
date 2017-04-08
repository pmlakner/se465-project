#include "common.h"
#include <iomanip>
#include <iostream>

using namespace std;


// UnorderedPair Class
// ---------------------------------

UnorderedPair::UnorderedPair(NodePtr a, NodePtr b) : a(a), b(b) {
    // Sort such that this->a < this->b
    if (this->b->name < this->a->name) swap(this->a, this->b);
}

bool UnorderedPair::operator==(const UnorderedPair &other) const {
    return a->name == other.a->name && b->name == other.b->name;
}

ostream& operator<<(ostream &out, const UnorderedPair &pair) {
    out << "(" << pair.a->name << ", " << pair.b->name << ")";
    return out;
}


// Bug Class
// ---------------------------------

Bug::Bug(string func, string parent, UnorderedPair pair, int support, float confidence)
    : func(func), parent(parent), pair(pair), support(support), confidence(confidence) {}

ostream& operator<<(ostream &out, const Bug &bug) {
    out << "bug: " << bug.func << " in " << bug.parent << ", ";
    out << "pair: " << bug.pair << ", ";
    out << "support: " << bug.support << ", ";
    out << "confidence: " << fixed << setprecision(2) << bug.confidence << "%";
    return out;
}
