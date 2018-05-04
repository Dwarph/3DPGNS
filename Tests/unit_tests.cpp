//
// Created by pip on 19/04/18.
//

#define BOOST_TEST_MODULE MyTest

#include <boost/test/included/unit_test.hpp>
#include <glm/detail/func_trigonometric.inl>

#include "../diamond_square.h"
#include "../l_system.h"
#include "../terrain_quad_tree.h"


/* =============== Diamond Square Tests =============== */

/**
 * Checks to see if the map size was allocated correctly
 */
BOOST_AUTO_TEST_CASE(diamond_square_map_size) {

    int rough_max = 10;
    int terrain_scale = 9;

    DiamondSquare diamond_square = DiamondSquare(rough_max, terrain_scale);

    vector<vector<vector<float>>> height_map = diamond_square.get_height_map();
    vector<vector<vector<GLfloat>>> terrain_verts;

    //Check height map's size was allocated correctly
    BOOST_CHECK(height_map.size() == terrain_scale);
}

/**
 * Checks to see if the width and depth are allocated correctly
 */
BOOST_AUTO_TEST_CASE(diamond_square_width_and_depth) {
    int rough_max = 10;
    int terrain_scale = 9;

    DiamondSquare diamond_square = DiamondSquare(rough_max, terrain_scale);

    vector<vector<vector<float>>> height_map = diamond_square.get_height_map();
    vector<vector<vector<GLfloat>>> terrain_verts;


    //check the width and depth of the heightmap are the same
    BOOST_CHECK(height_map.at(0).at(0).size() == height_map.at(0).size());
}

/**
 * Checks to see if the vertice generation works
 */
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

/**
 * Checks to see if the size is correct
 */
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

/* =============== L-System Tests =============== */

/**
 * Checks to see if the fractal string is generated correctly
 */
BOOST_AUTO_TEST_CASE(l_system_fractal_string_generation) {
    LSystem tree = LSystem("B", 6, 0, 1);

    Rule rule_a, rule_b;
    rule_a.axiom = 'A';
    rule_a.rule = "AB";
    rule_b.axiom = 'B';
    rule_b.rule = "A";
    tree.AddRule(rule_a);
    tree.AddRule(rule_b);
    tree.GenerateFractal();
    cout << tree.get_fractal_string_() << endl;
    BOOST_CHECK("ABAABABAABAAB" == tree.get_fractal_string_());

}

/**
 * Checks to see if rules are added sucessfully
 */
BOOST_AUTO_TEST_CASE(l_system_rule_addition) {
    LSystem tree = LSystem("B", 6, 0, 1);
    Rule rule_a;
    rule_a.axiom = 'A';
    rule_a.rule = "AB";
    tree.AddRule(rule_a);
    vector<Rule> rules = tree.get_rules_();
    BOOST_CHECK(find(rules.begin(), rules.end(), rule_a) != rules.end());

}

/**
 * Checks to see if the constructor assigns the instance variables correctly
 */
BOOST_AUTO_TEST_CASE(l_system_constructor) {
    LSystem tree = LSystem("B", 6, 23.0, 1);
    Rule rule_a;
    float rads = glm::radians(23.0);

    BOOST_CHECK(tree.get_fractal_string_() == "B");
    BOOST_CHECK(tree.get_no_of_iterations_() == 6);
    BOOST_CHECK(tree.get_angle_mod_() == rads);
    BOOST_CHECK(tree.get_scale_() == 1);

}

/* =============== Quad-Tree Tests =============== */

/**
 * Checks to see if the constructor assigns the instance variables correctly
 */
BOOST_AUTO_TEST_CASE(quad_tree_constructor) {
    TerrainQuadTree terrain_quad_tree = TerrainQuadTree(4);

    BOOST_CHECK(terrain_quad_tree.get_no_of_iterations() == 4);
}

/**
 * Counts the amount of nodes recursively
 * @param node
 * @param no_of_nodes
 * @return
 */
int CountNodes(Node *node, int no_of_nodes) {
    if (node->is_leaf) {
        return no_of_nodes + 1;
    }

    no_of_nodes = CountNodes(node->top_right, no_of_nodes);
    no_of_nodes = CountNodes(node->top_left, no_of_nodes);
    no_of_nodes = CountNodes(node->bottom_right, no_of_nodes);
    no_of_nodes = CountNodes(node->bottom_left, no_of_nodes);

    return no_of_nodes + 1;
}

/**
 * Checks to see if the correct number of nodes were created, using CountNodes()
 */
BOOST_AUTO_TEST_CASE(quad_tree_iterations) {

    DiamondSquare diamondSquare = DiamondSquare(10, 9);

    auto terrain_quad_tree = TerrainQuadTree(4);

    glm::vec3 position = glm::vec3(0, 0, 0);

    vector<vector<vector<float>>> height_map = diamondSquare.get_height_map();

    Node *node = terrain_quad_tree.get_root();

    terrain_quad_tree.GenerateQuadTree(position, height_map);

    BOOST_CHECK(CountNodes(node, 0) == 17);
}

