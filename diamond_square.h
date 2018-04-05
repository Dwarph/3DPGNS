//
// Created by pip on 22/02/18.
//

#ifndef INC_3DPGNS_DIAMONDSQUARE_H
#define INC_3DPGNS_DIAMONDSQUARE_H

#include <vector>
#include <GL/glew.h>
#include <string>

#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <iomanip>

using namespace std;


class DiamondSquare {

public:

    DiamondSquare(int max, int rough_max, int no_of_terrain_vertex_arrays);

    vector<vector<float>> getHeightMap();

    void divide();

    void getVertices(vector<vector<GLfloat>> &gl_terrain_verts, float scale);

    float get_height(int x, int z);

    int rand_in_range(int range);

    int get_max_size() const;

    int get_no_of_vertices();


private :

    int max_size;
    int no_of_vertices;
    float roughness;
    vector<vector<float> > height_map;
    int no_of_terrain_vertex_arrays;

    float getValue(int x, int z);

    void diamond_step(int x, int z, int step, float offset);

    void square_step(int x, int z, int step, float offset);

    void resize_vec(std::vector<std::vector<float> > &vec, const unsigned short ROWS, const unsigned short COLUMNS);

    int set_vert_at_point(vector<GLfloat> &verts, int index, int x, int z, float scale);

    void print_grid(string initial);

    float average_corners(float *values);

};


#endif //INC_3DPGNS_DIAMONDSQUARE_H
