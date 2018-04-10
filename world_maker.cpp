//
// Created by pip on 23/03/18.
//

#include "world_maker.h"


WorldMaker::WorldMaker(int terrain_size, float scale) : terrain_size_(terrain_size), scale_(scale) {
    int max = (pow(2, terrain_size)) + 1;
    this->diamond_square_ = new DiamondSquare(max, 10, get_no_of_terrain_vertex_arrays());
}

int WorldMaker::get_terrain_size() const {
    return terrain_size_;
}

float WorldMaker::get_scale() const {
    return scale_;
}

int WorldMaker::get_no_of_terrain_vertex_arrays() {
    if (terrain_size_ < 7) {
        return 1;
    } else {
        return pow(2, (terrain_size_ - 6));
    }
}

DiamondSquare *WorldMaker::get_diamond_square() const {
    return diamond_square_;
}

void WorldMaker::MakeWorld(GLuint *terrainVertexBuffers, GLuint *diamondSquareColourBuffer, GLuint *treeVertexBuffer) {
    ComputeDiamondSquareBuffers(terrainVertexBuffers, diamondSquareColourBuffer);
    ComputeLSystemVertexBuffer(treeVertexBuffer);

}

void WorldMaker::ComputeLSystemVertexBuffer(GLuint *vertexBuffer) {

    tree_ = new LSystem("X", 7, 20, 0.3);

    Rule rule;
    rule.axiom = 'X';
    rule.rule = "F[+X]F[-X]+X";

    tree_->AddRule(rule);

    rule.axiom = 'F';
    rule.rule = "FF";

    tree_->AddRule(rule);

    tree_->GenerateFractal();
    tree_->GenerateVertices();

    for (int i = 0; i < tree_->get_vertices().size(); i += 3) {
        cout << "{x: " << tree_->get_vertices()[i] <<
             ", y: " << tree_->get_vertices()[i + 1] <<
             ", z: " << tree_->get_vertices()[i + 2] << "}, ";
    }
    cout << endl;


    glGenBuffers(1, vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, *vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, tree_->get_vertices().size() * sizeof(float), &tree_->get_vertices()[0],
                 GL_STATIC_DRAW);
}

void WorldMaker::GenerateTreePositionBuffer(int no_of_trees) {

    for (int x = 0; x < diamond_square_->get_max_size(); x++) {
        for (int z = 0; z < diamond_square_->get_max_size(); z++) {
            if (diamond_square_->RandInRange(40) % 4 == 0) {
                this->tree_positions_.push_back(x * this->scale_);
                this->tree_positions_.push_back(diamond_square_->GetHeight(x, z) * this->scale_);
                this->tree_positions_.push_back(z * this->scale_);
            }
        }
    }

}

void
WorldMaker::ComputeDiamondSquareBuffers(GLuint *vertexBuffers, GLuint *diamondSquareColourBuffer) {

    vector<vector<GLfloat>> gl_terrain_verts;
    gl_terrain_verts.resize(get_no_of_terrain_vertex_arrays(),
                            vector<GLfloat>(diamond_square_->get_no_of_vertices(), 0));

    diamond_square_->GenerateVertices(gl_terrain_verts, this->scale_);

    for (int i = 0; i < get_no_of_terrain_vertex_arrays(); i++) {
        glGenBuffers(1, &vertexBuffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, gl_terrain_verts[i].size() * sizeof(float), &gl_terrain_verts[i][0],
                     GL_STATIC_DRAW);
    }


    ComputeDiamondSquareColourBuffers(gl_terrain_verts, diamondSquareColourBuffer);

}

void
WorldMaker::ComputeDiamondSquareColourBuffers(vector<vector<GLfloat>> gl_terrain_verts,
                                              GLuint *diamondSquareColourBuffers) {
    ColourList colours;
    vector<vector<GLfloat>> g_color_buffer_data;
    g_color_buffer_data.resize(get_no_of_terrain_vertex_arrays(),
                               vector<GLfloat>(diamond_square_->get_no_of_vertices(), 0));
    float min_height = gl_terrain_verts[0][1], max_height = gl_terrain_verts[0][1];

    for (int i = 0; i < get_no_of_terrain_vertex_arrays(); i++) {
        for (int j = 1; j < diamond_square_->get_no_of_vertices(); j += 3) {
            if (min_height > gl_terrain_verts[i][j]) {
                min_height = gl_terrain_verts[i][j];
            }

            if (max_height < gl_terrain_verts[i][j]) {
                max_height = gl_terrain_verts[i][j];
            }
        }
    }


    cout << "MAX: " << max_height << endl;
    cout << "MIN: " << min_height << endl;

    float height_range = max_height - min_height;
    float height, rand_num;

//    VertexColourRainbow(g_color_buffer_data);
//    VertexColourGreyscale(g_color_buffer_data, gl_terrain_verts, min_height, height_range);
//    VertexColourReal(g_color_buffer_data, gl_terrain_verts, colours, min_height, max_height);
    VertexColourRealBlended(g_color_buffer_data, gl_terrain_verts, colours, min_height, max_height);


    for (int i = 0; i < get_no_of_terrain_vertex_arrays(); i++) {
        glGenBuffers(1, &diamondSquareColourBuffers[i]);
        glBindBuffer(GL_ARRAY_BUFFER, diamondSquareColourBuffers[i]);
        glBufferData(GL_ARRAY_BUFFER, g_color_buffer_data[i].size() * sizeof(float), &g_color_buffer_data[i][0],
                     GL_STATIC_DRAW);
    }
}

