#define GLEW_STATIC 1


#include <iostream>
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


vector<GLdouble> pos_data0 = {0, 0, 0,
                              5, 0, 0,
                              0, 0, 0,
                              0, 5, 0,
                              0, 0, 0,
                              0, 0, 5};

vector<GLdouble> color_data0 = {1, 0, 0,
                                1, 0, 0,
                                0, 1, 0,
                                0, 1, 0,
                                0, 0, 1,
                                0, 0, 1};

vector<GLuint> index_data0 = {0, 1,
                              2, 3,
                              4, 5};


vector<GLdouble> pos_data1 = {-2, -2, -2,
                              -2, -2,  2,
                              -2,  2, -2,
                              -2,  2,  2,
                               2, -2, -2,
                               2, -2,  2,
                               2,  2, -2,
                               2,  2,  2};

vector<GLdouble> color_data1 = {0.5, 0.5, 0.5,
                                0.5, 0.5, 0.5,
                                0.5, 0.5, 0.5,
                                0.5, 0.5, 0.5,
                                0.5, 0.5, 0.5,
                                0.5, 0.5, 0.5,
                                0.5, 0.5, 0.5,
                                0.5, 0.5, 0.5};

vector<GLuint> index_data1 = {0, 1, 2,
                              1, 2, 3,
                              0, 1, 4,
                              1, 4, 5,
                              0, 2, 4,
                              2, 4, 6,
                              1, 3, 5,
                              3, 5, 7,
                              2, 3, 6,
                              3, 6, 7,
                              4, 5, 6,
                              5, 6, 7};


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
	loadShader(shader_program, "source/vertex-shader.glsl", GL_VERTEX_SHADER);
	loadShader(shader_program, "source/fragment-shader.glsl", GL_FRAGMENT_SHADER);

    // Use the program 启用shader程序
    glUseProgram(shader_program);

    GLuint mvp_mat_location = glGetUniformLocation(shader_program, "mvp_mat");


    Control::window = window;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

    glfwSetKeyCallback(window, Control::key_callback);
    glfwSetMouseButtonCallback(window, Control::mouse_button_callback);


    Model model0 = Model(pos_data0, color_data0, index_data0, GL_LINES, 0, 0, 0);
    Control::setModel(&model0, 0);

    Model model1 = Model(pos_data1, color_data1, index_data1, GL_TRIANGLES, 0, 2, 0);
    Control::setModel(&model1, 1);


    Camera camera;


    // Enable the depth test 启用景深测试
    glEnable(GL_DEPTH_TEST);

    // Objects with less depth will be displayed 显示近景的物体
    glDepthFunc(GL_LESS);


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