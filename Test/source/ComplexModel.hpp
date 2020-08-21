#pragma once
#ifndef COMPLEXMODEL_H
#define COMPLEXMODEL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader_v2.hpp"
#include "stb_image.h"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Box.hpp"


class ComplexModel {
public:
	ComplexModel(string path, glm::vec3 position, glm::vec3 scale);
	void draw(int width, int height, Shader_v2 &shader, Camera& camera, Light& light, GLuint shadow_map, bool render_shadow);
	vector<Mesh> getMeshes();
	Box getBoundaryBox();
	
	
private:
	// model data
	vector<Mesh> meshes;
	string directory;
	vector<mTexture> textures_loaded;
	float boundary_box[6] = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN };
	Box mBoundary_box;
	

	void loadModel(string path, glm::vec3 position, glm::vec3 scale);
	void processNode(aiNode *node, const aiScene *scene, glm::vec3 position, glm::vec3 scale);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene, glm::vec3 position, glm::vec3 scale);
	vector<mTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char *path, const string &directory);
	
};
#endif