void WorldMaker::VertexColourRainbow(vector<vector<GLfloat>> &g_color_buffer_data) {
    for (int i = 0; i < get_no_of_terrain_vertex_arrays(); i++) {
        for (int j = 0; j < diamond_square_->get_no_of_vertices(); j++) {
            GLfloat col = rand() % (101);
            col /= 100;
            g_color_buffer_data[i][j] = col;
        }
    }
}

void WorldMaker::VertexColourGreyscale(vector<vector<GLfloat>> &g_color_buffer_data,
                                       vector<vector<GLfloat>> gl_terrain_verts, float min_height,
                                       float height_range) {

    for (int i = 0; i < get_no_of_terrain_vertex_arrays(); i++) {
        for (int j = 1; j < diamond_square_->get_no_of_vertices(); j += 3) {
            g_color_buffer_data[i][j - 1] = (gl_terrain_verts[i][j] - min_height) / height_range;
            g_color_buffer_data[i][j] = (gl_terrain_verts[i][j] - min_height) / height_range;
            g_color_buffer_data[i][j + 1] = (gl_terrain_verts[i][j] - min_height) / height_range;
        }
    }
}

void
WorldMaker::VertexColourReal(vector<vector<GLfloat>> &g_color_buffer_data, vector<vector<GLfloat>> gl_terrain_verts,
                             ColourList colours, float min_height, float max_height) {
    float height = 0;
    float rand_num = 0;
    for (int i = 0; i < get_no_of_terrain_vertex_arrays(); i++) {
        for (int j = 1; j < diamond_square_->get_no_of_vertices(); j += 3) {
            int min = 97, max = 103;

            height = (gl_terrain_verts[i][j] - min_height) / (max_height - min_height);
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

            } else if (height <= 1) {
                g_color_buffer_data[i][j - 1] = colours.snow_white[0] * rand_num;
                g_color_buffer_data[i][j] = colours.snow_white[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.snow_white[2] * rand_num;
            }

        }
    }
}

void WorldMaker::VertexColourRealBlended(vector<vector<GLfloat>> &g_color_buffer_data,
                                         vector<vector<GLfloat>> gl_terrain_verts, ColourList colours,
                                         float min_height, float max_height) {
    GLfloat blended_colour[3];
    float height, rand_num;

    for (int i = 0; i < get_no_of_terrain_vertex_arrays(); i++) {
        for (int j = 1; j < diamond_square_->get_no_of_vertices(); j += 3) {
            int min = 97, max = 103;

            height = (gl_terrain_verts[i][j] - min_height) / (max_height - min_height);
            rand_num = (float) (rand() % (max + 1 - min) + min);
            rand_num /= 100;


            if (height < 0.225) {
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

            } else if (height <= 1) {
                g_color_buffer_data[i][j - 1] = colours.snow_white[0] * rand_num;
                g_color_buffer_data[i][j] = colours.snow_white[1] * rand_num;
                g_color_buffer_data[i][j + 1] = colours.snow_white[2] * rand_num;
            }

        }
    }
}


GLfloat *WorldMaker::BlendColours(GLfloat colour_one[3], GLfloat colour_two[3], GLfloat blended_colour[3],
                                  float percentage) {
    GLfloat range;
    for (int i = 0; i < 3; i++) {
        range = colour_one[i] - colour_two[i];
        blended_colour[i] = colour_two[i] + (range * percentage);
    }
    return blended_colour;
}

LSystem *WorldMaker::get_tree() const {
    return tree_;
}

const vector<GLfloat> &WorldMaker::get_tree_positions_() const {
    return tree_positions_;
}







