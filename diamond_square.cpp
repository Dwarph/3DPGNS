//
// Created by pip on 22/02/18.
//
#include "diamond_square.h"

/**
 * This class handles all code relating to the Diamond Square algorithm -
 * This class generates a heightmap, which we apply to give us procedural terrain
 */


/* Constructors */

/**
 * Our constructor intialises the size of the width and depth using max,
 * the range of roughness using rough_max and tells our class the number of
 * terrain vertex arrays we will be using
 * @param max
 * @param rough_max
 * @param no_of_terrain_vertex_arrays
 */

DiamondSquare::DiamondSquare(int max, int rough_max, int no_of_terrain_vertex_arrays) {

    // sets our width & depth size
    this->max_size_ = max;

    //resizes our height_map_ vector appropriately
    ResizeVector(height_map_, max, max);

    //provides us with the number of terrain vertex arrays
    this->no_of_terrain_vertex_arrays_ = no_of_terrain_vertex_arrays;

    //calculates the number of vertices of the terrain
    this->no_of_vertices_ =
            (((this->max_size_ - 1) * (this->max_size_ - 1) * 2) / this->no_of_terrain_vertex_arrays_) * 3 * 3;

    //sets the seed_ of the random number generator algorithm used by the function rand using the internal clock.
    srand(time(NULL));

    //calculates the roughness_ of the terrain. Currently random each time.
    this->roughness_ = (((float) (rand() % (rough_max) + 1)) * 0.01) + 0.05;

    cout << "ROUGH: " << this->roughness_ << endl;


    //seeds the initial values of the heightmap to 0
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < max; j++) {
            height_map_.at(i).at(j) = 0;
        }
    }

    //sets the four corners of the heightmap to a random height between 0 & 1
    this->height_map_.at(0).at(0) = (max * RandInRange(10) % 10);
    this->height_map_.at(max - 1).at(0) = (max * RandInRange(10) % 10);
    this->height_map_.at(max - 1).at(max - 1) = (max * RandInRange(10) % 10);
    this->height_map_.at(0).at(max - 1) = (max * RandInRange(10) % 10);

    //generates the height map
    this->GenerateHeightMap();
}

/* Getters and Setters */


/**
 * Returns the int "max_size"
 * @return
 */
int DiamondSquare::get_max_size() const {
    return max_size_;
}

/**
 * Returns the height at the point {x, z}
 * @param x
 * @param z
 * @return
 */
float DiamondSquare::GetHeight(int x, int z) {
    //gets the heights at the appropriate index
    return height_map_.at(x).at(z);
}

/**
 * Gets a value at the point {x, z}.
 * If the values given are outside of the grid, this function wraps them to the other side of the grid.
 * E.G. if the grid is x: 0-4, z: 0-4 and this function is given {5,5}, it will return the value at {0,0}
 * @param x
 * @param z
 * @return
 */
float DiamondSquare::GetWrappedHeight(int x, int z) {

    //this allows for all values to be based off of 4 values, by wrapping the squares and diamonds.
    if (x < 0) {
        x = this->max_size_ + x - 1;
    } else if (x >= this->max_size_) {
        x = x - (this->max_size_ - 1);
    }

    if (z < 0) {
        z = this->max_size_ + z - 1;
    } else if (z >= this->max_size_) {
        z = z - (this->max_size_ - 1);
    }

    return this->height_map_.at(x).at(z);
}

/**
 * Returns the number of vertices
 * @return
 */
int DiamondSquare::get_no_of_vertices() {
    return this->no_of_vertices_;
}

/**
 * Returns the height_map
 * @return
 */
vector<vector<float>> DiamondSquare::get_height_map() {
    return height_map_;
}

/**
 * Sets the vertice at the point [index] to the point {x * scale, get_height(x, z) * scale, z * scale}
 * @param vertices
 * @param index
 * @param x
 * @param z
 * @param scale
 */
void DiamondSquare::set_vert_at_point(vector<GLfloat> &vertices, int *index, int x, int z, float scale) {
    vertices[*index] = x * scale;
    (*index)++;
    vertices[*index] = this->GetHeight(x, z) * scale;
    (*index)++;
    vertices[*index] = z * scale;
    (*index)++;
}

/* Main Functions */

/**
 * This function generates the height map using the Diamond Square algorithm
 */
void DiamondSquare::GenerateHeightMap() {

    float randNum;
    int stepSize = max_size_ - 1;
    int randInt = 100;

    while (stepSize > 1) {

        int halfSize = stepSize / 2;
        float scale = roughness_ * stepSize;
        float offset;

        for (int z = 0; z < max_size_ - 1; z += stepSize) {
            for (int x = 0; x < max_size_ - 1; x += stepSize) {

                randNum = ((float) RandInRange(randInt)) / randInt;
                offset = randNum * scale * 2 - scale;
                DiamondStep(x, z, stepSize, offset);
            }
        }

        //   PrintGrid("diamond: ");

        for (int z = 0; z < max_size_; z += halfSize) {
            for (int x = 0; x < max_size_; x += halfSize) {
                randNum = ((float) RandInRange(randInt)) / randInt;
                offset = randNum * scale * 2 - scale;
                SquareStep(x, z, halfSize, offset);
            }
        }

        //   PrintGrid( "square: ");

        stepSize /= 2;
    }

    //This brings all the values down to where the camera initially begins
    float heightOffset = height_map_[0][0];
    for (int z = 0; z < max_size_; z++) {
        for (int x = 0; x < max_size_; x++) {
            height_map_[z][x] -= heightOffset - 1;
        }
    }
}

