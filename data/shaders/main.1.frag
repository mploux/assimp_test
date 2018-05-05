#version 400 core

out vec4 out_color;

uniform vec3 cameraPosition;

in float v_color;

void main()
{
    out_color = vec4(0, v_color, 1 - v_color, 1);
}
