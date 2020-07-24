#pragma once


#include <vector>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.hpp"
#include "Shader.hpp"


// Draw the models
void draw(glm::mat4 world_mat, std::vector<Model *> models, glm::mat4 camera_mat, Shader shader, GLuint shadow_map);

// Draw a model
void draw(glm::mat4 world_mat, Model & model, glm::mat4 camera_mat, Shader shader, GLuint shadow_map);