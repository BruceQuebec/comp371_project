#define PI 3.141592653589793


#include <GLFW/glfw3.h>

#include "control.hpp"

#include "Camera.hpp"


using namespace glm;


Camera::Camera(vec3 pos, double aspect)
{
    x_pos = pos[0];
    y_pos = pos[1];
    z_pos = pos[2];

    theta = -PI / 4;
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
    mat4 projection_mat = perspective((float)fov, (float)aspect, 0.001f, 1000.0f);

    return projection_mat;
}

vec3 Camera::getCameraPos()
{
    return vec3(x_pos, y_pos, z_pos);
}

void Camera::cursor_pos_callback(GLFWwindow * window, double x_pos, double y_pos)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    if (Control::is_mouse_button_left_pressed)
    {
        double dy = (height / 2 - y_pos) * 0.01;

        if (fov - dy <= 0)
            fov = 0;
        else if (fov - dy >= PI)
            fov = PI;
        else
            fov -= dy;
    }

    // When the right mouse button is pressed, move the mouse to pan the camera
    if (Control::is_mouse_button_right_pressed)
    {
        double drho = (x_pos - width / 2) * 0.01;

        rho += drho;
    }

    // When the middle mouse button is pressed, move the mouse to tilt the camera
    if (Control::is_mouse_button_middle_pressed)
    {
        double dtheta = (height / 2 - y_pos) * 0.01;

        if (theta + dtheta >= PI / 2 - 0.00001)
            theta = PI / 2 - 0.00001;
        else if (theta + dtheta <= -PI / 2 + 0.00001)
            theta = -PI / 2 + 0.00001;
        else
            theta += dtheta;
    }

    glfwSetCursorPos(window, width / 2, height / 2);
}

void Camera::window_size_callback(int width, int height)
{
    aspect = (double)width / height;
}