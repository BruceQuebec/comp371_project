#pragma once


void loadShader(const GLuint & program, GLenum type, const char * file_path);


void loadModel(std::vector<GLdouble> & pos_data, std::vector<GLdouble> & color_data, std::vector<GLuint> & index_data, GLenum mode, const char * file_path);