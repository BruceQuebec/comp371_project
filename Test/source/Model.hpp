#pragma once


#include <vector>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


class Model
{
public:
	// Vector of models
	static std::vector<Model *> models;

private:
	// Vertex array
	GLuint vertex_array;

	// Position, color and index data
	std::vector<GLdouble> pos_data;
	std::vector<GLdouble> color_data;
	std::vector<GLuint> index_data;

	// Number of elements
	int num_elements;

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
	Model(std::vector<GLdouble> pos_data, std::vector<GLdouble> color_data, std::vector<GLuint> index_data, GLenum mode, double x_pos, double y_pos, double z_pos);
	Model(GLenum mode, double x_pos, double y_pos, double z_pos, const char * file_path);

	// Initialze the model
	void init(std::vector<GLdouble> pos_data, std::vector<GLdouble> color_data, std::vector<GLuint> index_data, GLenum mode, double x_pos, double y_pos, double z_pos);

	void setPos(double x_pos, double y_pos, double z_pos);

	// Get the vertex array of the model
	GLuint getVertexArray();

	// Get the number of elements of the model
	int getNumElements();

	// Get the mode of the model
	GLenum getMode();

	// Get the model matrix of the model
	glm::mat4 getModelMat();

	// Key callback of the model
	void key_callback(int key, int action, int mods);
};