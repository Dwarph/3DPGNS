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

    DiamondSquare(int max, int rough_max, int no_of_terrain_vertex_arrays);

    vector<vector<float>> get_height_map();

    void GenerateHeightMap();

    void GenerateVertices(vector<vector<GLfloat>> &gl_terrain_verts, float scale);

    float GetHeight(int x, int z);

    int RandInRange(int range);

    int get_max_size() const;

    int get_no_of_vertices();

private :

    int max_size_;
    int no_of_vertices_;
    float roughness_;
    vector<vector<float> > height_map_;
    int no_of_terrain_vertex_arrays_;

    float GetWrappedHeight(int x, int z);

    void DiamondStep(int x, int z, int step, float offset);

    void SquareStep(int x, int z, int step, float offset);

    void ResizeVector(std::vector<std::vector<float> > &vec, const unsigned short ROWS, const unsigned short COLUMNS);

    void set_vert_at_point(vector<GLfloat> &verts, int *index, int x, int z, float scale);

    void PrintGrid(string initial, bool show_zero);

    float AverageValues(float *values);

};


#endif //INC_3DPGNS_DIAMONDSQUARE_H
