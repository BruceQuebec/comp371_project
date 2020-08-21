#include "draw.hpp"


#include "Material.hpp"
#include "control.hpp"


using namespace glm;
using namespace std;


void draw(int width, int height, vector<Model *> models, Light & light, Camera & camera, Shader shader, GLuint shadow_map)
{
	glViewport(0, 0, width, height);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw each model in the vector
	for (vector<Model *>::iterator it = models.begin(); it != models.end(); it++)
		draw(**it, light, camera, shader, shadow_map);
}

void draw(Model & model, Light & light, Camera & camera, Shader shader, GLuint shadow_map)
{
	vec3 eye = light.getLightPos();
	vec3 center = vec3(0, 0, 0);
	vec3 up = vec3(1, 0, 0);
	mat4 view_mat = lookAt(eye, center, up);

	mat4 projection_mat = perspective((float)90, 1.0f, 1.0f, 200.0f);

	mat4 light_camera_mat = projection_mat * view_mat;

	mat4 depth_mvp_mat = light_camera_mat * model.getModelMat();


	shader.bind(model.getModelMat(), depth_mvp_mat, light, camera, model.getTexture(), model.getMaterial(), shadow_map, Control::render_with_texture, Control::render_shadow);


	// Bind the vertex array of the model
	glBindVertexArray(model.getVertexArray());

	// Draw the model
	glDrawArrays(model.getMode(), 0, model.getNumVertices() / 3);
}