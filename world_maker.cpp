//
// Created by pip on 23/03/18.
//

#include "world_maker.h"

/**
 * This class handles all the vertice generation for the world, passing it back to main
 */


/* Constructors */

/**
 * Constructor.
 * @param terrain_size
 * @param scale
 */

WorldMaker::WorldMaker(int terrain_size, float scale) : terrain_size_(terrain_size), terrain_scale_(scale) {
    this->diamond_square_ = new DiamondSquare(10, terrain_size);
    this->num_l_systems_ = {0, 0, 0};
    this->trees_;
    this->diamond_square_->ResizeVector2(this->tree_positions_, 3, 0);
}

/* Getters and Setters */

/**
 * Gets the terrain vertices
 * @return
 */
const vector<vector<vector<GLfloat>>> &WorldMaker::get_gl_terrain_verts() const {
    return gl_terrain_verts;
}

/**
 * Gets the terrain size
 * @return
 */
int WorldMaker::get_terrain_size() const {
    return terrain_size_;
}

/**
 * Gets the scale used for resizing the terrain and subsequently tree positioning
 * @return
 */
float WorldMaker::get_terrain_scale() const {
    return terrain_scale_;
}

/**
 * Returns a list of trees being used.
 * @return
 */
std::vector<LSystem> WorldMaker::get_tree() const {
    return trees_;
}

/**
 * Returns the tree positions vertices
 * @return
 */
const vector<vector<GLfloat>> &WorldMaker::get_tree_positions_() const {
    return tree_positions_;
}

/**
 * Returns the number of trees vector
 * @return
 */
vector<int> WorldMaker::get_num_trees_() const {
    return num_l_systems_;
}

/**
 * Returns the diamond_square_ terrain object
 * @return
 */
DiamondSquare *WorldMaker::get_diamond_square() const {
    return diamond_square_;
}

/**
 * Returns the number of l_systems
 * @return
 */
const int WorldMaker::get_num_l_systems() const {
    return this->num_l_systems_.size();
}

/* Main Functions */

/**
 * Generates the world in this order:
 * Terrain
 * Terrain Colour
 * Trees
 * Tree Positions
 *
 * It also sets the Gluints for each of the above buffers
 *
 * @param terrain_vertex_buffers
 * @param diamond_square_colour_buffers
 * @param tree_vertex_buffer
 * @param tree_position_vertex_buffer
 */
void WorldMaker::MakeWorld(vector<vector<GLuint >> &terrain_vertex_buffers,
                           GLuint *diamond_square_colour_buffers,
                           GLuint *tree_vertex_buffer,
                           GLuint *tree_position_vertex_buffer) {
    ComputeFullDiamondSquareBuffers(terrain_vertex_buffers, diamond_square_colour_buffers);
    ComputeLSystemVertexBuffer(tree_vertex_buffer);
    GenerateTreePositionBuffer(tree_position_vertex_buffer);
}

/**
 * Works out the vertices needed to display the L-Systems
 * @param vertex_buffer
 */
void WorldMaker::ComputeLSystemVertexBuffer(GLuint *vertex_buffers) {

    trees_.emplace_back(LSystem("X", 7, 20, 0.3));

    Rule rule_x, rule_f;
    rule_x.axiom = 'X';
    rule_x.rule = "F[+X]F[-X]+X";
    rule_f.axiom = 'F';
    rule_f.rule = "FF";
    trees_.at(0).AddRule(rule_x);
    trees_.at(0).AddRule(rule_f);

    trees_.emplace_back(LSystem("X", 7, 25.7, 0.3));
    rule_x.axiom = 'X';
    rule_x.rule = "F[+X][-X]FX";
    trees_.at(1).AddRule(rule_x);
    trees_.at(1).AddRule(rule_f);

    trees_.emplace_back(LSystem("X", 5, 22.7, 0.8));
    rule_x.axiom = 'X';
    rule_x.rule = "F-[[X]+]+F[+FX]-X";
    trees_.at(2).AddRule(rule_x);
    trees_.at(2).AddRule(rule_f);


    for (unsigned long i = 0; i < num_l_systems_.size(); ++i) {
        trees_.at(i).GenerateFractal();
        trees_.at(i).GenerateVertices();

        glGenBuffers(1, &vertex_buffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, trees_.at(i).get_vertices().size() * sizeof(float), &trees_[i].get_vertices()[0],
                     GL_STATIC_DRAW);
    }
}


