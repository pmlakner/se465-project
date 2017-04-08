#include "callgraph.h"
#include "common.h"

#include <iostream>
#include <cassert>
#include <cstdlib>

using namespace std;

int main(int argc, char **argv) {
    // Get thresholds from command-line arguments
    int t_support = 3;
    float t_confidence = 65;
    int expand_levels = 0;

    if (argc >= 3) {
        t_support = atoi(argv[1]);
        t_confidence = atof(argv[2]);
    } else {
        assert(argc == 1);
    }

    if (argc == 4) {
        expand_levels = atoi(argv[3]);
    }


    // Create CallGraph from input
    CallGraph cg;
    cin >> cg;

    // Expand levels
    for (int i = 0; i < expand_levels; i++)
        cg.expandFunctions();

    // Print bugs
    for (Bug bug : cg.getBugs(t_support, t_confidence)) {
        cout << bug << endl;
    }
}
