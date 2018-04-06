//
// Created by pip on 23/03/18.
//

#ifndef INC_3DPGNS_WORLD_MAKER_H
#define INC_3DPGNS_WORLD_MAKER_H

#include <math.h>
#include <GL/glew.h>

#include "./diamond_square.h"

struct colour_list {
    GLfloat ivy_green[3] = {0.38, 0.47, 0.28};
    GLfloat moss_green[3] = {0.47, 0.56, 0.28};
    GLfloat snow_white[3] = {0.94, 0.94, 0.85};
    GLfloat dusty_brown[3] = {0.65, 0.55, 0.41};
    GLfloat wet_brown[3] = {0.35, 0.27, 0.2};
    GLfloat light_grey[3] = {0.85, 0.84, 0.8};
    GLfloat med_grey[3] = {0.72, 0.68, 0.6};
};

class world_maker {

public:

    world_maker(int terrain_size, float scale);

    void makeWorld(GLuint *vertexBuffers, GLuint *diamondSquareColourBuffer);

    int get_no_of_terrain_vertex_arrays();

    int getTerrain_size() const;

    float getScale() const;

    DiamondSquare *getDiamondSquare() const;

private:
    int terrain_size;
    float scale;
    DiamondSquare *diamondSquare;

    void
    compute_diamond_square_vertex_buffers(GLuint *vertexBuffers, GLuint *diamondSquareColourBuffer);

    void compute_diamond_square_colour_buffer(vector<vector<GLfloat>> gl_terrain_verts,
                                              GLuint *diamondSquareColourBuffers);

    void vertex_colour_rainbow(vector<vector<GLfloat>> &g_color_buffer_data);

    void vertex_colour_greyscale(vector<vector<GLfloat>> &g_color_buffer_data, vector<vector<GLfloat>> gl_terrain_verts,
                                 float min_height, float height_range);

    void vertex_colour_real(vector<vector<GLfloat>> &g_color_buffer_data,
                            vector<vector<GLfloat>> gl_terrain_verts,
                            colour_list colours,
                            float min_height, float max_height);

    void
    vertex_colour_real_blended(vector<vector<GLfloat>> &g_color_buffer_data, vector<vector<GLfloat>> gl_terrain_verts,
                               colour_list colours, float min_height, float max_height);

    GLfloat *get_blended_colour(GLfloat *colour_one, GLfloat *colour_two, GLfloat *blended_colour, float percent);

};


#endif //INC_3DPGNS_WORLD_MAKER_H