/**
 * Generates positions for the trees
 *
 * Currently randomly places them
 *
 * @param tree_position_vertex_buffer
 */
void WorldMaker::GenerateTreePositionBuffer(GLuint *tree_position_vertex_buffer) {
    int tree_num = 0;
    auto num_l_systems_size = (int) this->num_l_systems_.size();

    /*
     * Loop through the entire grid
     * 1 in 200 chance of displaying a randomly selected L-System
     */
    for (int x = 0; x < diamond_square_->get_max_size(); x++) {
        for (int z = 0; z < diamond_square_->get_max_size(); z++) {
            if (diamond_square_->RandInRange(200, false) == 3) {

                tree_num = diamond_square_->RandInRange(num_l_systems_size, false) - 1;
                this->tree_positions_.at(tree_num).push_back(x * this->terrain_scale_);
                this->tree_positions_.at(tree_num).push_back(
                        diamond_square_->get_height(diamond_square_->get_no_of_iterations() - 1, x, z) *
                        this->terrain_scale_);
                this->tree_positions_.at(tree_num).push_back(z * this->terrain_scale_);
                this->num_l_systems_.at(tree_num)++;
            }
        }
    }

    for (int i = 0; i < num_l_systems_size; ++i) {

        glGenBuffers(1, &tree_position_vertex_buffer[i]);
        glBindBuffer(GL_ARRAY_BUFFER, tree_position_vertex_buffer[i]);
        glBufferData(GL_ARRAY_BUFFER, tree_positions_[i].size() * sizeof(float), &tree_positions_[i][0],
                     GL_STATIC_DRAW);
    }
}

/**
 * Works out the Quadtree vertices and binds them to the buffer
 * @param terrain_vertex_buffer
 * @param diamond_square_colour_buffers
 * @param position
 */
void WorldMaker::ComputeDiamondSquareLOD(GLuint &terrain_vertex_buffer, GLuint *diamond_square_colour_buffers,
                                         glm::vec3 position) {

    std::vector<GLfloat> gl_terrain_verts_lod;
    auto terrainQuadTree = TerrainQuadTree(4);
    std::vector<std::vector<std::vector<float>>> height_map = diamond_square_->get_height_map();

    terrainQuadTree.GenerateQuadTree(position, height_map);
    terrainQuadTree.GenerateHeightMap(gl_terrain_verts_lod, height_map, gl_terrain_verts);

    glGenBuffers(1, &terrain_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, (terrain_vertex_buffer));
    glBufferData(GL_ARRAY_BUFFER, gl_terrain_verts_lod.size() * sizeof(float), &gl_terrain_verts_lod[0],
                 GL_STATIC_DRAW);

}


/**
 * Works out the Diamond Square Vertex buffers
 * @param terrain_vertex_buffers
 * @param diamond_square_colour_buffers
 */
void
WorldMaker::ComputeFullDiamondSquareBuffers(vector<vector<GLuint >> &terrain_vertex_buffers,
                                            GLuint *diamond_square_colour_buffers) {

    gl_terrain_verts.resize(diamond_square_->get_no_of_iterations());
    for (int i = 0; i < diamond_square_->get_no_of_iterations(); ++i) {
        gl_terrain_verts.at(i).resize(diamond_square_->get_no_of_terrain_vertex_arrays(),
                                      vector<GLfloat>(diamond_square_->get_no_of_vertices(), 0));
    }

    diamond_square_->GenerateVertices(gl_terrain_verts, this->terrain_scale_);

    for (int i = 0; i < diamond_square_->get_no_of_iterations(); ++i) {
        for (int j = 0; j < diamond_square_->get_no_of_terrain_vertex_arrays(); j++) {
            glGenBuffers(1, &terrain_vertex_buffers[i][j]);
            glBindBuffer(GL_ARRAY_BUFFER, (terrain_vertex_buffers[i][j]));
            glBufferData(GL_ARRAY_BUFFER, gl_terrain_verts[i][j].size() * sizeof(float), &gl_terrain_verts[i][j][0],
                         GL_STATIC_DRAW);
        }
    }

    ComputeDiamondSquareColourBuffers(gl_terrain_verts.at(diamond_square_->get_no_of_iterations() - 1),
                                      diamond_square_colour_buffers, 3);

}

/**
 * Works out the Diamond Square Colour Buffers
 * @param gl_terrain_verts
 * @param diamond_square_colour_buffers
 */
