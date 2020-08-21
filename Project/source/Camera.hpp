#pragma once


#include <glm/glm.hpp>


#include "Box.hpp"


class Camera
{
private:
	// Position of the camera
	double x_pos;
	double y_pos;
	double z_pos;

	// Angle of the camera
	double theta;
	double rho;

	// Angle of the field of view
	float fov;

	// Aspect ratio of the field of view
	float aspect;

	Box * box;

	bool move_x;
	bool move_x_;
	bool move_y;
	bool move_y_;
	bool move_z;
	bool move_z_;

public:
	Camera(glm::vec3 pos, float aspect);

	glm::mat4 getViewMat();

	glm::mat4 getProjectionMat();

	glm::vec3 getCameraPos();

	void key_callback(int key, int action);

	void cursor_pos_callback(GLFWwindow * window, double x_pos, double y_pos);

	void window_size_callback(int width, int height);

	void scroll_callback(double yoffset);
};