//
// Created by pip on 19/04/18.
//

#define BOOST_TEST_MODULE MyTest

#include <boost/test/included/unit_test.hpp>

#include "../diamond_square.h"


BOOST_AUTO_TEST_CASE(diamond_square_map_size) {

    int rough_max = 10;
    int terrain_scale = 9;

    DiamondSquare diamond_square = DiamondSquare(rough_max, terrain_scale);

    vector<vector<vector<float>>> height_map = diamond_square.get_height_map();
    vector<vector<vector<GLfloat>>> terrain_verts;

    //Check height map's size was allocated correctly
    BOOST_CHECK(height_map.size() == terrain_scale);
}

BOOST_AUTO_TEST_CASE(diamond_square_width_and_depth) {
    int rough_max = 10;
    int terrain_scale = 9;

    DiamondSquare diamond_square = DiamondSquare(rough_max, terrain_scale);

    vector<vector<vector<float>>> height_map = diamond_square.get_height_map();
    vector<vector<vector<GLfloat>>> terrain_verts;


    //check the width and depth of the heightmap are the same
    BOOST_CHECK(height_map.at(0).at(0).size() == height_map.at(0).size());
}

BOOST_AUTO_TEST_CASE(diamond_square_vertice_generation) {

    int rough_max = 10;
    int terrain_scale = 9;

    DiamondSquare diamond_square = DiamondSquare(rough_max, terrain_scale);

    vector<vector<vector<float>>> height_map = diamond_square.get_height_map();
    vector<vector<vector<GLfloat>>> terrain_verts;

    terrain_verts.resize(diamond_square.get_no_of_iterations());
    for (int i = 0; i < diamond_square.get_no_of_iterations(); ++i) {
        terrain_verts.at(i).resize(diamond_square.get_no_of_terrain_vertex_arrays(),
                                   vector<GLfloat>(diamond_square.get_no_of_vertices(), 0));
    }

    diamond_square.GenerateVertices(terrain_verts, 1);

    for (int i = 0; i < terrain_verts.size(); i++) {

        //Check vertices are being copied over correctly
        BOOST_CHECK(height_map.at(i).at(0).at(0) ==
                    terrain_verts.at(i).at(0).at(1));

    }
}

BOOST_AUTO_TEST_CASE(diamond_square_size_check) {

    int rough_max = 10;
    int terrain_scale = 9;

    DiamondSquare diamond_square = DiamondSquare(rough_max, terrain_scale);

    vector<vector<vector<float>>> height_map = diamond_square.get_height_map();

    for (int i = 1; i < height_map.size(); i++) {
        //check size increases correctly each time
        BOOST_CHECK(height_map.at(i).at(0).size() == (height_map.at(i - 1).at(0).size() * 2) - 1);
    }
}


BOOST_AUTO_TEST_CASE(l_system_initial_checks) {

}