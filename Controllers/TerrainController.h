//
// Created by pip on 21/02/18.
//

#ifndef INC_3DPGNS_TERRAINCONTROLLER_H
#define INC_3DPGNS_TERRAINCONTROLLER_H


class TerrainController {
public:
    TerrainController();

    int randInRange(int range);

    void generateHeightMap(int x, int y, int initialRange);

};


#endif //INC_3DPGNS_TERRAINCONTROLLER_H
