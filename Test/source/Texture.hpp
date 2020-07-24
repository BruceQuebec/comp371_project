#pragma once


#include <unordered_map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.hpp"

#include "stb_image.h"

class Texture {
	public:
		Texture(string imgFilePath);
		unsigned char * loadTextImg(string imgFilePath);
		void init(string imgFilePath);
		unsigned int getTextureID();

	private:
		unsigned int textureID;
		int mipmap_level = 0;
	
};