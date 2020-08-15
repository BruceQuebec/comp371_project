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
#include "Shader.hpp"
#include "stb_image.h"
#include "Mesh.hpp"
#include "Camera.hpp"

class ComplexModel {
public:
	ComplexModel(string path, glm::vec3 position, glm::vec3 scale);
	void draw(Shader &shader, Camera& camera);
	void key_callback(int key, int action, int mods);
	void update_meshes(char key, float value);
	vector<Mesh> getMeshes();
private:
	// model data
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;

	void loadModel(string path, glm::vec3 position, glm::vec3 scale);
	void processNode(aiNode *node, const aiScene *scene, glm::vec3 position, glm::vec3 scale);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene, glm::vec3 position, glm::vec3 scale);
	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char *path, const string &directory);
};
#endif
