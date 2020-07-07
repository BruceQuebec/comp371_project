//
// COMP 371 Assignment1
//
//

#include <iostream>
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include "grid.hpp"
#include "Utils/Shader.hpp"
#include "./Utils/ModelMeta.hpp"

// Constant vectors
const glm::vec3 center(0.0f, 0.0f, -1.0f);
const glm::vec3 up(0.0f, 1.0f, 0.0f);
const glm::vec3 eye(0.0f, 0.0f, 0.8f);

//for camera 
glm::vec3 v_direction = center;
glm::vec3 v_position = eye;
float rotateX = 0;
float rotateY = 0;

glm::mat4 model_matrix = glm::mat4(1.0f);
glm::mat4 view_matrix = glm::mat4(1.0f);
glm::mat4 projection_matrix = glm::mat4(1.0f);

void cursor_control(GLFWwindow* window, double xpos, double ypos) {

	int pan = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	int zoom = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int tilt = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

	if (zoom == GLFW_PRESS) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		double xdiff = xpos - (width / 2);
		double ydiff = ypos - (height / 2);
		v_position += v_direction * (float)ydiff*-0.005f*0.04f;
		glfwSetCursorPos(window, width / 2, height / 2);
	}

	if (pan == GLFW_PRESS) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		double xdiff = xpos - (width / 2);
		double ydiff = ypos - (height / 2);

		glm::vec3 mod_v = glm::vec3(xdiff*0.005, 0, 0);
		glm::vec4 mod_v4 = glm::vec4(mod_v, 1);
		v_direction = glm::normalize(v_direction + glm::vec3(mod_v4 * 0.05f));
		glfwSetCursorPos(window, width / 2, height / 2);
	}

	if (tilt == GLFW_PRESS) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		double xdiff = xpos - (width / 2);
		double ydiff = ypos - (height / 2);

		glm::vec3 mod_v = glm::vec3(0, ydiff*-0.005, 0);
		glm::vec4 mod_v4 = glm::vec4(mod_v, 1);
		v_direction = glm::normalize(v_direction + glm::vec3(mod_v4 * 0.05f));
		glfwSetCursorPos(window, width / 2, height / 2);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	//update projection matrix to new width and height
	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);
}

