#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.hpp"
#include "Camera.hpp"

using namespace std;

struct Vertex {
	// position
	glm::vec3 Position;
	// normal
	glm::vec3 Normal;
	// texCoords
	glm::vec2 TexCoords;
	// tangent
	glm::vec3 Tangent;
	// bitangent
	glm::vec3 Bitangent;
};

struct Material {
	glm::vec4 Ka;
	glm::vec4 Kd;
	glm::vec4 Ks;
	float shininess;
};

struct Texture {
	unsigned int textureID;
	string type;
	string path;
};

class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	Material mats;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, Material mats, glm::vec3 position, glm::vec3 scale);
	void setXpos(float xpos);
	void setYpos(float ypos);
	void setZpos(float zpos);
	void setScale(glm::vec3 scale);
	void setX_angle(double X_angle);
	void setY_angle(double Y_angle);
	void setZ_angle(double Z_angle);
	float getXpos();
	float getYpos();
	float getZpos();
	glm::vec3 getScale();
	double getX_angle();
	double getY_angle();
	double getZ_angle();
	glm::mat4 getModelMat();

	void draw(Shader& shader, Camera &camera);

	// now that we have all the required data, set the vertex buffers and its attribute pointers.
private:
	unsigned int VAO, VBO, EBO;
	glm::vec3 position;
	glm::vec3 scale;
	
	double x_angle;
	double y_angle;
	double z_angle;
	void setupMesh();
};