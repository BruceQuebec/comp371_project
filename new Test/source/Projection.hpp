#pragma once


class Projection
{
public:
	static const double PI;

	static GLFWwindow * window;

	static bool is_mouse_button_left_pressed;

	static float fov;

	static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);

	static glm::mat4 getProjectionMat();

	static void setWindow(GLFWwindow * window);
};