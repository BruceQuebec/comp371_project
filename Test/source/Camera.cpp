#define PI 3.141592653589793


#include <GLFW/glfw3.h>

#include "control.hpp"

#include "Camera.hpp"


using namespace glm;


Camera::Camera()
{
    x_pos = 0;
    y_pos = 10;
    z_pos = 10;

    theta = -PI / 4;
    rho = 0;

    fov = PI / 2;

    aspect = 4.0 / 3;
}

mat4 Camera::getCameraMat()
{
    // Get the width and height of the window
    int window_width, window_height;
    glfwGetWindowSize(Control::window, &window_width, &window_height);
    
    // Get the position of the cursor
    double cursor_x_pos, cursor_y_pos;
    glfwGetCursorPos(Control::window, &cursor_x_pos, &cursor_y_pos);

    // When the left mouse button is pressed, move the mouse to zoom in or out
    if (Control::is_mouse_button_left_pressed)
    {
        double dy = (window_height / 2 - cursor_y_pos) * 0.01;

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
        double drho = (cursor_x_pos - window_width / 2) * 0.01;

        rho += drho;
    }

    // When the middle mouse button is pressed, move the mouse to tilt the camera
    if (Control::is_mouse_button_middle_pressed)
    {
        double dtheta = (window_height / 2 - cursor_y_pos) * 0.01;

        if (theta + dtheta >= PI / 2 - 0.00001)
            theta = PI / 2 - 0.00001;
        else if (theta + dtheta <= -PI / 2 + 0.00001)
            theta = -PI / 2 + 0.00001;
        else
            theta += dtheta;
    }

    // Reset the position of the cursor to the center of the window
    glfwSetCursorPos(Control::window, window_width / 2, window_height / 2);

    vec3 eye = vec3(x_pos, y_pos, z_pos);
    vec3 center = vec3(x_pos + cos(theta) * sin(rho), y_pos + sin(theta), z_pos - cos(theta) * cos(rho));
    vec3 up = vec3(0, 1, 0);
    mat4 view_mat = lookAt(eye, center, up);

    mat4 projection_mat = perspective((float)fov, (float)aspect, 0.001f, 1000.0f);

    mat4 camera_mat = projection_mat * view_mat;

    return camera_mat;
}

void Camera::window_size_callback(int width, int height)
{
    aspect = (double)width / height;
}