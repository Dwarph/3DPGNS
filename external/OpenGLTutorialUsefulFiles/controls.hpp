#ifndef CONTROLS_HPP
#define CONTROLS_HPP

// Include GLFW
#include <glfw3.h>

extern GLFWwindow *window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;


class Controls {

public:
    Controls();

    void computeMatricesFromInputs();

    glm::mat4 getViewMatrix();

    glm::mat4 getProjectionMatrix();

    glm::vec3 get_position();

    int get_lod_level();

    int get_colour_palette() const;

    bool is_show_l_systems() const;

    bool is_show_terrain() const;

    void check_keypress();


private:

    // Initial position : on +Z
    glm::vec3 position;
    // Initial horizontal angle : toward -Z
    float horizontalAngle;
    // Initial vertical angle : none
    float verticalAngle;
    // Initial Field of View
    float initialFoV;

    float speed;
    float mouseSpeed;
    float deltaTime;
    glm::vec3 direction;
    glm::vec3 right;
    int lod_level;
    int colour_palette;
    bool show_l_systems;
    bool show_terrain;

    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;


};

#endif