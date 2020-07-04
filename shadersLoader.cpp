#include <fstream>
#include <sstream>
#include <GL/glew.h>

#include "shadersLoader.hpp"

using namespace std;


void loadShaders(const GLuint & program, const char * vertex_shader_file_path, GLenum shaderType)
{
	ifstream vertexShaderStream(vertex_shader_file_path, ifstream::in);

	stringstream vertexShaderStringStream;
	vertexShaderStringStream << vertexShaderStream.rdbuf();

	vertexShaderStream.close();

	string vertexShaderString = vertexShaderStringStream.str();

	char const * VertexShaderCharPointer = vertexShaderString.c_str();

	GLuint vertexShader = glCreateShader(shaderType);

	glShaderSource(vertexShader, 1, &VertexShaderCharPointer, NULL);

	glCompileShader(vertexShader);

	glAttachShader(program, vertexShader);

	glLinkProgram(program);
}