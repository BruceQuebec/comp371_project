#include <vector>

#include "World.hpp"
#include "Model.hpp"
#include "Camera.hpp"


#include "control.hpp"


using namespace std;


GLFWwindow * Control::window = nullptr;

bool Control::is_mouse_button_left_pressed = false;
bool Control::is_mouse_button_right_pressed = false;
bool Control::is_mouse_button_middle_pressed = false;

bool Control::view_mode = false;

bool Control::render_with_texture = true;
bool Control::render_shadow = true;

vector<Model *> Control::models[10];
int Control::model_index = 1;

Camera * Control::camera = nullptr;


void Control::setModel(Model * model, int index)
{
    models[index].push_back(model);
}

void Control::setCamera(Camera * camera)
{
    Control::camera = camera;
}

void Control::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    // Press Escape to close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Call the key callback of the World
    World::key_callback(key, action);

    // Press P to display in points, L to display in lines, T to display in triangles
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Press 0 to control model 0, 1 to control model 1, 2 to control model 2...
    if (key == GLFW_KEY_0 && action == GLFW_PRESS)
        model_index = 0;
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        model_index = 1;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        model_index = 2;
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        model_index = 3;
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        model_index = 4;
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
        model_index = 5;
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        model_index = 6;
    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
        model_index = 7;
    if (key == GLFW_KEY_8 && action == GLFW_PRESS)
        model_index = 8;
    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
        model_index = 9;

    // If an index is assigned to models, call the key callback of the models
    if (models[model_index].size() > 0)
    {
        for (vector<Model *>::iterator it = models[model_index].begin(); it != models[model_index].end(); it++)
            (*it)->key_callback(key, action, mods);
    }

    if (key == GLFW_KEY_V && action == GLFW_PRESS)
        view_mode = !view_mode;

    camera->key_callback(key, action, view_mode);

    if (key == GLFW_KEY_B && action == GLFW_PRESS)
        render_shadow = !render_shadow;
}

void Control::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
    // If a mouse button is pressed, change the condition of its label
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        is_mouse_button_left_pressed = true;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        is_mouse_button_left_pressed = false;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        is_mouse_button_right_pressed = true;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        is_mouse_button_right_pressed = false;
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
        is_mouse_button_middle_pressed = true;
    if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
        is_mouse_button_middle_pressed = false;
}

void Control::cursor_pos_callback(GLFWwindow * window, double x_pos, double y_pos)
{
    camera->cursor_pos_callback(window, x_pos, y_pos);
}

void Control::window_size_callback(GLFWwindow * window, int width, int height)
{
    glViewport(0, 0, width, height);

    camera->window_size_callback(width, height);
}

void Control::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
    camera->scroll_callback(yoffset);
}