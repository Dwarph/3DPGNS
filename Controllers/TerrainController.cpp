//
// Created by pip on 21/02/18.
//

#include "TerrainController.h"
#include <stdlib.h>     /* srand, rand */
#include <iostream>


int width;
int height;


int randInRange(int range) {
    srand(time(NULL));
    return rand() % range + 1;
}

void diamond_step(int x, int y, int step_size, int r, float heightMap) {

}

void square_step(int x, int y, int step_size, int r, float heightMap) {

}

void generateHeightMap(int x, int y, int range) {
    float heightMap[x][y] = {};
    int stepSize = x - 1;
    int randNum;

    while (stepSize > 1) {

        randNum = randInRange(range);

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {

            }
        }

        for (int i = 0; i < x; i++) {
            for (int j = 0; j < y; j++) {

            }
        }

        stepSize /= 2;
        range /= 2;


    }


}
