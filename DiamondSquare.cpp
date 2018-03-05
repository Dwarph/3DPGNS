//
// Created by pip on 22/02/18.
//

#include "DiamondSquare.h"
#include <stdlib.h>     /* srand, rand */
#include <iostream>

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
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++) {
            heightMap.at(i).at(j) = 0;
        }
    }

    this->heightMap.at(0).at(0) = max / 2;
    this->heightMap.at(max - 1).at(0) = max / 2;
    this->heightMap.at(max - 1).at(max - 1) = max / 2;
    this->heightMap.at(0).at(max - 1) = max / 2;

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


    for (int i = 0; i < this->maxX; i++) {
        for (int j = 0; j < this->maxZ; j++) {
            cout << " " << this->heightMap.at(i).at(j);
        }
        cout << endl;
    }
    cout << "===============" << endl;

    int halfSize = size / 2;
    if (halfSize < 1) {
        return;
    }
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

    divide(size / 2);

}


vector<vector<float>> DiamondSquare::getHeightMap() {
    return heightMap;
}

void *DiamondSquare::getVertices(GLfloat *vertices, float scale) {

    int index = 0;
    //iterates over the depth (z) and the width (x)
    //then iterates 3 times for each vertice in a triangle
    //and 2 times for 2 triangles per square
    for (int z = 0; z < this->maxZ; z++) {
        for (int x = 0; x < this->maxX; x++) {
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 2; j++) {

                    switch (i) {
                        case 0:
                            if (j == 0) {
                                vertices[index] = x * scale;
                                index++;
                                vertices[index] = this->getHeight(x, z) * scale;

                            } else {
                                vertices[index] = (x + 1) * scale;
                                index++;
                                vertices[index] = this->getHeight(x + 1, z) * scale;
                            }
                            index++;
                            vertices[index] = z * scale;
                            index++;
                            break;


                        case 1:

                            if (j == 0) {
                                vertices[index] = x * scale;
                                index++;
                                vertices[index] = this->getHeight(x, z + 1) * scale;
                                index++;
                                vertices[index] = (z + 1) * scale;
                                index++;
                            } else {
                                vertices[index] = (x + 1) * scale;
                                index++;
                                vertices[index] = this->getHeight(x + 1, z) * scale;
                                index++;
                                vertices[index] = z * scale;
                                index++;
                            }
                            break;
                        case 2:

                            if (j == 0) {
                                vertices[index] = (x + 1) * scale;
                                index++;
                                vertices[index] = this->getHeight(x + 1, z + 1) * scale;
                                index++;
                                vertices[index] = (z + 1) * scale;
                                index++;

                            } else {
                                vertices[index] = x * scale;
                                index++;
                                vertices[index] = this->getHeight(x, z + 1) * scale;
                                index++;
                                vertices[index] = (z + 1) * scale;
                                index++;
                            }
                            break;
                    }
                }
            }
        }
    }
}


float DiamondSquare::getHeight(int x, int z) {
    int newX, newZ;
    newX = (x == 0) ? 0 : (x + 2 - 1) / 2;
    newZ = (z == 0) ? 0 : (z + 2 - 1) / 2;
    return heightMap.at(newX).at(newZ);
}

//http://www.cplusplus.com/forum/beginner/102670/ source
void DiamondSquare::resizeVec(std::vector<std::vector<float> > &vec, const unsigned short ROWS,
                              const unsigned short COLUMNS) {
    vec.resize(ROWS);
    for (auto &it : vec) {
        it.resize(COLUMNS);
    }
}


