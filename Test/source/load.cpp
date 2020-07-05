#include <fstream>
#include <sstream>

#include <GL/glew.h>

#include "load.hpp"

using namespace std;


void loadShader(const GLuint & program, const char * file_path, GLenum type)
{
	ifstream shader_stream(file_path, ifstream::in);

	stringstream shader_string_stream;
	shader_string_stream << shader_stream.rdbuf();

	shader_stream.close();

	string shader_string = shader_string_stream.str();

	char const * shader_char_pointer = shader_string.c_str();

	GLuint shader = glCreateShader(type);

	glShaderSource(shader, 1, &shader_char_pointer, NULL);

	glCompileShader(shader);

	glAttachShader(program, shader);

	glLinkProgram(program);
}