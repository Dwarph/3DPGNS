//
// Created by pip on 22/02/18.
//

#include "DiamondSquare.h"
#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <vector>

using namespace std;

int maxX, maxZ, maxY;
float roughness;
vector<vector<float> > heightMap;


DiamondSquare::DiamondSquare(int max, float roughness) {
    this->maxX = max;       //width
    this->maxZ = max;       //depth
    this->maxY = (maxX + maxZ) / 3; //height
    this->roughness = roughness;
    resizeVec(heightMap, max, max);
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            heightMap.at(i).at(j) = 0;
        }
    }
    this->divide(max);
}


float averageCorners(float values[4]) {
    int counter = 0;
    float average = 0;

    for (int i = 0; i < 4; i++) {
        if (values[i] != -1) {
            counter++;
            average += values[i];
        }
    }
    if (counter == 0) {
        return 0;
    } else {
        return average / 4;
    }
}

void DiamondSquare::diamond_step(int x, int z, int size, float offset) {
    float values[4] = {getValue(x, z - size),
                       getValue(x + size, z),
                       getValue(x, z + size),
                       getValue(x - size, z)};

    float average = averageCorners(values);
    this->heightMap.at(x).at(z) = average + offset;
}

void DiamondSquare::square_step(int x, int z, int size, float offset) {
    float values[4] = {getValue(x - size, z - size),
                       getValue(x + size, z - size),
                       getValue(x + size, z + size),
                       getValue(x - size, z + size)};
    float average = averageCorners(values);
    this->heightMap.at(x).at(z) = average + offset;
}

//TEMP SOLUTION - final solution is to move this to getAverage method
float DiamondSquare::getValue(int x, int z) {
    if (x < 0 || x > this->maxX - 1) {
        return -1;
    } else if (z < 0 || z > this->maxZ - 1) {
        return -1;
    } else {
        return this->heightMap.at(x).at(z);
    }
}

int randInRange(int range) {
    srand(time(NULL));
    return rand() % range + 1;
}

void DiamondSquare::divide(int size) {

    int halfSize = size / 2;
    float scale = roughness * size;

    float randNum = ((float) randInRange(10)) / 10;

    for (int z = halfSize; z < maxZ; z++) {
        for (int x = halfSize; x < maxX; x++) {
            square_step(x, z, halfSize, randNum * scale * 2 - scale);
        }
    }

    for (int z = halfSize; z < maxZ; z++) {
        for (int x = halfSize; x < maxX; x++) {
            diamond_step(x, z, halfSize, randNum * scale * 2 - scale);
        }
    }

}


vector<vector<float>> DiamondSquare::getHeightMap() {
    return heightMap;
}

//http://www.cplusplus.com/forum/beginner/102670/ source
void DiamondSquare::resizeVec(std::vector<std::vector<float> > &vec, const unsigned short ROWS,
                              const unsigned short COLUMNS) {
    vec.resize(ROWS);
    for (auto &it : vec) {
        it.resize(COLUMNS);
    }
}


