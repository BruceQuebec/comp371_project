#pragma once


#include <vector>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.hpp"


// Draw the models
void draw(glm::mat4 world_mat, std::vector<Model *> models, glm::mat4 camera_mat, GLuint mvp_mat_location, GLuint depth_mvp_mat_location, GLuint shadow_map_location);

// Draw a model
void draw(glm::mat4 world_mat, Model & model, glm::mat4 camera_mat, GLuint mvp_mat_location, GLuint depth_mvp_mat_location, GLuint shadow_map_location);