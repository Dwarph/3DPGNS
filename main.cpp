#include <iostream>

#include "./DiamondSquare.h"

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


    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);
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



    /** MVP Matrix Transformation setup for Cube**/

//    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
//    mat4 Projection = perspective(radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
//    // Camera matrix
//    mat4 View       = lookAt(
//            vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
//            vec3(0,0,0), // and looks at the origin
//            vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
//    );
//    // Model matrix : an identity matrix (model will be at the origin)
//    mat4 Model      = mat4(1.0f);
//    // Our ModelViewProjection : multiplication of our 3 matrices
//    mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around


    //our width and depth of our grid
    int max = 100;

    //creates a new diamondSquare heightMap
    DiamondSquare *diamondSquare = new DiamondSquare(max, 0.3);

    //works out the needed number of vertices
    int noOfVertices = (((max) * (max)) * 2 * 3 *
                        3) / 4; //*2 for noOfTriangles, then *3 for noOfVerts, then 3 for number of points.
    GLfloat g_vertex_buffer_data_one[noOfVertices];
    GLfloat g_vertex_buffer_data_two[noOfVertices];
    GLfloat g_vertex_buffer_data_three[noOfVertices];
    GLfloat g_vertex_buffer_data_four[noOfVertices];


    diamondSquare->getVertices(g_vertex_buffer_data_one, g_vertex_buffer_data_two, g_vertex_buffer_data_three,
                               g_vertex_buffer_data_four, 1.0);

    // diamondSquare->getVertices(g_vertex_buffer_data);

    // One color for each vertex. Generated randomly each time.
    GLfloat g_color_buffer_data[noOfVertices];

    for (int i = 0; i < noOfVertices; i++) {
        GLfloat col = (rand() % 100);
        col /= 100;
        g_color_buffer_data[i] = col;
    }
//
////    prints out vertices
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
    GLuint vertexbufferOne;
    glGenBuffers(1, &vertexbufferOne);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferOne);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_one), g_vertex_buffer_data_one, GL_STATIC_DRAW);

    GLuint vertexbufferTwo;
    glGenBuffers(1, &vertexbufferTwo);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferTwo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_two), g_vertex_buffer_data_two, GL_STATIC_DRAW);

    GLuint vertexbufferThree;
    glGenBuffers(1, &vertexbufferThree);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferThree);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_three), g_vertex_buffer_data_three, GL_STATIC_DRAW);

    GLuint vertexbufferFour;
    glGenBuffers(1, &vertexbufferFour);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferFour);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_four), g_vertex_buffer_data_four, GL_STATIC_DRAW);



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

// 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferOne);
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
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
                3,                                // size
                GL_FLOAT,                         // type
                GL_FALSE,                         // normalized?
                0,                                // stride
                (void *) 0                          // array buffer offset
        );

// Draw the Terrain !
        glDrawArrays(GL_TRIANGLES, 0, noOfVertices); // 12*3 indices starting at 0 -> 12 triangles


        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferTwo);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );

        glDrawArrays(GL_TRIANGLES, 0, noOfVertices); // 12*3 indices starting at 0 -> 12 triangles
//
//
        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferThree);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );
//
        glDrawArrays(GL_TRIANGLES, 0, noOfVertices); // 12*3 indices starting at 0 -> 12 triangles


        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferFour);
        glVertexAttribPointer(
                0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );

        glDrawArrays(GL_TRIANGLES, 0, noOfVertices); // 12*3 indices starting at 0 -> 12 triangles


        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

// Swap buffers
        glfwSwapBuffers(window);

        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (
            glfwGetKey(window,
                       GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window)
            == 0);

// Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbufferOne);
    glDeleteBuffers(1, &vertexbufferTwo);
    glDeleteBuffers(1, &vertexbufferThree);
    glDeleteBuffers(1, &vertexbufferFour);

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