void
WorldMaker::ComputeDiamondSquareColourBuffers(vector<vector<GLfloat>> &gl_terrain_verts,
                                              GLuint *diamond_square_colour_buffers,
                                              int colour_palette) {
    ColourList colours;
    vector<vector<GLfloat>> g_color_buffer_data;
    g_color_buffer_data.resize(diamond_square_->get_no_of_terrain_vertex_arrays(),
                               vector<GLfloat>(diamond_square_->get_no_of_vertices(), 0));

    switch (colour_palette) {
        case 0:
            VertexColourRainbow(g_color_buffer_data);
            break;

        case 1:
            VertexColourGreyscale(g_color_buffer_data, gl_terrain_verts);
            break;

        case 2:
            VertexColourReal(g_color_buffer_data, gl_terrain_verts, colours);
            break;

        case 3:
        default:
            VertexColourRealBlended(g_color_buffer_data, gl_terrain_verts, colours);
            break;
    }


    for (int i = 0; i < diamond_square_->get_no_of_terrain_vertex_arrays(); i++) {
        glGenBuffers(1, &diamond_square_colour_buffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, diamond_square_colour_buffers[i]);
        glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data[i].size() * sizeof(float), &g_color_buffer_data[i][0],
                     GL_STATIC_DRAW);
    }
}

/**
 * Assigns a random colour to each vertex of the terrain
 * @param g_color_buffer_data
 */
void WorldMaker::VertexColourRainbow(vector<vector<GLfloat>> &g_color_buffer_data) {
    for (int i = 0; i < diamond_square_->get_no_of_terrain_vertex_arrays(); i++) {
        for (int j = 0; j < diamond_square_->get_no_of_vertices(); j++) {
            GLfloat col = rand() % (101);
            col /= 100;
            g_color_buffer_data[i][j] = col;
        }
    }
}

/**
 * Assigns a shade of grey based on the height of the vertex
 * @param g_color_buffer_data
 * @param gl_terrain_verts
 * @param height_range
 */
void WorldMaker::VertexColourGreyscale(vector<vector<GLfloat>> &g_color_buffer_data,
                                       vector<vector<GLfloat>> &gl_terrain_verts) {
    float height_range = diamond_square_->get_max_height() - diamond_square_->get_min_height();

    for (int i = 0; i < diamond_square_->get_no_of_terrain_vertex_arrays(); i++) {
        for (int j = 1; j < diamond_square_->get_no_of_vertices(); j += 3) {
            float colour = (gl_terrain_verts[i][j] - diamond_square_->get_min_height()) / height_range;
            g_color_buffer_data[i][j - 1] = colour;
            g_color_buffer_data[i][j] = colour;
            g_color_buffer_data[i][j + 1] = colour;
        }
    }
}

/**
 * Assigns a more "realistic" colour to the terrain
 * @param g_color_buffer_data
 * @param gl_terrain_verts
 * @param colours
 */
