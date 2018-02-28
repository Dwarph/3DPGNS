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
            //      cout << " " << this->heightMap.at(i).at(j);
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

GLfloat *DiamondSquare::getVertices(GLfloat *vertices) {
    int noOfVertices = ((this->maxZ - 1) * (this->maxX - 1)) * 2 * 3 *
                       3; //*2 for noOfTriangles, then *3 for noOfVerts, then 3 for number of points.
    vertices = new GLfloat[noOfVertices];
    for (int x = 0; x < this->maxX - 1; x++) {
        for (int z = 0; z < this->maxZ - 1; z++) {
            int index = (x + z) + z * 3 * 3;
            for (int i = 0; i < 3; i++) {
                switch (i) {
                    case 0:
                        vertices[index] = x;
                        vertices[++index] = this->heightMap.at(x).at(z);
                        vertices[++index] = z;

                        cout << "x: " << x << ", z: " << z << endl;
                        break;
                    case 1:
                        vertices[index] = x;
                        vertices[++index] = this->heightMap.at(x).at(z + 1);
                        vertices[++index] = z;

                        break;
                    case 2:
                        vertices[index] = x;
                        if (z % 2 == 0) {
                            vertices[++index] = this->heightMap.at(x + 1).at(z + 1);
                        } else {
                            vertices[++index] = this->heightMap.at(x).at(z + 1);
                        }
                        vertices[++index] = z;

                        break;
                }
                index++;
            }


        }
    }
    return vertices;
}


float DiamondSquare::getHeight(int x, int z) {
    return heightMap.at(x).at(z);
}
//http://www.cplusplus.com/forum/beginner/102670/ source
void DiamondSquare::resizeVec(std::vector<std::vector<float> > &vec, const unsigned short ROWS,
                              const unsigned short COLUMNS) {
    vec.resize(ROWS);
    for (auto &it : vec) {
        it.resize(COLUMNS);
    }
}


