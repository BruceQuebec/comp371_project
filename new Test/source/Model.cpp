#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.hpp"


using namespace std;
using namespace glm;


GLFWwindow * Model::window;

Model * Model::instance;

GLuint Model::shader_program;

const GLchar * Model::mvp_mat_string;


void Model::setWindow(GLFWwindow * window)
{
	Model::window = window;
}

void Model::setShaderProgram(GLuint shader_program)
{
	Model::shader_program = shader_program;
}

void Model::setMVPMatString(const GLchar * mvp_mat_string)
{
	Model::mvp_mat_string = mvp_mat_string;
}

void Model::key_callback_dispatch(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	cout << key <<endl;

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	instance->key_callback(window, key, scancode, action, mods);
}

void Model::setInstance(int index)
{
	instance = this;
}

Model::Model(int startIndex, int endIndex)
{
	this->startIndex = startIndex;
	this->endIndex = endIndex;

	x_pos = 0;
	y_pos = 0;
	z_pos = 0;
	scale = 1;
	x_angle = 0;
	y_angle = 0;
	z_angle = 0;
}

void Model::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_W && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		z_pos -= 0.01;
	if (key == GLFW_KEY_S && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		z_pos += 0.01;
	if (key == GLFW_KEY_A && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		x_pos -= 0.01;
	if (key == GLFW_KEY_D && action == GLFW_REPEAT && mods == GLFW_MOD_CAPS_LOCK)
		x_pos += 0.01;
	if (key == GLFW_KEY_U && action == GLFW_REPEAT)
		scale += 0.01;
	if (key == GLFW_KEY_J && action == GLFW_REPEAT)
		scale -= 0.01;
	if (key == GLFW_KEY_W && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		x_angle += 0.01;
	if (key == GLFW_KEY_S && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		x_angle -= 0.01;
	if (key == GLFW_KEY_A && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		y_angle += 0.01;
	if (key == GLFW_KEY_D && action == GLFW_REPEAT && mods != GLFW_MOD_CAPS_LOCK)
		y_angle -= 0.01;
}

void Model::draw(mat4 view_mat, mat4 projection_mat)
{
	mat4 translate_mat = translate(mat4(1), vec3(x_pos, y_pos, z_pos));

	mat4 scale_mat = glm::scale(mat4(1), vec3(scale, scale, scale));

	mat4 rotate_x_mat = rotate(mat4(1), float(x_angle), vec3(1, 0, 0));
	mat4 rotate_y_mat = rotate(mat4(1), float(y_angle), vec3(0, 1, 0));
	mat4 rotate_z_mat = rotate(mat4(1), float(z_angle), vec3(0, 0, 1));
	mat4 rotate_mat = rotate_x_mat * rotate_y_mat * rotate_z_mat;

	mat4 model_mat = translate_mat * rotate_mat * scale_mat;

	mat4 mvp_mat = projection_mat * view_mat * model_mat;

	GLuint mvpMatLocation = glGetUniformLocation(shader_program, mvp_mat_string);
	glUniformMatrix4fv(mvpMatLocation, 1, GL_FALSE, &mvp_mat[0][0]);

	for (int i = startIndex; i < endIndex; i += 3)
		glDrawArrays(GL_TRIANGLES, i, 3);
}