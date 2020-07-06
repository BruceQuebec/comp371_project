#define GLEW_STATIC 1


#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "load.hpp"
#include "Model.hpp"


using namespace std;
using namespace glm;


mat4 viewMat(double x_pos, double y_pos, double z_pos, double x_angle, double y_angle);

mat4 projectionMat(float fov, float aspect);

mat4 mvpMat(mat4 model_mat, mat4 view_mat, mat4 projection_mat);


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


    // Create a vertex array 创建vertex array
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);


    GLuint pos_buffer;
    glGenBuffers(1, &pos_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, pos_buffer);

    GLdouble pos_data[] = {0, 0, 0,
                           5, 0, 0,
                           0, 0, 0,
                           0, 5, 0,
                           0, 0, 0,
                           0, 0, 5,
                           0, 0, 0,
                           2, 0, 0,
                           0, 2, 0,
                           2, 0, 0,
                           0, 2, 0,
                           2, 2, 0,
                           0, 0, 0,
                           2, 0, 0,
                           0, 0, 2,
                           2, 0, 0,
                           0, 0, 2,
                           2, 0, 2,
                           0, 0, 0,
                           0, 2, 0,
                           0, 0, 2,
                           0, 2, 0,
                           0, 0, 2,
                           0, 2, 2,
                           0, 0, 2,
                           2, 0, 2,
                           0, 2, 2,
                           2, 0, 2,
                           0, 2, 2,
                           2, 2, 2,
                           0, 2, 0,
                           2, 2, 0,
                           0, 2, 2,
                           2, 2, 0,
                           0, 2, 2,
                           2, 2, 2,
                           2, 0, 0,
                           2, 2, 0,
                           2, 0, 2,
                           2, 2, 0,
                           2, 0, 2,
                           2, 2, 2};

    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_data), pos_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (void*)0);


    GLuint color_buffer;
    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);

    GLdouble color_data[] = {1, 0, 0,
                             1, 0, 0,
                             0, 1, 0,
                             0, 1, 0,
                             0, 0, 1,
                             0, 0, 1,
                             0, 0, 0,
                             1, 0, 0,
                             0, 1, 0,
                             1, 0, 0,
                             0, 1, 0,
                             1, 1, 0,
                             0, 0, 0,
                             1, 0, 0,
                             0, 0, 1,
                             1, 0, 0,
                             0, 0, 1,
                             1, 0, 1,
                             0, 0, 0,
                             0, 1, 0,
                             0, 0, 1,
                             0, 1, 0,
                             0, 0, 1,
                             0, 1, 1,
                             0, 0, 1,
                             1, 0, 1,
                             0, 1, 1,
                             1, 0, 1,
                             0, 1, 1,
                             1, 1, 1,
                             0, 1, 0,
                             1, 1, 0,
                             0, 1, 1,
                             1, 1, 0,
                             0, 1, 1,
                             1, 1, 1,
                             1, 0, 0,
                             1, 1, 0,
                             1, 0, 1,
                             1, 1, 0,
                             1, 0, 1,
                             1, 1, 1};

    glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_DOUBLE, GL_FALSE, 0, (void*)0);


    // Create a shader program 创建shader程序
    GLuint shader_program = glCreateProgram();

    // load shaders from files 加载shaders
	loadShader(shader_program, "source/vertex-shader.glsl", GL_VERTEX_SHADER);
	loadShader(shader_program, "source/fragment-shader.glsl", GL_FRAGMENT_SHADER);

    // Use the program 启用shader程序
    glUseProgram(shader_program);


    // Enable the depth test 启用景深测试
    glEnable(GL_DEPTH_TEST);

    // Objects with less depth will be displayed 显示近景的物体
    glDepthFunc(GL_LESS);


    

    glfwSetKeyCallback(window, Model::key_callback_dispatch);

    glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);


    Model::setWindow(window);

    Model::setShaderProgram(shader_program);

    Model::setMVPMatString("mvp_mat");

    Model model1 = Model(0, 6, GL_LINES);
    Model model2 = Model(6, 42, GL_TRIANGLES);
    model1.setInstance(0);
    model2.setInstance(1);
    


    double view_x_pos = 0;
    double view_y_pos = 0;
    double view_z_pos = 0;
    double view_x_angle = 0;
    double view_y_angle = 0;

    double projection_fov = 90;

    mat4 view_mat = viewMat(view_x_pos, view_y_pos, view_z_pos, view_x_angle, view_y_angle);
    mat4 projection_mat = projectionMat(projection_fov, (float)4.0 / 3);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        model1.draw(view_mat, projection_mat);
        model2.draw(view_mat, projection_mat);


        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDisableVertexAttribArray(0);

    glfwTerminate();

    return 0;
}


mat4 viewMat(double x_pos, double y_pos, double z_pos, double x_angle, double y_angle)
{
    mat4 view_mat = lookAt(vec3(2, 4, 4),
                           vec3(0, 0, 0),
                           vec3(0, 1, 0)); // TODO

    return view_mat;
}


mat4 projectionMat(float fov, float aspect)
{
    return perspective(radians(fov), aspect, 0.01f, 100.0f);
}


mat4 mvpMat(mat4 modelMat, mat4 viewMat, mat4 projectionMat)
{
    return projectionMat * viewMat * modelMat;
}