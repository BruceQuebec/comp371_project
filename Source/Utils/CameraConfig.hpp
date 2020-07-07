//
//  CameraConfig.hpp
//  COMP371_Labs
//
//  Created by ZhangDong on 2020-07-06.
//  Copyright © 2020 Concordia. All rights reserved.
//
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#ifndef CameraConfig_h
#define CameraConfig_h
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

glm::vec3 camera_position;
glm::vec3 triangle_scale;
glm::mat4 projection_matrix;


// Constant vectors
glm::vec3 center(0.0f, 0.0f, -1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 eye(0.0f, 0.5f, 3.0f); // 右侧 45 度向左前方观看

//global variables for important functions
//for scaling
float object_size = 1;
//for pacman's position
float position_x = 0;
float position_y = 0;
//pacman's orientation
float orientation = 0;
//for camera
glm::vec3 v_d = center;
glm::vec3 v_p = eye;
float rotateX = 0;
float rotateY = 0;
//for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void cursor_cb(GLFWwindow* window, double xpos, double ypos) {

    int pan = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
    int zoom = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    int tilt = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

    if (zoom == GLFW_PRESS) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        double xdiff = xpos - (width / 2);
        double ydiff = ypos - (height / 2);
        v_p += v_d*(float)ydiff*-0.005f*0.04f;
        glfwSetCursorPos(window, width / 2, height / 2);
    }

    if (pan == GLFW_PRESS) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        double xdiff = xpos - (width / 2);
        double ydiff = ypos - (height / 2);

        glm::vec3 mod_v = glm::vec3(xdiff*0.005, 0, 0);
        glm::vec4 mod_v4 = glm::vec4(mod_v, 1);
        v_d = glm::normalize(v_d + glm::vec3(mod_v4 * 0.05f));
        glfwSetCursorPos(window, width / 2, height / 2);
    }

    if (tilt == GLFW_PRESS) {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        double xdiff = xpos - (width / 2);
        double ydiff = ypos - (height / 2);

        glm::vec3 mod_v = glm::vec3(0, ydiff*-0.005, 0);
        glm::vec4 mod_v4 = glm::vec4(mod_v, 1);
        v_d = glm::normalize(v_d + glm::vec3(mod_v4 * 0.05f));
        glfwSetCursorPos(window, width / 2, height / 2);
    }
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    
    //rotate camera
    if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE) {
        
        rotateX -= 0.5;
    }
    if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE) {

        rotateX += 0.5;
    }
    if (key == GLFW_KEY_UP && action != GLFW_RELEASE) {

        rotateY -= 0.5;
    }
    if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE) {

        rotateY += 0.5;
    }
    //Show line view
    if (key == GLFW_KEY_L && action != GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    //show vertex view
    if (key == GLFW_KEY_P && action != GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
    //show triangle view
    if (key == GLFW_KEY_T && action != GLFW_RELEASE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    //Scale up
    if (key == GLFW_KEY_U && action != GLFW_RELEASE) {
        object_size += 0.1;
    }
    //scale down
    if (key == GLFW_KEY_J && action != GLFW_RELEASE) {
        object_size -= 0.1;
    }
    //resets everything (play again)
    if (key == GLFW_KEY_R && action != GLFW_RELEASE) {
        object_size = 1;
        position_x = 0;
        position_y = 0;
    }
    //move up (also face up, also cannot go past walls)
    if (key == GLFW_KEY_W && action != GLFW_RELEASE) {
        position_y += 0.1;
        if (position_y >= 1.0) position_y = 1.0;
        orientation = 90;
        std::cout << position_x << ", " << position_y << std::endl;
    }
    //move down (also face down, also cannot go past walls)
    if (key == GLFW_KEY_S && action != GLFW_RELEASE) {
        position_y -= 0.1;
        if (position_y <= -1.0) position_y = -1.0;
        orientation = 270;
        std::cout << position_x << ", " << position_y << std::endl;
    }
    //go left (also face left, also cannot go past walls)
    if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
        position_x -= 0.1;
        if (position_x <= -1.0) position_x = -1.0;
        orientation = 180;
        std::cout << position_x << ", " << position_y << std::endl;
    }
    //go right (also face right, also cannot go past walls)
    if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
        position_x += 0.1;
        if (position_x >= 1.0) position_x = 1.0;
        orientation = 0;
        std::cout << position_x << ", " << position_y << std::endl;
    }
    //reset camera
    if (key == GLFW_KEY_HOME && action == GLFW_PRESS) {
        v_d = center;
        v_p = eye;
        rotateY = 0;
        rotateX = 0;
    }
}
#endif /* CameraConfig_h */
