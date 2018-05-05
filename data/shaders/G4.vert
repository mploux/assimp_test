#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in lowp int in_face;
layout (location = 2) in lowp int in_texture;

uniform mat4 projectionMatrix;

out vec4 v_position;
out lowp int v_texture;

out vec3 frag_pos;

out lowp int v_face;

void main()
{
	v_position = vec4(in_position, 1.0);
	v_texture = in_texture;
	v_face = in_face;

	frag_pos = vec4(in_position, 1.0).xyz;
	gl_Position = projectionMatrix * vec4(in_position, 1.0);
}