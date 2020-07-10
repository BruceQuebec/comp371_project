#include <vector>

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>


#include "draw.hpp"


using namespace std;
using namespace glm;


void draw(mat4 world_mat, vector<Model *> models, mat4 camera_mat, GLuint mvp_mat_location)
{
	for (vector<Model *>::iterator it = models.begin(); it != models.end(); it++)
		draw(world_mat, **it, camera_mat, mvp_mat_location);
}

void draw(mat4 world_mat, Model & model, mat4 camera_mat, GLuint mvp_mat_location)
{
	glBindVertexArray(model.getVertexArray());

	mat4 mvp_mat = camera_mat * world_mat * model.getModelMat();

	glUniformMatrix4fv(mvp_mat_location, 1, GL_FALSE, &mvp_mat[0][0]);

	glDrawElements(model.getMode(), model.getNumElements(), GL_UNSIGNED_INT, 0);
}