#pragma once


#include <vector>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.hpp"
#include "Light.hpp"
#include "Camera.hpp"
#include "Shader.hpp"


// Draw all the models
void draw(int width, int height, std::vector<Model *> models, Light & light, Camera & camera, Shader shader, GLuint shadow_map);

// Draw a model
void draw(Model & model, Light & light, Camera & camera, Shader shader, GLuint shadow_map);