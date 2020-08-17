#include <fstream>
#include <sstream>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"

#include "load.hpp"


using namespace std;
using namespace glm;


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


void loadModel(vector<GLdouble> & pos_data, vector<GLdouble> & color_data, vector<GLdouble> & uv_data, vector<GLdouble> & normal_data, GLenum mode, const char * file_path)
{
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
			else if (label == 'u')
			{
				string_stream >> x_data >> y_data;

				uv_data.push_back(x_data);
				uv_data.push_back(y_data);
			}
			else if (label == 'n')
			{
				string_stream >> x_data >> y_data >> z_data;

				normal_data.push_back(x_data);
				normal_data.push_back(y_data);
				normal_data.push_back(z_data);
			}
		}
	}
}


void loadObject(vector<GLdouble> & pos_data, vector<GLdouble> & color_data, vector<GLdouble> & uv_data, vector<GLdouble> & normal_data, GLenum mode, const char * file_path)
{
	ifstream model_stream(file_path);

	vector<vec3> temp_pos_data; temp_pos_data.push_back(vec3(0));
	vector<vec3> temp_color_data; temp_color_data.push_back(vec3(0));
	vector<vec2> temp_uv_data; temp_uv_data.push_back(vec2(0));
	vector<vec3> temp_normal_data; temp_normal_data.push_back(vec3(0));

	string line;
	while (getline(model_stream, line))
	{
		if (line.length() != 0)
		{
			stringstream line_stream(line);

			string label;
			line_stream >> label;
			if (label == "v")
			{
				float x_pos, y_pos, z_pos;
				float r_color, g_color, b_color;
				line_stream >> x_pos >> y_pos >> z_pos >> r_color >> g_color >> b_color;

				temp_pos_data.push_back(vec3(x_pos, y_pos, z_pos));
				temp_color_data.push_back(vec3(r_color, g_color, b_color));
			}
			else if (label == "vt")
			{
				float x_vt, y_vt;
				line_stream >> x_vt >> y_vt;

				temp_uv_data.push_back(vec2(x_vt, y_vt));
			}
			else if (label == "vn")
			{
				float x_vn, y_vn, z_vn;
				line_stream >> x_vn >> y_vn >> z_vn;

				temp_normal_data.push_back(vec3(x_vn, y_vn, z_vn));
			}
			else if (label == "f")
			{
				for (int i = 0; i < 3; i++)
				{
					string face;
					line_stream >> face;

					stringstream face_stream(face);

					string v, vt, vn;
					getline(face_stream,  v, '/');
					getline(face_stream, vt, '/');
					getline(face_stream, vn, '/');

					int v_index = stoi(v), vt_index = stoi(vt), vn_index = stoi(vn);

					pos_data.push_back(temp_pos_data[v_index][0]);
					pos_data.push_back(temp_pos_data[v_index][1]);
					pos_data.push_back(temp_pos_data[v_index][2]);
					color_data.push_back(temp_color_data[v_index][0]);
					color_data.push_back(temp_color_data[v_index][1]);
					color_data.push_back(temp_color_data[v_index][2]);
					uv_data.push_back(temp_uv_data[vt_index][0]);
					uv_data.push_back(temp_uv_data[vt_index][1]);
					normal_data.push_back(temp_normal_data[vn_index][0]);
					normal_data.push_back(temp_normal_data[vn_index][1]);
					normal_data.push_back(temp_normal_data[vn_index][2]);
				}
			}
		}
	}
}


void loadTexture(GLuint & texture, const char * file_path)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	unsigned char * data = stbi_load(file_path, &width, &height, &channels, 0);

	if (channels == STBI_rgb)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,  GL_RGB, GL_UNSIGNED_BYTE, data);
	else if (channels == STBI_rgb_alpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
}