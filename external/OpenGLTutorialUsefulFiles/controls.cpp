

#include "controls.hpp"


/**
 * This class is a bit of a mess, as it is an adapted version of the controls class provided by
 * http://opengl-tutorial.org
 *
 * Therefore, whilst attempts have been made to tidy it up, please excuse the mess.
 *
 * This class computes the camera's MVP & handles key inputs.
 */

/* Constructors */

Controls::Controls() {

    //Initial position
    position = glm::vec3(0, 5, 0);

    // Initial horizontal angle : toward -Z
    horizontalAngle = 4.0f;

    // Initial vertical angle : none
    verticalAngle = 9.0f;

    // Initial Field of View
    initialFoV = 45.0f;

    // Camera speed
    speed = 2000.0f;

    // Mouse speed
    mouseSpeed = 0.005f;

    //Initial lod_level
    lod_level = 8;

    //Initial colour palette
    colour_palette = 3;

    //Initial draw state of l-systems
    show_l_systems = true;

    //Initial draw state of terrain
    show_terrain = true;

}

/* Getters and Setters */

/**
 * Returns the view matrix
 * @return mat4
 */
glm::mat4 Controls::getViewMatrix() {
    return ViewMatrix;
}

/**
 * Returns the projection matrix
 * @return mat4
 */
glm::mat4 Controls::getProjectionMatrix() {
    return ProjectionMatrix;
}

/**
 * Returns the current camera position
 * @return vec3
 */
glm::vec3 Controls::get_position() {
    return position;
}

/**
 * Returns the current lod_level
 * @return
 */
int Controls::get_lod_level() {
    return lod_level;
}

/**
 * Returns the current colour palette
 * @return
 */
int Controls::get_colour_palette() const {
    return colour_palette;
}

/**
 * Returns whether to show l_systems
 * @return
 */
bool Controls::is_show_l_systems() const {
    return show_l_systems;
}

/**
 * Returns whether to show terrain
 * @return
 */
bool Controls::is_show_terrain() const {
    return show_terrain;
}

/* Main methods */

/**
 * Works out the camera's MVP matrix
 */
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
}

/**
 * Handles key inputs
 */
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


