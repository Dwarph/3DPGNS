//
// Created by pip on 22/02/18.
//

#ifndef INC_3DPGNS_DIAMONDSQUARE_H
#define INC_3DPGNS_DIAMONDSQUARE_H

#include <vector>
#include <GL/glew.h>


using namespace std;

class DiamondSquare {

public:
    DiamondSquare(int max, float roughness);

    vector<vector<float>> getHeightMap();

    void divide(int size);

    void *
    getVertices(GLfloat *verticesOne, GLfloat *verticesTwo, GLfloat *verticesThree, GLfloat *verticesFour, float scale);

    float getHeight(int x, int z);


private :

    int maxX, maxZ, maxY;
    float roughness;
    vector<vector<float> > heightMap;

    float getValue(int x, int z);

    void square_step(int x, int z, int size, float offset);

    void diamond_step(int x, int z, int size, float offset);

    void resizeVec(std::vector<std::vector<float> > &vec, const unsigned short ROWS, const unsigned short COLUMNS);

    int setVertAtPoint(GLfloat *verts, int index, int x, int z, float scale);


};


#endif //INC_3DPGNS_DIAMONDSQUARE_H
