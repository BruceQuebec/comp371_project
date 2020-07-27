#include "draw.hpp"
#include "World.hpp"
#include "control.hpp"


using namespace glm;
using namespace std;

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

void draw(mat4 world_mat, unordered_map<string, Model *> models, mat4 cameraMat, vec3 cameraPos, unordered_map<string, unsigned int> depthMapIDs, float far_plane)
{
	// Draw each model in the map
	unordered_map<string, Model *>::iterator it = models.begin();
	while (it != models.end()) {
		draw(world_mat, *it->second, cameraMat, cameraPos, it->first, depthMapIDs, far_plane);
		it++;
	}
}

void draw(mat4 world_mat, Model & model, mat4 cameraMat, vec3 cameraPos, string model_name, unordered_map<string, unsigned int> depthMapIDs, float far_plane) {

	model.getShader().use();
	if (model_name != "lightCube") {
		lighting_render(model, cameraPos, model_name, far_plane);
		if (model.getTextureID() != 0 && Control::render_with_texture) {
			model.getShader().setInt("byColor", 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, model.getTextureID());
		}
		else {
			model.getShader().setInt("byColor", 1);
		}
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapIDs["depthCubeMap"]);
	}
		
	model.getShader().setMat4("model", model.getModelMat());
	model.getShader().setMat4("view_projection", cameraMat);

	// Bind the vertex array of the model
	glBindVertexArray(model.getVertexArray());
	glDrawArrays(model.getMode(), 0, model.getNumVertices() / 3);
	
}

void lighting_render(Model model, vec3 cameraPos, string model_name, float far_plane) {
	// update the light and view position in fragment shader
	model.getShader().setVec3("light.position", Model::models["lightCube"]->getPosition());
	model.getShader().setVec3("view_position", cameraPos);
	model.getShader().setFloat("far_plane", far_plane);

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
	model.getShader().setFloat("light.linear", 0.07f);
	model.getShader().setFloat("light.quadratic", 0.017f);
}

unordered_map<string, unsigned int> depthMappingSetup() {
	// configure depth map FBO
	unordered_map<string, unsigned int> depthMapIDs;
	
	glGenFramebuffers(1, &depthMapIDs["depthMapFBO"]);
	// create depth texture
	glGenTextures(1, &depthMapIDs["depthCubeMap"]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthMapIDs["depthCubeMap"]);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapIDs["depthMapFBO"]);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMapIDs["depthCubeMap"], 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return depthMapIDs;
}

float depthMappingPerform(Shader shadowMappingShader, glm::vec3 lightSourcePos, unordered_map<string, unsigned int> depthMapIDs, mat4 world_mat, unordered_map<string, Model *> models) {
	// 1. render scene to depth cubemap
	float near_plane = 1.0f, far_plane = 2500.0f;
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightSourcePos, lightSourcePos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightSourcePos, lightSourcePos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightSourcePos, lightSourcePos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightSourcePos, lightSourcePos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightSourcePos, lightSourcePos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(lightSourcePos, lightSourcePos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
	
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapIDs["depthMapFBO"]);
	glClear(GL_DEPTH_BUFFER_BIT);
	shadowMappingShader.use();
	for (unsigned int i = 0; i < 6; ++i) {
		shadowMappingShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}
	shadowMappingShader.setFloat("far_plane", far_plane);
	shadowMappingShader.setVec3("lightPos", lightSourcePos);
	renderDepthMapScene(shadowMappingShader, world_mat, models);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	// reset viewport
	glViewport(0, 0, 1024, 768);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return far_plane;
}

void renderDepthMapScene(Shader &depth_mapping_shader, glm::mat4 world_mat, unordered_map<string, Model *> models) {
	unordered_map<string, Model *>::iterator it = models.begin();
	while (it != models.end()) {
		depth_mapping_shader.setMat4("model", world_mat);
		glBindVertexArray(it->second->getVertexArray());
		glDrawArrays(it->second->getMode(), 0, it->second->getNumVertices() / 3);
		glBindVertexArray(0);
		it++;
	}
}