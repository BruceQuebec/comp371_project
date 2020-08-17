#pragma once


#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


#include "Light.hpp"
#include "Camera.hpp"
#include "Material.hpp"


class Shader
{
private:
    GLuint program;

    GLuint model_mat;
    GLuint view_mat;
    GLuint projection_mat;

    GLuint light_pos;
    GLuint light_color;
    GLuint camera_pos;

    GLuint texture;
    GLuint ambient;
    GLuint diffuse;
    GLuint specular;
    GLuint shininess;

    GLuint light_mvp_mat;
    
    GLuint shadow_map;

    GLuint render_texture;
    GLuint render_shadow;

public:
    Shader();
    Shader(const char * vertex_shader_file_path, const char * fragment_shader_file_path);

    void bind(glm::mat4 model_mat, glm::mat4 light_mvp_mat, Light & light, Camera & camera, GLuint texture, Material & material, GLuint shadow_map, bool render_texture, bool render_shadow);

    void bindModelMat(glm::mat4 model_mat);

    void bindLight(Light & light);

    void bindCamera(Camera & camera);

    void bindTexture(GLuint texture);

    void bindMaterial(Material & material);

    void bindLightMVPMat(glm::mat4 light_mvp_mat);

    void bindShadowMap(GLuint shadow_map);

    void bindRenderMode(bool render_texture, bool render_shadow);
};