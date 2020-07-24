#include <GL/glew.h>

#include "load.hpp"

#include "Shader.hpp"


using namespace glm;


Shader::Shader(const char * vertex_shader_file_path, const char * fragment_shader_file_path)
{
    program = glCreateProgram();

    loadShader(program, GL_VERTEX_SHADER, vertex_shader_file_path);
    loadShader(program, GL_FRAGMENT_SHADER, fragment_shader_file_path);

    mvp_mat = glGetUniformLocation(program, "mvp_mat");
    light_mvp_mat = glGetUniformLocation(program, "light_mvp_mat");
    texture = glGetUniformLocation(program, "texture_sampler");
    shadow_map = glGetUniformLocation(program, "shadow_map");
}

void Shader::bindShader()
{
    glUseProgram(program);
}

void Shader::bindMVPMat(mat4 mvp_mat)
{
    glUniformMatrix4fv(this->mvp_mat, 1, GL_FALSE, &mvp_mat[0][0]);
}

void Shader::bindLightMVPMat(mat4 light_mvp_mat)
{
    glUniformMatrix4fv(this->light_mvp_mat, 1, GL_FALSE, &light_mvp_mat[0][0]);
}

void Shader::bindTexture(GLuint texture)
{
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture);

    glUniform1i(this->texture, 0);
}

void Shader::bindShadowMap(GLuint shadow_map)
{
    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, shadow_map);

    glUniform1i(this->shadow_map, 1);
}