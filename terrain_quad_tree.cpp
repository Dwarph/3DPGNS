//
// Created by pip on 23/04/18.
//

#include "terrain_quad_tree.h"

/**
 * This class handles all code relating to Quad Trees
 * This class generates a quadtree of levels of details to represent the terrain by
 * And attempts to generate the appropriate vertices to display the terrain
 */

/* Constructors */

/**
 * Initialises number of iterations of the quadtree
 * @param no_of_iterations
 */
TerrainQuadTree::TerrainQuadTree(int no_of_iterations) : no_of_iterations(no_of_iterations) {}

/* Getters and Setters */

Node *TerrainQuadTree::get_root() {
    return &root;
}

int TerrainQuadTree::get_no_of_iterations() const {
    return no_of_iterations;
}

/* Main Functions */

/**
 * Generates a quad tree by iterating the number specified by no_of_iterations
 * Each iteration, identifying which node needs to be split and then splitting that node
 * into four seperate leaves.
 *
 * @param camera_position
 * @param height_map
 */
void TerrainQuadTree::GenerateQuadTree(glm::vec3 camera_position,
                                       std::vector<std::vector<std::vector<GLfloat>>> &height_map) {

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

/**
 * Returns true if camera_position is in the node given to it
 * @param node
 * @param step_size
 * @param camera_position
 * @return
 */
bool TerrainQuadTree::IsInNode(Node *node, float step_size, glm::vec3 camera_position) {

    return (camera_position.x < node->boundary_top_right.x
            && camera_position.x >= node->boundary_top_right.x - step_size
            && camera_position.z < node->boundary_top_right.y
            && camera_position.z >= node->boundary_top_right.y - step_size);
}

/**
 * Adds four leaves to a node
 * @param node
 * @param step_size
 * @param iteration
 */
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

/**
 * Generates a heightmap based on the quadtree generated
 * @param vertices
 * @param height_map
 * @param gl_terrain_verts
 */
void TerrainQuadTree::GenerateHeightMap(std::vector<GLfloat> &vertices,
                                        std::vector<std::vector<std::vector<GLfloat>>> &height_map,
                                        std::vector<std::vector<std::vector<GLfloat>>> &gl_terrain_verts) {

    Node *current_node = &root;
    AssignVertices(current_node, vertices, height_map, gl_terrain_verts);
}

/**
 * Gets the vertices needed for the heightmap
 * @param node
 * @param vertices
 * @param height_map
 * @param gl_terrain_verts
 */
void TerrainQuadTree::AssignVertices(Node *node, std::vector<GLfloat> &vertices,
                                     std::vector<std::vector<std::vector<GLfloat>>> &height_map,
                                     std::vector<std::vector<std::vector<GLfloat>>> &gl_terrain_verts) {

    //If the node is a leaf, get the vertices dependingon the level of detail.
    if (node->is_leaf) {
        switch (node->level_of_detail) {
            case 0:
                PushVertices(node, vertices, height_map.at(3), gl_terrain_verts.at(height_map.size() - 1));
                return;
            case 1:
                PushVertices(node, vertices, height_map.at(height_map.size() - 1),
                             gl_terrain_verts.at(height_map.size() - 3));
                return;
            case 2:
                PushVertices(node, vertices, height_map.at(height_map.size() - 1),
                             gl_terrain_verts.at(height_map.size() - 2));
                return;
            case 3:
                PushVertices(node, vertices, height_map.back(), gl_terrain_verts.at(height_map.size() - 1));
                return;
        }

        //otherwise, recursively check for more leaves
    } else {
        AssignVertices(node->bottom_left, vertices, height_map, gl_terrain_verts);
        AssignVertices(node->bottom_right, vertices, height_map, gl_terrain_verts);
        AssignVertices(node->top_left, vertices, height_map, gl_terrain_verts);
        AssignVertices(node->top_right, vertices, height_map, gl_terrain_verts);
    }
}

/**
 * Sets a vertices at a point given to it.
 * @param vertices
 * @param height_map
 * @param x
 * @param z
 * @param scale
 * @param height_map_size
 * @param max_size
 */
void set_vert_at_point(std::vector<GLfloat> &vertices, std::vector<std::vector<GLfloat>> &height_map,
                       int x, int z, float scale, int height_map_size, int max_size) {

    //scales height_map vertex points to the appropriate size (for LOD)
    float x_value = ((float) x / (float) height_map_size) * max_size;
    float z_value = ((float) z / (float) height_map_size) * max_size;

    vertices.push_back(x_value * scale);
    vertices.push_back(height_map.at(x).at(z) * scale);
    vertices.push_back(z_value * scale);
}

/**
 * Pushes vertices onto the vertex buffer
 * @param node
 * @param vertices
 * @param height_map
 * @param gl_terrain_verts
 */
void TerrainQuadTree::PushVertices(Node *node, std::vector<GLfloat> &vertices,
                                   std::vector<std::vector<GLfloat>> &height_map,
                                   std::vector<std::vector<GLfloat>> &gl_terrain_verts) {
    int lod_size = (int) height_map.size();
    float scale = 50;
    for (int z = (int) node->boundary_bottom_left.y * lod_size; z < (node->boundary_top_right.y * lod_size) - 1; z++) {
        for (int x = (int) node->boundary_bottom_left.x * lod_size;
             x < (node->boundary_top_right.x * lod_size) - 1; x++) {
            for (int j = 0; j < 2; j++) {
                for (int i = 0; i < 3; i++) {
                    switch (i) {
                        case 0:
                            if (j == 0) {
                                set_vert_at_point(vertices, height_map, x, z, scale, (int) height_map.size(),
                                                  (int) height_map.at(height_map.size() - 1).size());
                            } else {
                                set_vert_at_point(vertices, height_map, x + 1, z, scale, (int) height_map.size(),
                                                  (int) height_map.at(height_map.size() - 1).size());
                            }
                            break;

                        case 1:
                            if (j == 0) {
                                set_vert_at_point(vertices, height_map, x, z + 1, scale, (int) height_map.size(),
                                                  (int) height_map.at(height_map.size() - 1).size());

                            } else {
                                set_vert_at_point(vertices, height_map, x, z + 1, scale, (int) height_map.size(),
                                                  (int) height_map.at(height_map.size() - 1).size());
                            }
                            break;

                        case 2:
                            if (j == 0) {
                                set_vert_at_point(vertices, height_map, x + 1, z, scale, (int) height_map.size(),
                                                  (int) height_map.at(height_map.size() - 1).size());
                            } else {
                                set_vert_at_point(vertices, height_map, x + 1, z + 1, scale, (int) height_map.size(),
                                                  (int) height_map.at(height_map.size() - 1).size());
                            }
                            break;
                    }
                }
            }
        }
    }
}