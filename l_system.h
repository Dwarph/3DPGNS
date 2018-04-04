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

    L_System(const std::string &seed, int no_of_iterations, float angleMod);

    void generateFractal();

    void addRule(Rule rule);

    void generateVertices();

    void setSeed(const std::string &seed);

    const std::vector<GLfloat> &getVertices() const;


private:

    std::string seed;
    std::string fractalString;
    int no_of_iterations;
    std::vector<Rule> rules;
    std::vector<GLfloat> vertices;
    float angleMod;

    float getLineLength(int length);
};


#endif //INC_3DPGNS_L_SYSTEM_H
