#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec2 in_texture;
layout (location = 2) in vec3 in_normal;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec4 v_position;
out vec2 v_texture;
out vec3 v_normal;
out vec3 frag_pos;

void main()
{
	v_position = vec4(in_position, 1.0);
	v_texture = in_texture;
	v_normal = (modelMatrix * vec4(in_normal, 0.0)).xyz;

	frag_pos = vec4(in_position, 1.0).xyz;
	gl_Position = projectionMatrix * modelMatrix * vec4(in_position, 1.0);
}