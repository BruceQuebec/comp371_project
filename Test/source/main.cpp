#define GLEW_STATIC 1


#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "control.hpp"
#include "load.hpp"
#include "World.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "draw.hpp"


using namespace std;
using namespace glm;


int main()
{
    // Initialize GLFW 初始化GLFW
    glfwInit();

    // Get the primary monitor of the device 获取设备的主监视器（显示屏）
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();

    // Get the mode of the monitor 获取监视器的参数
    const GLFWvidmode * video_mode = glfwGetVideoMode(monitor);

    // Create a window 创建窗口
    GLFWwindow * window = glfwCreateWindow(1024, 768, "", NULL, NULL); //宽和高为监视器的宽和高，显示模式为窗口

    // Make the context of the window current on the calling thread
    glfwMakeContextCurrent(window);


    // Enable all supported extensions if from experimental graphics drivers 确保开启包括实验版本图形驱动的所有扩展
    glewExperimental = GL_TRUE;

    // Initialize GLEW 初始化GLEW
    glewInit();


    // Create a shader program 创建shader程序
    GLuint shader_program = glCreateProgram();

    // load shaders from files 加载shaders
	loadShader(shader_program, GL_VERTEX_SHADER, "source/vertex-shader.glsl");
	loadShader(shader_program, GL_FRAGMENT_SHADER, "source/fragment-shader.glsl");

    // Use the program 启用shader程序
    glUseProgram(shader_program);

    GLuint mvp_mat_location = glGetUniformLocation(shader_program, "mvp_mat");


    Control::window = window;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

    glfwSetKeyCallback(window, Control::key_callback);
    glfwSetMouseButtonCallback(window, Control::mouse_button_callback);


    Model grid = Model(GL_LINES, 0, 0, 0, "resource/grid.txt");
    Control::setModel(&grid, 9);

    Model axes = Model(GL_LINES, 0, 0, 0, "resource/axes.txt");
    Control::setModel(&axes, 0);

    Model N4 = Model(GL_TRIANGLES, 0, 0, 0, "resource/N4.txt");
    Control::setModel(&N4, 1);

    Model L8 = Model(GL_TRIANGLES, -40, 0, -40, "resource/L8.txt");
    Control::setModel(&L8, 2);

    Model Z7 = Model(GL_TRIANGLES, 40, 0, -40, "resource/Z7.txt");
    Control::setModel(&Z7, 3);

    Model I4 = Model(GL_TRIANGLES, -40, 0, 40, "resource/I4.txt");
    Control::setModel(&I4, 4);

    Model E7 = Model(GL_TRIANGLES, 40, 0, 40, "resource/E7.txt");
    Control::setModel(&E7, 5);


    Camera camera;


    // Enable the depth test 启用景深测试
    glEnable(GL_DEPTH_TEST);

    // Objects with less depth will be displayed 显示近景的物体
    glDepthFunc(GL_LEQUAL);

    //glEnable(GL_CULL_FACE);


    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();

        draw(World::getWorldMat(), Model::models, camera.getCameraMat(), mvp_mat_location);

        glfwSwapBuffers(window);
    }

    glDisableVertexAttribArray(0);

    glfwTerminate();

    return 0;
}