#include <iostream>

#include "./l_system.h"
#include "world_maker.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "external/OpenGLTutorialUsefulFiles/controls.hpp"
#include <external/OpenGLTutorialUsefulFiles/shader.hpp>

using namespace glm;
using namespace std;


int windowSetup() {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }


    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "3DPGNS", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    //hide the mouse and let use move infinitely
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    //blue background
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);


    glDepthMask(GL_TRUE);

    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
//    glCullFace(GL_FRONT);
}

int openGLMagic() {
    if (windowSetup() == -1) {
        return -1;
    };

    /** Open GL Setup**/
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("external/OpenGLTutorialUsefulFiles/TransformVertexShader.vertexshader",
                                   "external/OpenGLTutorialUsefulFiles/ColorFragmentShader.fragmentshader");

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    world_maker *world_mkr = new world_maker(9, 0.5);


    /** Bind vertices to buffer**/
    GLuint diamondSquareVertexBuffers[world_mkr->get_no_of_terrain_vertex_arrays()];
    GLuint diamondSquareColourBuffer[world_mkr->get_no_of_terrain_vertex_arrays()];

    world_mkr->makeWorld(diamondSquareVertexBuffers, diamondSquareColourBuffer);

    L_System *tree = new L_System("X", 7, 20);

    Rule rule;
    rule.axiom = 'X';
    rule.rule = "F[+X]F[-X]+X";

    tree->addRule(rule);

    rule.axiom = 'F';
    rule.rule = "FF";

    tree->addRule(rule);


    tree->generateFractal();
    tree->generateVertices();


    GLuint treeVertexBuffer;
    glGenBuffers(1, &treeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, treeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, tree->getVertices().size() * sizeof(float), &tree->getVertices()[0],
                 GL_STATIC_DRAW);



    /** Main Draw Loop **/
    do {

// Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Use our shader
        glUseProgram(programID);

// Compute the MVP matrix from keyboard and mouse input
        computeMatricesFromInputs();

        mat4 ProjectionMatrix = getProjectionMatrix();
        mat4 ViewMatrix = getViewMatrix();
        mat4 ModelMatrix = glm::mat4(1.0);
        mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

/** Terrain display commands**/
// Send our transformation to the currently bound shader,
// in the "MVP" uniform

        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        for (int i = 0; i < world_mkr->get_no_of_terrain_vertex_arrays(); i++) {
            // Attribute buffer - vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, diamondSquareVertexBuffers[i]);
            glVertexAttribPointer(
                    0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void *) 0            // array buffer offset
            );

            // 2nd attribute buffer : colors
            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, diamondSquareColourBuffer[i]);
            glVertexAttribPointer(
                    1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                    3,                                // size
                    GL_FLOAT,                         // type
                    GL_FALSE,                         // normalized?
                    0,                                // stride
                    (void *) 0                          // array buffer offset
            );

            // Draw the Terrain !
            glDrawArrays(GL_TRIANGLES, 0, world_mkr->getDiamondSquare()->get_no_of_vertices());
        }

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, treeVertexBuffer);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );
        glDrawArrays(GL_LINES, 0, tree->getVertices().size() / 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

// Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
    // Check if the ESC key was pressed or the window was closed

// Cleanup VBO and shader
    for (int i = 0; i < world_mkr->get_no_of_terrain_vertex_arrays(); i++) {
        glDeleteBuffers(1, &diamondSquareVertexBuffers[i]);
        glDeleteBuffers(1, &diamondSquareColourBuffer[i]);
    }

    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

// Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

int main() {
    return openGLMagic();
}

