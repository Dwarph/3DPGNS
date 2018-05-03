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

    bool operator==(const Rule &rule) const {
        return rule.axiom == axiom && rule.rule == this->rule;
    };

};

class LSystem {

public:

    LSystem(const std::string &seed, int no_of_iterations, float angleMod, float scale);

    void set_seed(const std::string &seed);

    const std::vector<GLfloat> &get_vertices() const;

    const std::string &get_seed_() const;

    int get_no_of_iterations_() const;

    const std::vector<Rule> &get_rules_() const;

    float get_angle_mod_() const;

    float get_scale_() const;

    const std::string &get_fractal_string_() const;

    void GenerateFractal();

    void AddRule(Rule rule);

    void GenerateVertices();

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
