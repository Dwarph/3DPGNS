//
// Created by pip on 22/02/18.
//
#include "diamond_square.h"


DiamondSquare::DiamondSquare(int max, int rough_max, int no_of_terrain_vertex_arrays) {
    this->max_size = max;       //width
    this->no_of_terrain_vertex_arrays = no_of_terrain_vertex_arrays;
    this->no_of_vertices =
            (((this->max_size - 1) * (this->max_size - 1) * 2) / this->no_of_terrain_vertex_arrays) * 3 * 3;
//    this->roughness = 0.09;
    resize_vec(height_map, max, max);
    srand(time(NULL));

    this->roughness = (float) (rand() % (rough_max) + 1);
    this->roughness = this->roughness * 0.01;
    this->roughness += 0.05;
    cout << "ROUGH: " << this->roughness << endl;


    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++) {
            height_map.at(i).at(j) = 0;
        }
    }


    this->height_map.at(0).at(0) = (max * rand_in_range(10) % 10);
    this->height_map.at(max - 1).at(0) = (max * rand_in_range(10) % 10);
    this->height_map.at(max - 1).at(max - 1) = (max * rand_in_range(10) % 10);
    this->height_map.at(0).at(max - 1) = (max * rand_in_range(10) % 10);

    this->divide();
}


void DiamondSquare::print_grid(string initial) {

    std::cout << std::fixed;
    std::cout << std::setprecision(0);

    cout << initial << endl;
    for (int j = 0; j < this->max_size; j++) {
        for (int i = 0; i < this->max_size; i++) {

            if (this->height_map.at(i).at(j) == 0) {
                cout << " " << " " << "\t";
                //  cout << " " << this->height_map.at(i).at(j) << "\t";


            } else {
                cout << " " << this->height_map.at(i).at(j) << "\t";
            }
        }
        cout << endl;
    }
    cout << "===============" << endl;
}

void DiamondSquare::divide() {

    float randNum;
    int stepSize = max_size - 1;
    int randInt = 100;
    int finalStep;

    while (stepSize > 1) {

        int halfSize = stepSize / 2;
        float scale = roughness * stepSize;
        randNum = ((float) rand_in_range(randInt)) / randInt;
        if (randNum < 0) {
            randNum *= -1;
        }

//        if(stepSize/2 < 1){
//            randNum = 1;
//        }
        // float offset = randNum * scale;
        float offset = randNum * scale * 2 - scale;
        //  cout << offset << endl;

        for (int z = 0; z < max_size - 1; z += stepSize) {
            for (int x = 0; x < max_size - 1; x += stepSize) {

                randNum = ((float) rand_in_range(randInt)) / randInt;
                offset = randNum * scale * 2 - scale;
                diamond_step(x, z, stepSize, offset);
            }
        }
        //   print_grid("diamond: ");

        for (int z = 0; z < max_size; z += halfSize) {
            for (int x = 0; x < max_size; x += halfSize) {
                randNum = ((float) rand_in_range(randInt)) / randInt;
                offset = randNum * scale * 2 - scale;
                square_step(x, z, halfSize, offset);
            }
        }
        //   print_grid( "square: ");


        stepSize /= 2;
    }

    float heightOffset = height_map[0][0];
    for (int z = 0; z < max_size; z++) {
        for (int x = 0; x < max_size; x++) {
            height_map[z][x] -= heightOffset - 1;
        }
    }
}

void DiamondSquare::square_step(int x, int z, int step, float offset) {
    float values[4] = {getValue(x, z),
                       getValue(x, z + step),
                       getValue(x + step, z),
                       getValue(x + step, z + step)};

    float average = average_corners(values);
    this->height_map.at(x).at(z) = average + offset;
}

void DiamondSquare::diamond_step(int x, int z, int step, float offset) {
    int halfStep = step / 2;

    float values[4] = {getValue(x - step, z),
                       getValue(x + step, z),
                       getValue(x, z + step),
                       getValue(x, z - step)};
    float average = average_corners(values);
    this->height_map.at(x + halfStep).at(z + halfStep) = average + offset;
}

int DiamondSquare::get_no_of_vertices() {
    return this->no_of_vertices;
}

void DiamondSquare::getVertices(vector<vector<GLfloat>> &gl_terrain_verts, float scale) {

    int index = 0;


    //*2 for noOfTriangles, then *3 for noOfVerts, then 3 for number of points. Divide by number of arrays
    int count = 0;


    //iterates over the depth (z) and the width (x)
    //then iterates 3 times for each vertice in a triangle
    //and 2 times for 2 triangles per square
    for (int z = 0; z < this->max_size - 1; z++) {
        for (int x = 0; x < this->max_size - 1; x++) {
            for (int j = 0; j < 2; j++) {
                for (int i = 0; i < 3; i++) {
                    if (index >= this->no_of_vertices) {
                        count++;
                        index = 0;
                    }

                    switch (i) {
                        case 0:
                            if (j == 0) {
                                index = set_vert_at_point(gl_terrain_verts[count], index, x, z, scale);
                            } else {
                                index = set_vert_at_point(gl_terrain_verts[count], index, x + 1, z, scale);
                            }
                            break;

                        case 1:
                            if (j == 0) {
                                index = set_vert_at_point(gl_terrain_verts[count], index, x, z + 1, scale);
                            } else {
                                index = set_vert_at_point(gl_terrain_verts[count], index, x, z + 1, scale);
                            }
                            break;

                        case 2:
                            if (j == 0) {
                                index = set_vert_at_point(gl_terrain_verts[count], index, x + 1, z, scale);
                            } else {
                                index = set_vert_at_point(gl_terrain_verts[count], index, x + 1, z + 1, scale);
                            }
                            break;
                    }
                }
            }
        }
    }
}

float DiamondSquare::getValue(int x, int z) {

    //this allows for all values to be based off of 4 values, by wrapping the squares and diamonds.
    if (x < 0) {
        x = this->max_size + x - 1;
    } else if (x >= this->max_size) {
        x = x - (this->max_size - 1);
    }


    if (z < 0) {
        z = this->max_size + z - 1;
    } else if (z >= this->max_size) {
        z = z - (this->max_size - 1);
    }
    return this->height_map.at(x).at(z);

}


float DiamondSquare::average_corners(float *values) {
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

vector<vector<float>> DiamondSquare::getHeightMap() {
    return height_map;
}

int DiamondSquare::set_vert_at_point(vector<GLfloat> &vertices, int index, int x, int z, float scale) {
    vertices[index] = x * scale;
    index++;
    vertices[index] = this->get_height(x, z) * scale;
    index++;
    vertices[index] = z * scale;
    index++;
    return index;
}

float DiamondSquare::get_height(int x, int z) {
    //gets the heights at the appropriate index
    return height_map.at(x).at(z);
}

//http://www.cplusplus.com/forum/beginner/102670/ source
void DiamondSquare::resize_vec(std::vector<std::vector<float> > &vec, const unsigned short ROWS,
                               const unsigned short COLUMNS) {
    vec.resize(ROWS);
    for (auto &it : vec) {
        it.resize(COLUMNS);
    }
}

int DiamondSquare::rand_in_range(int range) {
    int randNum = rand() % range + 1;

    if (rand() % 3 == 1) {
        return randNum * -1;
    } else {
        return randNum;
    }
}

int DiamondSquare::get_max_size() const {
    return max_size;
}


