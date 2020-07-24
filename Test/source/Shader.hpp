#pragma once


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


class Shader
{
private:
    GLuint program;

    GLuint mvp_mat;
    GLuint light_mvp_mat;
    GLuint texture;
    GLuint shadow_map;

public:
    Shader(const char * vertex_shader_file_path, const char * fragment_shader_file_path);

    void bindShader();

    void bindMVPMat(glm::mat4 mvp_mat);

    void bindLightMVPMat(glm::mat4 light_mvp_mat);

    void bindTexture(GLuint texture);

    void bindShadowMap(GLuint shadow_map);
};