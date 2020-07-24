#include "load.hpp"

#include "Model.hpp"


using namespace glm;
using namespace std;


vector<Model *> Model::models;


Model::Model(vector<GLdouble> pos_data, vector<GLdouble> color_data, vector<GLdouble> uv_data, vector<GLuint> index_data, GLenum mode, double x_pos, double y_pos, double z_pos)
{
	init(pos_data, color_data, uv_data, index_data, mode, x_pos, y_pos, z_pos);
}

Model::Model(GLenum mode, double x_pos, double y_pos, double z_pos, const char * model_file_path, const char * texture_file_path)
{
	vector<GLdouble> pos_data;
	vector<GLdouble> color_data;
	vector<GLdouble> uv_data;
	vector<GLuint> index_data;

	loadModel(pos_data, color_data, uv_data, index_data, mode, model_file_path);


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	loadTexture(texture, texture_file_path);


	init(pos_data, color_data, uv_data, index_data, mode, x_pos, y_pos, z_pos);
}

void Model::init(vector<GLdouble> pos_data, vector<GLdouble> color_data, vector<GLdouble> uv_data, vector<GLuint> index_data, GLenum mode, double x_pos, double y_pos, double z_pos)
{
	// Add the model the vector of models
	models.push_back(this);

	// Initialize the vertex array
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);
	

	// Initialize and bind the position buffer
	GLuint pos_buffer;
	glGenBuffers(1, &pos_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
	if (pos_data.size() > 0)
		glBufferData(GL_ARRAY_BUFFER, pos_data.size() * sizeof(GLdouble), &pos_data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (void *)0);

	// Initialize and bind the color buffer
	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	if (color_data.size() > 0)
		glBufferData(GL_ARRAY_BUFFER, color_data.size() * sizeof(GLdouble), &color_data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (void *)0);

	GLuint uv_buffer;
	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	if (uv_data.size() > 0)
		glBufferData(GL_ARRAY_BUFFER, uv_data.size() * sizeof(GLdouble), &uv_data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, 0, (void *)0);

	// Initialize and bind the index buffer
	GLuint index_buffer;
	glGenBuffers(1, &index_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	if (index_data.size() > 0)
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), &index_data[0], GL_STATIC_DRAW);

	num_elements = index_data.size();
	num_vertices = pos_data.size();

	this->mode = mode;

	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->z_pos = z_pos;

	scale = 1;

	x_angle = 0;
	y_angle = 0;
	z_angle = 0;
}

void Model::setPos(double x_pos, double y_pos, double z_pos)
{
	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->z_pos = z_pos;
}

GLuint Model::getVertexArray()
{
	return vertex_array;
}

int Model::getNumElements()
{
	return num_elements;
}

int Model::getNumVertices()
{
	return num_vertices;
}

GLuint Model::getTexture()
{
	return texture;
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
	// If W, S, A or D is pressed, shift the model
	if (key == GLFW_KEY_W && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		z_pos -= 0.1;
	if (key == GLFW_KEY_S && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		z_pos += 0.1;
	if (key == GLFW_KEY_A && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		x_pos -= 0.1;
	if (key == GLFW_KEY_D && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		x_pos += 0.1;

	// If U or J is pressed, scale the model
	if (key == GLFW_KEY_U && action == GLFW_REPEAT)
		scale += 0.01;
	if (key == GLFW_KEY_J && action == GLFW_REPEAT)
		scale -= 0.01;

	// If w, s, a, or d is pressed, rotate the model
	if (key == GLFW_KEY_W && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		x_angle += 0.01;
	if (key == GLFW_KEY_S && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		x_angle -= 0.01;
	if (key == GLFW_KEY_A && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		y_angle += 0.01;
	if (key == GLFW_KEY_D && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		y_angle -= 0.01;
}