#pragma once


class View
{
public:
	static GLFWwindow * window;

	static View * instance;

	static bool is_mouse_button_right_pressed;
	static bool is_mouse_button_middle_pressed;

	static void mouse_button_callback_dispatch(GLFWwindow * window, int button, int action, int mods);

	static void setWindow(GLFWwindow* window);

private:
	double x_pos;
	double y_pos;
	double z_pos;

	double theta;
	double rho;

public:
	View();

	glm::mat4 viewMat();

	void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
};