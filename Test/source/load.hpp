#pragma once


#include <vector>

#include <GL/glew.h>


// Load a shader from a file
void loadShader(const GLuint & program, GLenum type, const char * file_path);

// Load a model from a file
void loadModel(std::vector<GLdouble> & pos_data, std::vector<GLdouble> & color_data, std::vector<GLdouble> & uv_data, std::vector<GLdouble> & normal_data, std::vector<GLuint> & index_data, GLenum mode, const char * file_path);


void loadTexture(GLuint & texture, const char * file_path);