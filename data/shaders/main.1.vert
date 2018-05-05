#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in float in_color;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec4 v_position;
out float v_color;

void main()
{
	v_position = vec4(in_position, 1.0);
    v_color = in_color;


	gl_Position = projectionMatrix * modelMatrix * vec4(in_position, 1.0);
}