#pragma once

#include <unordered_map>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Model.hpp"
#include "Camera.hpp"


// Draw the models
void draw(glm::mat4 world_mat, unordered_map<string, Model *> models, glm::mat4 cameraMat, glm::vec3 cameraPos);

// Draw a model
void draw(glm::mat4 world_mat, Model & model, glm::mat4 cameraMat, glm::vec3 cameraPos, string model_name);

void lighting_render(Model model, glm::vec3 cameraPos, string model_name);