void
WorldMaker::VertexColourReal(vector<vector<GLfloat>> &g_color_buffer_data, vector<vector<GLfloat>> &gl_terrain_verts,
                             ColourList colours) {
    float height = 0;
    float rand_num = 0;
    for (int i = 0; i < diamond_square_->get_no_of_terrain_vertex_arrays(); i++) {
        for (int j = 1; j < diamond_square_->get_no_of_vertices(); j += 3) {
            int min = 97, max = 103;

            height = (gl_terrain_verts[i][j] - diamond_square_->get_min_height()) /
                     (diamond_square_->get_max_height() - diamond_square_->get_min_height());
            rand_num = (float) (rand() % (max + 1 - min) + min);
            rand_num /= 100;

            if (height < 0.225) {
                g_color_buffer_data[i][j - 1] = colours.wet_brown[0] * rand_num;
                g_color_buffer_data[i][j] = colours.wet_brown[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.wet_brown[2] * rand_num;

            } else if (height < 0.35) {
                g_color_buffer_data[i][j - 1] = colours.dusty_brown[0] * rand_num;
                g_color_buffer_data[i][j] = colours.dusty_brown[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.dusty_brown[2] * rand_num;

            } else if (height < 0.4) {
                g_color_buffer_data[i][j - 1] = colours.ivy_green[0] * rand_num;
                g_color_buffer_data[i][j] = colours.ivy_green[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.ivy_green[2] * rand_num;

            } else if (height < 0.7) {
                g_color_buffer_data[i][j - 1] = colours.moss_green[0] * rand_num;
                g_color_buffer_data[i][j] = colours.moss_green[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.moss_green[2] * rand_num;

            } else if (height < 0.8) {
                g_color_buffer_data[i][j - 1] = colours.med_grey[0] * rand_num;
                g_color_buffer_data[i][j] = colours.med_grey[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.med_grey[2] * rand_num;

            } else if (height < 0.95) {
                g_color_buffer_data[i][j - 1] = colours.light_grey[0] * rand_num;
                g_color_buffer_data[i][j] = colours.light_grey[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.light_grey[2] * rand_num;

            } else {
                g_color_buffer_data[i][j - 1] = colours.snow_white[0] * rand_num;
                g_color_buffer_data[i][j] = colours.snow_white[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.snow_white[2] * rand_num;
            }
        }
    }
}

/**
 * Assigns a more realistic colour, but also makes some of the colours a gradient
 * @param g_color_buffer_data
 * @param gl_terrain_verts
 * @param colours
 */
void WorldMaker::VertexColourRealBlended(vector<vector<GLfloat>> &g_color_buffer_data,
                                         vector<vector<GLfloat>> &gl_terrain_verts, ColourList colours) {
    GLfloat blended_colour[3];
    float height, rand_num;

    for (int i = 0; i < diamond_square_->get_no_of_terrain_vertex_arrays(); i++) {
        for (int j = 1; j < diamond_square_->get_no_of_vertices(); j += 3) {
            int min = 97, max = 103;

            height = (gl_terrain_verts[i][j] - diamond_square_->get_min_height()) /
                     ((diamond_square_->get_max_height()) - (diamond_square_->get_min_height()));
            rand_num = (float) (rand() % (max + 1 - min) + min);
            rand_num /= 100;


            if (height < 0.25) {
                g_color_buffer_data[i][j - 1] = colours.wet_brown[0] * rand_num;
                g_color_buffer_data[i][j] = colours.wet_brown[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.wet_brown[2] * rand_num;


            } else if (height < 0.35) {

                this->BlendColours(colours.dusty_brown, colours.wet_brown, blended_colour, height / 0.35);

                g_color_buffer_data[i][j - 1] = blended_colour[0] * rand_num;
                g_color_buffer_data[i][j] = blended_colour[1] * rand_num;
                g_color_buffer_data[i][j + 1] = blended_colour[2] * rand_num;

            } else if (height < 0.7) {

                this->BlendColours(colours.moss_green, colours.ivy_green, blended_colour, height / 0.7);

                g_color_buffer_data[i][j - 1] = blended_colour[0] * rand_num;
                g_color_buffer_data[i][j] = blended_colour[1] * rand_num;
                g_color_buffer_data[i][j + 1] = blended_colour[2] * rand_num;

            } else if (height < 0.7) {

                g_color_buffer_data[i][j - 1] = colours.moss_green[0] * rand_num;
                g_color_buffer_data[i][j] = colours.moss_green[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.moss_green[2] * rand_num;

            } else if (height < 0.8) {

                g_color_buffer_data[i][j - 1] = colours.med_grey[0] * rand_num;
                g_color_buffer_data[i][j] = colours.med_grey[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.med_grey[2] * rand_num;

            } else if (height < 0.95) {

                g_color_buffer_data[i][j - 1] = colours.light_grey[0] * rand_num;
                g_color_buffer_data[i][j] = colours.light_grey[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.light_grey[2] * rand_num;

            } else {

                g_color_buffer_data[i][j - 1] = colours.snow_white[0] * rand_num;
                g_color_buffer_data[i][j] = colours.snow_white[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.snow_white[2] * rand_num;
            }

        }
    }
}

/**
 * Blends colour one with colour two by a given percentage
 * and assigns the result to blended colour
 * @param colour_one
 * @param colour_two
 * @param blended_colour
 * @param percentage
 * @return
 */
GLfloat *WorldMaker::BlendColours(GLfloat colour_one[3], GLfloat colour_two[3], GLfloat blended_colour[3],
                                  float percentage) {
    GLfloat range;
    for (int i = 0; i < 3; i++) {
        range = colour_one[i] - colour_two[i];
        blended_colour[i] = colour_two[i] + (range * percentage);
    }
    return blended_colour;
}