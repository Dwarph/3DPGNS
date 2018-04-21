//
// Created by pip on 23/03/18.
//

#ifndef INC_3DPGNS_WORLD_MAKER_H
#define INC_3DPGNS_WORLD_MAKER_H

#include <math.h>
#include <GL/glew.h>

#include "./diamond_square.h"
#include "./l_system.h"


struct ColourList {
    GLfloat ivy_green[3] = {0.38, 0.47, 0.28};
    GLfloat moss_green[3] = {0.47, 0.56, 0.28};
    GLfloat snow_white[3] = {0.94, 0.94, 0.85};
    GLfloat dusty_brown[3] = {0.65, 0.55, 0.41};
    GLfloat wet_brown[3] = {0.35, 0.27, 0.2};
    GLfloat light_grey[3] = {0.85, 0.84, 0.8};
    GLfloat med_grey[3] = {0.72, 0.68, 0.6};
};

class WorldMaker {

public:

    WorldMaker(int terrain_size, float scale);

    void MakeWorld(vector<vector<GLuint >> &terrain_vertex_buffers,
                   GLuint *diamond_square_colour_buffers,
                   GLuint *tree_vertex_buffer,
                   GLuint *tree_position_vertex_buffer);

    int get_terrain_size() const;

    vector<int> get_num_trees_() const;

    DiamondSquare *get_diamond_square() const;

    std::vector<LSystem> get_tree() const;

    const vector<vector<GLfloat>> &get_tree_positions_() const;

    const int get_num_l_systems() const;

    float get_terrain_scale() const;

private:
    int terrain_size_;
    int terrain_scale_;
    DiamondSquare *diamond_square_;
    std::vector<LSystem> trees_;
    std::vector<int> num_l_systems_;
    std::vector<vector<GLfloat>> tree_positions_;


    void
    ComputeDiamondSquareBuffers(vector<vector<GLuint >> &terrain_vertex_buffers, GLuint *diamond_square_colour_buffers);

    void ComputeDiamondSquareColourBuffers(vector<vector<GLfloat>> &gl_terrain_verts,
                                           GLuint *diamond_square_colour_buffers);

    void ComputeLSystemVertexBuffer(GLuint *vertex_buffer);

    void VertexColourRainbow(vector<vector<GLfloat>> &g_color_buffer_data);

    void VertexColourGreyscale(vector<vector<GLfloat>> &g_color_buffer_data, vector<vector<GLfloat>> &gl_terrain_verts);

    void VertexColourReal(vector<vector<GLfloat>> &g_color_buffer_data,
                          vector<vector<GLfloat>> &gl_terrain_verts,
                          ColourList colours);

    void VertexColourRealBlended(vector<vector<GLfloat>> &g_color_buffer_data,
                                 vector<vector<GLfloat>> &gl_terrain_verts,
                                 ColourList colours);

    GLfloat *BlendColours(GLfloat *colour_one, GLfloat *colour_two, GLfloat *blended_colour, float percent);

    void GenerateTreePositionBuffer(GLuint *tree_position_vertex_buffer);
};

#endif //INC_3DPGNS_WORLD_MAKER_H
