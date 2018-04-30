// Include GLFW
#include <glfw3.h>

extern GLFWwindow *window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;


Controls::Controls() {

    position = glm::vec3(0, 5, 0);
// Initial horizontal angle : toward -Z
    horizontalAngle = 4.0f;
// Initial vertical angle : none
    verticalAngle = 9.0f;
// Initial Field of View
    initialFoV = 45.0f;

    speed = 2000.0f; // 3 units / second
    mouseSpeed = 0.005f;
    lod_level = 8;
    colour_palette = 3;
    show_l_systems = true;
    show_terrain = true;

}

glm::mat4 Controls::getViewMatrix() {
    return ViewMatrix;
}

glm::mat4 Controls::getProjectionMatrix() {
    return ProjectionMatrix;
}


glm::vec3 Controls::get_position() {
    return position;
}

int Controls::get_lod_level() {
    return lod_level;
}

int Controls::get_colour_palette() const {
    return colour_palette;
}

bool Controls::is_show_l_systems() const {
    return show_l_systems;
}

bool Controls::is_show_terrain() const {
    return show_terrain;
}

void Controls::computeMatricesFromInputs() {

    // glfwGetTime is called only once, the first time this function is called
    static double lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();

    deltaTime = float(currentTime - lastTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Compute new orientation
    horizontalAngle += mouseSpeed * float(1024 / 2 - xpos);
    verticalAngle += mouseSpeed * float(768 / 2 - ypos);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    direction = glm::vec3(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
    );


    // Right vector
    right = glm::vec3(
            sin(horizontalAngle - 3.14f / 2.0f),
            0,
            cos(horizontalAngle - 3.14f / 2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 10000000.0f);
    // Camera matrix
    ViewMatrix = glm::lookAt(
            position,           // Camera is here
            position + direction, // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;

//    std::cout << "{ " << position.x << ", " << position.y << ", " << position.z << "}" << std::endl;

//    std::cout << "horizon: " << horizontalAngle << ", verticalAngle: " << verticalAngle << std::endl;
}


void Controls::check_keypress() {


    //Movement Checker
    // Move forward
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        position += direction * deltaTime * speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        position -= direction * deltaTime * speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        position += right * deltaTime * speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        position -= right * deltaTime * speed;
    }

    //LOD Checker
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        lod_level = 0;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        lod_level = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
        lod_level = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
        lod_level = 3;
    }
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        lod_level = 4;
    }
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) {
        lod_level = 5;
    }
    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
        lod_level = 6;
    }
    if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
        lod_level = 7;
    }
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) {
        lod_level = 8;
    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        lod_level = 9;
    }


    //Terrain Colour Checker
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        colour_palette = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        colour_palette = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        colour_palette = 2;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        colour_palette = 3;
    }

    //L_system Checker
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        show_l_systems = false;
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        show_l_systems = true;
    }

    //Terrain checker

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        show_terrain = false;
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        show_terrain = true;
    }


}


