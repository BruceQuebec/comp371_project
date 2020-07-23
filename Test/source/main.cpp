#define GLEW_STATIC


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "load.hpp"
#include "World.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "control.hpp"
#include "draw.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace glm;
using namespace std;


int main()
{
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




    // The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
    GLuint FramebufferName;
    glGenFramebuffers(1, &FramebufferName);
    glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

    // Depth texture. Slower than a depth buffer, but you can sample it later in your shader
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

    //glDrawBuffer(GL_NONE); // No color buffer is drawn to.



    // Initialize models and load from files
    Model ground = Model(GL_TRIANGLES, 0, 0, 0, "resource/ground.txt");
    //Model grid = Model(GL_LINES, 0, 0, 0, "resource/grid.txt");
    //Model axes = Model(GL_LINES, 0, 0, 0, "resource/axes.txt");
    //Model N4 = Model(GL_TRIANGLES, 0, 0, 0, "resource/N4.txt");
    //Model L8 = Model(GL_TRIANGLES, -40, 0, -40, "resource/L8.txt");
    //Model Z7 = Model(GL_TRIANGLES, 40, 0, -40, "resource/Z7.txt");
    //Model I4 = Model(GL_TRIANGLES, -40, 0, 40, "resource/I4.txt");
    //Model E7 = Model(GL_TRIANGLES, 40, 0, 40, "resource/E7.txt");
    


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
    //Control::setModel(&N4, 1);
    //Control::setModel(&L8, 2);
    //Control::setModel(&Z7, 3);
    //Control::setModel(&I4, 4);
    //Control::setModel(&E7, 5);

    Control::setCamera(&camera);




    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLint width, height, nrChannels;
    unsigned char * data = stbi_load("resource/ground.jpg", &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);





    // Create a shader program
    GLuint depth_shader_program = glCreateProgram();

    // load shaders from files
    loadShader(depth_shader_program, GL_VERTEX_SHADER, "source/shaders/DepthRTT.vertexshader");
    loadShader(depth_shader_program, GL_FRAGMENT_SHADER, "source/shaders/DepthRTT.fragmentshader");

    // Enable the shader program
    glUseProgram(depth_shader_program);




    // Create a shader program
    GLuint shader_program = glCreateProgram();

    // load shaders from files
    loadShader(shader_program, GL_VERTEX_SHADER, "source/shaders/vertex-shader.glsl");
    loadShader(shader_program, GL_FRAGMENT_SHADER, "source/shaders/fragment-shader.glsl");

    

    // Get the location of "mvp_mat" variable
    GLuint mvp_mat_location = glGetUniformLocation(shader_program, "mvp_mat");

    GLuint depth_mvp_mat_location = glGetUniformLocation(depth_shader_program, "depth_mvp_mat");

    GLuint shadow_map_location = glGetUniformLocation(shader_program, "shadow_map");

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glUniform1i(shadow_map_location, 1);



    GLuint uv_location = glGetUniformLocation(shader_program, "vertex_uv");

    GLuint texture_location = glGetUniformLocation(shader_program, "texture_sampler");

    

    // Enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Display pixels with less or equal depth
    glDepthFunc(GL_LEQUAL);


    // Displaying loop
    while (!glfwWindowShouldClose(window))
    {
        //glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

        //glViewport(0, 0, 1024, 1024);

        //// Clear the color and depth buffers
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //depth_mvp_mat_location = glGetUniformLocation(depth_shader_program, "depth_mvp_mat");

        //glUseProgram(depth_shader_program);

        //draw(World::getWorldMat(), Model::models, camera.getCameraMat(), mvp_mat_location, depth_mvp_mat_location, shadow_map_location);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(texture_location, 0);

        depth_mvp_mat_location = glGetUniformLocation(shader_program, "depth_mvp_mat");

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Enable the shader program
        glUseProgram(shader_program);

        

        // Draw the models
        draw(World::getWorldMat(), Model::models, camera.getCameraMat(), mvp_mat_location, depth_mvp_mat_location, shadow_map_location);
        
        // Process events in the event queue
        glfwPollEvents();

        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}