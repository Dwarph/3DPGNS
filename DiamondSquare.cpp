//
// Created by pip on 22/02/18.
//

#include "DiamondSquare.h"
#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <iomanip>

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
    srand(time(NULL));

    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++) {
            heightMap.at(i).at(j) = 0;
        }
    }


    this->heightMap.at(0).at(0) = randInRange(10) % 10;
    this->heightMap.at(max - 1).at(0) = randInRange(10) % 10;
    this->heightMap.at(max - 1).at(max - 1) = randInRange(10) % 10;
    this->heightMap.at(0).at(max - 1) = randInRange(10) % 10;

    this->divide();
}

float averageCorners(float values[4]) {
    float average = 0.0;
    int avgTot = 0;

    for (int i = 0; i < 4; i++) {
        if (values[i] != 0) {
            average += values[i];
            avgTot++;
        }
    }

    if (avgTot == 0) {
        return 0;
    } else {
        return average / avgTot;
    }

}

void DiamondSquare::printGrid() {

    std::cout << std::fixed;
    std::cout << std::setprecision(0);

    for (int j = 0; j < this->maxZ; j++) {
        for (int i = 0; i < this->maxX; i++) {

            if (this->heightMap.at(i).at(j) == 0) {
                cout << " " << " " << "\t";
                //  cout << " " << this->heightMap.at(i).at(j) << "\t";


            } else {
                cout << " " << this->heightMap.at(i).at(j) << "\t";
            }
        }
        cout << endl;
    }
    cout << "===============" << endl;
}

void DiamondSquare::divide() {

    int stepSize = maxX - 1;
    bool isEven = true;
    float offset = 100;
    int randInt = 100;
    float randNum = ((float) randInRange(randInt)) / 100;;

    //  stepSize--;


    float values[4] = {getValue(0, maxX - 1),
                       getValue(0, maxZ - 1),
                       getValue(maxX - 1, 0),
                       getValue(maxZ - 1, 0)};
    float average = averageCorners(values);
    this->heightMap.at((maxX - 1) / 2).at((maxZ - 1) / 2) = average + randNum;


    while (stepSize > 1) {
        randNum = ((float) randInRange(randInt)) / 100;

        int halfSize = stepSize / 2;

        // halfSize--;
        // float scale = roughness * stepSize;


        //   cout << "stepSize: " << stepSize << endl;

        //   cout << "SCALE: " << scale  << endl;

//        cout << "Rand Offset: " << offset * randNum << endl;

        //   cout << randNum << endl;


        for (int z = 0; z < maxZ - 1; z += stepSize) {
            for (int x = 0; x < maxX - 1; x += stepSize) {
                diamond_step(x, z, stepSize, randNum * roughness);
            }
        }
        cout << "diamond: " << endl;
        printGrid();

        for (int z = 0; z < maxZ; z += halfSize) {

            for (int x = 0; x < maxX; x += halfSize) {
                square_step(x, z, halfSize, randNum * roughness);
            }
            isEven = !isEven;
        }
        cout << "square: " << endl;
        printGrid();

        stepSize /= 2;
        randInt - 5;
    }
}

void DiamondSquare::square_step(int x, int z, int step, float offset) {
    float values[4] = {getValue(x, z),
                       getValue(x, z + step),
                       getValue(x + step, z),
                       getValue(x + step, z + step)};

    float average = averageCorners(values);
    this->heightMap.at(x).at(z) = average + offset;
}

void DiamondSquare::diamond_step(int x, int z, int step, float offset) {
    int halfStep = step / 2;

    float values[4] = {getValue(x - step, z),
                       getValue(x + step, z),
                       getValue(x, z + step),
                       getValue(x, z - step)};
    float average = averageCorners(values);
    this->heightMap.at(x + halfStep).at(z + halfStep) = average + offset;
}

float DiamondSquare::getValue(int x, int z) {

    //this allows for all values to be based off of 4 values, by wrapping the squares and diamonds.
    if (x < 0) {
        x = this->maxX + x - 1;
    } else if (x >= this->maxX) {
        x = x - (this->maxX - 1);
    }

    if (z < 0) {
        z = this->maxZ + z - 1;
    } else if (z >= this->maxZ) {
        z = z - (this->maxZ - 1);
    }
    return this->heightMap.at(x).at(z);

}

vector<vector<float>> DiamondSquare::getHeightMap() {
    return heightMap;
}

void *DiamondSquare::getVertices(GLfloat *verticesOne,
                                 GLfloat *verticesTwo,
                                 GLfloat *verticesThree,
                                 GLfloat *verticesFour,
                                 float scale) {

    int index = 0;
    int maxIteration = (((this->maxX - 1) * (this->maxZ - 1)) * 2 * 3 *
                        3); //*2 for noOfTriangles, then *3 for noOfVerts, then 3 for number of points. Divide by number of arrays
    int count = 0;

    GLfloat *vertices = verticesOne;

    //iterates over the depth (z) and the width (x)
    //then iterates 3 times for each vertice in a triangle
    //and 2 times for 2 triangles per square
    for (int z = 0; z < this->maxZ - 1; z++) {
        for (int x = 0; x < this->maxX - 1; x++) {
            for (int j = 0; j < 2; j++) {
                for (int i = 0; i < 3; i++) {


//                    if (index >= maxIteration) {
//                        count++;
//                        index = 0;
//
//
//                        switch (count) {
//                            case 0:
//                                vertices = verticesOne;
//                                break;
//                            case 1:
//                                vertices = verticesTwo;
//                                break;
//                            case 2:
//                                vertices = verticesThree;
//                                break;
//                            case 3:
//                                vertices = verticesFour;
//                                break;
//                        }
//                    }
                    switch (i) {
                        case 0:
                            if (j == 0) {
                                index = setVertAtPoint(vertices, index, x, z, scale);
                            } else {
                                index = setVertAtPoint(vertices, index, x + 1, z, scale);
                            }
                            break;

                        case 1:
                            if (j == 0) {
                                index = setVertAtPoint(vertices, index, x, z + 1, scale);
                            } else {
                                index = setVertAtPoint(vertices, index, x + 1, z + 1, scale);
                            }
                            break;

                        case 2:

                            if (j == 0) {
                                index = setVertAtPoint(vertices, index, x + 1, z, scale);
                            } else {
                                index = setVertAtPoint(vertices, index, x, z + 1, scale);
                            }
                            break;
                    }
                }
            }
        }
    }
}

int DiamondSquare::setVertAtPoint(GLfloat *vertices, int index, int x, int z, float scale) {
    vertices[index] = x * scale;
    index++;
    vertices[index] = this->getHeight(x, z) * scale;
    index++;
    vertices[index] = z * scale;
    index++;
    return index;
}

float DiamondSquare::getHeight(int x, int z) {
    //gets the heights at the appropriate index

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

int DiamondSquare::randInRange(int range) {
    int randNum = rand() % range + 1;

    if (rand() % 3 == 1) {
        return randNum * -1;
    } else {
        return randNum;
    }
}


