#version 330 core


layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_normal_pos;
layout(location = 2) in vec2 vertext_texture_coords;

out vec3 fragment_position;
out vec3 fragment_normal;
out vec2 fragment_texCoords;

uniform mat4 model;
uniform mat4 view_projection;


void main()
{
	fragment_position = vec3(model * vec4(vertex_pos, 1.0));
    fragment_normal = mat3(transpose(inverse(model))) * vertex_normal_pos;  
	fragment_texCoords = vertext_texture_coords;
	gl_Position = view_projection * vec4(fragment_position, 1.0);
}