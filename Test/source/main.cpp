#define GLEW_STATIC 1


#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "load.hpp"


using namespace std;
using namespace glm;


int main()
{
    // Initialize GLFW
    glfwInit();

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);

    GLFWwindow* window = glfwCreateWindow(videoMode->width, videoMode->height, "", monitor, NULL);

    glfwMakeContextCurrent(window);


    glewExperimental = GL_TRUE;

    // Initialize GLEW
    glewInit();

    glClearColor(0, 0, 0, 1);

    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);


    GLfloat vertex_data[] = { 0, 0, 0,
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

    GLfloat color_data[] = { 1, 0, 0,
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

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(color_data), color_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );


    GLuint program = glCreateProgram();

    loadShader(program, "source/vertex-shader.glsl", GL_VERTEX_SHADER);
    loadShader(program, "source/fragment-shader.glsl", GL_FRAGMENT_SHADER);

    glUseProgram(program);


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    float xPos = 0;
    float zPos = 0;
    float scale = 1;

    glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);



        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



        mat4 projectionMat = perspective(radians(90.0f), (float)16.0 / 9, 0.01f, 100.0f);

        mat4 viewMat = lookAt(vec3(2, 4, 4),
            vec3(0, 0, 0),
            vec3(0, 1, 0));

        mat4 modelMat = mat4(1);

        mat4 mvpMat = projectionMat * viewMat * modelMat;

        GLuint mvpMatLocation = glGetUniformLocation(program, "mvpMat");

        glUniformMatrix4fv(mvpMatLocation, 1, GL_FALSE, &mvpMat[0][0]);

        glDrawArrays(GL_LINES, 0, 2);
        glDrawArrays(GL_LINES, 2, 2);
        glDrawArrays(GL_LINES, 4, 2);



        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            xPos += 0.001;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            xPos -= 0.001;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            zPos -= 0.001;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            zPos += 0.001;
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
            scale += 0.001;
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
            scale -= 0.001;

        projectionMat = perspective(radians(90.0f), (float)16.0 / 9, 0.01f, 100.0f);

        viewMat = lookAt(vec3(2, 2, 4),
            vec3(0, 0, 0),
            vec3(0, 1, 0));

        mat4 translateMat = translate(mat4(1.0f), vec3(xPos, 0, zPos));
        mat4 scaleMat = glm::scale(mat4(1.0f), vec3(scale, scale, scale));
        modelMat = translateMat * scaleMat;
        mvpMat = projectionMat * viewMat * modelMat;

        mvpMatLocation = glGetUniformLocation(program, "mvpMat");
        glUniformMatrix4fv(mvpMatLocation, 1, GL_FALSE, &mvpMat[0][0]);

        for (int i = 6; i < 42; i += 3)
            glDrawArrays(GL_TRIANGLES, i, 3);



        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDisableVertexAttribArray(0);

    glfwTerminate();

    return 0;
}