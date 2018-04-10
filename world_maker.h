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

    void MakeWorld(GLuint *terrainVertexBuffers, GLuint *diamondSquareColourBuffer, GLuint *treeVertexBuffer);

    int get_no_of_terrain_vertex_arrays();

    int get_terrain_size() const;

    float get_scale() const;

    DiamondSquare *get_diamond_square() const;

    LSystem *get_tree() const;

    const vector<GLfloat> &get_tree_positions_() const;

private:
    int terrain_size_;
    float scale_;
    DiamondSquare *diamond_square_;
    LSystem *tree_;
    std::vector<GLfloat> tree_positions_;



    void
    ComputeDiamondSquareBuffers(GLuint *vertexBuffers, GLuint *diamondSquareColourBuffer);

    void ComputeDiamondSquareColourBuffers(vector<vector<GLfloat>> gl_terrain_verts,
                                           GLuint *diamondSquareColourBuffers);

    void ComputeLSystemVertexBuffer(GLuint *vertexBuffer);

    void VertexColourRainbow(vector<vector<GLfloat>> &g_color_buffer_data);

    void VertexColourGreyscale(vector<vector<GLfloat>> &g_color_buffer_data, vector<vector<GLfloat>> gl_terrain_verts,
                               float min_height, float height_range);

    void VertexColourReal(vector<vector<GLfloat>> &g_color_buffer_data,
                          vector<vector<GLfloat>> gl_terrain_verts,
                          ColourList colours,
                          float min_height, float max_height);

    void VertexColourRealBlended(vector<vector<GLfloat>> &g_color_buffer_data,
                                 vector<vector<GLfloat>> gl_terrain_verts,
                                 ColourList colours,
                                 float min_height, float max_height);

    GLfloat *BlendColours(GLfloat *colour_one, GLfloat *colour_two, GLfloat *blended_colour, float percent);

    void GenerateTreePositionBuffer(int no_of_trees);

};


#endif //INC_3DPGNS_WORLD_MAKER_H
