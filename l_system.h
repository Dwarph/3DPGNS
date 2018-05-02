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

class LSystem {

public:

    LSystem(const std::string &seed, int no_of_iterations, float angleMod, float scale);

    void GenerateFractal();

    void AddRule(Rule rule);

    void GenerateVertices();

    void set_seed(const std::string &seed);

    const std::vector<GLfloat> &get_vertices() const;


private:

    std::string seed_;
    std::string fractal_string_;
    int no_of_iterations_;
    std::vector<Rule> rules_;
    std::vector<GLfloat> vertices_;
    float angle_mod_;
    float scale_;

    void update_orientation(glm::vec3 &orientation, int axis, float angle);

};


#endif //INC_3DPGNS_L_SYSTEM_H
