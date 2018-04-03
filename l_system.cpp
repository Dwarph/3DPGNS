//
// Created by pip on 22/03/18.
//

#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/glm.hpp>
#include "l_system.h"

std::string seed;
std::string fractalString;
int no_of_iterations;

std::vector<Rule> rules;

L_System::L_System(const std::string &seed, int no_of_iterations) {
    this->seed = seed;
    this->no_of_iterations = no_of_iterations;
    this->fractalString = this->seed;
}


void L_System::addRule(Rule rule) {
    rules.push_back(rule);
}

std::string L_System::generateFractal() {
    this->fractalString = this->seed;

    for (int i = 0; i < this->no_of_iterations; i++) {
        for (int j = 0; i < this->rules.size(); j++) {
            if (this->fractalString[i] == this->rules[j].axiom) {
                this->fractalString.replace(i, 1, this->rules[j].rule);
            }
        }
    }
}


void L_System::getVertsAsLines(std::vector<GLfloat> &vertices) {

    glm::mat4x4 homogenousCoords;
    homogenousCoords[1];

    glm::fvec4 currentPosition = {0, 0, 0, 1};
    int levelNum = 1;
    int index = 0;
    float angle = 0;
    float angleModifier = glm::degrees(6);

    std::vector<glm::fvec4> positionBuffer;


    glm::fmat4 translation;
    translation[3] = {1, 0, 0, 1};
    translation[2] = {0, -glm::sin(angle), glm::cos(angle), 0};
    translation[1] = {0, glm::cos(angle), glm::sin(angle), 0};
    translation[0] = {1, 0, 0, 0};

    for (int i = 0; i < this->fractalString.length(); i++) {
        if (this->fractalString[i] == 'F') {

            vertices[index] = currentPosition.x;
            index++;
            vertices[index] = currentPosition.y;
            index++;
            vertices[index] = currentPosition.z;
            index++;

            currentPosition *= translation;

            vertices[index] = currentPosition.x;
            index++;
            vertices[index] = currentPosition.y;
            index++;
            vertices[index] = currentPosition.z;
            index++;

        } else if (this->fractalString[i] == 'f') {
            currentPosition *= translation;

        } else if (this->fractalString[i] == '+') {
            angle += angleModifier;

        } else if (this->fractalString[i] == '-') {
            angle -= angleModifier;

        } else if (this->fractalString[i] == '&') {

        } else if (this->fractalString[i] == '\\') {

        } else if (this->fractalString[i] == '^') {

        } else if (this->fractalString[i] == '/') {

        } else if (this->fractalString[i] == '|') {

        } else if (this->fractalString[i] == '[') {
            positionBuffer.push_back(currentPosition);
            levelNum++;
        } else if (this->fractalString[i] == ']') {
            currentPosition = positionBuffer.back();
            positionBuffer.pop_back();
            levelNum--;
        }
    }
}



