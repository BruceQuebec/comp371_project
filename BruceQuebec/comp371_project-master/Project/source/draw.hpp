#pragma once

#include <unordered_map>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.hpp"
#include "Camera.hpp"


// Draw the models
void draw(glm::mat4 world_mat, unordered_map<string, Model *> models, glm::mat4 cameraMat, glm::vec3 cameraPos, unordered_map<string, unsigned int> depthMapIDs, glm::mat4 lightSpaceMatrix);

// Draw a model
void draw(glm::mat4 world_mat, Model & model, glm::mat4 cameraMat, glm::vec3 cameraPos, string model_name, unordered_map<string, unsigned int> depthMapIDs, glm::mat4 lightSpaceMatrix);

void lighting_render(Model model, glm::vec3 cameraPos, string model_name);

unordered_map<string, unsigned int> depthMappingSetup(Shader debugDepthQuadShader);

glm::mat4 depthMappingPerform(Shader debugDepthQuadShader, Shader shadowMappingShader, glm::vec3 lightSourcePos, Model ground, unordered_map<string, unsigned int> depthMapIDs, glm::mat4 world_mat, unordered_map<string, Model *> models);

void renderDepthMapScene(Shader &depth_mapping_shader, glm::mat4 world_mat, unordered_map<string, Model *> models);

void renderQuad();

void renderCube();