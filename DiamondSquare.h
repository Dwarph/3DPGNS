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

private :
    void divide(int size);

    int maxX, maxZ, maxY;
    float roughness;
};


#endif //INC_3DPGNS_DIAMONDSQUARE_H
