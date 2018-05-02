//
// Created by pip on 22/02/18.
//
#include <cmath>
#include <algorithm>
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
 * @param int no_of_iterations
 */

DiamondSquare::DiamondSquare(int rough_max, int no_of_iterations) {

    this->no_of_iterations = no_of_iterations;

    this->max_height_ = 0;
    this->min_height_ = 0;

    // sets our width & depth size
    // has to be a multiple of 2
    // no_of_iterations directly corresponds with the size
    //so this works!
    this->max_size_ = (pow(2, no_of_iterations)) + 1;;

    //resizes our height_map_ vector appropriately
    height_map_.resize(1);
    ResizeVector2(height_map_.at(0), this->max_size_, this->max_size_);


    //provides us with the number of terrain vertex arrays

    if (no_of_iterations < 7) {
        this->no_of_terrain_vertex_arrays_ = 1;
    } else {
        this->no_of_terrain_vertex_arrays_ = pow(2, (no_of_iterations - 6));
    }

    //calculates the number of vertices of the terrain
    this->no_of_vertices_ =
            (((this->max_size_ - 1) * (this->max_size_ - 1) * 2) / this->no_of_terrain_vertex_arrays_) * 3 * 3;

    //sets the seed_ of the random number generator algorithm used by the function rand using the internal clock.
    srand(time(NULL));

    //calculates the roughness_ of the terrain randomly
//    this->roughness_ = (((float) (rand() % (rough_max) + 1)) * 0.01) + 0.05;

    this->roughness_ = 0.15;

    cout << "ROUGH: " << this->roughness_ << endl;

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
float DiamondSquare::get_height(int grid_num, int x, int z) {
    //gets the heights at the appropriate index
    return height_map_.at(grid_num).at(x).at(z);
}

/**
 * Gets a value at the point {x, z}.
 * If the values given are outside of the grid, this function wraps them to the other side of the grid.
 * E.G. if the grid is x: 0-4, z: 0-4 and this function is given {5,5}, it will return the value at {0,0}
 * @param x
 * @param z
 * @return
 */
float DiamondSquare::get_wrapped_height(int grid_num, int x, int z) {

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

    return this->height_map_.at(grid_num).at(x).at(z);
}

/**
 * Returns the number of vertices
 * @return
 */
int DiamondSquare::get_no_of_vertices() {
    return this->no_of_vertices_;
}

/**
 * Returns the number of iterations
 * @return
 */
int DiamondSquare::get_no_of_iterations() {
    return this->no_of_iterations;
}

/**
 * Gets the max height of the terrain
 * @return
 */
float DiamondSquare::get_max_height() const {
    return max_height_;
}

/**
 * Gets the min height of the terrain
 * @return
 */
float DiamondSquare::get_min_height() const {
    return min_height_;
}

/**
 * Returns the number of terrain_vertex_arrays
 * @return
 */

int DiamondSquare::get_no_of_terrain_vertex_arrays() const {
    return no_of_terrain_vertex_arrays_;
}

/**
 * Returns the height_map
 * @return
 */
vector<vector<vector<float>>> DiamondSquare::get_height_map() {
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
void DiamondSquare::set_vert_at_point(vector<GLfloat> &vertices, int *index, int grid_num, int x, int z, float scale,
                                      int height_map_size) {
    if (this->get_height(grid_num, x, z) != 0) {

        //scales height_map vertex points to the appropriate size (for LOD)
        float x_value = ((float) x / (float) height_map_size) * this->max_size_;
        float z_value = ((float) z / (float) height_map_size) * this->max_size_;

        vertices[*index] = x_value * scale;
        (*index)++;
        vertices[*index] = this->get_height(grid_num, x, z) * scale;
        (*index)++;
        vertices[*index] = z_value * scale;
        (*index)++;
    }
}

/* Main Functions */

/**
 * This function generates the height map using the Diamond Square algorithm
 */
void DiamondSquare::GenerateHeightMap() {

    float randNum;
    int stepSize = max_size_ - 1;
    int randInt = 100;
    int iterations = 0;

    //seeds the initial values of the heightmap to 0
    for (int i = 0; i < this->max_size_; i++) {
        for (int j = 0; j < this->max_size_; j++) {
            height_map_.at(0).at(i).at(j) = 0.0f;
        }
    }

    //sets the four corners of the heightmap to a random height between 0 & 1
    this->height_map_.at(0).at(0).at(0) = (this->max_size_ * RandInRange(10, true) % 10);
    this->height_map_.at(0).at(this->max_size_ - 1).at(0) = (this->max_size_ * RandInRange(10, true) % 10);
    this->height_map_.at(0).at(this->max_size_ - 1).at(this->max_size_ - 1) = (this->max_size_ * RandInRange(10, true) %
                                                                               10);
    this->height_map_.at(0).at(0).at(this->max_size_ - 1) = (this->max_size_ * RandInRange(10, true) % 10);


    while (stepSize > 1) {

        int halfSize = stepSize / 2;
        float scale = roughness_ * stepSize;
        float offset;

        for (int z = 0; z < max_size_ - 1; z += stepSize) {
            for (int x = 0; x < max_size_ - 1; x += stepSize) {

                randNum = ((float) RandInRange(randInt, true)) / randInt;
                offset = randNum * scale * 2 - scale;
                DiamondStep(iterations, x, z, stepSize, offset);
            }
        }

        for (int z = 0; z < max_size_; z += halfSize) {
            for (int x = 0; x < max_size_; x += halfSize) {
                randNum = ((float) RandInRange(randInt, true)) / randInt;
                offset = randNum * scale * 2 - scale;
                SquareStep(iterations, x, z, halfSize, offset);
            }
        }
        stepSize /= 2;
        iterations++;

        if (iterations != this->no_of_iterations) {
            this->height_map_.push_back(this->height_map_.at(iterations - 1));
        }

    }
    cout << "ITERATIONS:" << iterations << endl;


    float heightOffset = height_map_[height_map_.size() - 1][0][0];


    auto removeZeros = [&](float number) -> bool {
        return number == 0;
    };


    auto removeEmpty = [&](vector<float> vec) -> bool {
        return vec.size() == 0;
    };


    //This brings all the values down to where the camera initially begins
    //It also gets rid of zero values in early iterations
    for (int itr = 0; itr < height_map_.size(); itr++) {
        int z_size = height_map_.at(itr).size();
        for (int z = 0; z < z_size; z++) {

            auto iterator_x = remove_if(height_map_[itr][z].begin(), height_map_[itr][z].end(), removeZeros);
            height_map_[itr][z].erase(iterator_x, height_map_[itr][z].end());

            int x_size = height_map_[itr][z].size();
            for (int x = 0; x < x_size; x++) {
                height_map_[itr][z][x] -= heightOffset - 1;

                if (itr == height_map_.size() - 1) {
                    if (height_map_[itr][z][x] < this->min_height_) {
                        this->min_height_ = height_map_[itr][z][x];
                    } else if (height_map_[itr][z][x] > this->max_height_) {
                        this->max_height_ = height_map_[itr][z][x];
                    }
                }
            }
        }
        auto iterator_z = remove_if(height_map_[itr].begin(), height_map_[itr].end(), removeEmpty);
        height_map_[itr].erase(iterator_z, height_map_[itr].end());
    }
    rand();
}

//    this->PrintGrid(this->get_no_of_iterations() - 1, "GRID", false);


/**
 * This performs a "Square Step", getting values from 4 points surrounding the current point,
 * averaging them and setting the current point to them, before adding the offset
 * @param x
 * @param z
 * @param step
 * @param offset
 */
void DiamondSquare::SquareStep(int grid_num, int x, int z, int step, float offset) {
    float values[4] = {get_wrapped_height(grid_num, x, z),
                       get_wrapped_height(grid_num, x, z + step),
                       get_wrapped_height(grid_num, x + step, z),
                       get_wrapped_height(grid_num, x + step, z + step)};

    float average = AverageValues(values);
    this->height_map_.at(grid_num).at(x).at(z) = average + offset;
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
void DiamondSquare::DiamondStep(int grid_num, int x, int z, int step, float offset) {
    int halfStep = step / 2;

    float values[4] = {get_wrapped_height(grid_num, x - step, z),
                       get_wrapped_height(grid_num, x + step, z),
                       get_wrapped_height(grid_num, x, z + step),
                       get_wrapped_height(grid_num, x, z - step)};
    float average = AverageValues(values);
    this->height_map_.at(grid_num).at(x + halfStep).at(z + halfStep) = average + offset;
}

/**
 * This function returns the heightmap in the form of vertices interpretable by OpenGL
 * @param gl_terrain_verts
 * @param scale
 */
void DiamondSquare::GenerateVertices(vector<vector<vector<GLfloat>>> &gl_terrain_verts, float scale) {

    //used to keep track of the index we are currently on
    int index = 0;

    //*2 for noOfTriangles, then *3 for noOfVerts, then 3 for number of points. Divide by number of arrays
    int count = 0;

    //iterates over the depth (z) and the width (x)
    //then iterates 3 times for each vertice in a triangle
    //and 2 times for 2 triangles per square
    for (int itr = 0; itr < no_of_iterations; itr++) {
        count = 0;
        index = 0;

        int size = get_height_map().at(itr).size() - 1;

        for (int z = 0; z < size; z++) {
            for (int x = 0; x < size; x++) {
                for (int j = 0; j < 2; j++) {
                    for (int i = 0; i < 3; i++) {
                        if (index >= this->no_of_vertices_) {
                            count++;
                            index = 0;
                        }

                        switch (i) {
                            case 0:
                                if (j == 0) {
                                    set_vert_at_point(gl_terrain_verts[itr][count], &index, itr, x, z, scale, size);
                                } else {
                                    set_vert_at_point(gl_terrain_verts[itr][count], &index, itr, x + 1, z, scale, size);
                                }
                                break;

                            case 1:
                                if (j == 0) {
                                    set_vert_at_point(gl_terrain_verts[itr][count], &index, itr, x, z + 1, scale, size);
                                } else {
                                    set_vert_at_point(gl_terrain_verts[itr][count], &index, itr, x, z + 1, scale, size);
                                }
                                break;

                            case 2:
                                if (j == 0) {
                                    set_vert_at_point(gl_terrain_verts[itr][count], &index, itr, x + 1, z, scale, size);
                                } else {
                                    set_vert_at_point(gl_terrain_verts[itr][count], &index, itr, x + 1, z + 1, scale,
                                                      size);
                                }
                                break;
                        }
                    }
                }
            }
        }
    }
    //ensure the max and min heighpip_is_coolts adhere to the scale
    this->max_height_ *= scale;
    this->min_height_ *= scale;

    cout << "MAX: " << this->get_max_height() << endl;
    cout << "MIN: " << this->get_min_height() << endl;
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
void DiamondSquare::PrintGrid(int grid_num, string initial, bool show_zero) {

    //sets the precision of the outputted values
    std::cout << std::fixed;
    std::cout << std::setprecision(2);

    cout << initial << endl;
    for (int j = 0; j < this->max_size_; j++) {
        for (int i = 0; i < this->max_size_; i++) {

            if (this->height_map_.at(grid_num).at(i).at(j) == 0) {

                //if show_zero is true, display the values with zero. Else just display a space char
                !show_zero ? cout << " " << " " << "\t" : cout << " " << this->height_map_.at(grid_num).at(i).at(j)
                                                               << "\t";

            } else {
                cout << " " << this->height_map_.at(grid_num).at(i).at(j) << "\t";

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
int DiamondSquare::RandInRange(int range, bool negative) {
    int randNum = rand() % range + 1;

    if (negative && rand() % 3 == 1) {
        return randNum * -1;
    } else {
        return randNum;
    }
}

//http://www.cplusplus.com/forum/beginner/102670/ source
void DiamondSquare::ResizeVector2(std::vector<std::vector<float> > &vec, const unsigned short ROWS,
                                  const unsigned short COLUMNS) {
    vec.resize(ROWS);
    for (auto &it : vec) {
        it.resize(COLUMNS);
    }
}