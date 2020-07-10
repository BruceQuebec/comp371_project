#pragma once


#include <vector>


class Model
{
public:
	static std::vector<Model *> models;

private:
	GLuint vertex_array;

	std::vector<GLdouble> pos_data;
	std::vector<GLdouble> color_data;
	std::vector<GLuint> index_data;

	int num_elements;

	GLenum mode;

	double x_pos;
	double y_pos;
	double z_pos;

	double scale;

	double x_angle;
	double y_angle;
	double z_angle;

public:
	Model(std::vector<GLdouble> pos_data, std::vector<GLdouble> color_data, std::vector<GLuint> index_data, GLenum mode, double x_pos, double y_pos, double z_pos);

	GLuint getVertexArray();

	int getNumElements();

	GLenum getMode();

	glm::mat4 getModelMat();

	void key_callback(int key, int action, int mods);
};