#define GLEW_STATIC 1


#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <fstream>

#include "load.hpp"


using namespace std;
using namespace glm;


mat4 modelMat(double x_pos, double y_pos, double z_pos, double scale, double x_angle, double y_angle, double z_angle);

mat4 viewMat(double x_pos, double y_pos, double z_pos, double x_angle, double y_angle);

mat4 projectionMat(float fov, float aspect);

mat4 mvpMat(mat4 model_mat, mat4 view_mat, mat4 projection_mat);



// Constant vectors
const glm::vec3 center(0, 0, -1);
const glm::vec3 camera_up(0, 1, 0);
glm::vec3 eye(2, 4, 4);

//vec3(2, 4, 4),
//vec3(0, 0, 0),
//vec3(0, 1, 0))

//for camera 
glm::vec3 camera_front = center;
glm::vec3 camera_position = eye;





int main()
{
    // Initialize GLFW 初始化GLFW
    glfwInit();

    // Get the primary monitor of the device 获取设备的主监视器（显示屏）
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    // Get the mode of the monitor 获取监视器的参数
    const GLFWvidmode* video_mode = glfwGetVideoMode(monitor);

    // Create a window 创建窗口
    GLFWwindow* window = glfwCreateWindow(1024, 768, "", NULL, NULL); //宽和高为监视器的宽和高，显示模式为全屏

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

    GLdouble pos_data[] = { 0, 0, 0,
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
                           2, 2, 2 };

    glBufferData(GL_ARRAY_BUFFER, sizeof(pos_data), pos_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_DOUBLE, GL_FALSE, 0, (void*)0);


    GLuint color_buffer;
    glGenBuffers(1, &color_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);

    GLdouble color_data[] = { 1, 0, 0,
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
                             1, 1, 1 };

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


    double grid_x_pos = 0;
    double grid_y_pos = 0;
    double grid_z_pos = 0;
    double grid_scale = 1;
    double grid_x_angle = 0;
    double grid_y_angle = 0;
    double grid_z_angle = 0;

    double model_x_pos = 0;
    double model_y_pos = 0;
    double model_z_pos = 0;
    double model_scale = 1;
    double model_x_angle = 0;
    double model_y_angle = 0;
    double model_z_angle = 0;

    double view_x_pos = 0;
    double view_y_pos = 0;
    double view_z_pos = 0;
    double view_x_angle = 0;
    double view_y_angle = 0;

    double projection_fov = 90;

    mat4 model_mat = modelMat(grid_x_pos, grid_y_pos, grid_z_pos, grid_scale, grid_x_angle, grid_y_angle, grid_z_angle);
    mat4 view_mat = viewMat(view_x_pos, view_y_pos, view_z_pos, view_x_angle, view_y_angle);
    mat4 projection_mat = projectionMat(projection_fov, (float)4.0 / 3);

    mat4 mvp_mat = mvpMat(model_mat, view_mat, projection_mat);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        model_mat = modelMat(grid_x_pos, grid_y_pos, grid_z_pos, grid_scale, grid_x_angle, grid_y_angle, grid_z_angle);

        mvp_mat = mvpMat(model_mat, view_mat, projection_mat);

        GLuint mvpMatLocation = glGetUniformLocation(shader_program, "mvp_mat");
        glUniformMatrix4fv(mvpMatLocation, 1, GL_FALSE, &mvp_mat[0][0]);

        for (int i = 0; i < 6; i += 2)
            glDrawArrays(GL_LINES, i, 2);


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


        // camera control: move up/down/left/right
        const float camera_speed = 0.05f;

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            camera_position += camera_speed * camera_front;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            camera_position -= camera_speed * camera_front;
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
            camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
            camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
        }
        // camera position reset
        if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
            camera_position = eye;
        }



        // move the camera 
        view_mat = glm::lookAt(camera_position, camera_position + camera_front, camera_up);


        model_mat = modelMat(model_x_pos, model_y_pos, model_z_pos, model_scale, model_x_angle, model_y_angle, model_z_angle);

        mvp_mat = mvpMat(model_mat, view_mat, projection_mat);

        mvpMatLocation = glGetUniformLocation(shader_program, "mvp_mat");
        glUniformMatrix4fv(mvpMatLocation, 1, GL_FALSE, &mvp_mat[0][0]);

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


mat4 projectionMat(float fov, float aspect)
{
    return perspective(radians(fov), aspect, 0.01f, 100.0f);
}


mat4 mvpMat(mat4 modelMat, mat4 viewMat, mat4 projectionMat)
{
    return projectionMat * viewMat * modelMat;
}



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

    // camera control: up/down/left/right
    const float camera_speed = 0.05f;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera_position += camera_speed * camera_front;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera_position -= camera_speed * camera_front;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera_position -= glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera_position += glm::normalize(glm::cross(camera_front, camera_up)) * camera_speed;
    }


    // object control: 


}
