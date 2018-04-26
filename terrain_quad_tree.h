//
// Created by pip on 23/04/18.
//

#ifndef INC_3DPGNS_TERRAIN_QUAD_TREE_H
#define INC_3DPGNS_TERRAIN_QUAD_TREE_H


#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <GL/glew.h>
#include <iostream>



struct Node {
    Node *top_left;
    Node *top_right;
    Node *bottom_left;
    Node *bottom_right;
    glm::vec2 boundary_bottom_left;
    glm::vec2 boundary_top_right;
    int level_of_detail;
    bool is_leaf = true;
};

class TerrainQuadTree {

public:

    TerrainQuadTree(int no_of_iterations);


    void GenerateQuadTree(glm::vec3 position, std::vector<std::vector<std::vector<GLfloat>>> &height_map,
                          std::vector<std::vector<std::vector<GLfloat>>> &gl_terrain_verts);

    void GenerateHeightMap(std::vector<GLfloat> &vertices,
                           std::vector<std::vector<std::vector<GLfloat>>> &height_map,
                           std::vector<std::vector<std::vector<GLfloat>>> &gl_terrain_verts);

private:

    Node root;
    int no_of_iterations;


    void AssignVertices(Node *node, std::vector<GLfloat> &vertices,
                        std::vector<std::vector<std::vector<GLfloat>>> &height_map,
                        std::vector<std::vector<std::vector<GLfloat>>> &gl_terrain_verts);

    void
    PushVertices(Node *node, std::vector<GLfloat> &vertices,
                 std::vector<std::vector<GLfloat>> &height_map,
                 std::vector<std::vector<GLfloat>> &gl_terrain_verts);

    bool IsInNode(Node *n, float step_size, glm::vec3 position);

    void SplitNode(Node *n, float step_size, int iteration);

};


#endif //INC_3DPGNS_TERRAIN_QUAD_TREE_H