/**
 * This performs a "Square Step", getting values from 4 points surrounding the current point,
 * averaging them and setting the current point to them, before adding the offset
 * @param x
 * @param z
 * @param step
 * @param offset
 */
void DiamondSquare::SquareStep(int x, int z, int step, float offset) {
    float values[4] = {GetWrappedHeight(x, z),
                       GetWrappedHeight(x, z + step),
                       GetWrappedHeight(x + step, z),
                       GetWrappedHeight(x + step, z + step)};

    float average = AverageValues(values);
    this->height_map_.at(x).at(z) = average + offset;
}

/**
 *  This performs a "Diamond Step", getting values from 4 points different to the Square Step
 *  surrounding the current point, averaging them and setting the current point to them,
 *  before adding the offset
 * @param x
 * @param z
 * @param step
 * @param offset
 */
void DiamondSquare::DiamondStep(int x, int z, int step, float offset) {
    int halfStep = step / 2;

    float values[4] = {GetWrappedHeight(x - step, z),
                       GetWrappedHeight(x + step, z),
                       GetWrappedHeight(x, z + step),
                       GetWrappedHeight(x, z - step)};
    float average = AverageValues(values);
    this->height_map_.at(x + halfStep).at(z + halfStep) = average + offset;
}

/**
 * This function returns the heightmap in the form of vertices interpretable by OpenGL
 * @param gl_terrain_verts
 * @param scale
 */
void DiamondSquare::GenerateVertices(vector<vector<GLfloat>> &gl_terrain_verts, float scale) {

    //used to keep track of the index we are currently on
    int index = 0;

    //*2 for noOfTriangles, then *3 for noOfVerts, then 3 for number of points. Divide by number of arrays
    int count = 0;


    //iterates over the depth (z) and the width (x)
    //then iterates 3 times for each vertice in a triangle
    //and 2 times for 2 triangles per square
    for (int z = 0; z < this->max_size_ - 1; z++) {
        for (int x = 0; x < this->max_size_ - 1; x++) {
            for (int j = 0; j < 2; j++) {
                for (int i = 0; i < 3; i++) {
                    if (index >= this->no_of_vertices_) {
                        count++;
                        index = 0;
                    }

                    switch (i) {
                        case 0:
                            if (j == 0) {
                                set_vert_at_point(gl_terrain_verts[count], &index, x, z, scale);
                            } else {
                                set_vert_at_point(gl_terrain_verts[count], &index, x + 1, z, scale);
                            }
                            break;

                        case 1:
                            if (j == 0) {
                                set_vert_at_point(gl_terrain_verts[count], &index, x, z + 1, scale);
                            } else {
                                set_vert_at_point(gl_terrain_verts[count], &index, x, z + 1, scale);
                            }
                            break;

                        case 2:
                            if (j == 0) {
                                set_vert_at_point(gl_terrain_verts[count], &index, x + 1, z, scale);
                            } else {
                                set_vert_at_point(gl_terrain_verts[count], &index, x + 1, z + 1, scale);
                            }
                            break;
                    }
                }
            }
        }
    }
}

/* HELPER FUNCTIONS */

/**
 * Averages the four values given in the array values[4]
 * If a value is 0, then it won't be included in the average
 * @param values
 * @return
 */
float DiamondSquare::AverageValues(float *values) {
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

/* Helper Functions */

/**
 * Prints out the heightmap, beginning with the string "initial"
 * If show_zero is true, then zeros are also displayed
 * @param initial
 */
void DiamondSquare::PrintGrid(string initial, bool show_zero) {

    //sets the precision of the outputted values
    std::cout << std::fixed;
    std::cout << std::setprecision(0);

    cout << initial << endl;
    for (int j = 0; j < this->max_size_; j++) {
        for (int i = 0; i < this->max_size_; i++) {

            if (this->height_map_.at(i).at(j) == 0) {

                //if show_zero is true, display the values with zero. Else just display a space char
                !show_zero ? cout << " " << " " << "\t" : cout << " " << this->height_map_.at(i).at(j) << "\t";

            } else {
                cout << " " << this->height_map_.at(i).at(j) << "\t";

            }
        }
        cout << endl;
    }
    cout << "===============" << endl;
}

/**
 * Returns a random number in the range "range"
 * Is randomly a negative or positive number
 * @param range
 * @return
 */
int DiamondSquare::RandInRange(int range) {
    int randNum = rand() % range + 1;

    if (rand() % 3 == 1) {
        return randNum * -1;
    } else {
        return randNum;
    }
}

//http://www.cplusplus.com/forum/beginner/102670/ source
void DiamondSquare::ResizeVector(std::vector<std::vector<float> > &vec, const unsigned short ROWS,
                                 const unsigned short COLUMNS) {
    vec.resize(ROWS);
    for (auto &it : vec) {
        it.resize(COLUMNS);
    }
}

