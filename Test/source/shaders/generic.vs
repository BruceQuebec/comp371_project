#version 330 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_normal_pos;
layout(location = 2) in vec3 vertex_color;
layout(location = 3) in vec2 vertext_texture_coords;

out VS_OUT{
	vec3 fragment_position;
    vec3 fragment_normal;
	vec3 fragment_color;
    vec2 fragment_texCoords;
}vs_out;

uniform mat4 model;
uniform mat4 view_projection;


void main()
{
	vs_out.fragment_position = vec3(model * vec4(vertex_pos, 1.0));
    vs_out.fragment_normal = mat3(transpose(inverse(model))) * vertex_normal_pos; 
	vs_out.fragment_color = vertex_color;
	vs_out.fragment_texCoords = vertext_texture_coords;
	gl_Position = view_projection * vec4(vs_out.fragment_position, 1.0);
}