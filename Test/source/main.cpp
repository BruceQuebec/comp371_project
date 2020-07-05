#define GLEW_STATIC 1


#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "load.hpp"


using namespace std;
using namespace glm;


mat4 modelMat(double x_pos, double y_pos, double z_pos, double scale, double x_angle, double y_angle, double z_angle);

mat4 viewMat(double x_pos, double y_pos, double z_pos, double x_angle, double y_angle);

mat4 projectionMat(double projection_fov, double aspect);

mat4 mvpMat(mat4 modelMat, mat4 viewMat, mat4 projectionMat);


int main()
{
    // Initialize GLFW 初始化GLFW
    glfwInit();

    // Get the primary monitor of the device 获取设备的主监视器（显示屏）
    GLFWmonitor * monitor = glfwGetPrimaryMonitor();

    // Get the mode of the monitor 获取监视器的参数
    const GLFWvidmode * video_mode = glfwGetVideoMode(monitor);

    // Create a window 创建窗口
    GLFWwindow * window = glfwCreateWindow(video_mode->width, video_mode->height, "", monitor, NULL); //宽和高为监视器的宽和高，显示模式为全屏

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
    GLuint program = glCreateProgram();

    // load shaders from files 加载shaders
    loadShader(program, "source/vertex-shader.glsl", GL_VERTEX_SHADER);
    loadShader(program, "source/fragment-shader.glsl", GL_FRAGMENT_SHADER);

    // Use the program 启用shader程序
    glUseProgram(program);


    // Enable the depth test 启用景深测试
    glEnable(GL_DEPTH_TEST);

    // Objects with less depth will be displayed 显示近景的物体
    glDepthFunc(GL_LESS);


    double model_x_pos = 0;
    double model_y_pos = 0;
    double model_z_pos = 0;
    double model_x_angle = 0;
    double model_y_angle = 0;
    double model_z_angle = 0;
    double model_scale = 1;

    double view_x_pos = 0;
    double view_y_pos = 0;
    double view_z_pos = 0;
    double view_x_angle = 0;
    double view_y_angle = 0;

    double projection_fov = 90;

    mat4 model_mat = modelMat(model_x_pos, model_y_pos, model_z_pos, model_x_angle, model_y_angle, model_z_angle, model_scale);
    mat4 view_mat = viewMat(view_x_pos, view_y_pos, view_z_pos, view_x_angle, view_y_angle);
    mat4 projection_mat = projectionMat(projection_fov, video_mode->width / video_mode->height);

    mat4 mvp_mat = mvpMat(model_mat, view_mat, projection_mat);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        mat4 projectionMat = perspective(radians(90.0f), (float)16.0 / 9, 0.01f, 100.0f);

        mat4 viewMat = lookAt(vec3(2, 4, 4),
            vec3(0, 0, 0),
            vec3(0, 1, 0));

        mat4 modelmat = mat4(1);

        mat4 mvp = mvpMat(modelmat, viewMat, projectionMat);

        GLuint mvpMatLocation = glGetUniformLocation(program, "mvpMat");
        glUniformMatrix4fv(mvpMatLocation, 1, GL_FALSE, &mvp[0][0]);

        glDrawArrays(GL_LINES, 0, 2);
        glDrawArrays(GL_LINES, 2, 2);
        glDrawArrays(GL_LINES, 4, 2);



        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            model_x_pos += 0.001;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            model_x_pos -= 0.001;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            model_z_pos -= 0.001;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            model_z_pos += 0.001;
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
            model_scale += 0.001;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
            model_scale -= 0.001;


        projectionMat = perspective(radians(90.0f), (float)16.0 / 9, 0.01f, 100.0f);

        viewMat = lookAt(vec3(2, 4, 4),
            vec3(0, 0, 0),
            vec3(0, 1, 0));

        mat4 translateMat = translate(mat4(1), vec3(model_x_pos, 0, model_z_pos));
        mat4 scaleMat = glm::scale(mat4(1), vec3(model_scale, model_scale, model_scale));
        modelmat = translateMat * scaleMat;
        mvp = mvpMat(modelmat, viewMat, projectionMat);

        mvpMatLocation = glGetUniformLocation(program, "mvpMat");
        glUniformMatrix4fv(mvpMatLocation, 1, GL_FALSE, &mvp[0][0]);

        for (int i = 6; i < 42; i += 3)
            glDrawArrays(GL_TRIANGLES, i, 3);


        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDisableVertexAttribArray(0);

    glfwTerminate();

    return 0;
}


mat4 modelMat(double x_pos, double y_pos, double z_pos, double scale, double x_angle, double y_angle, double z_angle)
{
    mat4 translate_mat = translate(mat4(1), vec3(x_pos, y_pos, z_pos));

    mat4 scale_mat = glm::scale(mat4(1), vec3(scale, scale, scale));

    mat4 rotate_mat = mat4(1); // TODO

    return rotate_mat * scale_mat * translate_mat;
}


mat4 viewMat(double x_pos, double y_pos, double z_pos, double x_angle, double y_angle)
{
    mat4 view_mat = lookAt(vec3(2, 4, 4),
                           vec3(0, 0, 0),
                           vec3(0, 1, 0)); // TODO

    return view_mat;
}


mat4 projectionMat(double projection_fov, double aspect)
{
    return perspective(radians((float)projection_fov), (float)aspect, 0.01f, 100.0f);
}


mat4 mvpMat(mat4 modelMat, mat4 viewMat, mat4 projectionMat)
{
    return projectionMat * viewMat * modelMat;
}