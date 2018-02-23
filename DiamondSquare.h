//
// Created by pip on 22/02/18.
//

#ifndef INC_3DPGNS_DIAMONDSQUARE_H
#define INC_3DPGNS_DIAMONDSQUARE_H

#include <vector>

using namespace std;

class DiamondSquare {

public:
    DiamondSquare(int max, float roughness);

    vector<vector<float>> getHeightMap();

    void divide(int size);


private :

    int maxX, maxZ, maxY;
    float roughness;
    vector<vector<float> > heightMap;

    void square_step(int x, int z, int size, float offset);

    void diamond_step(int x, int z, int size, float offset);

    float getValue(int x, int z);

    void resizeVec(std::vector<std::vector<float> > &vec, const unsigned short ROWS, const unsigned short COLUMNS);

};


#endif //INC_3DPGNS_DIAMONDSQUARE_H
