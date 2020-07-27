#define GLEW_STATIC


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "load.hpp"
#include "World.hpp"
#include "Model.hpp"
#include "control.hpp"
#include "Camera.hpp"

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
	
	// Hide the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Enable lock keys
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	// Set up key and mouse button callbacks
	glfwSetKeyCallback(window, Control::key_callback);
	glfwSetMouseButtonCallback(window, Control::mouse_button_callback);
	glfwSetWindowSizeCallback(window, Control::window_size_callback);

	//Initialize shader(s)
	unordered_map<string, GLuint> shader_pointer_idx_map_generic = {
								{"pos_data_idx", 0},
								{"normal_data_idx", 1},
								{"color_data_idx", 2},
								{"texCoords_data_idx", 3} };
	unordered_map<string, GLuint> shader_pointer_idx_map_lightCube = {
								{"pos_data_idx", 0},
								{"normal_data_idx", 1},
								{"color_data_idx", 2}};
	unordered_map<string, GLuint> shader_pointer_idx_map_depthMap_debug = {
								{"pos_data_idx", 0},
								{"texCoords_data_idx", 1} };
	unordered_map<string, GLuint> shader_pointer_idx_map_depthMap = {
								{"pos_data_idx", 0}};

	Shader generic_shader = Shader("source/shaders/generic.vs", "source/shaders/generic.fs", shader_pointer_idx_map_generic);
	Shader lightCube_shader = Shader("source/shaders/lightCube.vs", "source/shaders/lightCube.fs", shader_pointer_idx_map_lightCube);
	Shader debugDepthQuadShader = Shader("source/shaders/debug_quad_depth.vs", "source/shaders/debug_quad_depth.fs", shader_pointer_idx_map_depthMap_debug);
	Shader shadowMappingShader = Shader("source/shaders/shadow_mapping_depth.vs", "source/shaders/shadow_mapping_depth.fs", shader_pointer_idx_map_depthMap);
    
	// Initialize models and load from files
    Model ground = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, 0, 0, 0, "resource/ground.txt", "ground", "resource/textures/tile.jpg");
	Model axes = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_LINES, 0, 0, 0, "resource/axes.txt", "axes","resource/textures/grid-and-axes.jpg");
	
	Model N = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, 0, 0, 0, "resource/N.txt", "N", "resource/textures/box.jpg");
	Model N4 = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, 0, 0, 0, "resource/4.txt", "N4", "resource/textures/metal.jpg");
	Model sphere_N4 = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLE_STRIP, 0, 13, 0, "resource/sphere.txt", "sphere_N4", "");
	sphere_N4.setScale(9);
	
	Model L = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, -40, 0, -40, "resource/L.txt", "L", "resource/textures/box.jpg");
	Model L8 = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, -40, 0, -40, "resource/8.txt", "L8", "resource/textures/metal.jpg");
	Model sphere_L8 = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLE_STRIP, -40, 13, -40, "resource/sphere.txt", "sphere_L8", "");
	sphere_L8.setScale(9);

	Model Z = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, 40, 0, -40, "resource/Z.txt", "Z", "resource/textures/box.jpg");
	Model Z7 = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, 40, 0, -40, "resource/7.txt", "Z7", "resource/textures/metal.jpg");
	Model sphere_Z7 = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLE_STRIP, 40, 13, -40, "resource/sphere.txt", "sphere_Z7", "");
	sphere_Z7.setScale(9);

	Model I = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, -40, 0, 40, "resource/I.txt", "I", "resource/textures/box.jpg");
	Model I4 = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, -40, 0, 40, "resource/4.txt", "I4", "resource/textures/metal.jpg");
	Model sphere_I4 = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLE_STRIP, -40, 13, 40, "resource/sphere.txt", "sphere_I4", "");
	sphere_I4.setScale(9);

	Model E = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, 40, 0, 40, "resource/E.txt", "E", "resource/textures/box.jpg");
	Model E7 = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLES, 40, 0, 40, "resource/7.txt", "E7", "resource/textures/metal.jpg");
	Model sphere_E7 = Model(generic_shader, generic_shader.shader_pointer_idx_map, GL_TRIANGLE_STRIP, 40, 13, 40, "resource/sphere.txt", "sphere_E7", "");
	sphere_E7.setScale(9);
	
	Model lightCube = Model(lightCube_shader, lightCube_shader.shader_pointer_idx_map, GL_TRIANGLES, -2.0f, 30.0f, -2.0f, "resource/lightCube.txt", "lightCube", "");

    // Initialize a camera
    Camera camera;
	Control::setCamera(&camera);

    

    // Set the window to be controlled
    Control::window = window;

    // Set the models to be controlled
	Control::setModel(&N, 1);
	Control::setModel(&N4, 1);
	Control::setModel(&sphere_N4, 1);

	Control::setModel(&L, 2);
	Control::setModel(&L8, 2);
	Control::setModel(&sphere_L8, 2);

	Control::setModel(&Z, 3);
    Control::setModel(&Z7, 3);
	Control::setModel(&sphere_Z7, 3);

	Control::setModel(&I, 4);
    Control::setModel(&I4, 4);
	Control::setModel(&sphere_I4, 4);

	Control::setModel(&E, 5);
    Control::setModel(&E7, 5);
	Control::setModel(&sphere_E7, 5);
	

    // Enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Display pixels with less or equal depth
    glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	unordered_map<string, unsigned int> depthMapIDs = depthMappingSetup(debugDepthQuadShader);
	
    // Displaying loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process events in the event queue
        glfwPollEvents();

        // Draw the models
		glm::mat4 lightSpaceMat = depthMappingPerform(debugDepthQuadShader, shadowMappingShader, Model::models["lightCube"]->getPosition(), *Model::models["ground"],depthMapIDs, World::getWorldMat(), Model::models);
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw(World::getWorldMat(), Model::models, camera.getCameraMat(), camera.getCameraPosition(), depthMapIDs, lightSpaceMat);
		
        // Swap the front and back buffers
        glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}