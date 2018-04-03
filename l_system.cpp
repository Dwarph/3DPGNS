//
// Created by pip on 22/03/18.
//

#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include "l_system.h"

L_System::L_System(const std::string &seed, int no_of_iterations) {
    this->seed = seed;
    this->no_of_iterations = no_of_iterations;
    this->fractalString = this->seed;
}


void L_System::addRule(Rule rule) {
    rules.push_back(rule);
}

void L_System::generateFractal() {
    std::string newString = "";
    this->fractalString = this->seed;
    bool found = false;
    int size = 0;

    for (int n = 0; n < this->no_of_iterations; n++) {
        for (int i = 0; i < this->fractalString.length(); i++) {
            for (int j = 0; j < this->rules.size(); j++) {
                if (this->fractalString[i] == this->rules[j].axiom) {
                    newString += this->rules[j].rule;
                    found = true;
                    break;
                }
            }
            if (!found) {
                newString += this->fractalString[i];
            }
            found = false;

        }
        fractalString = newString;
        newString.empty();
    }
}


void L_System::generateVertices() {

    glm::mat4x4 homogenousCoords;
    homogenousCoords[1];

    glm::fvec4 currentPosition = {0, 0, 0, 0};
    int levelNum = 1;
    int index = 0;
    float angle = 0;
    float angleModifier = glm::degrees(90.0f);

    std::vector<glm::fvec4> positionBuffer;

    glm::fmat4 translation;

    //xAxis rotationVector
    translation[3] = {0, 1, 0, 1};
    translation[2] = {0, -glm::sin(angle), glm::cos(angle), 0};
    translation[1] = {0, glm::cos(angle), glm::sin(angle), 0};
    translation[0] = {1, 0, 0, 0};

    for (int i = 0; i < this->fractalString.length(); i++) {
        if (this->fractalString[i] == 'F') {


            translation[2] = {0, -glm::sin(angle), glm::cos(angle), 0};
            translation[1] = {0, glm::cos(angle), glm::sin(angle), 0};

            for (int j = 0; j < 2; j++) {
                vertices.push_back(currentPosition.x);
                vertices.push_back(currentPosition.y);
                vertices.push_back(currentPosition.z);
                currentPosition.x = currentPosition.x + glm::cos(angle);
                currentPosition.y = currentPosition.y + glm::sin(angle);
                vertices.push_back(currentPosition.x);
                vertices.push_back(currentPosition.y);
                vertices.push_back(currentPosition.z);



//                if (j == 0) {currentPosition =  translation * currentPosition;}
//                std::cout << angle;
            }

        } else if (this->fractalString[i] == 'f') {
            currentPosition = currentPosition * translation;

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

void L_System::setSeed(const std::string &seed) {
    L_System::seed = seed;
}

const std::vector<GLfloat> &L_System::getVertices() const {
    return vertices;
}



