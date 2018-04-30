#include <iostream>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "world_maker.h"
#include "external/OpenGLTutorialUsefulFiles/controls.hpp"
#include "external/OpenGLTutorialUsefulFiles/shader.hpp"


using namespace glm;
using namespace std;

GLFWwindow *window;

/**
 * Sets up the GLFW window
 * Code initially taken from http://opengl-tutorial.org
 * @return
 */
int WindowSetup() {
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
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. "
                "Try the 2.1 version of the tutorials.\n");
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
}


/**
 * Gets the vertices needed to pass to OpenGL & displays them in a draw loop.
 * @return
 */
int OpenGLMagic() {
    if (WindowSetup() == -1) {
        return -1;
    };

    /** Open GL Setup**/
    Controls controls = Controls();
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint terrain_shaders = LoadShaders("external/OpenGLTutorialUsefulFiles/TransformVertexShader.vertexshader",
                                         "external/OpenGLTutorialUsefulFiles/ColorFragmentShader.fragmentshader");

    GLuint l_system_shaders = LoadShaders("Shaders/l_system_transform.vert.glsl",
                                          "Shaders/l_system_colour.frag.glsl");

    // Get a handle for our terrain_shaders uniform
    GLint terrain_shaders_uniform_id = glGetUniformLocation(terrain_shaders, "MVP");

    // Get a handle for our l_system shaders uniform
    GLint l_system_shaders_uniform_id = glGetUniformLocation(terrain_shaders, "MVP");

    WorldMaker world_maker = WorldMaker(9, 10);


    /** Bind vertices to buffer**/
    vector<vector<GLuint >> diamond_square_vertex_buffers;
    GLuint diamond_square_vertex_buffers_LOD;


    diamond_square_vertex_buffers.resize(world_maker.get_diamond_square()->get_no_of_iterations());
    for (int i = 0; i < diamond_square_vertex_buffers.size(); i++) {
        diamond_square_vertex_buffers.at(i).resize(world_maker.get_diamond_square()->get_no_of_terrain_vertex_arrays());
    }


    GLuint diamond_square_colour_buffers[world_maker.get_diamond_square()->get_no_of_terrain_vertex_arrays()];
    GLuint tree_vertex_buffer[world_maker.get_num_l_systems()];
    GLuint tree_position_vertex_buffer[world_maker.get_num_l_systems()];

    world_maker.MakeWorld(diamond_square_vertex_buffers,
                          diamond_square_colour_buffers,
                          tree_vertex_buffer,
                          tree_position_vertex_buffer);


    /** Main Draw Loop **/

    int lod_current = controls.get_lod_level();
    int lod_old = lod_current;

    int colour_palette_current = controls.get_colour_palette();
    int colour_palette_old = colour_palette_current;

    do {


// Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



// Compute the MVP matrix from keyboard and mouse input
        controls.check_keypress();
        controls.computeMatricesFromInputs();

        if (controls.get_colour_palette() != colour_palette_old) {
            colour_palette_old = colour_palette_current;
            colour_palette_current = controls.get_colour_palette();

            vector<vector<vector<GLfloat>>> vertices = world_maker.get_gl_terrain_verts();
            world_maker.ComputeDiamondSquareColourBuffers(vertices.at(lod_current), diamond_square_colour_buffers,
                                                          colour_palette_current);
        }

        if (controls.get_lod_level() != lod_old) {
            vector<vector<vector<GLfloat>>> vertices = world_maker.get_gl_terrain_verts();
            world_maker.ComputeDiamondSquareColourBuffers(vertices.at(lod_current), diamond_square_colour_buffers,
                                                          colour_palette_current);
            lod_old = lod_current;
            lod_current = controls.get_lod_level();
            if (lod_current >= diamond_square_vertex_buffers.size()) {
                lod_current = diamond_square_vertex_buffers.size() - 1;
                lod_old = lod_current;
            }
        }

        mat4 ProjectionMatrix = controls.getProjectionMatrix();
        mat4 ViewMatrix = controls.getViewMatrix();
        mat4 ModelMatrix = glm::mat4(1.0);
        mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;


/** Terrain display commands**/
// Send our transformation to the currently bound shader,
// in the "MVP" uniform



        if (controls.is_show_terrain()) {

// Use terrain shader
            glUseProgram(terrain_shaders);

            glVertexAttribDivisor(1, 0); // disables instancing for the specified index

            glUniformMatrix4fv(terrain_shaders_uniform_id, 1, GL_FALSE, &MVP[0][0]);
            for (int i = 0; i < world_maker.get_diamond_square()->get_no_of_terrain_vertex_arrays(); i++) {

                // Attribute buffer - vertices
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, diamond_square_vertex_buffers[lod_current][i]);
                glVertexAttribPointer(
                        0,               // attribute. No particular reason for 0, but must match the layout in the shader.
                        3,               // size
                        GL_FLOAT,        // type
                        GL_FALSE,        // normalized?
                        0,               // stride
                        (void *) 0       // array buffer offset
                );

                // 2nd attribute buffer : colors
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, diamond_square_colour_buffers[i]);
                glVertexAttribPointer(
                        1,               // attribute. No particular reason for 1, but must match the layout in the shader.
                        3,               // size
                        GL_FLOAT,        // type
                        GL_FALSE,        // normalized?
                        0,               // stride
                        (void *) 0       // array buffer offset
                );

                // Draw the Terrain !
                glDrawArrays(GL_TRIANGLES, 0, world_maker.get_diamond_square()->get_no_of_vertices());
            }
        }

        if (controls.is_show_l_systems()) {

            glUseProgram(l_system_shaders);

            glUniformMatrix4fv(l_system_shaders_uniform_id, 1, GL_FALSE, &MVP[0][0]);

            for (int i = 0; i < world_maker.get_num_l_systems(); i++) {

                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, tree_vertex_buffer[i]);
                glVertexAttribPointer(
                        0,               // attribute. No particular reason for 0, but must match the layout in the shader.
                        3,               // size
                        GL_FLOAT,        // type
                        GL_FALSE,        // normalized?
                        0,               // stride
                        (void *) 0       // array buffer offset
                );
                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, tree_position_vertex_buffer[i]);
                glVertexAttribPointer(
                        1,               // attribute. No particular reason for 0, but must match the layout in the shader.
                        3,               // size
                        GL_FLOAT,        // type
                        GL_FALSE,        // normalized?
                        0,               // stride
                        (void *) 0       // array buffer offset
                );
                glEnableVertexAttribArray(2);

                glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
                glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1


                glDrawArraysInstanced(GL_LINES, 0, world_maker.get_tree()[i].get_vertices().size() / 3,
                                      world_maker.get_num_trees_()[i]);
            }
        }
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);


// Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);
    // Check if the ESC key was pressed or the window was closed

// Cleanup VBO and shader

    for (int i = 0; i < diamond_square_vertex_buffers.size(); i++) {
        for (int j = 0; j < diamond_square_vertex_buffers.at(i).size(); j++) {
            glDeleteBuffers(1, &diamond_square_vertex_buffers[i][j]);
        }

    }
    for (int i = 0; i < world_maker.get_diamond_square()->get_no_of_terrain_vertex_arrays(); i++) {
        glDeleteBuffers(1, &diamond_square_colour_buffers[i]);
    }

    for (int i = 0; i < world_maker.get_num_l_systems(); ++i) {
        glDeleteBuffers(1, &tree_vertex_buffer[i]);
        glDeleteBuffers(1, &tree_position_vertex_buffer[i]);
    }

    glDeleteProgram(terrain_shaders);
    glDeleteProgram(l_system_shaders);
    glDeleteVertexArrays(1, &VertexArrayID);

// Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

int main() {
    return OpenGLMagic();
}

