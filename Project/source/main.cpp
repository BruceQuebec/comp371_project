#define GLEW_STATIC


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "load.hpp"
#include "World.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "control.hpp"
#include "draw.hpp"


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


    // Initialize models and load from files
    Model grid = Model(GL_LINES, 0, 0, 0, "resource/grid.txt");
    Model axes = Model(GL_LINES, 0, 0, 0, "resource/axes.txt");
    Model N4 = Model(GL_TRIANGLES, 0, 0, 0, "resource/N4.txt");
    Model L8 = Model(GL_TRIANGLES, -40, 0, -40, "resource/L8.txt");
    Model Z7 = Model(GL_TRIANGLES, 40, 0, -40, "resource/Z7.txt");
    Model I4 = Model(GL_TRIANGLES, -40, 0, 40, "resource/I4.txt");
    Model E7 = Model(GL_TRIANGLES, 40, 0, 40, "resource/E7.txt");


    // Initialize a camera
    Camera camera;


    // Hide the cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Enable lock keys
    glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

    // Set up key and mouse button callbacks
    glfwSetKeyCallback(window, Control::key_callback);
    glfwSetMouseButtonCallback(window, Control::mouse_button_callback);

    // Set the window to be controlled
    Control::window = window;

    // Set the models to be controlled
    Control::setModel(&N4, 1);
    Control::setModel(&L8, 2);
    Control::setModel(&Z7, 3);
    Control::setModel(&I4, 4);
    Control::setModel(&E7, 5);


    // Create a shader program
    GLuint shader_program = glCreateProgram();

    // load shaders from files
    loadShader(shader_program, GL_VERTEX_SHADER, "source/shaders/vertex-shader.glsl");
    loadShader(shader_program, GL_FRAGMENT_SHADER, "source/shaders/fragment-shader.glsl");

    // Enable the shader program
    glUseProgram(shader_program);

    // Get the location of "mvp_mat" variable
    GLuint mvp_mat_location = glGetUniformLocation(shader_program, "mvp_mat");


    // Enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Display pixels with less or equal depth
    glDepthFunc(GL_LEQUAL);


    // Displaying loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process events in the event queue
        glfwPollEvents();

        // Draw the models
        draw(World::getWorldMat(), Model::models, camera.getCameraMat(), mvp_mat_location);

        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}