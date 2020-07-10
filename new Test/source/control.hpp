#pragma once


#include "Model.hpp"


class Control
{
public:
    static GLFWwindow * window;

    static bool is_mouse_button_left_pressed;
    static bool is_mouse_button_right_pressed;
    static bool is_mouse_button_middle_pressed;

    static Model * models[];
    static int model_index;


    static void setModel(Model * model, int index);

    static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

    static void mouse_button_callback(GLFWwindow * window, int button, int action, int mods);
};