#include "draw.hpp"
#include "World.hpp"


using namespace glm;
using namespace std;


void draw(mat4 world_mat, unordered_map<string, Model *> models, mat4 cameraMat, vec3 cameraPos)
{
	// Draw each model in the map
	unordered_map<string, Model *>::iterator it = models.begin();
	while (it != models.end()) {
		draw(world_mat, *it->second, cameraMat, cameraPos, it->first);
		it++;
	}
}

void draw(mat4 world_mat, Model & model, mat4 cameraMat, vec3 cameraPos, string model_name)
{	
	// Process the model with the given matrix
	model.getShader().use();

	// if the models aren't light source model, render them with light computation
	if (model_name != "lightCube") {
		lighting_render(model, cameraPos, model_name);
	}

	if (model.getTextureID() != 0) {
		model.getShader().setInt("material.diffuse", 0);
		//std::cout << "Texture ID is: " << model.getTexture() << std::endl;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, model.getTextureID());
	}

	model.getShader().setMat4("model", model.getModelMat());
	model.getShader().setMat4("view_projection", cameraMat);
	
	// Bind the vertex array of the model
	glBindVertexArray(model.getVertexArray());
	glDrawArrays(model.getMode(), 0, model.getNumVertices() / 3);
}

void lighting_render(Model model, vec3 cameraPos, string model_name) {
	// update the light and view position in fragment shader
	model.getShader().setVec3("light.position", Model::models["lightCube"]->getPosition());
	model.getShader().setVec3("viewPos", cameraPos);

	// point light properties
	glm::vec3 lightColor(1.0, 1.0, 1.0);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.9f); // decrease the influence
	glm::vec3 ambientColor = diffuseColor * glm::vec3(0.6f); // low influence
	model.getShader().setVec3("light.ambient", ambientColor);
	model.getShader().setVec3("light.diffuse", diffuseColor);
	model.getShader().setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

	// material properties
	model.getShader().setVec3("material.ambient", glm::vec3(0.19225, 0.19225, 0.19225));
	if (model.getTextureID() == 0) {
		model.getShader().setVec3("material.diffuse", glm::vec3(0.50754, 0.50754, 0.50754));
	}
	model.getShader().setVec3("material.specular", glm::vec3(0.508273, 0.508273, 0.508273)); // specular lighting doesn't have full effect on this object's material
	model.getShader().setFloat("material.shininess", 32.0f);

	// point light attenuation calculation
	model.getShader().setFloat("light.constant", 1.0f);
	model.getShader().setFloat("light.linear", 0.09f);
	model.getShader().setFloat("light.quadratic", 0.032f);

	// spotlight 
	model.getShader().setVec3("light.direction", glm::vec3(0, 0, -1));
	model.getShader().setFloat("light.cutOff", glm::cos(glm::radians(32.5f)));
}