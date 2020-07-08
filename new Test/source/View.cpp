#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "View.hpp"
#include "Projection.hpp"


using namespace std;
using namespace glm;


GLFWwindow * View::window;

View * View::instance;

bool View::is_mouse_button_right_pressed = false;
bool View::is_mouse_button_middle_pressed = false;

void View::mouse_button_callback_dispatch(GLFWwindow * window, int button, int action, int mods)
{
    Projection::mouse_button_callback(window, button, action, mods);
    instance->mouse_button_callback(window, button, action, mods);
}

void View::setWindow(GLFWwindow * window)
{
    View::window = window;
}


View::View()
{
    x_pos = 2;
    y_pos = 2;
    z_pos = 4;

    theta = 0;
    rho = 0;

    instance = this;
}

mat4 View::viewMat()
{
    if (is_mouse_button_right_pressed)
    {
        double cursor_x_pos;
        glfwGetCursorPos(window, &cursor_x_pos, NULL);

        double drho = (cursor_x_pos - 512) * 0.01;

        rho += drho;
    }
    if (is_mouse_button_middle_pressed)
    {
        double cursor_y_pos;
        glfwGetCursorPos(window, NULL, &cursor_y_pos);

        double dtheta = (384 - cursor_y_pos) * 0.01;

        const double PI = 3.141592653589793;
        if (theta + dtheta > PI / 2)
            theta = PI / 2;
        else if (theta + dtheta < - PI / 2)
            theta = - PI / 2;
        else
            theta += dtheta;
    }
    glfwSetCursorPos(window, 512, 384);

    vec3 eye = vec3(x_pos, y_pos, z_pos);
    vec3 center = vec3(x_pos + cos(theta) * sin(rho), y_pos + sin(theta), z_pos - cos(theta) * cos(rho));
    vec3 up = vec3(0, 1, 0);

    mat4 view_mat = lookAt(eye, center, up);

    return view_mat;
}

void View::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
    glfwSetCursorPos(window, 512, 384);

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        is_mouse_button_right_pressed = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        is_mouse_button_right_pressed = false;
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
        is_mouse_button_middle_pressed = true;
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
        is_mouse_button_middle_pressed = false;
}