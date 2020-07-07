#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "View.hpp"


using namespace std;
using namespace glm;


View * View::instance;

void View::key_callback_dispatch(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	instance->key_callback(window, key, scancode, action, mods);
}


View::View()
{
    x_pos = 2;
    y_pos = 2;
    z_pos = 4;

    x_angle = 0;
    y_angle = 0;
    z_angle = 0;

    instance = this;
}


void View::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
        z_pos -= 0.01;
    if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
        z_pos += 0.01;
    if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
        x_pos -= 0.01;
    if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
        x_pos += 0.01;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        x_pos = 2;
        y_pos = 2;
        z_pos = 4;
    }
}

mat4 View::viewMat()
{
    vec3 eye = vec3(x_pos, y_pos, z_pos);
    vec3 center = vec3(x_pos, y_pos, z_pos - 1);
    vec3 up = vec3(0, 1, 0);

    mat4 view_mat = lookAt(eye, center, up);

    return view_mat;
}