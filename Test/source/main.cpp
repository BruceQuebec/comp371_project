#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "stb_image.h"

#include "load.hpp"
#include "World.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "control.hpp"
#include "draw.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "Light.hpp"
#include "load.hpp"


#include <fstream>
#include <sstream>
#include <iostream>

using namespace glm;
using namespace std;


int main()
{
    // Set OpenGL version to 3.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


    // Initialize GLFW
    glfwInit();

    GLFWmonitor * monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode * videoMode = glfwGetVideoMode(monitor);

    int width = videoMode->width, height = videoMode->height;

    // Initialize a window
    GLFWwindow * window = glfwCreateWindow(width, height, "Project", monitor, NULL);

    // Make the OpenGL or OpenGL ES context of the window current on the calling thread
    glfwMakeContextCurrent(window);


    // Ensures all supported extensions will be exposed from experimental graphics drivers
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    glewInit();


    GLuint frame_buffer;
    glGenFramebuffers(1, &frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);


    GLuint shadow_map;
    glGenTextures(1, &shadow_map);
    glBindTexture(GL_TEXTURE_2D, shadow_map);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_map, 0);


    Shader texture_shader = Shader("source/shaders/texture.vertexshader", "source/shaders/texture.fragmentshader");

    Shader shadow_shader = Shader("source/shaders/DepthRTT.vertexshader", "source/shaders/DepthRTT.fragmentshader");


    Material polished_silver(vec3(0.23125, 0.23125, 0.23125), vec3(0.2775, 0.2775, 0.2775), vec3(0.773911, 0.773911, 0.773911), 1000);;
    Material box(vec3(0.2, 0.2, 0.2), vec3(0.5, 0.5, 0.5), vec3(0.1, 0.1, 0.1), 1);

    Light light(vec3(0, 30, 0));


    // Initialize models and load from files
    Model ground = Model(GL_TRIANGLES, box, 0, 0, 0, "resource/objects/ground.obj", false, "resource/ground.png");
    Model N4_N = Model(GL_TRIANGLES, box, 0, 0, 0, "resource/objects/N.obj", false, "resource/box-top.png");
    //Model N4_4 = Model(GL_TRIANGLES, polished_silver, 0, 0, 0, "resource/objects/4.obj", false, "resource/silver.png");
    Model L8_L = Model(GL_TRIANGLES, box, -40, 0, -40, "resource/objects/L.obj", false, "resource/box-top.png");
    //Model L8_8 = Model(GL_TRIANGLES, polished_silver, -40, 0, -40, "resource/objects/8.obj", false, "resource/silver.png");
    Model Z7_Z = Model(GL_TRIANGLES, box, 40, 0, -40, "resource/objects/Z.obj", false, "resource/box-top.png");
    //Model Z7_7 = Model(GL_TRIANGLES, polished_silver, 40, 0, -40, "resource/objects/7.obj", false, "resource/silver.png");
    Model I4_I = Model(GL_TRIANGLES, box, -40, 0, 40, "resource/objects/I.obj", false, "resource/box-top.png");
    //Model I4_4 = Model(GL_TRIANGLES, polished_silver, -40, 0, 40, "resource/objects/4.obj", false, "resource/silver.png");
    Model E7_E = Model(GL_TRIANGLES, box, 40, 0, 40, "resource/objects/E.obj", false, "resource/box-top.png");
    //Model E7_7 = Model(GL_TRIANGLES, polished_silver, 40, 0, 40, "resource/objects/7.obj", false, "resource/silver.png");

    // Initialize a camera
    Camera camera(vec3(0, 12, 12), (float)width / height);


    // Hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Enable lock keys
    glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

    glfwSetCursorPos(window, width / 2, height / 2);

    // Set up key and mouse button callbacks
    glfwSetKeyCallback(window, Control::key_callback);
    glfwSetMouseButtonCallback(window, Control::mouse_button_callback);
    glfwSetWindowSizeCallback(window, Control::window_size_callback);
    glfwSetCursorPosCallback(window, Control::cursor_pos_callback);
    glfwSetScrollCallback(window, Control::scroll_callback);

    // Set the window to be controlled
    Control::window = window;

    // Set the models to be controlled
    Control::setModel(&N4_N, 1);
    //Control::setModel(&N4_4, 1);
    Control::setModel(&L8_L, 2);
    //Control::setModel(&L8_8, 2);
    Control::setModel(&Z7_Z, 3);
    //Control::setModel(&Z7_7, 3);
    Control::setModel(&I4_I, 4);
    //Control::setModel(&I4_4, 4);
    Control::setModel(&E7_E, 5);
    //Control::setModel(&E7_7, 5);

    Control::setCamera(&camera);

    

    // Enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Display pixels with less or equal depth
    glDepthFunc(GL_LEQUAL);




    // Displaying loop
    while (!glfwWindowShouldClose(window))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

        draw(2048, 2048, Model::models, light, camera, shadow_shader, NULL);


        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glfwGetWindowSize(window, &width, &height);

        // Draw the models
        draw(width, height, Model::models, light, camera, texture_shader, shadow_map);
        

        // Process events in the event queue
        glfwPollEvents();

        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}