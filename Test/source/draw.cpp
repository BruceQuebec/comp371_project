#include "draw.hpp"
#include "World.hpp"
#include "control.hpp"


using namespace glm;
using namespace std;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

void draw(mat4 world_mat, unordered_map<string, Model *> models, mat4 cameraMat, vec3 cameraPos, unordered_map<string, unsigned int> depthMapIDs, glm::mat4 lightSpaceMatrix)
{
	// Draw each model in the map
	unordered_map<string, Model *>::iterator it = models.begin();
	while (it != models.end()) {
		draw(world_mat, *it->second, cameraMat, cameraPos, it->first, depthMapIDs, lightSpaceMatrix);
		it++;
	}
}

void draw(mat4 world_mat, Model & model, mat4 cameraMat, vec3 cameraPos, string model_name, unordered_map<string, unsigned int> depthMapIDs, glm::mat4 lightSpaceMatrix)
{	
	
	// Process the model with the given matrix
	model.getShader().use();

	// if the models aren't light source model, render them with light computation
	if (model_name != "lightCube") {
		lighting_render(model, cameraPos, model_name);
		if (model.getTextureID() != 0 && Control::render_with_texture) {
			model.getShader().setInt("byColor", 0);
			model.getShader().setInt("material.diffuse", 0);
			//std::cout << "Texture ID is: " << model.getTexture() << std::endl;
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, model.getTextureID());
		}
		else {
			model.getShader().setInt("byColor", 1);
		}
	}
		
	model.getShader().setMat4("model", model.getModelMat());
	model.getShader().setMat4("view_projection", cameraMat);
	model.getShader().setMat4("lightSpaceMatrix", lightSpaceMatrix);

	// Bind the vertex array of the model
	glBindVertexArray(model.getVertexArray());
	glDrawArrays(model.getMode(), 0, model.getNumVertices() / 3);
	
}

void lighting_render(Model model, vec3 cameraPos, string model_name) {
	// update the light and view position in fragment shader
	model.getShader().setVec3("light.position", Model::models["lightCube"]->getPosition());
	model.getShader().setVec3("view_position", cameraPos);

	// point light properties
	glm::vec3 lightColor(1.0, 1.0, 1.0);
	glm::vec3 diffuseColor = lightColor * glm::vec3(7.0f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(2.0f); // low influence
	model.getShader().setVec3("light.ambient", ambientColor);
	model.getShader().setVec3("light.diffuse", diffuseColor);
	model.getShader().setVec3("light.specular", glm::vec3(10.0f, 10.0f, 10.0f));

	// material properties
	model.getShader().setFloat("material.shininess", 64.0f);
	model.getShader().setVec3("material.specular", glm::vec3(5, 5, 5));

	// point light attenuation calculation
	model.getShader().setFloat("light.constant", 1.0f);
	model.getShader().setFloat("light.linear", 0.09f);
	model.getShader().setFloat("light.quadratic", 0.032f);
}

unordered_map<string, unsigned int> depthMappingSetup(Shader debugDepthQuadShader) {
	// configure depth map FBO
	// -----------------------
	unordered_map<string, unsigned int> depthMapIDs;
	
	//unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapIDs["depthMapFBO"]);
	// create depth texture
	//unsigned int depthMap;
	glGenTextures(1, &depthMapIDs["depthMap"]);
	glBindTexture(GL_TEXTURE_2D, depthMapIDs["depthMap"]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapIDs["depthMapFBO"]);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapIDs["depthMap"], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	debugDepthQuadShader.use();
	debugDepthQuadShader.setInt("depthMap", 0);

	
	return depthMapIDs;
}

glm::mat4 depthMappingPerform(Shader debugDepthQuadShader, Shader shadowMappingShader, glm::vec3 lightSourcePos, Model ground, unordered_map<string, unsigned int> depthMapIDs, mat4 world_mat, unordered_map<string, Model *> models) {
	// 1. render depth of scene to texture (from light's perspective)
	   // --------------------------------------------------------------
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 30.0f;
	lightProjection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);
	lightView = glm::lookAt(lightSourcePos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	shadowMappingShader.use();
	shadowMappingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapIDs["depthMapFBO"]);
		glClear(GL_DEPTH_BUFFER_BIT);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, ground.getTextureID());
		renderDepthMapScene(shadowMappingShader, world_mat, models);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// reset viewport
	glViewport(0, 0, 1024, 768);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render Depth map to quad for visual debugging
	// ---------------------------------------------
	debugDepthQuadShader.use();
	debugDepthQuadShader.setFloat("near_plane", near_plane);
	debugDepthQuadShader.setFloat("far_plane", far_plane);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMapIDs["depthMap"]);
	//renderQuad();

	return lightSpaceMatrix;
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void renderDepthMapScene(Shader &depth_mapping_shader, glm::mat4 world_mat, unordered_map<string, Model *> models) {
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	depth_mapping_shader.setMat4("model", world_mat);
	glBindVertexArray(models["ground"]->getVertexArray());
	glDrawArrays(GL_TRIANGLES, 0, models["ground"]->getNumVertices()/3);

	unordered_map<string, Model *>::iterator it = models.begin();
	while (it != models.end()) {
		if (it->first != "axis") {
			//model = glm::mat4(1.0f);
			depth_mapping_shader.setMat4("model", world_mat);
			glBindVertexArray(it->second->getVertexArray());
			glDrawArrays(it->second->getMode(), 0, it->second->getNumVertices() / 3);
			glBindVertexArray(0);
		}
		it++;
	}
}

// renderCube() renders a 1x1 3D cube in NDC.
// -------------------------------------------------
unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[] = {
			// back face
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
			// front face
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
			// left face
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
			// right face
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
			// bottom face
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
			// top face
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
		};
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}