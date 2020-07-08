#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Model.hpp"
#include "View.hpp"


using namespace std;
using namespace glm;


double Model::world_x_angle = 0;
double Model::world_y_angle = 0;
double Model::world_z_angle = 0;

GLFWwindow * Model::window;

Model * Model::instances[10];

int Model::current_index = 1;

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
	if (key == GLFW_KEY_UP && action == GLFW_REPEAT)
		world_x_angle -= 0.01;
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT)
		world_x_angle += 0.01;
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT)
		world_z_angle += 0.01;
	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT)
		world_z_angle -= 0.01;
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		world_x_angle = 0;
		world_y_angle = 0;
		world_z_angle = 0;
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (key == GLFW_KEY_T && action == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		current_index = 1;
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		current_index = 2;
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		current_index = 3;
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
		current_index = 4;
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
		current_index = 5;
	if (key == GLFW_KEY_6 && action == GLFW_PRESS)
		current_index = 6;
	if (key == GLFW_KEY_7 && action == GLFW_PRESS)
		current_index = 7;
	if (key == GLFW_KEY_8 && action == GLFW_PRESS)
		current_index = 8;
	if (key == GLFW_KEY_9 && action == GLFW_PRESS)
		current_index = 9;
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
		current_index = 0;

	if (instances[current_index] != nullptr)
		instances[current_index]->key_callback(window, key, scancode, action, mods);
}

void Model::setInstance(int index)
{
	instances[index] = this;
}

Model::Model(int startIndex, int endIndex, GLenum mode)
{
	this->mode = mode;

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
	mat4 world_rotate_x_mat = rotate(mat4(1), float(world_x_angle), vec3(1, 0, 0));
	mat4 world_rotate_y_mat = rotate(mat4(1), float(world_y_angle), vec3(0, 1, 0));
	mat4 world_rotate_z_mat = rotate(mat4(1), float(world_z_angle), vec3(0, 0, 1));
	mat4 world_rotate_mat = world_rotate_x_mat * world_rotate_y_mat * world_rotate_z_mat;

	mat4 translate_mat = translate(mat4(1), vec3(x_pos, y_pos, z_pos));

	mat4 scale_mat = glm::scale(mat4(1), vec3(scale, scale, scale));

	mat4 rotate_x_mat = rotate(mat4(1), float(x_angle), vec3(1, 0, 0));
	mat4 rotate_y_mat = rotate(mat4(1), float(y_angle), vec3(0, 1, 0));
	mat4 rotate_z_mat = rotate(mat4(1), float(z_angle), vec3(0, 0, 1));
	mat4 rotate_mat = rotate_x_mat * rotate_y_mat * rotate_z_mat;

	mat4 model_mat = translate_mat * rotate_mat * scale_mat * world_rotate_mat;

	mat4 mvp_mat = projection_mat * view_mat * model_mat;

	GLuint mvpMatLocation = glGetUniformLocation(shader_program, mvp_mat_string);
	glUniformMatrix4fv(mvpMatLocation, 1, GL_FALSE, &mvp_mat[0][0]);

	if (mode == GL_LINES)
	{
		for (int i = startIndex; i < endIndex; i +=2)
			glDrawArrays(mode, i, 2);
	}
	else if (mode == GL_TRIANGLES)
	{
		for (int i = startIndex; i < endIndex; i += 3)
			glDrawArrays(mode, i, 3);
	}
}

mat4 Model::getModelMat()
{
	mat4 translate_mat = translate(mat4(1), vec3(x_pos, y_pos, z_pos));

	mat4 scale_mat = glm::scale(mat4(1), vec3(scale, scale, scale));

	mat4 rotate_x_mat = rotate(mat4(1), float(x_angle), vec3(1, 0, 0));
	mat4 rotate_y_mat = rotate(mat4(1), float(y_angle), vec3(0, 1, 0));
	mat4 rotate_z_mat = rotate(mat4(1), float(z_angle), vec3(0, 0, 1));
	mat4 rotate_mat = rotate_x_mat * rotate_y_mat * rotate_z_mat;

	mat4 model_mat = translate_mat * rotate_mat * scale_mat;

	return model_mat;
}