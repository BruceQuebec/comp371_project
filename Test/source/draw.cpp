#include "draw.hpp"


#include <iostream>


using namespace glm;
using namespace std;


void draw(mat4 world_mat, vector<Model *> models, mat4 camera_mat, Shader shader, GLuint shadow_map)
{
	// Draw each model in the vector
	for (vector<Model *>::iterator it = models.begin(); it != models.end(); it++)
		draw(world_mat, **it, camera_mat, shader, shadow_map);
}

void draw(mat4 world_mat, Model & model, mat4 camera_mat, Shader shader, GLuint shadow_map)
{
	// Bind the vertex array of the model
	glBindVertexArray(model.getVertexArray());

	// Calculate the mvp matrix of the model
	mat4 mvp_mat = camera_mat * world_mat * model.getModelMat();

	vec3 eye = vec3(0, 30, 0);
	vec3 center = vec3(0, 0, 0);
	vec3 up = vec3(1, 0, 0);
	mat4 view_mat = lookAt(eye, center, up);

	mat4 projection_mat = perspective((float)45, 1.0f, 10.0f, 40.0f);
	//projection_mat = ortho<float>(-20, 20, -20, 20, -10, 20);

	mat4 light_camera_mat = projection_mat * view_mat;

	mat4 depth_mvp_mat = light_camera_mat * world_mat * model.getModelMat();

	//glm::vec3 lightInvDir = glm::vec3(-20, 2, 2);

	//// Compute the MVP matrix from the light's point of view
	
	//glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	//mat4 depth_mvp_mat = depthProjectionMatrix * depthViewMatrix * world_mat * model.getModelMat();


	shader.bindShader();
	shader.bindMVPMat(mvp_mat);
	shader.bindLightMVPMat(depth_mvp_mat);
	shader.bindTexture(model.getTexture());
	shader.bindShadowMap(shadow_map);


	// Draw the model
	glDrawArrays(model.getMode(), 0, model.getNumVertices() / 3);
	//glDrawElements(model.getMode(), model.getNumElements(), GL_UNSIGNED_INT, 0);
}