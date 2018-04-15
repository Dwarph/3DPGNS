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

using namespace std;

/**
 * Header file for DiamondSquare.cpp
 * Functions functionality detailed in the .cpp file
 */
class DiamondSquare {

public:

    DiamondSquare(int max, int rough_max, int no_of_terrain_vertex_arrays, int no_of_iterations);

    vector<vector<vector<float>>> get_height_map();

    void GenerateHeightMap();

    void GenerateVertices(vector<vector<vector<GLfloat>>> &gl_terrain_verts, float scale);

    float get_height(int grid_num, int x, int z);

    int RandInRange(int range, bool negative);

    int get_max_size() const;

    int get_no_of_vertices();

    float get_max_height() const;

    float get_min_height() const;

    int get_no_of_iterations();

    void ResizeVector2(std::vector<std::vector<float> > &vec, const unsigned short ROWS, const unsigned short COLUMNS);

    void ResizeVector3(std::vector<std::vector<std::vector<float> > > &vec, const unsigned short ROWS,
                       const unsigned short COLUMNS, const unsigned short DEPTH);

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


    void set_vert_at_point(vector<GLfloat> &verts, int *index, int grid_num, int x, int z, float scale);

    void PrintGrid(int grid_num, string initial, bool show_zero);

    float AverageValues(float *values);

};


#endif //INC_3DPGNS_DIAMONDSQUARE_H
