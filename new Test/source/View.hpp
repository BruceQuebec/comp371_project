#pragma once


class View
{
public:
	static View * instance;

	static void key_callback_dispatch(GLFWwindow * window, int key, int scancode, int action, int mods);;


private:
	double x_pos;
	double y_pos;
	double z_pos;

	double x_angle;
	double y_angle;
	double z_angle;

public:
	View();

	glm::mat4 viewMat();

	void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
};