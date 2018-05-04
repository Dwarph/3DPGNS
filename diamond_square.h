//
// Created by pip on 22/02/18.
//

#ifndef INC_3DPGNS_DIAMONDSQUARE_H
#define INC_3DPGNS_DIAMONDSQUARE_H

#include <vector>
#include <GL/glew.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

/**
 * Header file for DiamondSquare.cpp
 * Functions functionality detailed in the .cpp file
 */
class DiamondSquare {

public:

    DiamondSquare(int rough_max, int no_of_iterations);

    vector<vector<vector<float>>> get_height_map();

    float get_height(int grid_num, int x, int z);

    int get_max_size() const;

    int get_no_of_vertices();

    float get_max_height() const;

    float get_min_height() const;

    int get_no_of_iterations();

    int get_no_of_terrain_vertex_arrays() const;

    void GenerateHeightMap();

    void GenerateVertices(vector<vector<vector<GLfloat>>> &gl_terrain_verts, float scale);

    int RandInRange(int range, bool negative);

    void ResizeVector2(std::vector<std::vector<float> > &vec, const unsigned short ROWS, const unsigned short COLUMNS);

private :

    int max_size_;
    int no_of_vertices_;
    int no_of_iterations;
    float roughness_;
    vector<vector<vector<float>>> height_map_;
    int no_of_terrain_vertex_arrays_;
    float max_height_;
    float min_height_;

    float get_wrapped_height(int grid_num, int x, int z);

    void DiamondStep(int grid_num, int x, int z, int step, float offset);

    void SquareStep(int grid_num, int x, int z, int step, float offset);

    void
    set_vert_at_point(vector<GLfloat> &verts, int *index, int grid_num, int x, int z, float scale, int height_map_size);

    void PrintGrid(int grid_num, string initial, bool show_zero);

    float AverageValues(float *values);

};


#endif //INC_3DPGNS_DIAMONDSQUARE_H
