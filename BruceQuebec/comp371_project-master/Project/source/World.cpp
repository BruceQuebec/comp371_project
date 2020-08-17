#include <GLFW/glfw3.h>
#include "World.hpp"


using namespace glm;


double World::x_angle = 0;
double World::y_angle = 0;
double World::z_angle = 0;


mat4 World::getWorldMat()
{
	mat4 rotate_x_mat = rotate(mat4(1), float(x_angle), vec3(1, 0, 0));
	mat4 rotate_y_mat = rotate(mat4(1), float(y_angle), vec3(0, 1, 0));
	mat4 rotate_z_mat = rotate(mat4(1), float(z_angle), vec3(0, 0, 1));
	mat4 rotate_mat = rotate_x_mat * rotate_y_mat * rotate_z_mat;

	mat4 world_mat = rotate_mat;

    return world_mat;
}

void World::key_callback(int key, int action)
{
	// If Up, down, left, or right arrow is pressed, rotate the world; if Home is pressed, reset its initial orientation
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
		x_angle -= 0.01;
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
		x_angle += 0.01;
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
		z_angle += 0.01;
	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
		z_angle -= 0.01;
	if (key == GLFW_KEY_HOME && action == GLFW_PRESS)
	{
		x_angle = 0;
		y_angle = 0;
		z_angle = 0;
	}
}