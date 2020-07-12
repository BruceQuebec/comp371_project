#include "draw.hpp"


using namespace glm;
using namespace std;


void draw(mat4 world_mat, vector<Model *> models, mat4 camera_mat, GLuint mvp_mat_location)
{
	// Draw each model in the vector
	for (vector<Model *>::iterator it = models.begin(); it != models.end(); it++)
		draw(world_mat, **it, camera_mat, mvp_mat_location);
}

void draw(mat4 world_mat, Model & model, mat4 camera_mat, GLuint mvp_mat_location)
{
	// Bind the vertex array of the model
	glBindVertexArray(model.getVertexArray());

	// Calculate the mvp matrix of the model
	mat4 mvp_mat = camera_mat * world_mat * model.getModelMat();

	// Process the model with the given matrix
	glUniformMatrix4fv(mvp_mat_location, 1, GL_FALSE, &mvp_mat[0][0]);

	// Draw the model
	glDrawElements(model.getMode(), model.getNumElements(), GL_UNSIGNED_INT, 0);
}