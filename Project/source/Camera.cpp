#define PI 3.141592653589793


#include <vector>

#include <GLFW/glfw3.h>

#include "control.hpp"

#include "Camera.hpp"

#include <iostream>


using namespace std;
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

    box = new Box(-0.5 + x_pos, 0.5 + x_pos, -1.5 + y_pos, 0.5 + y_pos, -0.5 + z_pos, 0.5 + z_pos);

    move_x  = true;
    move_x_ = true;
    move_y  = true;
    move_y_ = true;
    move_z  = true;
    move_z_ = true;
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
    mat4 projection_mat = perspective(fov, aspect, 0.1f, 200.0f);

    return projection_mat;
}

vec3 Camera::getCameraPos()
{
    return vec3(x_pos, y_pos, z_pos);
}

void Camera::key_callback(int key, int action)
{
    for (Box * i : Box::boxes)
    {
        int collide = box->detectCollision(*i);

        if (collide == 1)
            move_x = false;
        else if (collide == 2)
            move_x_ = false;
        else if (collide == 3)
            move_y = false;
        else if (collide == 4)
            move_y_ = false;
        else if (collide == 5)
            move_z = false;
        else if (collide == 6)
            move_z_ = false;
    }

    float dx = 0, dy = 0, dz = 0;

    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
    {
        dx = 0.2 * cos(theta) * sin(rho);
        dy = 0.2 * sin(theta);
        dz = -0.2 * cos(theta) * cos(rho);
    }
    else if (key == GLFW_KEY_S && action == GLFW_REPEAT)
    {
        dx = -0.2 * cos(theta) * sin(rho);
        dy = -0.2 * sin(theta);
        dz = 0.2 * cos(theta) * cos(rho);
    }
    if (key == GLFW_KEY_A && action == GLFW_REPEAT)
    {
        dx = -0.2 * cos(rho);
        dz = -0.2 * sin(rho);
    }
    if (key == GLFW_KEY_D && action == GLFW_REPEAT)
    {
        dx = 0.2 * cos(rho);
        dz = 0.2 * sin(rho);
    }

    if (dx < 0)
        dx = move_x ? dx : 0;
    else if (dx > 0)
        dx = move_x_ ? dx : 0;
    if (dy < 0)
        dy = move_y ? dy : 0;
    else if (dy > 0)
        dy = move_y_ ? dy : 0;
    if (dz < 0)
        dz = move_z ? dz : 0;
    else if (dz > 0)
        dz = move_z_ ? dz : 0;

    x_pos += dx;
    box->x_bound += dx;
    box->x_bound_ += dx;
    y_pos += dy;
    box->y_bound += dy;
    box->y_bound_ += dy;
    z_pos += dz;
    box->z_bound += dz;
    box->z_bound_ += dz;

    move_x = true;
    move_x_ = true;
    move_y = true;
    move_y_ = true;
    move_z = true;
    move_z_ = true;
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
        if (fov - PI / 12 <= PI / 6)
            fov = PI / 6;
        else
            fov -= PI / 12;
    }
    if (yoffset == -1)
    {
        if (fov + PI / 12 >= PI * 2 / 3)
            fov = PI * 2 / 3;
        else
            fov += PI / 12;
    }
}