#include <fstream>
#include <sstream>
#include "load.hpp"
#include <iostream>

using namespace std;


void loadShader(const GLuint & program, GLenum type, const char * file_path)
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


void loadModel(std::vector<GLdouble> & pos_data, std::vector<GLdouble> & color_data, std::vector<GLuint> & index_data, std::vector<GLdouble> & normal_data, std::vector<GLdouble> & texCoords_data, GLenum mode, const char * file_path) {
	ifstream model_stream(file_path);

	string line;
	while (getline(model_stream, line))
	{
		if (line.length() != 0)
		{
			stringstream string_stream(line);

			double x_data, y_data, z_data;
			unsigned int index1, index2, index3;

			char label;
			string_stream >> label;
			if (label == 'p')
			{
				string_stream >> x_data >> y_data >> z_data;

				pos_data.push_back(x_data);
				pos_data.push_back(y_data);
				pos_data.push_back(z_data);
			}
			else if (label == 'c')
			{
				string_stream >> x_data >> y_data >> z_data;

				color_data.push_back(x_data);
				color_data.push_back(y_data);
				color_data.push_back(z_data);
			}
			else if (label == 'i')
			{
				if (mode == GL_LINES)
				{
					string_stream >> index1 >> index2;

					index_data.push_back(index1);
					index_data.push_back(index2);
				}
				else if (mode == GL_TRIANGLES)
				{
					string_stream >> index1 >> index2 >> index3;

					index_data.push_back(index1);
					index_data.push_back(index2);
					index_data.push_back(index3);
				}
			}
			else if (label == 'n') {
				string_stream >> x_data >> y_data >> z_data;

				normal_data.push_back(x_data);
				normal_data.push_back(y_data);
				normal_data.push_back(z_data);
			}
			else if (label == 'u') {
				string_stream >> x_data >> y_data;

				texCoords_data.push_back(x_data);
				texCoords_data.push_back(y_data);
			}
		}
	}
}