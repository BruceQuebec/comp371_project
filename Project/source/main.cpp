#define GLEW_STATIC


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.hpp"
#include "load.hpp"
#include "World.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "control.hpp"
#include "draw.hpp"
#include "ComplexModel.hpp"
#include "skyCube.hpp"


using namespace glm;
using namespace std;


int main()
{
	// Initialize GLFW
	glfwInit();

	// Set OpenGL version to 3.1
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   
    // Initialize a window
    GLFWwindow * window = glfwCreateWindow(1024, 768, "Project", NULL, NULL);

    // Make the OpenGL or OpenGL ES context of the window current on the calling thread
    glfwMakeContextCurrent(window);


    // Ensures all supported extensions will be exposed from experimental graphics drivers
    glewExperimental = GL_TRUE;

    // Initialize GLEW
    glewInit();

	// Initialize a camera
	Camera camera;
	

	// Hide the cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Enable lock keys
	glfwSetInputMode(window, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

	// Set up key and mouse button callbacks
	glfwSetKeyCallback(window, Control::key_callback);
	glfwSetMouseButtonCallback(window, Control::mouse_button_callback);

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);


	// Set the window to be controlled
	Control::window = window;
	
	//Initialize shader(s)
	unordered_map<string, GLuint> shader_pointer_idx_map_generic_texture = {
								{"pos_data_idx", 0},
								{"normal_data_idx", 1},
								{"texCoords_data_idx", 2} };
	unordered_map<string, GLuint> shader_pointer_idx_map_skyCube = {
								{"pos_data_idx", 0}};
	unordered_map<string, GLuint> shader_pointer_idx_map_lightCube = {
								{"pos_data_idx", 0},
								{"normal_data_idx", 1},
								{"color_data_idx", 2}};

	Shader generic_texture_shader = Shader("source/shaders/generic_texture.vs", "source/shaders/generic_texture.fs", shader_pointer_idx_map_generic_texture);
	Shader lightCube_shader = Shader("source/shaders/lightCube.vs", "source/shaders/lightCube.fs", shader_pointer_idx_map_lightCube);
	Shader skyCube_shader = Shader("source/shaders/skyCube.vs", "source/shaders/skyCube.fs", shader_pointer_idx_map_skyCube);
	
	ComplexModel building1 = ComplexModel("resource/models/Cartoon_House.obj", glm::vec3(10, 0, 10), glm::vec3(0.01, 0.01, 0.01));
	ComplexModel building2 = ComplexModel("resource/models/house.obj", glm::vec3(-20, 0, 10), glm::vec3(0.05, 0.05, 0.05));
	ComplexModel building3 = ComplexModel("resource/models/restaurant.obj", glm::vec3(40, 0, 10), glm::vec3(6, 6, 6));
	
	Model lightCube = Model(lightCube_shader, lightCube_shader.shader_pointer_idx_map, GL_TRIANGLES, 0,25,10, "resource/lightCube.txt", "lightCube", "");
	Model skyCubeModel = Model(skyCube_shader, skyCube_shader.shader_pointer_idx_map, GL_TRIANGLES, 0, 0, 0, "resource/skyCube.txt", "skyCube", "");
	
	vector<std::string> faces
	{
		"resource/textures/skybox/right.jpg",
		"resource/textures/skybox/left.jpg",
		"resource/textures/skybox/top.jpg",
		"resource/textures/skybox/bottom.jpg",
		"resource/textures/skybox/front.jpg",
		"resource/textures/skybox/back.jpg"
	};
	skyCube skyCubeObj = skyCube(skyCubeModel, faces);
	skyCube_shader.setInt("skybox", 0);
	

	
    // Enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Display pixels with less or equal depth
    glDepthFunc(GL_LEQUAL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    // Displaying loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process events in the event queue
        glfwPollEvents();

        // Draw the models
		generic_texture_shader.use();
		//backpack.draw(generic_texture_shader, camera);
		building1.draw(generic_texture_shader, camera);
		building2.draw(generic_texture_shader, camera);
		building3.draw(generic_texture_shader, camera);

        draw(World::getWorldMat(), Model::models, camera.getCameraMat(), camera.getCameraPosition());
		skyCubeObj.drawSky(camera);
		glfwSwapBuffers(window);
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}