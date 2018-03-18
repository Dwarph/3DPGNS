#include <iostream>

#include "./DiamondSquare.h"

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
#include <math.h>

GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "external/OpenGLTutorialUsefulFiles/controls.hpp"

#include <external/OpenGLTutorialUsefulFiles/shader.hpp>

#define TERRAIN_SIZE 10
#define ROUGHNESS 10
#define SCALE 0.01

using namespace glm;

using namespace std;

struct colour_list {
    GLfloat ivy_green[3] = {0.38, 0.47, 0.28};
    GLfloat moss_green[3] = {0.47, 0.56, 0.28};
    GLfloat snow_white[3] = {0.94, 0.94, 0.85};
    GLfloat dusty_brown[3] = {0.65, 0.55, 0.41};
    GLfloat wet_brown[3] = {0.35, 0.27, 0.2};
    GLfloat light_grey[3] = {0.85, 0.84, 0.8};
    GLfloat med_grey[3] = {0.72, 0.68, 0.6};
};


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


    // Cull triangles which normal is not towards the camera
//     glEnable(GL_CULL_FACE);
}

GLfloat *getBlendedColours(GLfloat colour_one[3], GLfloat colour_two[3], GLfloat blended_colour[3], float percentage) {

    GLfloat range;

    for (int i = 0; i < 3; i++) {
        range = colour_one[i] - colour_two[i];
        blended_colour[i] = colour_two[i] + (range * percentage);
    }
    return blended_colour;
}

int getNoOfTerrainVertices() {

    if (TERRAIN_SIZE < 7) {
        return 1;
    } else {
        return pow(2, (TERRAIN_SIZE - 6));
    }

}

void
computeDiamondSquareColourBuffer(vector<vector<GLfloat>> gl_terrain_verts, GLuint *diamondSquareColourBuffers,
                                 int noOfVertices) {
    colour_list colours;
    vector<vector<GLfloat>> g_color_buffer_data;
    g_color_buffer_data.resize(getNoOfTerrainVertices(), vector<GLfloat>(noOfVertices, 0));
    float minHeight = gl_terrain_verts[0][1], maxHeight = gl_terrain_verts[0][1];

    /*
    for (int i = 0; i < getNoOfTerrainVertices(); i++) {
        for (int j = 0; j < noOfVertices; j++) {
            GLfloat col = rand() % (101);
            col /= 100;
            g_color_buffer_data[i][j] = col;
        }
    }
    */

    for (int i = 0; i < getNoOfTerrainVertices(); i++) {
        for (int j = 1; j < noOfVertices; j += 3) {
            if (minHeight > gl_terrain_verts[i][j]) {
                minHeight = gl_terrain_verts[i][j];
            }

            if (maxHeight < gl_terrain_verts[i][j]) {
                maxHeight = gl_terrain_verts[i][j];
            }
        }
    }

    cout << "MAX: " << maxHeight << endl;
    cout << "MIN: " << minHeight << endl;


    float heightRange = maxHeight - minHeight;

    float height, randNum;

    for (int i = 0; i < getNoOfTerrainVertices(); i++) {
        for (int j = 1; j < noOfVertices; j += 3) {
            int min = 97, max = 103;

            height = (gl_terrain_verts[i][j] - minHeight) / heightRange;
            randNum = (float) (rand() % (max + 1 - min) + min);
            randNum /= 100;


            if (height < 0.025) {
                g_color_buffer_data[i][j - 1] = colours.wet_brown[0] * randNum;
                g_color_buffer_data[i][j] = colours.wet_brown[1] * randNum;
                g_color_buffer_data[i][j + 1] = colours.wet_brown[2] * randNum;


            } else if (height < 0.05) {
                g_color_buffer_data[i][j - 1] = colours.dusty_brown[0] * randNum;
                g_color_buffer_data[i][j] = colours.dusty_brown[1] * randNum;
                g_color_buffer_data[i][j + 1] = colours.dusty_brown[2] * randNum;

            } else if (height < 0.4) {
                g_color_buffer_data[i][j - 1] = colours.ivy_green[0] * randNum;
                g_color_buffer_data[i][j] = colours.ivy_green[1] * randNum;
                g_color_buffer_data[i][j + 1] = colours.ivy_green[2] * randNum;

            } else if (height < 0.7) {
                g_color_buffer_data[i][j - 1] = colours.moss_green[0] * randNum;
                g_color_buffer_data[i][j] = colours.moss_green[1] * randNum;
                g_color_buffer_data[i][j + 1] = colours.moss_green[2] * randNum;

            } else if (height < 0.8) {
                g_color_buffer_data[i][j - 1] = colours.med_grey[0] * randNum;
                g_color_buffer_data[i][j] = colours.med_grey[1] * randNum;
                g_color_buffer_data[i][j + 1] = colours.med_grey[2] * randNum;

            } else if (height < 0.95) {
                g_color_buffer_data[i][j - 1] = colours.light_grey[0] * randNum;
                g_color_buffer_data[i][j] = colours.light_grey[1] * randNum;
                g_color_buffer_data[i][j + 1] = colours.light_grey[2] * randNum;

            } else if (height < 1) {
                g_color_buffer_data[i][j - 1] = colours.snow_white[0] * randNum;
                g_color_buffer_data[i][j] = colours.snow_white[1] * randNum;
                g_color_buffer_data[i][j + 1] = colours.snow_white[2] * randNum;
            }

        }
    }

    for (int i = 0; i < getNoOfTerrainVertices(); i++) {
        glGenBuffers(1, &diamondSquareColourBuffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, diamondSquareColourBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data[i].size() * sizeof(float), &g_color_buffer_data[i][0],
                     GL_STATIC_DRAW);
    }
}


