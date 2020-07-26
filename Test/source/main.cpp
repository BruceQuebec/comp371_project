#define GLEW_STATIC
#define STB_IMAGE_IMPLEMENTATION


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"

#include "load.hpp"
#include "World.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "control.hpp"
#include "draw.hpp"
#include "Shader.hpp"


using namespace glm;
using namespace std;


int main()
{
    // Set OpenGL version to 3.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);


    // Initialize GLFW
    glfwInit();

    // Initialize a window
    GLFWwindow * window = glfwCreateWindow(1024, 768, "Project", NULL, NULL);

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_map, 0);

    glDrawBuffer(GL_NONE);


    Shader texture_shader = Shader("source/shaders/vertex-shader.glsl", "source/shaders/fragment-shader.glsl");

    Shader color_shader = Shader("source/shaders/color.vertexshader", "source/shaders/color.fragmentshader");

    Shader shadow_shader = Shader("source/shaders/DepthRTT.vertexshader", "source/shaders/DepthRTT.fragmentshader");



    // Initialize models and load from files
    Model ground = Model(GL_TRIANGLES, 0, 0, 0, "resource/ground.txt", "resource/tile.jpg");
    //Model grid = Model(GL_LINES, 0, 0, 0, "resource/grid.txt");
    Model axes = Model(GL_LINES, 0, 0, 0, "resource/axes.txt", "resource/grid-and-axes.jpg");
    Model N4_N = Model(GL_TRIANGLES, 0, 0, 0, "resource/N.txt", "resource/box.jpg");
    Model N4_4 = Model(GL_TRIANGLES, 0, 0, 0, "resource/4.txt", "resource/metal.jpg");
    Model L8_L = Model(GL_TRIANGLES, -40, 0, -40, "resource/L.txt", "resource/box.jpg");
    Model L8_8 = Model(GL_TRIANGLES, -40, 0, -40, "resource/8.txt", "resource/metal.jpg");
    Model Z7_Z = Model(GL_TRIANGLES, 40, 0, -40, "resource/Z.txt", "resource/box.jpg");
    Model Z7_7 = Model(GL_TRIANGLES, 40, 0, -40, "resource/7.txt", "resource/metal.jpg");
    Model I4_I = Model(GL_TRIANGLES, -40, 0, 40, "resource/I.txt", "resource/box.jpg");
    Model I4_4 = Model(GL_TRIANGLES, -40, 0, 40, "resource/4.txt", "resource/metal.jpg");
    Model E7_E = Model(GL_TRIANGLES, 40, 0, 40, "resource/E.txt", "resource/box.jpg");
    Model E7_7 = Model(GL_TRIANGLES, 40, 0, 40, "resource/7.txt", "resource/metal.jpg");

    // Initialize a camera
    Camera camera;


    // Hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Enable lock keys
    glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

    // Set up key and mouse button callbacks
    glfwSetKeyCallback(window, Control::key_callback);
    glfwSetMouseButtonCallback(window, Control::mouse_button_callback);
    glfwSetWindowSizeCallback(window, Control::window_size_callback);

    // Set the window to be controlled
    Control::window = window;

    // Set the models to be controlled
    Control::setModel(&N4_N, 1);
    Control::setModel(&N4_4, 1);
    Control::setModel(&L8_L, 2);
    Control::setModel(&L8_8, 2);
    Control::setModel(&Z7_Z, 3);
    Control::setModel(&Z7_7, 3);
    Control::setModel(&I4_I, 4);
    Control::setModel(&I4_4, 4);
    Control::setModel(&E7_E, 5);
    Control::setModel(&E7_7, 5);

    Control::setCamera(&camera);

    

    // Enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Display pixels with less or equal depth
    glDepthFunc(GL_LEQUAL);


    // Displaying loop
    while (!glfwWindowShouldClose(window))
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);

        draw(World::getWorldMat(), Model::models, camera.getCameraMat(), shadow_shader, NULL);


        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Draw the models
        if (Control::render_with_texture)
            draw(World::getWorldMat(), Model::models, camera.getCameraMat(), texture_shader, shadow_map);
        else
            draw(World::getWorldMat(), Model::models, camera.getCameraMat(), color_shader, shadow_map);
        

        // Process events in the event queue
        glfwPollEvents();

        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}