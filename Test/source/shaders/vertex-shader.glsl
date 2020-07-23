#version 330 core


layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_uv;

out vec3 fragment_color;
out vec4 shadow_coord;
out vec2 uv;

uniform mat4 mvp_mat;
uniform mat4 depth_mvp_mat;


void main()
{
	gl_Position = mvp_mat * vec4(vertex_pos, 1);

	shadow_coord = depth_mvp_mat * vec4(vertex_pos, 1);

	fragment_color = vertex_color;

	uv = vertex_uv;
}