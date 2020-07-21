#pragma once


#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.hpp"


class Model
{
public:
	// Vector of models
	//static std::vector<Model *> models;
	static unordered_map<string, Model*> models;

private:
	// Vertex array
	GLuint vertex_array;

	// the instance of shader being used in the modle
	Shader shader;

	// Position, color and index data
	std::vector<GLdouble> pos_data;
	std::vector<GLdouble> color_data;
	std::vector<GLuint> index_data;
	std::vector<GLdouble> normal_data;
	std::vector<GLdouble> texCoords_data;

	// Number of elements
	int num_elements;
	int num_vertices = 0;

	// Mode (line or triangle)
	GLenum mode;

	// Position
	double x_pos;
	double y_pos;
	double z_pos;
	
	// Scale
	double scale;

	// Angle of rotation
	double x_angle;
	double y_angle;
	double z_angle;

public:
	Model(vector<GLdouble> pos_data, vector<GLdouble> color_data, vector<GLuint> index_data, vector<GLdouble> normal_data, vector<GLdouble> texCoords_data, Shader& shader, unordered_map<string, GLuint> shader_pointer_idx_map, GLenum mode, double x_pos, double y_pos, double z_pos, string model_name);
	Model(Shader& shader, unordered_map<string, GLuint> shader_pointer_idx_map, GLenum mode, double x_pos, double y_pos, double z_pos, const char * file_path, string model_name);

	// Initialze the model
	void init(std::vector<GLdouble> pos_data, std::vector<GLdouble> color_data, std::vector<GLuint> index_data, std::vector<GLdouble> normal_data, std::vector<GLdouble> texCoords_data, Shader shader, unordered_map<string, GLuint> shader_pointer_idx_map, GLenum mode, double x_pos, double y_pos, double z_pos, string model_name);

	// Get the vertex array of the model
	GLuint getVertexArray();

	// Get the number of elements of the model
	int getNumElements();

	// Get the number of Vertices of the model
	int getNumVertices();

	// Get the mode of the model
	GLenum getMode();

	// Get the model matrix of the model
	glm::mat4 getModelMat();

	// Get Shader of the model
	Shader getShader();

	// Get position of the model
	glm::vec3 getPosition();

	// Get normal of the model
	vector<GLdouble> getNormals(vector<GLdouble> pos_data, vector<GLuint> index_data);

	// Set model scale
	void setScale(double scale);

	// Key callback of the model
	void key_callback(int key, int action, int mods);
};