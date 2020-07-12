#pragma once


#include <glm/gtc/matrix_transform.hpp>


class World
{
private:
	// Angle of rotation of the world
	static double x_angle;
	static double y_angle;
	static double z_angle;

public:
	// Get the rotation matrix of the world
	static glm::mat4 getWorldMat();

	// key callback
	static void key_callback(int key, int action);
};