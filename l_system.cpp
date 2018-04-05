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

L_System::L_System(const std::string &seed, int no_of_iterations, float angleMod) {
    this->seed = seed;
    this->no_of_iterations = no_of_iterations;
    this->fractalString = this->seed;
    this->angleMod = glm::radians(angleMod);
}


void L_System::addRule(Rule rule) {
    rules.push_back(rule);
}

void L_System::generateFractal() {
    //could replace "rules" with a hashmap

    std::string newString = "";
    this->fractalString = this->seed;
    bool found = false;
    int size = 0;

    for (int n = 0; n < this->no_of_iterations; n++) {

        if (n == 4) {
            rand();
        }

        for (int i = 0; i < this->fractalString.length(); i++) {
            for (int j = 0; j < this->rules.size(); j++) {
                if (this->fractalString[i] == this->rules[j].axiom) {
                    newString.append(this->rules[j].rule);
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
        newString = "";
    }
}


void L_System::generateVertices() {

    glm::mat4x4 homogenousCoords;
    homogenousCoords[1];

    glm::fvec4 currentPosition = {1, 1, 1, 1};
    glm::fvec3 angle = {0, 0, 0};
    int levelNum = 1;
    int index = 0;

    glm::quat rotationQuat = glm::quat(angle);



    std::vector<glm::fvec4> positionBuffer;
    std::vector<glm::fvec3> angleBuffer;


    glm::fmat4 translation = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f));
//


    for (int i = 0; i < this->fractalString.length(); i++) {
        if (this->fractalString[i] == 'F') {
            // for (int j = 0; j < 2; j++) {
            rotationQuat = glm::quat(angle);

                vertices.push_back(currentPosition.x);
                vertices.push_back(currentPosition.y);
                vertices.push_back(currentPosition.z);

//                currentPosition.x = currentPosition.x +
            currentPosition.y = currentPosition.y + glm::sin(angle.z);
            currentPosition.z = currentPosition.z + glm::cos(angle.z);

                vertices.push_back(currentPosition.x);
                vertices.push_back(currentPosition.y);
                vertices.push_back(currentPosition.z);

//                if (j == 0) {currentPosition =  translation * currentPosition;}
//                std::cout << angle;
            //  }

        } else if (this->fractalString[i] == 'f') {
            currentPosition = currentPosition;

        } else if (this->fractalString[i] == '+') {
            //modify angle
            if (glm::degrees(angle.z + angleMod) > 360) {
                //prevents precision loss when dealing with large angles
                angle.z += this->angleMod - glm::radians(360.0f);
            } else {
                angle.z += this->angleMod;
            }


            //recalculate z matrix

        } else if (this->fractalString[i] == '-') {
            //work out angle
            if (glm::degrees(angle.z - angleMod) < 0) {
                //prevents precision loss when dealing with large angles
                angle.z = (angle.z - this->angleMod) + glm::radians(360.0f);
            } else {
                angle.z -= this->angleMod;
            }


            //recalculate z matrix


        } else if (this->fractalString[i] == '&') {
            //modify angle
            if (glm::degrees(angle.y + angleMod) > 360) {
                //prevents precision loss when dealing with large angles
                angle.y += this->angleMod - glm::radians(360.0f);
            } else {
                angle.y += this->angleMod;
            }


            //recalculate y matrix


        } else if (this->fractalString[i] == '^') {

            //work out angle
            if (glm::degrees(angle.y - angleMod) < 0) {
                //prevents precision loss when dealing with large angles
                angle.y = (angle.y - this->angleMod) + glm::radians(360.0f);
            } else {
                angle.y -= this->angleMod;
            }

            //recalculate y matrix


        } else if (this->fractalString[i] == '\\') {

            //modify anglefa
            if (glm::degrees(angle.x + angleMod) > 360) {
                //prevents precision loss when dealing with large angles
                angle.x += this->angleMod - glm::radians(360.0f);
            } else {
                angle.x += this->angleMod;
            }


            //recalculate x matrix


        } else if (this->fractalString[i] == '/') {


            //work out angle
            if (glm::degrees(angle.y - angleMod) < 0) {
                //prevents precision loss when dealing with large angles
                angle.x = (angle.x - this->angleMod) + glm::radians(360.0f);
            } else {
                angle.x -= this->angleMod;
            }

            //recalculate x matrix


        } else if (this->fractalString[i] == '|') {

            if (glm::degrees(angle.z) + 180 > 360) {
                //prevents precision loss when dealing with large angles
                angle.z -= glm::radians(180.0f);
            } else {
                angle.z += glm::radians(180.0f);
            }

            //recalculate z matrix


        } else if (this->fractalString[i] == '[') {
            positionBuffer.push_back(currentPosition);
            angleBuffer.push_back(angle);
            levelNum++;
        } else if (this->fractalString[i] == ']') {
            currentPosition = positionBuffer.back();
            angle = angleBuffer.back();
            positionBuffer.pop_back();
            angleBuffer.pop_back();
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



