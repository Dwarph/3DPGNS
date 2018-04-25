//
// Created by pip on 23/04/18.
//

#include <iostream>
#include "terrain_quad_tree.h"

TerrainQuadTree::TerrainQuadTree(int no_of_iterations) : no_of_iterations(no_of_iterations) {}


void TerrainQuadTree::GenerateQuadTree(glm::vec3 camera_position,
                                       std::vector<std::vector<std::vector<GLfloat>>> &height_map,
                                       std::vector<std::vector<std::vector<GLfloat>>> &gl_terrain_verts) {

    int final_size = height_map.back().size();
    float max_length = height_map.back().back().back();

    camera_position.x = camera_position.x / max_length;
    camera_position.y = camera_position.y / max_length;
    camera_position.z = camera_position.z / max_length;


    root.boundary_bottom_left = {0, 0};
    root.boundary_top_right = {1, 1};

    Node *current_node = &root;
    float step_size = 0.5;

    for (int itr = 0; itr < this->no_of_iterations; itr++) {

        SplitNode(current_node, step_size, itr);

        if (IsInNode(current_node->top_left, step_size, camera_position)) {
            current_node = current_node->top_left;

        } else if (IsInNode(current_node->top_right, step_size, camera_position)) {
            current_node = current_node->top_right;

        } else if (IsInNode(current_node->bottom_left, step_size, camera_position)) {
            current_node = current_node->bottom_left;

        } else if (IsInNode(current_node->bottom_right, step_size, camera_position)) {
            current_node = current_node->bottom_right;

        }
        step_size /= 2;
    }
}

void TerrainQuadTree::GenerateHeightMap(std::vector<GLfloat> &vertices,
                                        std::vector<std::vector<std::vector<GLfloat>>> &height_map,
                                        std::vector<std::vector<std::vector<GLfloat>>> &gl_terrain_verts) {


    Node *current_node = &root;

    AssignVertices(current_node, vertices, height_map, gl_terrain_verts);

}

void TerrainQuadTree::AssignVertices(Node *node, std::vector<GLfloat> &vertices,
                                     std::vector<std::vector<std::vector<GLfloat>>> &height_map,
                                     std::vector<std::vector<std::vector<GLfloat>>> &gl_terrain_verts) {
    float max_length = height_map.back().back().back();

    if (node->is_leaf) {
        switch (node->level_of_detail) {
            case 0:
                PushVertices(node, vertices, height_map.at(3), gl_terrain_verts.at(3));
                break;
            case 1:
                PushVertices(node, vertices, height_map.at(height_map.size() - 3),
                             gl_terrain_verts.at(height_map.size() - 3));
                break;
            case 2:
                PushVertices(node, vertices, height_map.at(height_map.size() - 2),
                             gl_terrain_verts.at(height_map.size() - 2));
                break;
            case 3:
                PushVertices(node, vertices, height_map.back(), gl_terrain_verts.at(height_map.size() - 1));
                break;
        }
        return;
    } else {
        AssignVertices(node->bottom_left, vertices, height_map, gl_terrain_verts);
        AssignVertices(node->bottom_right, vertices, height_map, gl_terrain_verts);
        AssignVertices(node->top_left, vertices, height_map, gl_terrain_verts);
        AssignVertices(node->top_right, vertices, height_map, gl_terrain_verts);
    }
}

void TerrainQuadTree::PushVertices(Node *node, std::vector<GLfloat> &vertices,
                                   std::vector<std::vector<GLfloat>> &height_map,
                                   std::vector<std::vector<GLfloat>> &gl_terrain_verts) {
    int lod_size = (height_map.size() - 1) / 3;
    int row_size = lod_size * 3;

    for (int z = node->boundary_bottom_left.y * lod_size; z < node->boundary_top_right.y * lod_size; z++) {
        for (int x = node->boundary_bottom_left.x * lod_size; x < node->boundary_top_right.x * lod_size; x++) {
            vertices.push_back(gl_terrain_verts.data()->at((z * row_size + (x + 1) * 3)));
        }
    }
}

bool TerrainQuadTree::IsInNode(Node *node, float step_size, glm::vec3 camera_position) {

    return (camera_position.x < node->boundary_top_right.x
            && camera_position.x >= node->boundary_top_right.x - step_size
            && camera_position.z < node->boundary_top_right.y
            && camera_position.z >= node->boundary_top_right.y - step_size);
}

void TerrainQuadTree::SplitNode(Node *node, float step_size, int iteration) {

    node->is_leaf = false;

    /*Define bottom left node boundaries*/
    node->bottom_left = new Node;
    node->bottom_left->level_of_detail = iteration;
    node->bottom_left->boundary_bottom_left = node->boundary_bottom_left;
    node->bottom_left->boundary_top_right = glm::vec2(node->boundary_bottom_left.x + step_size,
                                                      node->boundary_bottom_left.y + step_size);

    /*Define bottom right node boundaries*/


    node->bottom_right = new Node;
    node->bottom_right->level_of_detail = iteration;
    node->bottom_right->boundary_bottom_left = glm::vec2(node->boundary_bottom_left.x + step_size,
                                                         node->boundary_bottom_left.y);
    node->bottom_right->boundary_top_right = glm::vec2(node->boundary_top_right.x,
                                                       node->boundary_top_right.y - step_size);

    /*Define top left node boundaries*/

    node->top_left = new Node;
    node->top_left->level_of_detail = iteration;
    node->top_left->boundary_bottom_left = glm::vec2(node->boundary_bottom_left.x,
                                                     node->boundary_bottom_left.y + step_size);
    node->top_left->boundary_top_right = glm::vec2(node->boundary_top_right.x - step_size,
                                                   node->boundary_top_right.y);

    /*Define top right node boundaries*/

    node->top_right = new Node;
    node->top_right->level_of_detail = iteration;
    node->top_right->boundary_bottom_left = glm::vec2(node->boundary_top_right.x - step_size,
                                                      node->boundary_top_right.y - step_size);
    node->top_right->boundary_top_right = node->boundary_top_right;
}

