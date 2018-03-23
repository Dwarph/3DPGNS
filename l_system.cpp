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
    std::vector<Axiom> rules;
};


std::string generateFractal() {
    struct Axiom pip;


}