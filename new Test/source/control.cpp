#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "control.hpp"

#include "Model.hpp"
#include "World.hpp"


GLFWwindow * Control::window = nullptr;

bool Control::is_mouse_button_left_pressed = false;
bool Control::is_mouse_button_right_pressed = false;
bool Control::is_mouse_button_middle_pressed = false;

Model * Control::models[10];
int Control::model_index = 1;


void Control::setModel(Model * model, int index)
{
    models[index] = model;
}

void Control::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    World::key_callback(key, action);

    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    if (key == GLFW_KEY_L && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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

    if (models[model_index])
        models[model_index]->key_callback(key, action, mods);
}

void Control::mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
{
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