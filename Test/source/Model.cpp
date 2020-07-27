

#include "load.hpp"
#include "Model.hpp"
#include "utils.hpp"


using namespace std;
using namespace glm;


unordered_map<string, Model*> Model::models;


Model::Model(vector<GLdouble> pos_data, vector<GLdouble> color_data, vector<GLuint> index_data, vector<GLdouble> normal_data, vector<GLdouble> texCoords_data, Shader& shader, unordered_map<string, GLuint> shader_pointer_idx_map, GLenum mode, double x_pos, double y_pos, double z_pos, string model_name, string texture_file_path)
{
	init(pos_data, color_data, index_data, normal_data, texCoords_data, shader, shader_pointer_idx_map, mode, x_pos, y_pos, z_pos, model_name, texture_file_path);
}

Model::Model(Shader& shader, unordered_map<string, GLuint> shader_pointer_idx_map, GLenum mode, double x_pos, double y_pos, double z_pos, const char * file_path, string model_name, string texture_file_path)
{
	vector<GLdouble> pos_data;
	vector<GLdouble> color_data;
	vector<GLuint> index_data;
	vector<GLdouble> normal_data;
	vector<GLdouble> texCoords_data;

	loadModel(pos_data, color_data, index_data, normal_data, texCoords_data, mode, file_path);
	
	if (normal_data.size() == 0 && (mode==GL_TRIANGLES || mode==GL_TRIANGLE_STRIP)) {
		normal_data = Model::getNormals(pos_data);
	}
	
	init(pos_data, color_data, index_data, normal_data, texCoords_data, shader, shader_pointer_idx_map, mode, x_pos, y_pos, z_pos, model_name, texture_file_path);
}

void Model::init(std::vector<GLdouble> pos_data, std::vector<GLdouble> color_data, std::vector<GLuint> index_data, std::vector<GLdouble> normal_data, std::vector<GLdouble> texCoords_data, Shader shader, unordered_map<string, GLuint> shader_pointer_idx_map, GLenum mode, double x_pos, double y_pos, double z_pos, string model_name, string texture_file_path)
{
	num_vertices = pos_data.size();

	// Add the model the vector of models
	models[model_name] = this;

	// Initialize the vertex array
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	// Initialize and bind the position buffer
	if (pos_data.size() > 0) {
		GLuint pos_buffer;
		glGenBuffers(1, &pos_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);
		glBufferData(GL_ARRAY_BUFFER, pos_data.size() * sizeof(GLdouble), &pos_data[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(shader_pointer_idx_map["pos_data_idx"]);
		glVertexAttribPointer(shader_pointer_idx_map["pos_data_idx"], 3, GL_DOUBLE, GL_FALSE, 0, (void *)0);
	}
	

	// Initialize and bind the color buffer
	if (color_data.size() > 0) {
		GLuint color_buffer;
		glGenBuffers(1, &color_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
		glBufferData(GL_ARRAY_BUFFER, color_data.size() * sizeof(GLdouble), &color_data[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(shader_pointer_idx_map["color_data_idx"]);
		glVertexAttribPointer(shader_pointer_idx_map["color_data_idx"], 3, GL_DOUBLE, GL_FALSE, 0, (void *)0);
	}
	

	// Initialize and bind the normal buffer
	if (normal_data.size() > 0) {
		GLuint normal_buffer;
		glGenBuffers(1, &normal_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
		glBufferData(GL_ARRAY_BUFFER, normal_data.size() * sizeof(GLdouble), &normal_data[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(shader_pointer_idx_map["normal_data_idx"]);
		glVertexAttribPointer(shader_pointer_idx_map["normal_data_idx"], 3, GL_DOUBLE, GL_FALSE, 0, (void *)0);
	}

	// Initialize and bind the texture coordinates buffer
	if (texCoords_data.size() > 0 && texture_file_path!="") {
		GLuint texCoords_buffer;
		glGenBuffers(1, &texCoords_buffer);
		glBindBuffer(GL_ARRAY_BUFFER, texCoords_buffer);
		glBufferData(GL_ARRAY_BUFFER, texCoords_data.size() * sizeof(GLdouble), &texCoords_data[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(shader_pointer_idx_map["texCoords_data_idx"]);
		glVertexAttribPointer(shader_pointer_idx_map["texCoords_data_idx"], 2, GL_DOUBLE, GL_FALSE, 0, (void *)0);

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		unsigned char *data = stbi_load(texture_file_path.c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	

	// Initialize and bind the index buffer
	if (index_data.size() > 0) {
		GLuint index_buffer;
		glGenBuffers(1, &index_buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_data.size() * sizeof(GLuint), &index_data[0], GL_STATIC_DRAW);
	}	

	num_elements = index_data.size();
	this->shader = shader;
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

int Model::getNumVertices() {
	return num_vertices;
}

GLenum Model::getMode()
{
	return mode;
}
Shader Model::getShader() {
	return shader;
}

GLuint Model::getTextureID() {
	return textureID;
}

glm::vec3 Model::getPosition() {
	return glm::vec3(this->x_pos, this->y_pos, this->z_pos);
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

vector<GLdouble> Model::getNormals(vector<GLdouble> pos_data, vector<GLuint> index_data) {
	vector<GLdouble> normals = Utils::getNormalForVertex(pos_data, index_data);
	return normals;
}

vector<GLdouble> Model::getNormals(vector<GLdouble> pos_data) {
	vector<GLdouble> normals = Utils::getNormalForVertex(pos_data);
	return normals;
}

void Model::setScale(double scale) {
	this->scale = scale;
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