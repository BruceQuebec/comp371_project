#pragma once


#include <vector>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Material.hpp"
#include "Shader.hpp"


class Model
{
public:
	// Vector of models
	static std::vector<Model *> models;

private:
	// Vertex array
	GLuint vertex_array;

	GLuint texture;

	Material * material;

	Shader * shader;


	// Number of elements
	int num_elements;
	int num_vertices;

	// Mode (line or triangle)
	GLenum mode;

	// Position
	float x_pos;
	float y_pos;
	float z_pos;

	float x_shear;
	float y_shear;
	float z_shear;
	
	// Scale
	float scale;

	// Angle of rotation
	float x_angle;
	float y_angle;
	float z_angle;

public:
	Model(std::vector<GLdouble> pos_data, std::vector<GLdouble> color_data, std::vector<GLdouble> uv_data, std::vector<GLdouble> & normal_data, std::vector<GLuint> index_data, GLenum mode, Material & material, float x_pos, float y_pos, float z_pos);
	Model(GLenum mode, Material & material, float x_pos, float y_pos, float z_pos, const char * model_file_path, bool load_from_txt, const char * texture_file_path);

	// Initialze the model
	void init(std::vector<GLdouble> pos_data, std::vector<GLdouble> color_data, std::vector<GLdouble> uv_data, std::vector<GLdouble> & normal_data, std::vector<GLuint> index_data, GLenum mode, Material & material, float x_pos, float y_pos, float z_pos);

	void setPos(double x_pos, double y_pos, double z_pos);


	// Get the vertex array of the model
	GLuint getVertexArray();



	// Get the number of elements of the model
	int getNumElements();

	int getNumVertices();

	GLuint getTexture();

	Material & getMaterial();

	// Get the mode of the model
	GLenum getMode();

	// Get the model matrix of the model
	glm::mat4 getModelMat();

	// Key callback of the model
	void key_callback(int key, int action, int mods);
};