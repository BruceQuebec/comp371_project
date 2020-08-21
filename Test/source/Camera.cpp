#define PI 3.141592653589793


#include <GLFW/glfw3.h>

#include "control.hpp"

#include "Camera.hpp"


using namespace glm;


Camera::Camera(vec3 pos, float aspect)
{
    x_pos = pos[0];
    y_pos = pos[1];
    z_pos = pos[2];

    theta = 0;
    rho = 0;

    fov = PI / 2;

    this->aspect = aspect;
}

mat4 Camera::getViewMat()
{
    vec3 eye = vec3(x_pos, y_pos, z_pos);
    vec3 center = vec3(x_pos + cos(theta) * sin(rho), y_pos + sin(theta), z_pos - cos(theta) * cos(rho));
    vec3 up = vec3(0, 1, 0);
    mat4 view_mat = lookAt(eye, center, up);

    return view_mat;
}

mat4 Camera::getProjectionMat()
{
    mat4 projection_mat = perspective(fov, aspect, 0.1f, 100.0f);

    return projection_mat;
}

vec3 Camera::getCameraPos()
{
    return vec3(x_pos, y_pos, z_pos);
}

void Camera::key_callback(int key, int action)
{
    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
    {
        x_pos += 0.5 * cos(theta) * sin(rho);
        y_pos += 0.5 * sin(theta);
        z_pos -= 0.5 * cos(theta) * cos(rho);
    }
    if (key == GLFW_KEY_S && action == GLFW_REPEAT)
    {
        x_pos -= 0.5 * cos(theta) * sin(rho);
        y_pos -= 0.5 * sin(theta);
        z_pos += 0.5 * cos(theta) * cos(rho);
    }
    if (key == GLFW_KEY_A && action == GLFW_REPEAT)
    {
        x_pos -= 0.5 * cos(rho);
        z_pos -= 0.5 * sin(rho);
    }
    if (key == GLFW_KEY_D && action == GLFW_REPEAT)
    {
        x_pos += 0.5 * cos(rho);
        z_pos += 0.5 * sin(rho);
    }
}

void Camera::cursor_pos_callback(GLFWwindow * window, double x_pos, double y_pos)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double drho = (x_pos - width / 2) * 0.002, dtheta = (height / 2 - y_pos) * 0.002;

    rho += drho;

    if (theta + dtheta >= PI / 2 - 0.00001)
        theta = PI / 2 - 0.00001;
    else if (theta + dtheta <= -PI / 2 + 0.00001)
        theta = -PI / 2 + 0.00001;
    else
        theta += dtheta;

    glfwSetCursorPos(window, width / 2, height / 2);
}

void Camera::window_size_callback(int width, int height)
{
    aspect = (float)width / height;
}

void Camera::scroll_callback(double yoffset)
{
    if (yoffset == 1)
    {
        if (fov - 0.1 <= 0)
            fov = 0;
        else
            fov -= 0.1;
    }
    if (yoffset == -1)
    {
        if (fov + 0.1 >= pi<double>())
            fov = pi<double>();
        else
            fov += 0.1;
    }
}