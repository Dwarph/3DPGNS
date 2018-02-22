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
    this->divide(max);
}

float averageCorners(float a, float b, float c, float d) {
    return (a + b + c + d) / 4;
}

void diamond_step(int x, int z, int size, float offset) {
    float average = averageCorners(heightMap.at(x).at(z - size),
                                   heightMap.at(x + size).at(z),
                                   heightMap.at(x).at(z + size),
                                   heightMap.at(x - size).at(z));
    heightMap.at(x).at(z) = average + offset;
}

void square_step(int x, int z, int size, float offset) {
    float average = averageCorners(heightMap.at(x - size).at(z - size),
                                   heightMap.at(x + size).at(z - size),
                                   heightMap.at(x + size).at(z + size),
                                   heightMap.at(x - size).at(z + size));
    heightMap.at(x).at(z) = average + offset;
}


int randInRange(int range) {
    srand(time(NULL));
    return rand() % range + 1;
}

void divide(int size) {

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


vector<vector<float>> getHeightMap() {
    return heightMap;
}



