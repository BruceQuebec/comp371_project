#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Projection.hpp"
#include "Model.hpp"


using namespace std;
using namespace glm;


const double Projection::PI = 3.141592653589793;

GLFWwindow * Projection::window;

bool Projection::is_mouse_button_left_pressed = false;

float Projection::fov = PI / 2;

void Projection::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
    glfwSetCursorPos(window, 512, 384);

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        is_mouse_button_left_pressed = true;

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        is_mouse_button_left_pressed = false;
}

void Projection::setWindow(GLFWwindow* window)
{
    Projection::window = window;
}

mat4 Projection::getProjectionMat()
{
    if (is_mouse_button_left_pressed)
    {
        double y_pos;
        glfwGetCursorPos(window, NULL, &y_pos);

        double dy = (384 - y_pos) * 0.01;

        if (fov - dy <= 0)
            fov = 0;
        else if (fov - dy >= PI)
            fov = PI;
        else
            fov -= dy;
    }
    glfwSetCursorPos(window, 512, 384);

	return perspective(fov, (float)4.0 / 3, 0.01f, 100.0f);
}