//
// COMP 371 Labs Framework
//
// Created by Nicolas Bergeron on 20/06/2019.
//
// Inspired by the following tutorials:
// - https://learnopengl.com/Getting-started/Hello-Window
// - https://learnopengl.com/Getting-started/Hello-Triangle

#include <iostream>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // GLFW provides a cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include "grid.hpp"


// Constant vectors
const glm::vec3 center(0.0f, 0.0f, -1.0f);
const glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 eye(0.0f, 0.0f, 1.9f);

//for camera 
glm::vec3 v_d = center;
glm::vec3 v_p = eye;
float rotateX = 0;
float rotateY = 0;

glm::vec3 camera_position;
glm::vec3 triangle_scale;
glm::mat4 projection_matrix;

const char* getVertexShaderSource()
{
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 position;"
		"layout (location = 1) in vec3 vertexColor;"
		"out vec3 fragmentColor;"
		""
		"uniform mat4 model_matrix;"
        "uniform mat4 view_matrix;"
		"uniform mat4 projection_matrix;"

        "void main()"
        "{"
        "   gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position.x, position.y, position.z, 1.0);"
        "	fragmentColor = vertexColor;"
		"}";
}


const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
		"in vec3 fragmentColor;"
        "out vec4 color;"
		"uniform int fill;"
		""
        "void main()"
        "{"
        "   if (fill == 0) {"
		"		color = vec4(1.0f, 1.0f, 0.0f, 1.0f);"
		"	}"
		"   if (fill == 1) {"
		"		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);"
		"	}"
		"   if (fill == 2) {"
		"		color = vec4(1.0f, 0.0f, 0.0f, 1.0f);"
		"	}"
		"   if (fill == 3) {"
		"		color = vec4(0.0f, 1.0f, 0.0f, 1.0f);"
		"	}"
		"	color = vec4(fragmentColor.r, fragmentColor.g, fragmentColor.b, 1.0f);"
        "}";
}


int compileAndLinkShaders()
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

void cursor_cb(GLFWwindow* window, double xpos, double ypos) {

	int pan = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	int zoom = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int tilt = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE);

	if (zoom == GLFW_PRESS) {
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		double xdiff = xpos - (width / 2);
		double ydiff = ypos - (height / 2);
		v_p += v_d * (float)ydiff*-0.005f*0.04f;
		glfwSetCursorPos(window, width / 2, height / 2);
	}

	if (pan == GLFW_PRESS) {
		std::cout << "pan triggered!" << std::endl;
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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
	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, cursor_cb);

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
	
	//Capture mouse with glfw
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport(0, 0, width, height);

	projection_matrix = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.0f, 100.0f);


	//grid VAO
	GLuint grid_VAO, grid_VBO;
	glGenVertexArrays(1, &grid_VAO);
	glGenBuffers(1, &grid_VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(grid_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, grid_VBO);
	glBufferData(GL_ARRAY_BUFFER, grid_vertices.size() * sizeof(glm::vec3), &grid_vertices.front(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//axis VAO
	GLuint axis_VAO, axis_VBO;
	glGenVertexArrays(1, &axis_VAO);
	glGenBuffers(1, &axis_VBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(axis_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, axis_VBO);
	glBufferData(GL_ARRAY_BUFFER, axis_vertices.size() * sizeof(glm::vec3), &axis_vertices.front(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//axis color VBO
	GLuint axis_color_VBO;
	glGenBuffers(1, &axis_color_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, axis_color_VBO);
	glBufferData(GL_ARRAY_BUFFER, axis_colors.size()*sizeof(glm::vec3), &axis_colors.front(), GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, axis_color_VBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	

    // Black background
	glClearColor(0.1, 0.39, 0.88, 1.0);
	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	
	
    // Compile and link shaders here ...
    //int shaderProgram = compileAndLinkShaders();
    
    float angle = 30;

	//used to fill color
	//GLuint fillLoc = glGetUniformLocation(shaderProgram, "fill");
	
	//uniform declarations
	/*GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection_matrix");
	GLuint viewMatrixLoc = glGetUniformLocation(shaderProgram, "view_matrix");
	GLuint transformLoc = glGetUniformLocation(shaderProgram, "model_matrix");*/

	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 5);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	glTranslatef(0, 0, -5);
	glRotatef(20, 1, 0, 0);
	
	
	
    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
		
        // Each frame, reset color of each pixel to glClearColor
		glClear(GL_COLOR_BUFFER_BIT);
		glm::mat4 model_matrix;
		//model_matrix = glm::scale(model_matrix, triangle_scale);
		
		

		//draw grid
		//glUniform1i(fillLoc, 1);
		glBindVertexArray(grid_VAO);
		model_matrix = glm::mat4(1.0f);
		const float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(view));

		glDrawArrays(GL_LINES, 0, grid_vertices.size());
		glBindVertexArray(0);

		//draw axis
		//glUniform1i(fillLoc, 1);
		glBindVertexArray(axis_VAO);
		glDrawArrays(GL_LINES, 0, axis_vertices.size());
		glBindVertexArray(0);

		
        // End Frame
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        
    }
    
    // Shutdown GLFW
    glfwTerminate();
    
    return 0;
}
