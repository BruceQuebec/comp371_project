#include <fstream>
#include <sstream>
#include <unordered_map>

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "stb_image.h"

#include "load.hpp"


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


void loadModel(vector<GLdouble> & pos_data, vector<GLdouble> & color_data, vector<GLdouble> & uv_data, vector<GLdouble> & normal_data, vector<GLuint> & index_data, GLenum mode, const char * file_path)
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
		}
	}

	if (normal_data.size() == 0)
	{
		std::unordered_map<string, glm::vec3> verticesNormal;

		for (int i = 0; i < pos_data.size(); i += 9)
		{
			// Organize vertex position data from pos_data vectors
			glm::vec3 curVertex = glm::vec3(pos_data[i], pos_data[i + 1], pos_data[i + 2]);
			glm::vec3 firstNextVertex = glm::vec3(pos_data[i + 3], pos_data[i + 4], pos_data[i + 5]);
			glm::vec3 secondNextVertex = glm::vec3(pos_data[i + 6], pos_data[i + 7], pos_data[i + 8]);
			// Get the face normal
			glm::vec3 vector1 = firstNextVertex - curVertex;
			glm::vec3 vector2 = secondNextVertex - curVertex;
			glm::vec3 faceNormal = glm::cross(vector1, vector2);

			// Add the face normal to the 3 vertices normal touching this face
			verticesNormal[glm::to_string(curVertex)] += faceNormal;
			verticesNormal[glm::to_string(firstNextVertex)] += faceNormal;
			verticesNormal[glm::to_string(secondNextVertex)] += faceNormal;
		}

		// Normalize vertices normal
		std::unordered_map<string, glm::vec3>::iterator it = verticesNormal.begin();
		while (it != verticesNormal.end()) {
			if (!(it->second.x == 0 && it->second.y == 0 && it->second.z == 0)) {
				it->second = glm::normalize(it->second);
			}
			normal_data.push_back(it->second.x);
			normal_data.push_back(it->second.y);
			normal_data.push_back(it->second.z);
			it++;
		}
	}
}


void loadTexture(GLuint & texture, const char * file_path)
{
	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, comp;
	unsigned char * data = stbi_load(file_path, &width, &height, &comp, STBI_rgb);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);
}