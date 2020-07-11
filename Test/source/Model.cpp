#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "load.hpp"

#include "Model.hpp"


using namespace std;
using namespace glm;


vector<Model *> Model::models;


Model::Model(vector<GLdouble> pos_data, vector<GLdouble> color_data, vector<GLuint> index_data, GLenum mode, double x_pos, double y_pos, double z_pos)
{
	init(pos_data, color_data, index_data, mode, x_pos, y_pos, z_pos);
}

Model::Model(GLenum mode, double x_pos, double y_pos, double z_pos, const char * file_path)
{
	vector<GLdouble> pos_data;
	vector<GLdouble> color_data;
	vector<GLuint> index_data;

	loadModel(pos_data, color_data, index_data, mode, file_path);

	init(pos_data, color_data, index_data, mode, x_pos, y_pos, z_pos);
}

void Model::init(vector<GLdouble> pos_data, vector<GLdouble> color_data, vector<GLuint> index_data, GLenum mode, double x_pos, double y_pos, double z_pos)
{
	models.push_back(this);

	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	GLuint pos_buffer;
	glGenBuffers(1, &pos_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
	glBufferData(GL_ARRAY_BUFFER, pos_data.size() * sizeof(GLdouble), &pos_data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (void *)0);

	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, color_data.size() * sizeof(GLdouble), &color_data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (void *)0);

	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), &index_data[0], GL_STATIC_DRAW);

	num_elements = index_data.size();

	this->mode = mode;

	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->z_pos = z_pos;

	scale = 1;

	x_angle = 0;
	y_angle = 0;
	z_angle = 0;
}

GLuint Model::getVertexArray()
{
	return vertex_array;
}

int Model::getNumElements()
{
	return num_elements;
}

GLenum Model::getMode()
{
	return mode;
}

mat4 Model::getModelMat()
{
	mat4 translate_mat = translate(mat4(1), vec3(x_pos, y_pos, z_pos));

	mat4 scale_mat = glm::scale(mat4(1), vec3(scale, scale, scale));

	mat4 rotate_x_mat = rotate(mat4(1), float(x_angle), vec3(1, 0, 0));
	mat4 rotate_y_mat = rotate(mat4(1), float(y_angle), vec3(0, 1, 0));
	mat4 rotate_z_mat = rotate(mat4(1), float(z_angle), vec3(0, 0, 1));
	mat4 rotate_mat = rotate_x_mat * rotate_y_mat * rotate_z_mat;

	mat4 model_mat = translate_mat * scale_mat * rotate_mat;

	return model_mat;
}

void Model::key_callback(int key, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		z_pos -= 0.01;
	if (key == GLFW_KEY_S && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		z_pos += 0.01;
	if (key == GLFW_KEY_A && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		x_pos -= 0.01;
	if (key == GLFW_KEY_D && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		x_pos += 0.01;

	if (key == GLFW_KEY_U && action == GLFW_REPEAT)
		scale += 0.01;
	if (key == GLFW_KEY_J && action == GLFW_REPEAT)
		scale -= 0.01;

	if (key == GLFW_KEY_W && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		x_angle += 0.01;
	if (key == GLFW_KEY_S && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		x_angle -= 0.01;
	if (key == GLFW_KEY_A && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		y_angle += 0.01;
	if (key == GLFW_KEY_D && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		y_angle -= 0.01;
}