//
// Created by pip on 22/03/18.
//

#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
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

    std::string newString = "";
    this->fractal_string_ = this->seed_;
    bool found = false;

    for (int n = 0; n < this->no_of_iterations_; n++) {
        for (int i = 0; i < this->fractal_string_.length(); i++) {
            for (int j = 0; j < this->rules_.size(); j++) {
                if (this->fractal_string_[i] == this->rules_[j].axiom) {
                    newString.append(this->rules_[j].rule);
                    found = true;
                    break;
                }
            }
            if (!found) {
                newString += this->fractal_string_[i];
            }
            found = false;
        }
        fractal_string_ = newString;
        newString = "";
    }
}

/**
 * Generates Vertices based on the string generated in generate_fractal
 */
void LSystem::GenerateVertices() {

    glm::mat4x4 homogenous_coords;
    homogenous_coords[1];

    glm::fvec4 current_position = {1, 1, 1, 1};
    glm::fvec3 angle = {0, 0, 0};
    glm::fvec3 x_axis = {0, 1, 1};
    glm::fvec3 y_axis = {1, 0, 1};
    glm::fvec3 z_axis = {1, 1, 0};
    int levelNum = 1;

    std::vector<glm::fvec4> positionBuffer;
    std::vector<glm::fvec3> angleBuffer;

    glm::fmat4 rotation_matrix_z = glm::rotate(angle.z, z_axis);

    glm::fmat4 translation = glm::translate(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));

    for (int i = 0; i < this->fractal_string_.length(); i++) {

        //Draw a line and move forward by 1, following the orientation "angle"
        if (this->fractal_string_[i] == 'F') {

            glm::fvec4 old_position = current_position;

            vertices_.push_back(current_position.x * this->scale_);
            vertices_.push_back(current_position.y * this->scale_);
            vertices_.push_back(current_position.z * this->scale_);

            //correct way to rotate (can be applied in 3D Space)
//            z_axis.x = current_position.x;
//            z_axis.y = current_position.y;
//
//            glm::fmat4 rotationMatrixZ = glm::rotate(angle.z, z_axis);
//
//            //            std::cout << "angleZ: " << glm::degrees(angle.z) << std::endl;
//
//            current_position = translation * rotationMatrixZ * current_position;


            // rotation only able to be applied in 2D space - but it works in 2D!
            current_position.x = current_position.x + (glm::cos(angle.z + glm::radians(90.0f)));
            current_position.y = current_position.y + (glm::sin(angle.z + glm::radians(90.0f)));

            vertices_.push_back(current_position.x * this->scale_);
            vertices_.push_back(current_position.y * this->scale_);
            vertices_.push_back(current_position.z * this->scale_);

            //Move forward by 1 without drawing a line, following the orientation "angle"
        } else if (this->fractal_string_[i] == 'f') {
            current_position = current_position;

            //Turn left by angle_mod
        } else if (this->fractal_string_[i] == '+') {

            //modify angle
            if (glm::degrees(angle.z + angle_mod_) > 360) {
                //prevents precision loss when dealing with large angles
                angle.z = this->angle_mod_ - glm::radians(360.0f);
            } else {
                angle.z += this->angle_mod_;
            }

            //Turn right by angle_mod
        } else if (this->fractal_string_[i] == '-') {

            //work out angle
            if (glm::degrees(angle.z - angle_mod_) < 0) {
                //prevents precision loss when dealing with large angles
                angle.z = (angle.z - this->angle_mod_) + glm::radians(360.0f);
            } else {
                angle.z -= this->angle_mod_;
            }

            //Pitch down by angle_mod
        } else if (this->fractal_string_[i] == '&') {

            //modify angle
            if (glm::degrees(angle.y + angle_mod_) > 360) {
                //prevents precision loss when dealing with large angles
                angle.y = this->angle_mod_ - glm::radians(360.0f);
            } else {
                angle.y += this->angle_mod_;
            }

            //Pitch up by angle_mod
        } else if (this->fractal_string_[i] == '^') {

            //work out angle
            if (glm::degrees(angle.y - angle_mod_) < 0) {
                //prevents precision loss when dealing with large angles
                angle.y = (angle.y - this->angle_mod_) + glm::radians(360.0f);
            } else {
                angle.y -= this->angle_mod_;
            }

            //Roll left by angle_mod
        } else if (this->fractal_string_[i] == '\\') {

            //work out angle
            if (glm::degrees(angle.x + angle_mod_) > 360) {
                //prevents precision loss when dealing with large angles
                angle.x = this->angle_mod_ - glm::radians(360.0f);
            } else {
                angle.x += this->angle_mod_;
            }

            //Roll right by angle_mod
        } else if (this->fractal_string_[i] == '/') {

            //work out angle
            if (glm::degrees(angle.y - angle_mod_) < 0) {
                //prevents precision loss when dealing with large angles
                angle.x = (angle.x - this->angle_mod_) + glm::radians(360.0f);
            } else {
                angle.x -= this->angle_mod_;
            }

            //Turn around by 180 degrees
        } else if (this->fractal_string_[i] == '|') {

            if (glm::degrees(angle.z) + 180 > 360) {
                //prevents precision loss when dealing with large angles
                angle.z -= glm::radians(180.0f);
            } else {
                angle.z += glm::radians(180.0f);
            }

            //Push the current state of the turtle onto a stack
        } else if (this->fractal_string_[i] == '[') {
            positionBuffer.push_back(current_position);
            angleBuffer.push_back(angle);
            levelNum++;

            //Pop the state of the turtle off the stack
        } else if (this->fractal_string_[i] == ']') {
            current_position = positionBuffer.back();
            angle = angleBuffer.back();
            positionBuffer.pop_back();
            angleBuffer.pop_back();
            levelNum--;
        }
    }

    z_axis = {1, 1, 0};
    for (int i = 0; i < vertices_.size(); i += 3) {

        float vertex[] = {vertices_[i], vertices_[i + 1], vertices_[i + 2]};
        vertex[2] = vertex[0] + (glm::cos(90.0f));
        vertex[1] = vertex[1] + (glm::sin(90.0f));

        vertices_[i] = vertex[0];
        vertices_[i + 1] = vertex[1];
        vertices_[i + 2] = vertex[2];

    }
}





