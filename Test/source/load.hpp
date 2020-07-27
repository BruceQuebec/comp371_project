#pragma once

#include <GL/glew.h>
#include <vector>

using namespace std;

// Load a shader from a file
void loadShader(const GLuint & program, GLenum type, const char * file_path);

// Load a model from a file
void loadModel(std::vector<GLdouble> & pos_data, std::vector<GLdouble> & color_data, std::vector<GLuint> & index_data, std::vector<GLdouble> & normal_data, std::vector<GLdouble> & texCoords_data, GLenum mode, const char * file_path);