int main(int argc, char*argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();
    
#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(800, 600, "Comp371 - A1", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
	
	// Define the viewport dimensions
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	
	

	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, cursor_control);

	

	// create and compilation of shaders
	Shader generalShaderObj = Shader("../Source/Shaders/general.vertexshader", "../Source/Shaders/general.fragmentshader");

	// prepare VAO VBO for grid
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	GLuint grid_VAO, grid_VBO;
	glGenVertexArrays(1, &grid_VAO);
	glBindVertexArray(grid_VAO);
	glGenBuffers(1, &grid_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, grid_VBO);
	glBufferData(GL_ARRAY_BUFFER, grid_vertices.size() * sizeof(glm::vec3), &grid_vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	// prepare VAO VBO for axis
	GLuint axis_VAO = createVertexArrayObjectForAxis();
	
	// prepare VAO VBO for main models
	GLuint N4_VAO = createVertexArrayObjectForN4();
	GLuint Z7_VAO = createVertexArrayObjectForZ7();
	GLuint L8_VAO = createVertexArrayObjectForL8();
	GLuint I4_VAO = createVertexArrayObjectForI4();
	GLuint E7_VAO = createVertexArrayObjectForE7();

    // Black background
	
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
	

    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
		

        // Each frame, reset color of each pixel to glClearColor
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		generalShaderObj.use();
		
		//draw grid
		glBindVertexArray(grid_VAO);
		model_matrix = glm::mat4(1.0f);
		view_matrix = glm::mat4(1.0f);
		projection_matrix = glm::mat4(1.0f);
		projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		
		generalShaderObj.setMat4("projection_matrix", projection_matrix);
		view_matrix = glm::lookAt(v_position, v_direction, up);
		generalShaderObj.setMat4("view_matrix", view_matrix);
		model_matrix = glm::rotate(model_matrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		generalShaderObj.setMat4("model_matrix", model_matrix);
		generalShaderObj.setVec3("customizedColoir", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawArrays(GL_LINES, 0, grid_vertices.size());
		generalShaderObj.setVec3("customizedColoir", glm::vec3(0.0f, 0.0f, 0.0f));
		glBindVertexArray(0);

		//draw axis
		glBindVertexArray(axis_VAO);
		model_matrix = glm::mat4(1.0f);
		view_matrix = glm::mat4(1.0f);
		projection_matrix = glm::mat4(1.0f);
		projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		generalShaderObj.setMat4("projection_matrix", projection_matrix);
		view_matrix = glm::lookAt(v_position, v_direction, up);
		generalShaderObj.setMat4("view_matrix", view_matrix);
		model_matrix = glm::scale(model_matrix, glm::vec3(0.1, 0.1, 0.1));
		model_matrix = glm::rotate(model_matrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		generalShaderObj.setMat4("model_matrix", model_matrix);
		glDrawArrays(GL_LINES, 0, grid_vertices.size());
		glBindVertexArray(0);

		//draw N4_VAO
		glBindVertexArray(N4_VAO);
		model_matrix = glm::mat4(1.0f);
		view_matrix = glm::mat4(1.0f);
		projection_matrix = glm::mat4(1.0f);
		projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		generalShaderObj.setMat4("projection_matrix", projection_matrix);
		view_matrix = glm::lookAt(v_position, v_direction, up);
		generalShaderObj.setMat4("view_matrix", view_matrix);
		model_matrix = glm::scale(model_matrix, glm::vec3(0.15f, 0.15f, 0.15f));
		model_matrix = glm::translate(model_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		generalShaderObj.setMat4("model_matrix", model_matrix);
		glDrawElements(GL_TRIANGLES, sizeof(n4_vertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//draw Z7_VAO
		glBindVertexArray(Z7_VAO);
		model_matrix = glm::mat4(1.0f);
		view_matrix = glm::mat4(1.0f);
		projection_matrix = glm::mat4(1.0f);
		projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		generalShaderObj.setMat4("projection_matrix", projection_matrix);
		view_matrix = glm::lookAt(v_position, v_direction, up);
		generalShaderObj.setMat4("view_matrix", view_matrix);
		model_matrix = glm::scale(model_matrix, glm::vec3(0.15f, 0.15f, 0.15f));
		model_matrix = glm::translate(model_matrix, glm::vec3(1.5f, 1.2f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		generalShaderObj.setMat4("model_matrix", model_matrix);
		glDrawElements(GL_TRIANGLES, sizeof(z7_vertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//draw L8_VAO
		glBindVertexArray(L8_VAO);
		model_matrix = glm::mat4(1.0f);
		view_matrix = glm::mat4(1.0f);
		projection_matrix = glm::mat4(1.0f);
		projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		generalShaderObj.setMat4("projection_matrix", projection_matrix);
		view_matrix = glm::lookAt(v_position, v_direction, up);
		generalShaderObj.setMat4("view_matrix", view_matrix);
		model_matrix = glm::scale(model_matrix, glm::vec3(0.15f, 0.15f, 0.15f));
		model_matrix = glm::translate(model_matrix, glm::vec3(-1.5f, 1.2f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		generalShaderObj.setMat4("model_matrix", model_matrix);
		glDrawElements(GL_TRIANGLES, sizeof(L8_vertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//draw I4_VAO
		glBindVertexArray(I4_VAO);
		model_matrix = glm::mat4(1.0f);
		view_matrix = glm::mat4(1.0f);
		projection_matrix = glm::mat4(1.0f);
		projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		generalShaderObj.setMat4("projection_matrix", projection_matrix);
		view_matrix = glm::lookAt(v_position, v_direction, up);
		generalShaderObj.setMat4("view_matrix", view_matrix);
		model_matrix = glm::scale(model_matrix, glm::vec3(0.15f, 0.15f, 0.15f));
		model_matrix = glm::translate(model_matrix, glm::vec3(-2.1f, -1.7f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		generalShaderObj.setMat4("model_matrix", model_matrix);
		glDrawElements(GL_TRIANGLES, sizeof(I4_vertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//draw E7_VAO
		glBindVertexArray(E7_VAO);
		model_matrix = glm::mat4(1.0f);
		view_matrix = glm::mat4(1.0f);
		projection_matrix = glm::mat4(1.0f);
		projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
		generalShaderObj.setMat4("projection_matrix", projection_matrix);
		view_matrix = glm::lookAt(v_position, v_direction, up);
		generalShaderObj.setMat4("view_matrix", view_matrix);
		model_matrix = glm::scale(model_matrix, glm::vec3(0.15f, 0.15f, 0.15f));
		model_matrix = glm::translate(model_matrix, glm::vec3(1.5f, -1.7f, 0.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		generalShaderObj.setMat4("model_matrix", model_matrix);
		glDrawElements(GL_TRIANGLES, sizeof(E7_vertices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
    }
    
    // Shutdown GLFW
    glfwTerminate();
    
    return 0;
}
