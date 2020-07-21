#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


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
	double fov;

	// Aspect ratio of the field of view
	double aspect;

public:
	Camera();

	// Get the product of the projection and view matrix of the camera
	glm::mat4 getCameraMat();

	glm::vec3 getCameraPosition();
};