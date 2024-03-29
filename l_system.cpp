//
// Created by pip on 22/03/18.
//

#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "l_system.h"

/**
 * This Class holds details of an L_System, its rules, seed and eventual string
 */

/* Constructor */

/**
 * Constructor for L_System
 * @param seed
 * @param no_of_iterations
 * @param angleMod
 * @param scale
 */
LSystem::LSystem(const std::string &seed, int no_of_iterations, float angleMod, float scale) {
    this->seed_ = seed;
    this->no_of_iterations_ = no_of_iterations;
    this->fractal_string_ = this->seed_;
    this->angle_mod_ = glm::radians(angleMod);
    this->scale_ = scale;
}

/* Getters and Setters */

/**
 * Returns the vertices
 * @return vector<GLfloat> L_System
 */
const std::vector<GLfloat> &LSystem::get_vertices() const {
    return vertices_;
}

/**
 * Returns the fractal string generated
 * @return string
 */
const std::string &LSystem::get_fractal_string_() const {
    return fractal_string_;
}

/**
 * Returns the seed
 * @return string
 */
const std::string &LSystem::get_seed_() const {
    return seed_;
}

/**
 * Returns the no_of_iterations
 * @return int
 */
int LSystem::get_no_of_iterations_() const {
    return no_of_iterations_;
}

/**
 * Returns a vector containing the rules
 * @return
 */
const std::vector<Rule> &LSystem::get_rules_() const {
    return rules_;
}

/**
 * Returns the angle_mod_
 * @return float
 */
float LSystem::get_angle_mod_() const {
    return angle_mod_;
}

/**
 * Returns the scale
 * @return float
 */
float LSystem::get_scale_() const {
    return scale_;
}

/**
 * Sets the seed
 * @param seed
 */
void LSystem::set_seed(const std::string &seed) {
    this->seed_ = seed;
}

/**
 * Adds a rule to the set of rules for the L_System
 * @param rule
 */
void LSystem::AddRule(Rule rule) {
    rules_.push_back(rule);
}

/* Main Functions */

/**
 * Generates the fractal by looping through the string for n iterations,
 * building up the string each iteration according to the list of rules
 */
void LSystem::GenerateFractal() {

    //could replace "rules_" with a hashmap

    std::string new_string = "";
    this->fractal_string_ = this->seed_;
    bool found = false;

    for (int n = 0; n < this->no_of_iterations_; n++) {
        for (int i = 0; i < this->fractal_string_.length(); i++) {
            for (int j = 0; j < this->rules_.size(); j++) {
                if (this->fractal_string_[i] == this->rules_[j].axiom) {
                    new_string.append(this->rules_[j].rule);
                    found = true;
                    break;
                }
            }
            if (!found) {
                new_string += this->fractal_string_[i];
            }
            found = false;
        }
        fractal_string_ = new_string;
        new_string = "";
    }
}

/**
 * Generates Vertices based on the string generated in generate_fractal
 */
void LSystem::GenerateVertices() {

    int level_num = 1;
    int rand_range = 5;
    float rand_angle = 0.5;

    std::vector<glm::fvec3> position_buffer;
    std::vector<glm::fvec3> angle_buffer;

    glm::fvec3 current_position = {0, 0, 0};
    glm::vec3 orientation = glm::vec3(0, 1, 0);

    glm::quat full_turn = glm::quat(glm::vec3(0, 0, 180));

    for (int i = 0; i < this->fractal_string_.length(); i++) {

        //Draw a line and move forward by 1, by angle orientation
        if (this->fractal_string_[i] == 'F') {

            glm::fvec3 old_position = current_position;

            vertices_.push_back(current_position.x * this->scale_);
            vertices_.push_back(current_position.y * this->scale_);
            vertices_.push_back(current_position.z * this->scale_);

            current_position += orientation;

            vertices_.push_back(current_position.x * this->scale_);
            vertices_.push_back(current_position.y * this->scale_);
            vertices_.push_back(current_position.z * this->scale_);

            //Move forward by 1 without drawing a line, following the orientation "angle"
        } else if (this->fractal_string_[i] == 'f') {
            current_position += orientation;

            //Turn left by angle_mod
        } else if (this->fractal_string_[i] == '+') {

            if (1 != rand() % rand_range) {
                update_orientation(orientation, 2, this->angle_mod_);
            } else {
                update_orientation(orientation, 2, this->angle_mod_ + rand_angle);
            }

            //Turn right by angle_mod
        } else if (this->fractal_string_[i] == '-') {

            if (1 != rand() % rand_range) {
                update_orientation(orientation, 2, this->angle_mod_ * -1);
            } else {
                update_orientation(orientation, 2, (this->angle_mod_ * -1) + rand_angle);
            }

            //Pitch down by angle_mod
        } else if (this->fractal_string_[i] == '&') {

            if (1 != rand() % rand_range) {
                update_orientation(orientation, 1, this->angle_mod_);
            } else {
                update_orientation(orientation, 1, this->angle_mod_ + rand_angle);
            }

            //Pitch up by angle_mod
        } else if (this->fractal_string_[i] == '^') {

            if (1 != rand() % rand_range) {
                update_orientation(orientation, 1, this->angle_mod_ * -1);
            } else {
                update_orientation(orientation, 1, (this->angle_mod_ * -1) + rand_angle);
            }

            //Roll left by angle_mod
        } else if (this->fractal_string_[i] == '\\') {

            if (1 != rand() % rand_range) {
                update_orientation(orientation, 0, this->angle_mod_);
            } else {
                update_orientation(orientation, 0, this->angle_mod_ + rand_angle);
            }

            //Roll right by angle_mod
        } else if (this->fractal_string_[i] == '/') {

            if (1 != rand() % rand_range) {
                update_orientation(orientation, 0, this->angle_mod_ * -1);
            } else {
                update_orientation(orientation, 0, (this->angle_mod_ * -1) + rand_angle);
            }

            //Turn around by 180 degrees
        } else if (this->fractal_string_[i] == '|') {

            orientation = glm::rotate(full_turn, orientation);

            //Push the current state of the turtle onto the stack
        } else if (this->fractal_string_[i] == '[') {
            position_buffer.push_back(current_position);
            angle_buffer.push_back(orientation);
            level_num++;

            //Pop the state of the turtle off the stack
        } else if (this->fractal_string_[i] == ']') {
            current_position = position_buffer.back();
            orientation = angle_buffer.back();
            position_buffer.pop_back();
            angle_buffer.pop_back();
            level_num--;
        }
    }

    //Orients the tree the correct way
    for (int i = 0; i < vertices_.size(); i += 3) {

        float vertex[] = {vertices_[i], vertices_[i + 1], vertices_[i + 2]};
        vertex[2] = vertex[0] + (glm::cos(90.0f));
        vertex[1] = vertex[1] + (glm::sin(90.0f));

        vertices_[i] = vertex[0];
        vertices_[i + 1] = vertex[1];
        vertices_[i + 2] = vertex[2];

    }
}

/**
 * Updates the orientation according to the angle given to it
 * @param orientation
 * @param axis
 * @param angle
 */
void LSystem::update_orientation(glm::vec3 &orientation, int axis, float angle) {
    switch (axis) {

        //x
        case 0:
            orientation = glm::rotate(glm::quat(glm::vec3(angle, 0, 0)), orientation);
            return;

            //y
        case 1:
            orientation = glm::rotate(glm::quat(glm::vec3(0, angle, 0)), orientation);
            return;

            //z
        case 2:
            orientation = glm::rotate(glm::quat(glm::vec3(0, 0, angle)), orientation);
            return;

        default:
            return;
    }
}