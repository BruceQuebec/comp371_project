#include <GL/glew.h>

#include "load.hpp"

#include "Shader.hpp"


using namespace glm;


Shader::Shader() {}

Shader::Shader(const char * vertex_shader_file_path, const char * fragment_shader_file_path)
{
    program = glCreateProgram();

    loadShader(program, GL_VERTEX_SHADER, vertex_shader_file_path);
    loadShader(program, GL_FRAGMENT_SHADER, fragment_shader_file_path);

    model_mat = glGetUniformLocation(program, "model_mat");
    view_mat = glGetUniformLocation(program, "view_mat");
    projection_mat = glGetUniformLocation(program, "projection_mat");
    light_pos = glGetUniformLocation(program, "light_pos");
    light_color = glGetUniformLocation(program, "light_color");
    camera_pos = glGetUniformLocation(program, "camera_pos");
    texture = glGetUniformLocation(program, "texture_sampler");
    ambient = glGetUniformLocation(program, "ambient");
    diffuse = glGetUniformLocation(program, "diffuse");
    specular = glGetUniformLocation(program, "specular");
    shininess = glGetUniformLocation(program, "shininess");
    light_mvp_mat = glGetUniformLocation(program, "light_mvp_mat");
    shadow_map = glGetUniformLocation(program, "shadow_map");
    render_shadow = glGetUniformLocation(program, "render_shadow");
    render_texture = glGetUniformLocation(program, "render_texture");
}

void Shader::bind(mat4 model_mat, mat4 light_mvp_mat, Light & light, Camera & camera, GLuint texture, Material & material, GLuint shadow_map, bool render_texture, bool render_shadow)
{
    glUseProgram(program);

    bindModelMat(model_mat);
    bindLightMVPMat(light_mvp_mat);
    bindLight(light);
    bindCamera(camera);
    bindTexture(texture);
    bindMaterial(material);
    bindShadowMap(shadow_map);
    bindRenderMode(render_texture, render_shadow);
}

void Shader::bindModelMat(mat4 model_mat)
{
    glUniformMatrix4fv(this->model_mat, 1, GL_FALSE, &model_mat[0][0]);
}

void Shader::bindLightMVPMat(mat4 light_mvp_mat)
{
    glUniformMatrix4fv(this->light_mvp_mat, 1, GL_FALSE, &light_mvp_mat[0][0]);
}

void Shader::bindLight(Light & light)
{
    glUniform3fv(light_pos, 1, &light.getLightPos()[0]);

    glUniform3fv(light_color, 1, &light.getLightColor()[0]);
}

void Shader::bindCamera(Camera & camera)
{
    glUniformMatrix4fv(this->view_mat, 1, GL_FALSE, &camera.getViewMat()[0][0]);

    glUniformMatrix4fv(this->projection_mat, 1, GL_FALSE, &camera.getProjectionMat()[0][0]);

    glUniform3fv(this->camera_pos, 1, &camera.getCameraPos()[0]);
}

void Shader::bindTexture(GLuint texture)
{
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture);

    glUniform1i(this->texture, 0);
}

void Shader::bindMaterial(Material & material)
{
    glUniform3fv(this->ambient, 1, &material.getAmbient()[0]);
    glUniform3fv(this->diffuse, 1, &material.getDiffuse()[0]);
    glUniform3fv(this->specular, 1, &material.getSpecular()[0]);
    glUniform1f(this->shininess, material.getShininess());
}

void Shader::bindShadowMap(GLuint shadow_map)
{
    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, shadow_map);

    glUniform1i(this->shadow_map, 1);
}

void Shader::bindRenderMode(bool render_texture, bool render_shadow)
{
    glUniform1i(this->render_texture, render_texture);

    glUniform1i(this->render_shadow, render_shadow);
}