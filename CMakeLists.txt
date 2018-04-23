# CMake entry point
cmake_minimum_required(VERSION 3.9)

project(3DPGNS)

find_package(OpenGL REQUIRED)


set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
    message(FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)")
endif ()
if (CMAKE_SOURCE_DIR MATCHES " ")
    message("Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif ()
if (CMAKE_BINARY_DIR MATCHES " ")
    message("Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause.")
endif ()


# Compile external dependencies
add_subdirectory(external)


add_definitions(-DBOOST_ALL_NO_LIB)

# link against dynamic libraries
add_definitions(-DBOOST_ALL_DYN_LINK)
find_package(Boost COMPONENTS system unit_test_framework)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP

if (INCLUDE_DISTRIB)
    add_subdirectory(distrib)
endif (INCLUDE_DISTRIB)

include_directories(

        external/glfw-3.1.2/include/GLFW/
        external/glm-0.9.7.1/
        external/glew-1.13.0/include/
        external/nanogui/include/nanogui
)
find_package(Boost 1.62 REQUIRED COMPONENTS unit_test_framework)

set(ALL_LIBS
        ${OPENGL_LIBRARY}
        glfw
        GLEW_1130
        nanogui
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
        )

add_definitions(
        -DTW_STATIC
        -DTW_NO_LIB_PRAGMA
        -DTW_NO_DIRECT3D
        -DGLEW_STATIC
        -D_CRT_SECURE_NO_WARNINGS
)

add_executable(3DPGNS
        main.cpp
        diamond_square.cpp
        diamond_square.h
        l_system.cpp
        l_system.h

        world_maker.cpp
        world_maker.h

        external/OpenGLTutorialUsefulFiles/shader.cpp
        external/OpenGLTutorialUsefulFiles/shader.hpp

        external/OpenGLTutorialUsefulFiles/controls.cpp
        external/OpenGLTutorialUsefulFiles/controls.hpp

        external/OpenGLTutorialUsefulFiles/TransformVertexShader.vertexshader
        external/OpenGLTutorialUsefulFiles/ColorFragmentShader.fragmentshader)

add_executable(TESTING
        Tests/test_diamond_square.cpp

        diamond_square.cpp
        diamond_square.h
        l_system.cpp
        l_system.h

        world_maker.cpp
        world_maker.h

        Tests/test_diamond_square.cpp

        external/OpenGLTutorialUsefulFiles/shader.cpp
        external/OpenGLTutorialUsefulFiles/shader.hpp

        external/OpenGLTutorialUsefulFiles/TransformVertexShader.vertexshader
        external/OpenGLTutorialUsefulFiles/ColorFragmentShader.fragmentshader
        )


target_link_libraries(3DPGNS
        ${ALL_LIBS}
        )

target_link_libraries(TESTING
        ${ALL_LIBS}
        )


set_target_properties(3DPGNS PROPERTIES XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3DPGNS/")
create_target_launcher(3DPGNS WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/3DPGNS/")