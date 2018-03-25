//
// Created by pip on 22/03/18.
//

#include "l_system.h"

struct Axiom {
    char character;
    std::string rule;

};

struct L_system {
    std::string seed;
    int no_of_iterations;
    std::vector<Axiom> axioms;
};


std::string generateFractal(struct L_system *lin) {
    for (int i = 0; i < lin->no_of_iterations; i++) {
        for (int j = 0; i < lin->axioms.size(); j++) {
            if (lin->seed[i] == lin->axioms[j].character) {
                lin->seed.replace(i, 1, lin->axioms[j].rule);
            }
        }
    }
}