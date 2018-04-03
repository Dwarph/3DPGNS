//
// Created by pip on 22/03/18.
//

#ifndef INC_3DPGNS_L_SYSTEM_H
#define INC_3DPGNS_L_SYSTEM_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/vec3.hpp> // glm::vec3


struct Rule {
    char axiom;
    std::string rule;
};

class L_System {

public:

    L_System(const std::string &seed, int no_of_iterations);

    std::string generateFractal();

    void addRule(Rule rule);

    void getVertsAsLines(std::vector<GLfloat> &gl_terrain_verts);


private:

    std::string seed;
    std::string fractalString;
    int no_of_iterations;
    std::vector<Rule> rules;
};


#endif //INC_3DPGNS_L_SYSTEM_H
