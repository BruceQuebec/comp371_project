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
#include "Box.hpp"


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

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_map, 0);


    Shader texture_shader = Shader("source/shaders/texture.vertexshader", "source/shaders/texture.fragmentshader");

    Shader shadow_shader = Shader("source/shaders/DepthRTT.vertexshader", "source/shaders/DepthRTT.fragmentshader");


    Material polished_silver(vec3(0.23125, 0.23125, 0.23125), vec3(0.2775, 0.2775, 0.2775), vec3(0.773911, 0.773911, 0.773911), 1000);;
    Material rough(vec3(0.2, 0.2, 0.2), vec3(0.5, 0.5, 0.5), vec3(0.1, 0.1, 0.1), 1);

    Light light(vec3(0, 30, 0));

    //Model N4_N = Model(GL_TRIANGLES, rough, 0, 0, 0, "resource/objects/N.obj", false, "resource/box-top.png");
    Model L8_L = Model(GL_TRIANGLES, polished_silver, -10, 0, -40, vec3(1), "resource/objects/L.obj", false, "resource/silver.png");
    Model Z7_Z = Model(GL_TRIANGLES, polished_silver, -10, 0, 40, vec3(1), "resource/objects/Z.obj", false, "resource/silver.png");
    Model I4_I = Model(GL_TRIANGLES, rough, 10, 0, -40, vec3(1), "resource/objects/I.obj", false, "resource/box-top.png");
    //Model I4_4 = Model(GL_TRIANGLES, polished_silver, -40, 0, 40, "resource/objects/4.obj", false, "resource/silver.png");
    Model E7_E = Model(GL_TRIANGLES, rough, 10, 0, 40, vec3(1), "resource/objects/E.obj", false, "resource/box-top.png");
    //Model E7_7 = Model(GL_TRIANGLES, polished_silver, 40, 0, 40, "resource/objects/7.obj", false, "resource/silver.png");
    Model _4 = Model(GL_TRIANGLES, polished_silver, -100, 0, -5, vec3(1), "resource/objects/4.obj", false, "resource/silver.png");
    Model _7 = Model(GL_TRIANGLES, polished_silver, 100, 0, 0, vec3(1), "resource/objects/7.obj", false, "resource/silver.png");
    Model _8 = Model(GL_TRIANGLES, rough, -100, 0, 5, vec3(1), "resource/objects/8.obj", false, "resource/box-top.png");



    // Initialize models and load from files
    Model ground = Model(GL_TRIANGLES, rough, 0, 0, 0, vec3(1), "resource/objects/ground.obj", false, "resource/ground.png");
    Model bark1 = Model(GL_TRIANGLES, rough, 13, 0, 13, vec3(2), "resource/objects/bark.obj", false, "resource/bark.jpg");
    Model bark2 = Model(GL_TRIANGLES, rough, 13, 0, -13, vec3(2), "resource/objects/bark.obj", false, "resource/bark.jpg");
    Model bark3 = Model(GL_TRIANGLES, rough, -13, 0, 13, vec3(2), "resource/objects/bark.obj", false, "resource/bark.jpg");
    Model bark4 = Model(GL_TRIANGLES, rough, -13, 0, -13, vec3(2), "resource/objects/bark.obj", false, "resource/bark.jpg");
    Model leaves1 = Model(GL_TRIANGLES, rough, 13, 0, 13, vec3(2), "resource/objects/leaves.obj", false, "resource/leaves.png");
    Model leaves2 = Model(GL_TRIANGLES, rough, 13, 0, -13, vec3(2), "resource/objects/leaves.obj", false, "resource/leaves.png");
    Model leaves3 = Model(GL_TRIANGLES, rough, -13, 0, 13, vec3(2), "resource/objects/leaves.obj", false, "resource/leaves.png");
    Model leaves4 = Model(GL_TRIANGLES, rough, -13, 0, -13, vec3(2), "resource/objects/leaves.obj", false, "resource/leaves.png");
    //Model building = Model(GL_TRIANGLES, rough, -10, 0, 0, vec3(2), "resource/objects/BuildingA.obj", false, "resource/bark.jpg");

    Model Building1 = Model(GL_TRIANGLES, rough, -40, 0, -30, vec3(2), "resource/objects/building1.txt", true, "resource/buildings1.jpg");
    Model Building2 = Model(GL_TRIANGLES, rough, 40, 0, -30, vec3(2), "resource/objects/building2.txt", true, "resource/buildings2.jpg");
    Model Building3 = Model(GL_TRIANGLES, rough, -40, 0, 30, vec3(2), "resource/objects/building1.txt", true, "resource/buildings1.jpg");
    Model Building4 = Model(GL_TRIANGLES, rough, 50, 0, 40, vec3(2), "resource/objects/building3.txt", true, "resource/buildings3.jpg");

    Model Building5 = Model(GL_TRIANGLES, rough, -100, 0.5, 30, vec3(2), "resource/objects/building1.txt", true, "resource/buildings1.jpg");
    Model Building6 = Model(GL_TRIANGLES, rough, 120, 0.5, 40, vec3(2), "resource/objects/building3.txt", true, "resource/buildings3.jpg");
    Model Building7 = Model(GL_TRIANGLES, rough, -100, 0, -30, vec3(2), "resource/objects/building1.txt", true, "resource/buildings1.jpg");
    Model Building8 = Model(GL_TRIANGLES, rough, 100, 0, -30, vec3(2), "resource/objects/building2.txt", true, "resource/buildings2.jpg");

    Model Cross_Road_1 = Model(GL_TRIANGLES, rough, 0, 0.11, 0, vec3(2), "resource/objects/cross_road.txt", true, "resource/cross_road.png");
    Model Road_H_1 = Model(GL_TRIANGLES, rough, 0, 0.1, 0, vec3(2), "resource/objects/road_h.txt", true, "resource/road_h.png");
    Model Road_V_1 = Model(GL_TRIANGLES, rough, 0, 0.1, 0, vec3(2), "resource/objects/road_v.txt", true, "resource/road_v.png");

    Model Cross_Road_2 = Model(GL_TRIANGLES, rough, -200, 0.11, 0, vec3(2), "resource/objects/cross_road.txt", true, "resource/cross_road.png");
    Model Road_H_2 = Model(GL_TRIANGLES, rough, -200, 0.1, 0, vec3(2), "resource/objects/road_h.txt", true, "resource/road_h.png");
    Model Road_V_2 = Model(GL_TRIANGLES, rough, -200, 0.1, 0, vec3(2), "resource/objects/road_v.txt", true, "resource/road_v.png");

    Model Cross_Road_3 = Model(GL_TRIANGLES, rough, 200, 0.11, 0, vec3(2), "resource/objects/cross_road.txt", true, "resource/cross_road.png");
    Model Road_H_3 = Model(GL_TRIANGLES, rough, 200, 0.1, 0, vec3(2), "resource/objects/road_h.txt", true, "resource/road_h.png");
    Model Road_V_3 = Model(GL_TRIANGLES, rough, 200, 0.1, 0, vec3(2), "resource/objects/road_v.txt", true, "resource/road_v.png");

    Model Cross_Road_4 = Model(GL_TRIANGLES, rough, 0, 0.11, -200, vec3(2), "resource/objects/cross_road.txt", true, "resource/cross_road.png");
    Model Road_H_4 = Model(GL_TRIANGLES, rough, 0, 0.1, -200, vec3(2), "resource/objects/road_h.txt", true, "resource/road_h.png");
    Model Road_V_4 = Model(GL_TRIANGLES, rough, 0, 0.1, -200, vec3(2), "resource/objects/road_v.txt", true, "resource/road_v.png");

    Model Cross_Road_5 = Model(GL_TRIANGLES, rough, 0, 0.11, 200, vec3(2), "resource/objects/cross_road.txt", true, "resource/cross_road.png");
    Model Road_H_5 = Model(GL_TRIANGLES, rough, 0, 0.1, 200, vec3(2), "resource/objects/road_h.txt", true, "resource/road_h.png");
    Model Road_V_5 = Model(GL_TRIANGLES, rough, 0, 0.1, 200, vec3(2), "resource/objects/road_v.txt", true, "resource/road_v.png");

    Model Cross_Road_6 = Model(GL_TRIANGLES, rough, 200, 0.11, -200, vec3(2), "resource/objects/cross_road.txt", true, "resource/cross_road.png");
    Model Road_H_6 = Model(GL_TRIANGLES, rough, 200, 0.1, -200, vec3(2), "resource/objects/road_h.txt", true, "resource/road_h.png");
    Model Road_V_6 = Model(GL_TRIANGLES, rough, 200, 0.1, -200, vec3(2), "resource/objects/road_v.txt", true, "resource/road_v.png");

    Model Cross_Road_7 = Model(GL_TRIANGLES, rough, 200, 0.11, 200, vec3(2), "resource/objects/cross_road.txt", true, "resource/cross_road.png");
    Model Road_H_7 = Model(GL_TRIANGLES, rough, 200, 0.1, 200, vec3(2), "resource/objects/road_h.txt", true, "resource/road_h.png");
    Model Road_V_7 = Model(GL_TRIANGLES, rough, 200, 0.1, 200, vec3(2), "resource/objects/road_v.txt", true, "resource/road_v.png");

    Model Cross_Road_8 = Model(GL_TRIANGLES, rough, -200, 0.11, 200, vec3(2), "resource/objects/cross_road.txt", true, "resource/cross_road.png");
    Model Road_H_8 = Model(GL_TRIANGLES, rough, -200, 0.1, 200, vec3(2), "resource/objects/road_h.txt", true, "resource/road_h.png");
    Model Road_V_8 = Model(GL_TRIANGLES, rough, -200, 0.1, 200, vec3(2), "resource/objects/road_v.txt", true, "resource/road_v.png");

    Model Cross_Road_9 = Model(GL_TRIANGLES, rough, -200, 0.11, -200, vec3(2), "resource/objects/cross_road.txt", true, "resource/cross_road.png");
    Model Road_H_9 = Model(GL_TRIANGLES, rough, -200, 0.1, -200, vec3(2), "resource/objects/road_h.txt", true, "resource/road_h.png");
    Model Road_V_9 = Model(GL_TRIANGLES, rough, -200, 0.1, -200, vec3(2), "resource/objects/road_v.txt", true, "resource/road_v.png");

    Box box_ground(-500, 500, -0.1, 0, -500, 500);
    Box::boxes.push_back(&box_ground);
    Box box_L(-14, -6, 0, 12, -41, -39);
    Box::boxes.push_back(&box_L);
    Box box_Z(-14, -6, 0, 12, 39, 41);
    Box::boxes.push_back(&box_Z);
    Box box_I(6, 14, 0, 12, -41, -39);
    Box::boxes.push_back(&box_I);
    Box box_E(6, 14, 0, 12, 39, 41);
    Box::boxes.push_back(&box_E);
    Box box_4(-104, -96, 0, 12, -6, -4);
    Box::boxes.push_back(&box_4);
    Box box_7(96, 104, 0, 12, -1, 1);
    Box::boxes.push_back(&box_7);
    Box box_8(-104, -96, 0, 12, 4, 6);
    Box::boxes.push_back(&box_8);


    Box box_Building1(-60, -20, 0, 50, -50, -10);
    Box::boxes.push_back(&box_Building1);
    Box box_Building2(20, 60, 0, 30, -60, -10);
    Box::boxes.push_back(&box_Building2);
    Box box_Building3(-60, -20, 0, 50, 10, 50);
    Box::boxes.push_back(&box_Building3);
    Box box_Building4(20, 80, 0, 50, 10, 70);
    Box::boxes.push_back(&box_Building4);
    Box box_Building5(-120, -80, 0, 50, -50, -10);
    Box::boxes.push_back(&box_Building5);
    Box box_Building6(80, 120, 0, 30, -60, -10);
    Box::boxes.push_back(&box_Building6);
    Box box_Building7(-120, -80, 0, 50, 10, 50);
    Box::boxes.push_back(&box_Building7);
    Box box_Building8(90, 150, 0, 50, 10, 70);
    Box::boxes.push_back(&box_Building8);



    // Initialize a camera
    Camera camera(vec3(0, 1.5, 12), (float)width / height);


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
    //Control::setModel(&N4_N, 1);
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
        if (light.getLightPos()[1] > 50)
            glClearColor(135.0 / 255, 206.0 / 255, 235.0 / 255, 0);
        else if (light.getLightPos()[1] < -50)
            glClearColor(0, 0, 0, 0);
        else
        {
            float y = light.getLightPos()[1];
            float ratio = (y + 50) / 100;
            glClearColor(135.0 / 255 * ratio, 206.0 / 255 * ratio, 235.0 / 255 * ratio, 0);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

        draw(4096, 4096, Model::models, light, camera, shadow_shader, NULL);


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