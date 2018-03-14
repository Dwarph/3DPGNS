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

#define NO_OF_TERRAIN_VERT_ARRAYS 16

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


    // Cull triangles which normal is not towards the camera
    // glEnable(GL_CULL_FACE);
}


void assignVertexColours(GLfloat *colourBuffer, GLfloat *vertexBuffer, DiamondSquare *diamondSquare, int size,
                         int maxHeight) {

    for (int i = 0; i < size; i++) {
        GLfloat height = vertexBuffer[i + 2];
        float rand = (diamondSquare->randInRange(25) / 100);

        if (height < 0) {
            colourBuffer[i] = 0.169f * rand;
            i++;
            colourBuffer[i] = 0.169f * rand;
            i++;
            colourBuffer[i] = 0.169f * rand;
        } else if (height < maxHeight * 0.6) {
            colourBuffer[i] = 0;
            i++;
            colourBuffer[i] = 0.104f * rand;
            i++;
            colourBuffer[i] = 0.10f * rand;
        }


    }
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

    //our width and depth of our grid


    //8 = 4
    //9 = 8
    //10 = 16
    int max = (pow(2, 10)) + 1;

    //creates a new diamondSquare heightMap
    DiamondSquare *diamondSquare = new DiamondSquare(max, 1);

    //works out the needed number of vertices
    //*2 for noOfTriangles, then *3 for noOfVerts, then 3 for number of points.
    int noOfVertices = ((((max - 1) * (max - 1)) * 2) / NO_OF_TERRAIN_VERT_ARRAYS) * 3 * 3;

    vector<vector<GLfloat>> gl_terrain_verts;
    gl_terrain_verts.resize(NO_OF_TERRAIN_VERT_ARRAYS, vector<GLfloat>(noOfVertices, 0));

    diamondSquare->getVertices(gl_terrain_verts, NO_OF_TERRAIN_VERT_ARRAYS, 1);


    // One color for each vertex. Generated randomly each time.
    GLfloat g_color_buffer_data[noOfVertices];

    for (int i = 0; i < noOfVertices; i++) {
        GLfloat col = rand() % (101);
        col /= 100;
        g_color_buffer_data[i] = col;
    }

//    float minHeight = 0, maxHeight = 0;
//    for (int i = 1; i < noOfVertices; i += 3) {
//        if (g_vertex_buffer_data_one[i] < minHeight) {
//            minHeight = g_vertex_buffer_data_one[i];
//        }
//        if (g_vertex_buffer_data_one[i] > maxHeight) {
//            maxHeight = g_vertex_buffer_data_one[i];
//        }
//    }
//
//
//    for (int i = 1; i < noOfVertices; i += 3) {
//        g_color_buffer_data[i - 1] = g_vertex_buffer_data_one[i] / maxHeight;
//        g_color_buffer_data[i + 1] = g_vertex_buffer_data_one[i] / maxHeight;
//        g_color_buffer_data[i] = g_vertex_buffer_data_one[i] / maxHeight;
//        // cout << g_vertex_buffer_data_one[i] / maxHeight << endl;
//        //  i++;
//
//    }

    //assignVertexColours(g_color_buffer_data)

//    prints out vertices
//    for (int i = 0; i < noOfVertices; i++) {
//        cout << g_vertex_buffer_data_one[i] << ", ";
//        if ((i + 1) % 3 == 0) {
//            cout << endl;
//        }
//
//        if ((i + 1) % 18 == 0) {
//            cout << endl;
//        }
//
//    }


/** Bind vertices to buffer**/
    GLuint vertexBuffers[NO_OF_TERRAIN_VERT_ARRAYS];
    for (int i = 0; i < NO_OF_TERRAIN_VERT_ARRAYS; i++) {
        glGenBuffers(1, &vertexBuffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, gl_terrain_verts[i].size() * sizeof(float), &gl_terrain_verts[i][0],
                     GL_STATIC_DRAW);
    }

/** bind colours **/
    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

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
        glUniformMatrix4fv(MatrixID,
                           1, GL_FALSE, &MVP[0][0]);


        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                          // array buffer offset
        );


        for (int i = 0; i < NO_OF_TERRAIN_VERT_ARRAYS; i++) {
            // Attribute buffer - vertices
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
            glVertexAttribPointer(
                    0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void *) 0            // array buffer offset
            );

            // Draw the Terrain !
            glDrawArrays(GL_TRIANGLES, 0, noOfVertices); // 12*3 indices starting at 0 -> 12 triangles
        }






        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

// Swap buffers
        glfwSwapBuffers(window);

        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);


    // Check if the ESC key was pressed or the window was closed

// Cleanup VBO and shader
    for (int i = 0; i < NO_OF_TERRAIN_VERT_ARRAYS; i++) {
        glDeleteBuffers(1, &vertexBuffers[i]);

    }
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

// Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


int main() {
    return openGLMagic();
}