void
computeDiamondSquareVertexBuffers(GLuint *vertexBuffers, GLuint *diamondSquareColourBuffer, DiamondSquare diamondSquare,
                                  int noOfVertices) {

    //works out the needed number of vertices
    //*2 for noOfTriangles, then *3 for noOfVerts, then 3 for number of points.

    vector<vector<GLfloat>> gl_terrain_verts;
    gl_terrain_verts.resize(getNoOfTerrainVertices(), vector<GLfloat>(noOfVertices, 0));

    diamondSquare.getVertices(gl_terrain_verts, getNoOfTerrainVertices(), SCALE);

    for (int i = 0; i < getNoOfTerrainVertices(); i++) {
        glGenBuffers(1, &vertexBuffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, gl_terrain_verts[i].size() * sizeof(float), &gl_terrain_verts[i][0],
                     GL_STATIC_DRAW);
    }

    computeDiamondSquareColourBuffer(gl_terrain_verts, diamondSquareColourBuffer, noOfVertices);

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


    /** DIAMOND SQUARE CODE!!! **/

    //
    //7 = 2
    //8 = 4
    //9 = 8
    //10 = 16
    //our width and depth of our grid
    int max = (pow(2, TERRAIN_SIZE)) + 1;

    int noOfVertices = ((((max - 1) * (max - 1)) * 2) / getNoOfTerrainVertices()) * 3 * 3;

    //creates a new diamondSquare heightMap
    DiamondSquare *diamondSquare = new DiamondSquare(max, ROUGHNESS);

    /** Bind vertices to buffer**/
    GLuint diamondSquareVertexBuffers[getNoOfTerrainVertices()];
    GLuint diamondSquareColourBuffer[getNoOfTerrainVertices()];

    computeDiamondSquareVertexBuffers(diamondSquareVertexBuffers, diamondSquareColourBuffer, *diamondSquare,
                                      noOfVertices);


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

        for (int i = 0; i < getNoOfTerrainVertices(); i++) {
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
            glDrawArrays(GL_TRIANGLES, 0, noOfVertices);
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

// Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);


    // Check if the ESC key was pressed or the window was closed

// Cleanup VBO and shader
    for (int i = 0; i < getNoOfTerrainVertices(); i++) {
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

