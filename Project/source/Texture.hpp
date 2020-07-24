#pragma once


#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.hpp"

class Texture {
	public:
	void loadTextImg(string imgFilePath);
};
