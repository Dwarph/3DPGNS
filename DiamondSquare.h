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
};


#endif //INC_3DPGNS_DIAMONDSQUARE_H
