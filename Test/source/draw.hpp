#pragma once


#include "Model.hpp"


void draw(glm::mat4 world_mat, std::vector<Model *> models, glm::mat4 camera_mat, GLuint mvp_mat_location);

void draw(glm::mat4 world_mat, Model & model, glm::mat4 camera_mat, GLuint mvp_mat_location);