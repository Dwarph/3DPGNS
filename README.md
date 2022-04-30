# 3DPGNS
3D Procedurally Generated Nature Scene

This project is a 3D Procedurally Generated Nature Scene, using Diamond Square terrain and Lindenmayer Systems.

All code in the root folder was created individually by Pip Turner,
with the exception of main.cpp, which was initially based off of a tutorial from http://opengl-tutorial.org.

All code in external/OpenGLTutorialUsefulFiles remains untouched from the tutorial http://opengl-tutorial.org,
with the exception of controls.cpp & controls.hpp, which have been significantly modified.

See a devlog [here](https://twitter.com/dwarph/status/968892031220232192).


#Compile and Run Instructions

Whilst glew, glm and nanogui are all included with this build,
please ensure that you have the latest versions of CMake, C++, OpenGL and Boost installed.

This project has only been tested on Linux based systems.

# Compile instructions:

From a command line, navigate to the build folder.
type "cmake .."
type make all

## Run instructions:

In the build folder, type "./3DPGNS"


# How to Interact with the scene:

Mouse: Look around

WASD/Arrow Keys: Move around

Y: Hide Terrain
T: Show Terrain

K: Hide L-Systems
L: Show L-Systems

0-9: Change level of detail displayed

R: Display terrain using random colours
G: Display terrain with greyscale colours
C: Display terrain with non blended colours
B: Display terrain with blended colours.
