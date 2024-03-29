#version 330 core
/* Shader based off of http://opengl-tutorial.org */

//Mesh data, position is different for each execution of this shader

layout(location = 0) in vec3 l_system_vertices;
layout(location = 1) in vec3 l_system_position; // Position of the center of the particule and size of the square
layout(location = 2) in vec3 vertex_colour; // colour of the particle

out vec3 fragmentColor;

uniform mat4 MVP;


void main() {


	gl_Position =  MVP * vec4(l_system_vertices.x + l_system_position.x,
	                            l_system_vertices.y + l_system_position.y,
	                            l_system_vertices.z + l_system_position.z,
	                                    1);
	fragmentColor = vertex_colour;


}
