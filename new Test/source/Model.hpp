#pragma once


class Model
{
public:
	static GLFWwindow * window;

	static GLuint shader_program;

	static const GLchar * mvp_mat_string;

	static Model * instances[];
	
	static int current_index;

	static void setWindow(GLFWwindow * window);

	static void setShaderProgram(GLuint shader_program);

	static void setMVPMatString(const GLchar * mvp_mat_string);

	static void key_callback_dispatch(GLFWwindow * window, int key, int scancode, int action, int mods);

private:
	GLenum mode;

	int startIndex;
	int endIndex;

	double x_pos;
	double y_pos;
	double z_pos;
	double scale;
	double x_angle;
	double y_angle;
	double z_angle;

public:
	Model(int startIndex, int endIndex, GLenum mode);

	~Model() {}

	void setInstance(int index);

	void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);

	void draw(glm::mat4 view_mat, glm::mat4 projection_mat);
};