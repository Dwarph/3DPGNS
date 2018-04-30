#ifndef CONTROLS_HPP
#define CONTROLS_HPP


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


};

#endif