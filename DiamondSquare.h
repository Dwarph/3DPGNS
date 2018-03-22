//
// Created by pip on 22/02/18.
//

#ifndef INC_3DPGNS_DIAMONDSQUARE_H
#define INC_3DPGNS_DIAMONDSQUARE_H

#include <vector>
#include <GL/glew.h>
#include <string>


using namespace std;

class DiamondSquare {

public:
    DiamondSquare(int max, int roughMax);

    vector<vector<float>> getHeightMap();

    void divide();

    void getVertices(vector<vector<GLfloat>> &arr, int noOfArrays, float scale);

    float getHeight(int x, int z);

    int randInRange(int range);

    int getMaxSize() const;

private :

    int maxSize;
    float roughness;
    vector<vector<float> > heightMap;

    float getValue(int x, int z);

    void diamond_step(int x, int z, int step, float offset);

    void square_step(int x, int z, int step, float offset);

    void resizeVec(std::vector<std::vector<float> > &vec, const unsigned short ROWS, const unsigned short COLUMNS);

    int setVertAtPoint(vector<GLfloat> &verts, int index, int x, int z, float scale);

    void printGrid(string initial);

    float averageCorners(float values[4]);

};


#endif //INC_3DPGNS_DIAMONDSQUARE_H
