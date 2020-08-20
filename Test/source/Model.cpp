#include "glm/gtx/transform2.hpp"

#include "load.hpp"
#include "Shader.hpp"
#include "World.hpp"

#include "Model.hpp"


using namespace glm;
using namespace std;


vector<Model *> Model::models;


Model::Model(vector<GLdouble> pos_data, vector<GLdouble> color_data, vector<GLdouble> uv_data, vector<GLdouble> & normal_data, vector<GLuint> index_data, GLenum mode, Material & material, float x_pos, float y_pos, float z_pos, vec3 scale)
{
	init(pos_data, color_data, uv_data, normal_data, index_data, mode, material, x_pos, y_pos, z_pos, scale);
}

Model::Model(GLenum mode, Material & material, float x_pos, float y_pos, float z_pos, vec3 scale, const char * model_file_path, bool load_from_txt, const char * texture_file_path)
{
	vector<GLdouble> pos_data;
	vector<GLdouble> color_data;
	vector<GLdouble> uv_data;
	vector<GLdouble> normal_data;
	vector<GLuint> index_data;

	if (load_from_txt)
		loadModel(pos_data, color_data, uv_data, normal_data, mode, model_file_path);
	else
		loadObject(pos_data, color_data, uv_data, normal_data, mode, model_file_path);


	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	loadTexture(texture, texture_file_path);


	init(pos_data, color_data, uv_data, normal_data, index_data, mode, material, x_pos, y_pos, z_pos, scale);
}

void Model::init(vector<GLdouble> pos_data, vector<GLdouble> color_data,  vector<GLdouble> uv_data, vector<GLdouble> & normal_data, vector<GLuint> index_data, GLenum mode, Material & material, float x_pos, float y_pos, float z_pos, vec3 scale)
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
	glBufferData(GL_ARRAY_BUFFER, pos_data.size() * sizeof(GLdouble), &pos_data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (void *)0);

	// Initialize and bind the color buffer
	GLuint color_buffer;
	glGenBuffers(1, &color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
	glBufferData(GL_ARRAY_BUFFER, color_data.size() * sizeof(GLdouble), &color_data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (void *)0);

	GLuint uv_buffer;
	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, uv_data.size() * sizeof(GLdouble), &uv_data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_DOUBLE, GL_FALSE, 0, (void *)0);

	GLuint normal_buffer;
	glGenBuffers(1, &normal_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, normal_data.size() * sizeof(GLdouble), &normal_data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_DOUBLE, GL_FALSE, 0, (void *)0);

	num_elements = index_data.size();
	num_vertices = pos_data.size();

	this->material = &material;

	this->mode = mode;

	this->x_pos = x_pos;
	this->y_pos = y_pos;
	this->z_pos = z_pos;

	x_shear = 0;
	z_shear = 0;

	x_scale = scale[0];
	y_scale = scale[1];
	z_scale = scale[2];

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

Material & Model::getMaterial()
{
	return *material;
}

GLenum Model::getMode()
{
	return mode;
}

mat4 Model::getModelMat()
{
	mat4 scale_mat = glm::scale(mat4(1), vec3(x_scale, y_scale, z_scale));

	mat4 rotate_x_mat = rotate(mat4(1), float(x_angle), vec3(1, 0, 0));
	mat4 rotate_y_mat = rotate(mat4(1), float(y_angle), vec3(0, 1, 0));
	mat4 rotate_z_mat = rotate(mat4(1), float(z_angle), vec3(0, 0, 1));
	mat4 rotate_mat = rotate_x_mat * rotate_y_mat * rotate_z_mat;

	mat4 translate_mat = translate(mat4(1), vec3(x_pos, y_pos, z_pos));

	mat4 shear_mat = shearY3D(mat4(1), (float)x_shear, (float)z_shear);

	mat4 model_mat = shear_mat * translate_mat * rotate_mat * scale_mat;

	mat4 world_mat = World::getWorldMat();
	model_mat = world_mat * model_mat;

	return model_mat;
}

void Model::key_callback(int key, int action, int mods)
{
	// If U or J is pressed, scale the model
	if (key == GLFW_KEY_U && action == GLFW_REPEAT)
	{
		x_scale += 0.01;
		y_scale += 0.01;
		z_scale += 0.01;
	}
	if (key == GLFW_KEY_J && action == GLFW_REPEAT)
	{
		x_scale -= 0.01;
		y_scale -= 0.01;
		z_scale -= 0.01;
	}

	if (key == GLFW_KEY_F && action == GLFW_REPEAT)
		x_shear -= 0.01;
	if (key == GLFW_KEY_H && action == GLFW_REPEAT)
		x_shear += 0.01;
	if (key == GLFW_KEY_T && action == GLFW_REPEAT)
		z_shear -= 0.01;
	if (key == GLFW_KEY_G && action == GLFW_REPEAT)
		z_shear += 0.01;
}